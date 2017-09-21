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

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn) {
    FVector Min(0, -2000, 0);
	FVector Max(4000, 2000, 0);
	auto Box = FBox(Min, Max);

    auto ActorsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    for (int i = 0; i < ActorsToSpawn; i++) {
		const FVector SpawnPoint = FMath::RandPointInBox(Box);
        const FRotator Rotation = FRotator(0.f);
        auto Spawned = GetWorld()->SpawnActor(ToSpawn);
        Spawned->SetActorRelativeLocation(SpawnPoint);
//        CastSphere(Spawned->GetActorLocation(), 300);
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
}

bool ATile::CastSphere(FVector Location, float Radius) {
    FHitResult HitResult;
    DrawDebugSphere(GetWorld(), Location, Radius, 32, FColor::Green);
    auto HasCollision = GetWorld()->SweepSingleByChannel(
                HitResult,
                Location,
                Location,
                FQuat::Identity,
                ECollisionChannel::ECC_GameTraceChannel2, // Spawn channel
                FCollisionShape::MakeSphere(Radius)
        );
    auto ResultColor = HasCollision ? FColor::Red : FColor::Green;
    DrawDebugCapsule(GetWorld(), Location, 0, Radius, FQuat::Identity, ResultColor, true, 100);
    return HasCollision;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	CastSphere(GetActorLocation(), 300);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
