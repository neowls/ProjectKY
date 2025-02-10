// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_CheckDistance.h"

#include "AIController.h"
#include "ProjectKY.h"
#include "AI/KYAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("Check Distance");
}

void UBTDecorator_CheckDistance::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
	
	CachedAIPawn = OwnerComp.GetAIOwner()->GetPawn();
}


bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (BlackboardComp == nullptr) return false;
	if (CachedAIPawn == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("Cached AI Pawn Is Invalid."))
		return false;
	}
	
	if (BlackboardComp->IsVectorValueSet(BlackboardKey.SelectedKeyName))
	{
		FVector TargetLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
		float Distance = FVector::Dist(CachedAIPawn->GetActorLocation(), TargetLocation);
		return Distance <= DistanceOffset;
	}
	
	if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName)))
	{
		float Distance = FVector::Dist(CachedAIPawn->GetActorLocation(), TargetActor->GetActorLocation());
		return Distance <= DistanceOffset;
	}



	KY_LOG(LogKY, Warning, TEXT("Selected Blackboard Key Is Invalid."))
	return false;
}