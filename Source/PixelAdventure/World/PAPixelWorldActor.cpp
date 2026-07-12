#include "World/PAPixelWorldActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"

APAPixelWorldActor::APAPixelWorldActor()
{
    PrimaryActorTick.bCanEverTick = false;
    TileSize = 200.0f;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneRoot;

    GroundInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("GroundInstances"));
    GroundInstances->SetupAttachment(RootComponent);
    GroundInstances->SetCollisionProfileName(TEXT("BlockAll"));

    WallInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallInstances"));
    WallInstances->SetupAttachment(RootComponent);
    WallInstances->SetCollisionProfileName(TEXT("BlockAll"));

    TreeInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TreeInstances"));
    TreeInstances->SetupAttachment(RootComponent);
    TreeInstances->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        GroundInstances->SetStaticMesh(CubeMesh.Object);
        WallInstances->SetStaticMesh(CubeMesh.Object);
        TreeInstances->SetStaticMesh(CubeMesh.Object);
    }

    TileMap = {
        TEXT("####################"),
        TEXT("#..T....V.##.....VN#"),
        TEXT("#.####....##..###..#"),
        TEXT("#...G..T..........##"),
        TEXT("#..#######..###.G..#"),
        TEXT("#...........###....#"),
        TEXT("#.######...........#"),
        TEXT("#....V.........#####"),
        TEXT("#..S...............#"),
        TEXT("####################")
    };
}

void APAPixelWorldActor::BeginPlay()
{
    Super::BeginPlay();
    BuildWorld();
}

FVector APAPixelWorldActor::GetNpcSpawnLocation() const
{
    const FIntPoint NpcTile = FindSymbol(TEXT('N'));
    return TileToWorld(NpcTile.X, NpcTile.Y, TileSize * 0.6f);
}

FVector APAPixelWorldActor::GetPlayerSpawnLocation() const
{
    const FIntPoint PlayerTile = FindSymbol(TEXT('S'));
    return TileToWorld(PlayerTile.X, PlayerTile.Y, TileSize * 0.6f);
}

TArray<FVector> APAPixelWorldActor::GetSpawnLocationsForSymbol(TCHAR Symbol, float ZOffset) const
{
    TArray<FVector> SpawnLocations;

    for (int32 Y = 0; Y < TileMap.Num(); ++Y)
    {
        const FString& Row = TileMap[Y];
        for (int32 X = 0; X < Row.Len(); ++X)
        {
            if (Row[X] == Symbol)
            {
                SpawnLocations.Add(TileToWorld(X, Y, ZOffset));
            }
        }
    }

    return SpawnLocations;
}

void APAPixelWorldActor::BuildWorld()
{
    GroundInstances->ClearInstances();
    WallInstances->ClearInstances();
    TreeInstances->ClearInstances();

    const float Scale = TileSize / 100.0f;
    const FTransform GroundScale(FRotator::ZeroRotator, FVector::ZeroVector, FVector(Scale, Scale, 0.15f));
    const FTransform WallScale(FRotator::ZeroRotator, FVector::ZeroVector, FVector(Scale, Scale, 1.0f));
    const FTransform TreeScale(FRotator::ZeroRotator, FVector::ZeroVector, FVector(Scale * 0.6f, Scale * 0.6f, 1.4f));

    for (int32 Y = 0; Y < TileMap.Num(); ++Y)
    {
        const FString& Row = TileMap[Y];
        for (int32 X = 0; X < Row.Len(); ++X)
        {
            const FVector BaseLocation = TileToWorld(X, Y);

            FTransform GroundTransform = GroundScale;
            GroundTransform.SetLocation(BaseLocation);
            GroundInstances->AddInstance(GroundTransform);

            const TCHAR Symbol = Row[X];
            if (Symbol == TCHAR('#'))
            {
                FTransform WallTransform = WallScale;
                WallTransform.SetLocation(BaseLocation + FVector(0.0f, 0.0f, TileSize * 0.5f));
                WallInstances->AddInstance(WallTransform);
            }
            else if (Symbol == TCHAR('T'))
            {
                FTransform TreeTransform = TreeScale;
                TreeTransform.SetLocation(BaseLocation + FVector(0.0f, 0.0f, TileSize * 0.7f));
                TreeInstances->AddInstance(TreeTransform);
            }
        }
    }
}

FVector APAPixelWorldActor::TileToWorld(int32 X, int32 Y, float ZOffset) const
{
    return GetActorLocation() + FVector(static_cast<float>(X) * TileSize, static_cast<float>(Y) * TileSize, ZOffset);
}

FIntPoint APAPixelWorldActor::FindSymbol(TCHAR Symbol) const
{
    for (int32 Y = 0; Y < TileMap.Num(); ++Y)
    {
        const FString& Row = TileMap[Y];
        for (int32 X = 0; X < Row.Len(); ++X)
        {
            if (Row[X] == Symbol)
            {
                return FIntPoint(X, Y);
            }
        }
    }

    return FIntPoint(1, 1);
}
