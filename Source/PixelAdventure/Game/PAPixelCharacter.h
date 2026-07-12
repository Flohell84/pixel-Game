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
    void ReceiveDamage(float DamageAmount, const FString& DamageSourceName);
    bool IsAlive() const;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Interact();
    void Attack();
    AActor* FindInteractableActor() const;

    UPROPERTY(VisibleAnywhere, Category="Quest")
    TObjectPtr<UPAQuestComponent> QuestComponent;

    UPROPERTY(EditDefaultsOnly, Category="Interaction")
    float InteractionRadius;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackRadius;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackDamage;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float EnemyCounterDamage;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float MaxHealth;

    float CurrentHealth;
};
