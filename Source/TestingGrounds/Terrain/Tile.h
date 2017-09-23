// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location; 
	float Rotation; 
	float Scale;
};


class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Generation")
    void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.f, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* Pool);


private:
	FBox BoundsBox;

	int MAX_ATTEMPTS = 100;

	UActorPool* NavMeshPool;

    UActorPool* Pool;

    FVector FindEmptyLocation(float Radius);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

    bool CanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(FVector &OutLocation, float Radius);

	TArray<FSpawnPosition> GenerateSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	AActor* NavMeshBoundsVolume;

	void PositionNavMeshBoundsVolume();
};
