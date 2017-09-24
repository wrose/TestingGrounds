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


template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, const FGenerationParameters GenerationParameters)
{
	const auto ActorsToSpawn = FMath::RandRange(GenerationParameters.MinSpawn, GenerationParameters.MaxSpawn);

	for (size_t i = 0; i < ActorsToSpawn; i++) {
		PlaceActor(ToSpawn, GenerateSpawnPosition(GenerationParameters));
	}
}

void ATile::PlaceActors(const TSubclassOf<AActor> ToSpawn, const FGenerationParameters GenerationParameters) {
	RandomlyPlaceActors(ToSpawn, GenerationParameters);
}

void ATile::PlaceAIPawns(const TSubclassOf<APawn> ToSpawn, const FGenerationParameters GenerationParameters)
{
	RandomlyPlaceActors(ToSpawn, GenerationParameters);
}


TArray<FSpawnPosition> ATile::GenerateSpawnPositions(const FGenerationParameters GenerationParameters) const {
	const auto ActorsToSpawn = FMath::RandRange(GenerationParameters.MinSpawn, GenerationParameters.MaxSpawn);

	TArray<FSpawnPosition> SpawnPositions;
    for (int i = 0; i < ActorsToSpawn; i++) {
		SpawnPositions.Add(GenerateSpawnPosition(GenerationParameters));
	}
	return SpawnPositions;
}

FSpawnPosition ATile::GenerateSpawnPosition(const FGenerationParameters GenerationParameters) const {
	FSpawnPosition SpawnPosition;
	SpawnPosition.Scale = FMath::RandRange(GenerationParameters.MinScale, GenerationParameters.MaxScale);
	SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
	FindEmptyLocation(SpawnPosition.Location, GenerationParameters.Radius * SpawnPosition.Scale);
	return SpawnPosition;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, const float Radius) const {
	const FBox Bounds(MinExtent, MaxExtent);

    for(int i = 0; i < MAX_ATTEMPTS; i++) {
	    const auto PossibleLocation = FMath::RandPointInBox(Bounds);
        if (CanSpawnAtLocation(PossibleLocation, Radius)) {
            OutLocation = PossibleLocation;
            return true;
        }
    }
    return false;
}

void ATile::PlaceActor(const TSubclassOf<APawn> ToSpawn, const FSpawnPosition SpawnPosition) {
	auto Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
	}
}

void ATile::PlaceActor(const TSubclassOf<AActor> ToSpawn, const FSpawnPosition SpawnPosition) {
    auto Spawned = GetWorld()->SpawnActor(ToSpawn);
	if (Spawned) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}

bool ATile::CanSpawnAtLocation(const FVector Location, const float Radius) const {
    FHitResult HitResult;
	const auto GlobalLocation = ActorToWorld().TransformPosition(Location);
	const auto HasCollision = GetWorld()->SweepSingleByChannel(
                HitResult,
                GlobalLocation,
                GlobalLocation,
                FQuat::Identity,
                ECollisionChannel::ECC_GameTraceChannel2, // Spawn channel
                FCollisionShape::MakeSphere(Radius)
        );
    //auto ResultColor = HasCollision ? FColor::Red : FColor::Green;
//    DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
    return !HasCollision;
}

void ATile::SetPool(UActorPool* InPool) {
    Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName())
		return;
	}
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
	Pool->Return(NavMeshBoundsVolume);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
