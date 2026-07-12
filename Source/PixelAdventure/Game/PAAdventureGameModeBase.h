#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PAAdventureGameModeBase.generated.h"

class APAPixelWorldActor;

UCLASS()
class PIXELADVENTURE_API APAAdventureGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    APAAdventureGameModeBase();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category="Adventure")
    TSubclassOf<APAPixelWorldActor> PixelWorldClass;
};
