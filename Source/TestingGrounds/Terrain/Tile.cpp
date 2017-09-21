// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale) {

    auto ActorsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

    for (int i = 0; i < ActorsToSpawn; i++) {
        FVector SpawnPoint;
        float Scale = FMath::RandRange(MinScale, MaxScale);
        if (FindEmptyLocation(SpawnPoint, Radius * Scale)) {
            float Rotation = FMath::RandRange(-180.f, 180.f);
            PlaceActor(ToSpawn, SpawnPoint, Rotation, Scale);
        }
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius) {
    FVector Min(0, -2000, 0);
    FVector Max(4000, 2000, 0);
    FBox Bounds(Min, Max);

    for(int i = 0; i < MAX_ATTEMPTS; i++) {
        auto PossibleLocation = FMath::RandPointInBox(Bounds);
        if (CanSpawnAtLocation(PossibleLocation, Radius)) {
            OutLocation = PossibleLocation;
            return true;
        }
    }
    return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale) {
    auto Spawned = GetWorld()->SpawnActor(ToSpawn);
    Spawned->SetActorRelativeLocation(SpawnPoint);
    Spawned->SetActorRotation(FRotator(0, Rotation, 0));
    Spawned->SetActorScale3D(FVector(Scale));
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

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
