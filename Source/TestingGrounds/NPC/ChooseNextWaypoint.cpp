// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) {
    // Get patrol points
	const auto AIController = OwnerComp.GetAIOwner();
	const auto ControlledPawn = AIController->GetPawn();
    auto PatrolComponent = ControlledPawn->FindComponentByClass<UPatrolComponent>();

    if (!ensure(PatrolComponent)) { return EBTNodeResult::Failed; }

    auto PatrolPoints = PatrolComponent->GetPatrolPoints();

    if (PatrolPoints.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("Guard is missing patrol points"));
        return EBTNodeResult::Failed;
    }

    // Set next waypoint
    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	const auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
    BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

    // Cycle to next waypoint
	const auto NextIndex = (Index + 1) % PatrolPoints.Num();
    BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

    return EBTNodeResult::Succeeded;
}