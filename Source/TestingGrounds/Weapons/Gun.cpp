// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Weapons/BallProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonCharacter.h"

// Sets default values
AGun::AGun() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//    PrimaryActorTick.bCanEverTick = true;
    // Create a gun mesh component
    FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
    FP_Gun->bCastDynamicShadow = false;
    FP_Gun->CastShadow = false;

    FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));

    FP_MuzzleLocation->SetupAttachment(FP_Gun);
    FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

// Called when the game starts or when spawned
void AGun::BeginPlay() {
    Super::BeginPlay();
}

//void AGun::Tick(float DeltaSeconds) {
//    Super::Tick(DeltaSeconds);
//}

void AGun::OnFire() {
    // try and fire a projectile
    if (ProjectileClass != NULL) {

        // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
        const FRotator SpawnRotation =
                FP_MuzzleLocation->GetComponentRotation();
        const FVector SpawnLocation =
                FP_MuzzleLocation->GetComponentLocation();

        UWorld *const World = GetWorld();
        if (World != NULL) {
            if (!ensure(&FP_MuzzleLocation)) { return; }

            //Set Spawn Collision Handling Override
            FActorSpawnParameters ActorSpawnParams;
            ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

            // spawn the projectile at the muzzle
            World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
        } else {
            UE_LOG(LogTemp, Error, TEXT("No World Set"))
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT("No ProjectileClass Set"))
    }

    // try and play the sound if specified
    if (FireSound != NULL) {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    } else {
        UE_LOG(LogTemp, Error, TEXT("No FireSound Set"))
    }

    // try and play a firing animation if specified
    if (FireAnimation1P != NULL && AnimInstance1P != NULL) {
        AnimInstance1P->Montage_Play(FireAnimation1P, 1.f);
    }
    if (FireAnimation3P != NULL && AnimInstance3P != NULL) {
        AnimInstance3P->Montage_Play(FireAnimation3P, 1.f);
    }
}