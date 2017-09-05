// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrollingGuard.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) {
    // Get patrol points
    auto AIController = OwnerComp.GetAIOwner();
    auto ControlledPawn = AIController->GetPawn();
    auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn); // TODO fix later
    auto PatrolPoints = PatrollingGuard->PatrolPoints;

    if (PatrolPoints.Num() > 0) {
        // Set next waypoint
        auto BlackboardComp = OwnerComp.GetBlackboardComponent();
        auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
        BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

        // Cycle to next waypoint
        auto NextIndex = (Index + 1) % PatrolPoints.Num();
        BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

    }
//    UE_LOG(LogTemp, Warning, TEXT("Waypoint Index %i"), Index);
    return EBTNodeResult::Succeeded;
}