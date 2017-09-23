// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool() {
	Pool = TArray<AActor*>();
}

AActor* UActorPool::Checkout() {
	if (Pool.Num() == 0) {
		return nullptr;
	}
	return Pool.Pop();
}

void UActorPool::Return(AActor* ActorToReturn) {
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd) {
	Pool.Push(ActorToAdd);
}