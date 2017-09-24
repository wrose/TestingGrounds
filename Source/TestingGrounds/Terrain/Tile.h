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

USTRUCT(BlueprintType)
struct FGenerationParameters
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int MinSpawn = 1;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int MaxSpawn = 1;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float Radius = 500.f;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float MinScale = 1;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	float MaxScale = 1;

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
	void PlaceActors(TSubclassOf<AActor> ToSpawn, FGenerationParameters GenerationParameters);

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FGenerationParameters GenerationParameters);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* Pool);


private:
	FBox BoundsBox;

	int MAX_ATTEMPTS = 100;

	UActorPool* NavMeshPool;

    UActorPool* Pool;

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, FGenerationParameters GenerationParameters);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);

    bool CanSpawnAtLocation(FVector Location, float Radius);

	bool FindEmptyLocation(FVector &OutLocation, float Radius);

	TArray<FSpawnPosition> GenerateSpawnPositions(FGenerationParameters GenerationParameters);

	FSpawnPosition GenerateSpawnPosition(FGenerationParameters GenerationParameters);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	AActor* NavMeshBoundsVolume;

	void PositionNavMeshBoundsVolume();
};
