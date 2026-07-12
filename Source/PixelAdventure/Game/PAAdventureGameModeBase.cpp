#include "Game/PAAdventureGameModeBase.h"
#include "Game/PANpcActor.h"
#include "Game/PAPixelCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "World/PAPixelWorldActor.h"

APAAdventureGameModeBase::APAAdventureGameModeBase()
{
    DefaultPawnClass = APAPixelCharacter::StaticClass();
    PixelWorldClass = APAPixelWorldActor::StaticClass();
}

void APAAdventureGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(GameMusic))
    {
        UGameplayStatics::SpawnSound2D(this, GameMusic);
    }

    if (!PixelWorldClass)
    {
        return;
    }

    APAPixelWorldActor* WorldActor = GetWorld()->SpawnActor<APAPixelWorldActor>(PixelWorldClass, FVector::ZeroVector, FRotator::ZeroRotator);
    if (!IsValid(WorldActor))
    {
        return;
    }

    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        PlayerPawn->SetActorLocation(WorldActor->GetPlayerSpawnLocation());
    }

    if (APANpcActor* ElderNpc = GetWorld()->SpawnActor<APANpcActor>(APANpcActor::StaticClass(), WorldActor->GetNpcSpawnLocation(), FRotator::ZeroRotator))
    {
        ElderNpc->SetNpcRole(EPANpcRole::Elder);
    }

    const TArray<FVector> VillagerSpawns = WorldActor->GetSpawnLocationsForSymbol(TEXT('V'), 120.0f);
    for (const FVector& SpawnLocation : VillagerSpawns)
    {
        if (APANpcActor* Villager = GetWorld()->SpawnActor<APANpcActor>(APANpcActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator))
        {
            Villager->SetNpcRole(EPANpcRole::Villager);
        }
    }

    const TArray<FVector> EnemySpawns = WorldActor->GetSpawnLocationsForSymbol(TEXT('G'), 120.0f);
    for (const FVector& SpawnLocation : EnemySpawns)
    {
        if (APANpcActor* Enemy = GetWorld()->SpawnActor<APANpcActor>(APANpcActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator))
        {
            Enemy->SetNpcRole(EPANpcRole::Enemy);
        }
    }
}
