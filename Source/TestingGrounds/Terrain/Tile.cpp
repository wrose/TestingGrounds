// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {

	auto SpawnPositions = GenerateSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

    for (auto SpawnPosition : SpawnPositions) {
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

TArray<FSpawnPosition> ATile::GenerateSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {
    auto ActorsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	TArray<FSpawnPosition> SpawnPositions;
    for (int i = 0; i < ActorsToSpawn; i++) {
		FSpawnPosition SpawnPosition;
        SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
        SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
		FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		SpawnPositions.Add(SpawnPosition);
	}
	return SpawnPositions;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {
    FBox Bounds(MinExtent, MaxExtent);

    for(int i = 0; i < MAX_ATTEMPTS; i++) {
        auto PossibleLocation = FMath::RandPointInBox(Bounds);
        if (CanSpawnAtLocation(PossibleLocation, Radius)) {
            OutLocation = PossibleLocation;
            return true;
        }
    }
    return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition) {
    auto Spawned = GetWorld()->SpawnActor(ToSpawn);
    Spawned->SetActorRelativeLocation(SpawnPosition.Location);
    Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
    Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
    Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius) {
    FHitResult HitResult;
    FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
    auto HasCollision = GetWorld()->SweepSingleByChannel(
                HitResult,
                GlobalLocation,
                GlobalLocation,
                FQuat::Identity,
                ECollisionChannel::ECC_GameTraceChannel2, // Spawn channel
                FCollisionShape::MakeSphere(Radius)
        );
    auto ResultColor = HasCollision ? FColor::Red : FColor::Green;
//    DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
    return !HasCollision;
}

void ATile::SetPool(UActorPool* InPool) {
    UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));
    Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == NULL) {
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName())
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("[%s] Retrieved %s from pool."), *GetName(), *NavMeshBoundsVolume->GetName())
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation());
	GetWorld()->GetNavigationSystem()->Build();
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] EndPlay"), *GetName());
	Pool->Return(NavMeshBoundsVolume);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
