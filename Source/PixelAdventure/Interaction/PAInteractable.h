#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PAInteractable.generated.h"

class APAPixelCharacter;

UINTERFACE(MinimalAPI)
class UPAInteractable : public UInterface
{
    GENERATED_BODY()
};

class PIXELADVENTURE_API IPAInteractable
{
    GENERATED_BODY()

public:
    virtual FString GetInteractPrompt() const = 0;
    virtual bool Interact(APAPixelCharacter* InteractingCharacter) = 0;
};
