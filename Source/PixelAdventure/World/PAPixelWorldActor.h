#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PAPixelWorldActor.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;

UCLASS()
class PIXELADVENTURE_API APAPixelWorldActor : public AActor
{
    GENERATED_BODY()

public:
    APAPixelWorldActor();

    virtual void BeginPlay() override;

    FVector GetNpcSpawnLocation() const;
    FVector GetPlayerSpawnLocation() const;

private:
    void BuildWorld();
    FVector TileToWorld(int32 X, int32 Y, float ZOffset = 0.0f) const;
    FIntPoint FindSymbol(TCHAR Symbol) const;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UInstancedStaticMeshComponent> GroundInstances;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UInstancedStaticMeshComponent> WallInstances;

    UPROPERTY(VisibleAnywhere, Category="Components")
    TObjectPtr<UInstancedStaticMeshComponent> TreeInstances;

    UPROPERTY(EditDefaultsOnly, Category="World")
    float TileSize;

    UPROPERTY(EditDefaultsOnly, Category="World")
    TArray<FString> TileMap;
};
