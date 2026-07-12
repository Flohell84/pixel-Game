#include "Game/PANpcActor.h"
#include "Components/StaticMeshComponent.h"
#include "Game/PAPixelCharacter.h"
#include "Quest/PAQuestComponent.h"
#include "UObject/ConstructorHelpers.h"

APANpcActor::APANpcActor()
{
    PrimaryActorTick.bCanEverTick = false;
    NpcRole = EPANpcRole::Elder;
    EnemyMaxHealth = 40.0f;
    EnemyCurrentHealth = EnemyMaxHealth;
    bHasTalked = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NpcMesh"));
    RootComponent = MeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
    }

    MeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.2f));
    MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

FString APANpcActor::GetInteractPrompt() const
{
    switch (NpcRole)
    {
    case EPANpcRole::Villager:
        return TEXT("Drücke E, um mit dem Bewohner zu sprechen.");
    case EPANpcRole::Enemy:
        return TEXT("Ein Gegner steht vor dir. Drücke Leertaste zum Angriff.");
    case EPANpcRole::Elder:
    default:
        return bHasTalked ? TEXT("Der Dorfälteste nickt dir zu.") : TEXT("Drücke E, um mit dem Dorfältesten zu sprechen.");
    }
}

bool APANpcActor::Interact(APAPixelCharacter* InteractingCharacter)
{
    if (!IsValid(InteractingCharacter))
    {
        return false;
    }

    if (NpcRole == EPANpcRole::Enemy)
    {
        InteractingCharacter->ShowMessage(TEXT("Der Gegner knurrt dich an. Nutze Leertaste zum Angriff!"));
        return false;
    }

    UPAQuestComponent* QuestComponent = InteractingCharacter->GetQuestComponent();
    if (NpcRole == EPANpcRole::Elder && !bHasTalked && IsValid(QuestComponent))
    {
        QuestComponent->HandleTalkedToElder();
    }

    const FString Dialogue = (NpcRole == EPANpcRole::Villager)
        ? TEXT("Bewohner: Pass auf die Gegner außerhalb des Dorfes auf!")
        : (!bHasTalked
            ? TEXT("Dorfältester: Willkommen, Abenteurer. Deine Reise hat begonnen!")
            : TEXT("Dorfältester: Viel Glück auf deinem weiteren Weg."));

    InteractingCharacter->ShowMessage(Dialogue);
    bHasTalked = true;
    return true;
}

void APANpcActor::SetNpcRole(EPANpcRole NewRole)
{
    NpcRole = NewRole;
    if (NpcRole == EPANpcRole::Enemy)
    {
        bHasTalked = true;
        EnemyCurrentHealth = EnemyMaxHealth;
        MeshComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 1.1f));
    }
    else
    {
        bHasTalked = false;
        MeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.2f));
    }
}

bool APANpcActor::IsEnemy() const
{
    return NpcRole == EPANpcRole::Enemy;
}

bool APANpcActor::ReceiveHit(APAPixelCharacter* AttackingCharacter, float DamageAmount)
{
    if (!IsValid(AttackingCharacter) || !IsEnemy() || DamageAmount <= 0.0f)
    {
        return false;
    }

    EnemyCurrentHealth = FMath::Clamp(EnemyCurrentHealth - DamageAmount, 0.0f, EnemyMaxHealth);
    if (EnemyCurrentHealth <= 0.0f)
    {
        AttackingCharacter->ShowMessage(TEXT("Gegner besiegt!"));
        Destroy();
        return true;
    }

    AttackingCharacter->ShowMessage(FString::Printf(TEXT("Treffer! Gegner-Leben: %.0f"), EnemyCurrentHealth));
    return false;
}
