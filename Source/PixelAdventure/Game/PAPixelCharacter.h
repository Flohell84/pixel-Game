#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PAPixelCharacter.generated.h"

class UPAQuestComponent;

UCLASS()
class PIXELADVENTURE_API APAPixelCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APAPixelCharacter();
    UPAQuestComponent* GetQuestComponent() const;
    void ShowMessage(const FString& Text) const;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Interact();
    AActor* FindInteractableActor() const;

    UPROPERTY(VisibleAnywhere, Category="Quest")
    TObjectPtr<UPAQuestComponent> QuestComponent;

    UPROPERTY(EditDefaultsOnly, Category="Interaction")
    float InteractionRadius;
};
