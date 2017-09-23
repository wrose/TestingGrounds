// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "ActorPool.h"
#include "EngineUtils.h"
#include <EngineClasses.h>

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode() : Super() {
    NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
}

void AInfiniteTerrainGameMode::AddToPool(class ANavMeshBoundsVolume* NavMeshBoundsVolume) {
    UE_LOG(LogTemp, Warning, TEXT("adding to pool: %s"), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolumePool->Add(NavMeshBoundsVolume);
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool() {
    auto NavMeshIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
    while(NavMeshIterator) {
        AddToPool(*NavMeshIterator);
        ++NavMeshIterator;
    }
}
