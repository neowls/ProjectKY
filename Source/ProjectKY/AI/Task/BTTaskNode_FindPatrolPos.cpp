// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_FindPatrolPos.h"
#include "NavigationSystem.h"
#include "ProjectKY.h"
#include "AI/KYAI.h"
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
	if(ControlledPawn == nullptr)
	{
		KY_LOG(LogKY, Log, TEXT("Can't find Controlled Pawn"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if(NavSystem == nullptr)
	{
		KY_LOG(LogKY, Log, TEXT("Can't find NavSystem"));
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_BASEPOS);
	
	FNavLocation NextPatrol;

	if(NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrol.Location);
		KY_LOG(LogKY, Log, TEXT("Patrol Pos : %s"), *NextPatrol.Location.ToString());
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

