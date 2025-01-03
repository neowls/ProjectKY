// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_FindPatrolPos.h"
#include "NavigationSystem.h"
#include "AI/KYAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTaskNode_FindPatrolPos::UBTTaskNode_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTaskNode_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControlledPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlledPawn->GetWorld());

	if(NavSystem == nullptr) return EBTNodeResult::Failed;

	

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AKYAIController::HomePosKey);
	FNavLocation NextPatrol;

	if(NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AKYAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

