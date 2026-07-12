#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/PAInteractable.h"
#include "PANpcActor.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EPANpcRole : uint8
{
    Elder,
    Villager,
    Enemy
};

UCLASS()
class PIXELADVENTURE_API APANpcActor : public AActor, public IPAInteractable
{
    GENERATED_BODY()

public:
    APANpcActor();

    virtual FString GetInteractPrompt() const override;
    virtual bool Interact(APAPixelCharacter* InteractingCharacter) override;
    void SetNpcRole(EPANpcRole NewRole);
    bool IsEnemy() const;
    bool ReceiveHit(APAPixelCharacter* AttackingCharacter, float DamageAmount);

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditAnywhere, Category="NPC")
    EPANpcRole NpcRole;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float EnemyMaxHealth;

    float EnemyCurrentHealth;
    bool bHasTalked;
};
