#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAQuestComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIXELADVENTURE_API UPAQuestComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPAQuestComponent();

    bool IsQuestCompleted() const;
    FString GetObjectiveText() const;
    void HandleTalkedToElder();

private:
    bool bQuestCompleted;
};
