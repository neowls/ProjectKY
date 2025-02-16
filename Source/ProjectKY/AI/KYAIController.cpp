// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/KYAIController.h"
#include "KYAI.h"
#include "ProjectKY.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AKYAIController::AKYAIController()
{
	InitializeObjectFinder(BBAsset, TEXT("/Game/_Dev/AI/BB_Enemy.BB_Enemy"));
	InitializeObjectFinder(BTAsset, TEXT("/Game/_Dev/AI/BT_Enemy.BT_Enemy"));
	bIsAutoTarget = true;
}

void AKYAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void AKYAIController::OnUnPossess()
{
	Super::OnUnPossess();
	StopAI();
}

void AKYAIController::RunAI()
{
	UBlackboardComponent* BBComponent = Blackboard.Get();
	
	if(UseBlackboard(BBAsset, BBComponent))
	{
		
		if (bIsAutoTarget)
		{
			Blackboard->SetValueAsObject(BBKEY_TARGET, GetWorld()->GetFirstPlayerController()->GetPawn());
		}
		
		bool RunResult = RunBehaviorTree(BTAsset);
		
		ensure(RunResult);
	}
}

void AKYAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent.Get());
	if(BTComponent)
	{
		BTComponent->StopTree();
	}
	
}

void AKYAIController::SetHitStatus(bool bIsHit)
{
	UBlackboardComponent* BBComponent = Blackboard.Get();
	if(UseBlackboard(BBAsset, BBComponent))
	{
		Blackboard->SetValueAsBool(BBKEY_ISHIT, bIsHit);
	}
}
