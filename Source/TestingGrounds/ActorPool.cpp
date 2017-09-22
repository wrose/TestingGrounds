// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool() {
}

AActor* UActorPool::Checkout() {
   return nullptr;
}

void UActorPool::Return(AActor* ActorToReturn) {

}

void UActorPool::Add(AActor* ActorToAdd) {

}