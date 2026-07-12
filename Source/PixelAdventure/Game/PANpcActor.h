#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/PAInteractable.h"
#include "PANpcActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class PIXELADVENTURE_API APANpcActor : public AActor, public IPAInteractable
{
    GENERATED_BODY()

public:
    APANpcActor();

    virtual FString GetInteractPrompt() const override;
    virtual bool Interact(APAPixelCharacter* InteractingCharacter) override;

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    bool bHasTalked;
};
