// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "AI/KYAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "ProjectKY.h"
#include "AI/KYAI.h"
#include "GameFramework/Character.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
	
	CachedAIPawn = OwnerComp.GetAIOwner()->GetPawn();
}


void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (CachedAIPawn == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("Cached AI Pawn Is Invalid."))
		return;
	}
	
	UWorld* World = CachedAIPawn->GetWorld();
	FVector Center = CachedAIPawn->GetActorLocation();


	if(World == nullptr) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, CachedAIPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);
	if(bResult)
	{
		for(auto const& OverlapResult : OverlapResults)
		{
			ACharacter* Character = Cast<ACharacter>(OverlapResult.GetActor());
			if(Character && Character->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Character);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_BASEPOS, Character->GetActorLocation());

				if (bShowDebugInfo)
				{
					DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, CachedAIPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 0.2f);
				}
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	}
}
