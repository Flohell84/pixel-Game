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

    GetWorld()->SpawnActor<APANpcActor>(APANpcActor::StaticClass(), WorldActor->GetNpcSpawnLocation(), FRotator::ZeroRotator);
}
