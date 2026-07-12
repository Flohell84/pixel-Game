#include <cfloat>

#include "Game/PAPixelCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Game/PANpcActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interaction/PAInteractable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Quest/PAQuestComponent.h"

APAPixelCharacter::APAPixelCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    InteractionRadius = 220.0f;
    AttackRadius = 240.0f;
    AttackDamage = 20.0f;
    EnemyCounterDamage = 10.0f;
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = 325.f;

    USpringArmComponent* CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 700.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bUsePawnControlRotation = false;

    UCameraComponent* TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCamera->bUsePawnControlRotation = false;

    QuestComponent = CreateDefaultSubobject<UPAQuestComponent>(TEXT("QuestComponent"));
}

UPAQuestComponent* APAPixelCharacter::GetQuestComponent() const
{
    return QuestComponent;
}

void APAPixelCharacter::ShowMessage(const FString& Text) const
{
    UKismetSystemLibrary::PrintString(this, Text, true, false, FLinearColor::Yellow, 2.5f);
}

void APAPixelCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
    if (IsValid(QuestComponent))
    {
        ShowMessage(QuestComponent->GetObjectiveText());
    }
    ShowMessage(FString::Printf(TEXT("Lebenspunkte: %.0f"), CurrentHealth));
}

void APAPixelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APAPixelCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APAPixelCharacter::MoveRight);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APAPixelCharacter::Interact);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APAPixelCharacter::Attack);
}

void APAPixelCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        AddMovementInput(FVector::ForwardVector, Value);
    }
}

void APAPixelCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        AddMovementInput(FVector::RightVector, Value);
    }
}

void APAPixelCharacter::Interact()
{
    if (!IsAlive())
    {
        return;
    }

    AActor* InteractableActor = FindInteractableActor();
    if (!IsValid(InteractableActor))
    {
        ShowMessage(TEXT("Niemand in Reichweite für eine Interaktion."));
        return;
    }

    IPAInteractable* Interactable = Cast<IPAInteractable>(InteractableActor);
    if (!Interactable)
    {
        ShowMessage(TEXT("Dieses Objekt reagiert nicht."));
        return;
    }

    if (Interactable->Interact(this) && IsValid(QuestComponent))
    {
        ShowMessage(QuestComponent->GetObjectiveText());
    }
}

void APAPixelCharacter::Attack()
{
    if (!IsAlive() || !GetWorld())
    {
        return;
    }

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PACombatTrace), false, this);
    const FCollisionObjectQueryParams ObjectParams(FCollisionObjectQueryParams::AllDynamicObjects);
    const bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ObjectParams,
        FCollisionShape::MakeSphere(AttackRadius),
        QueryParams
    );

    if (!bHasOverlap)
    {
        ShowMessage(TEXT("Kein Gegner in Angriffsreichweite."));
        return;
    }

    APANpcActor* ClosestEnemy = nullptr;
    float ClosestDistanceSquared = FLT_MAX;

    for (const FOverlapResult& Result : OverlapResults)
    {
        APANpcActor* Candidate = Cast<APANpcActor>(Result.GetActor());
        if (!IsValid(Candidate) || !Candidate->IsEnemy())
        {
            continue;
        }

        const float DistanceSquared = FVector::DistSquared(Candidate->GetActorLocation(), GetActorLocation());
        if (DistanceSquared < ClosestDistanceSquared)
        {
            ClosestDistanceSquared = DistanceSquared;
            ClosestEnemy = Candidate;
        }
    }

    if (!IsValid(ClosestEnemy))
    {
        ShowMessage(TEXT("Kein Gegner in Angriffsreichweite."));
        return;
    }

    const bool bEnemyDefeated = ClosestEnemy->ReceiveHit(this, AttackDamage);
    if (!bEnemyDefeated)
    {
        ReceiveDamage(EnemyCounterDamage, TEXT("Gegner"));
    }
}

AActor* APAPixelCharacter::FindInteractableActor() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PAInteractTrace), false, this);
    const FCollisionObjectQueryParams ObjectParams(FCollisionObjectQueryParams::AllDynamicObjects);
    const bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ObjectParams,
        FCollisionShape::MakeSphere(InteractionRadius),
        QueryParams
    );

    if (!bHasOverlap)
    {
        return nullptr;
    }

    for (const FOverlapResult& Result : OverlapResults)
    {
        AActor* Candidate = Result.GetActor();
        if (IsValid(Candidate) && Candidate != this && Candidate->GetClass()->ImplementsInterface(UPAInteractable::StaticClass()))
        {
            return Candidate;
        }
    }

    return nullptr;
}

void APAPixelCharacter::ReceiveDamage(float DamageAmount, const FString& DamageSourceName)
{
    if (!IsAlive() || DamageAmount <= 0.0f)
    {
        return;
    }

    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
    ShowMessage(FString::Printf(TEXT("%s trifft dich! Leben: %.0f"), *DamageSourceName, CurrentHealth));

    if (CurrentHealth <= 0.0f)
    {
        ShowMessage(TEXT("Du wurdest besiegt!"));
        GetCharacterMovement()->DisableMovement();
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            DisableInput(PlayerController);
        }
    }
}

bool APAPixelCharacter::IsAlive() const
{
    return CurrentHealth > 0.0f;
}
