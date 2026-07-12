#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PAPixelCharacter.generated.h"

UCLASS()
class PIXELADVENTURE_API APAPixelCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APAPixelCharacter();

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Interact();
};
