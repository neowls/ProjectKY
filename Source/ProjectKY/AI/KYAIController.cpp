// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/KYAIController.h"

#include "ProjectKY.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AKYAIController::HomePosKey(TEXT("HomePos"));
const FName AKYAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AKYAIController::TargetKey(TEXT("Target"));

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
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		
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

