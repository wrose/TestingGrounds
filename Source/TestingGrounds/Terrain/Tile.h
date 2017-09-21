// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Generation")
    void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.f, float MinScale = 1, float MaxScale = 1);

private:
	FBox BoundsBox;

    FVector FindEmptyLocation(float Radius);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint, float Rotation, float Scale);

    bool CanSpawnAtLocation(FVector Location, float Radius);

	int MAX_ATTEMPTS = 100;

	bool FindEmptyLocation(FVector &OutLocation, float Radius);
};
