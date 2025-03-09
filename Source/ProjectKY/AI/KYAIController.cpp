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

	    // 로드 확인
    if (!BBAsset)
    {
        UE_LOG(LogKY, Error, TEXT("Failed to load BB asset in constructor"));
    }
    
    if (!BTAsset)
    {
        UE_LOG(LogKY, Error, TEXT("Failed to load BT asset in constructor"));
    }
	
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	bIsAutoTarget = true;
}

void AKYAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// 지연된 초기화 사용
	FTimerHandle InitTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		InitTimerHandle, 
		this, 
		&AKYAIController::DelayedInitAI, 
		0.1f, 
		false
	);
	
	StopAI();
}

void AKYAIController::DelayedInitAI()
{
	// 에셋 확인 및 로드
	if (!BBAsset || !BTAsset)
	{
		BBAsset = LoadObject<UBlackboardData>(nullptr, TEXT("/Game/_Dev/AI/BB_Enemy.BB_Enemy"));
		BTAsset = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/_Dev/AI/BT_Enemy.BT_Enemy"));
        
		KY_LOG(LogKY, Warning, TEXT("Delayed loading - BB: %s, BT: %s"), 
			BBAsset ? TEXT("Success") : TEXT("Failed"),
			BTAsset ? TEXT("Success") : TEXT("Failed"));
	}
    
	InitAI();
}

void AKYAIController::OnUnPossess()
{
	Super::OnUnPossess();
	StopAI();
}

void AKYAIController::InitAI()
{
	KY_LOG(LogKY, Log, TEXT("Init AI"));
	
	// Blackboard 할당
	if (!Blackboard.Get())
	{
		Blackboard = BlackboardComponent;
	}
	
	UBlackboardComponent* BBComponent = Blackboard.Get();
	if (!BBComponent)
	{
		KY_LOG(LogKY, Warning, TEXT("Blackboard is Null"));
		return;
	}
	
	if(UseBlackboard(BBAsset, BBComponent))
	{
		bool RunResult = RunBehaviorTree(BTAsset);

		if (bIsAutoTarget)
		{
			if (GetWorld()->GetFirstPlayerController() && GetWorld()->GetFirstPlayerController()->GetPawn())
			{
				Blackboard->SetValueAsObject(BBKEY_TARGET, GetWorld()->GetFirstPlayerController()->GetPawn());
			}
			else
			{
				KY_LOG(LogKY, Warning, TEXT("Player Pawn is NULL"));
			}
		}
		
		ensure(RunResult);
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Access Blackboard"));
		
	}
}

void AKYAIController::RunAI()
{
	KY_LOG(LogKY, Warning, TEXT("Run AI"));
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent.Get());
	if (BTComponent)
	{
		KY_LOG(LogKY, Warning, TEXT("Run AI"));
		BTComponent->StartTree(*BTAsset);
	}
}


void AKYAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent.Get());
	if(BTComponent)
	{
		KY_LOG(LogKY, Warning, TEXT("Stop AI"));
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
