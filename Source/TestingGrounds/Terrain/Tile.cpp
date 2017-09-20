// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"

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
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}
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
