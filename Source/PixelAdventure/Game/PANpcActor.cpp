#include "Game/PANpcActor.h"
#include "Components/StaticMeshComponent.h"
#include "Game/PAPixelCharacter.h"
#include "Quest/PAQuestComponent.h"
#include "UObject/ConstructorHelpers.h"

APANpcActor::APANpcActor()
{
    PrimaryActorTick.bCanEverTick = false;
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
    return bHasTalked ? TEXT("Der Dorfälteste nickt dir zu.") : TEXT("Drücke E, um mit dem Dorfältesten zu sprechen.");
}

bool APANpcActor::Interact(APAPixelCharacter* InteractingCharacter)
{
    if (!IsValid(InteractingCharacter))
    {
        return false;
    }

    UPAQuestComponent* QuestComponent = InteractingCharacter->GetQuestComponent();
    if (!bHasTalked && IsValid(QuestComponent))
    {
        QuestComponent->HandleTalkedToElder();
    }

    const FString Dialogue = !bHasTalked
        ? TEXT("Dorfältester: Willkommen, Abenteurer. Deine Reise hat begonnen!")
        : TEXT("Dorfältester: Viel Glück auf deinem weiteren Weg.");

    InteractingCharacter->ShowMessage(Dialogue);
    bHasTalked = true;
    return true;
}
