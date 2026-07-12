#include "Game/PAPixelCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APAPixelCharacter::APAPixelCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

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
}

void APAPixelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APAPixelCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APAPixelCharacter::MoveRight);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APAPixelCharacter::Interact);
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
    UKismetSystemLibrary::PrintString(this, TEXT("Interaktion ausgelöst"), true, false, FLinearColor::Yellow, 1.5f);
}
