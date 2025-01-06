// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/KYAIController.h"

#include "KYAI.h"
#include "ProjectKY.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/KYCharacterNonPlayer.h"
#include "GAS/Tag/KYGameplayTag.h"


AKYAIController::AKYAIController()
{
	InitializeObjectFinder(BBAsset, TEXT("/Game/_Dev/AI/BB_Enemy.BB_Enemy"));
	InitializeObjectFinder(BTAsset, TEXT("/Game/_Dev/AI/BT_Enemy.BT_Enemy"));
}

void AKYAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void AKYAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
}

void AKYAIController::RunAI()
{
	UBlackboardComponent* BBComponent = Blackboard.Get();
	if(UseBlackboard(BBAsset, BBComponent))
	{
		Blackboard->SetValueAsVector(BBKEY_BASEPOS, GetPawn()->GetActorLocation());
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AKYAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent.Get());
	if (BTAsset)
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
