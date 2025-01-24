// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_IsInAttackAngle.h"

#include "AIController.h"
#include "ProjectKY.h"
#include "AI/KYAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackAngle::UBTDecorator_IsInAttackAngle()
{
	NodeName = TEXT("Check Attack Angle");
	AttackAngle = 20.0f;
}

bool UBTDecorator_IsInAttackAngle::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	

	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	
	if(ControlledPawn == nullptr || Target == nullptr) return false;

	FVector PawnLocation = ControlledPawn->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector TargetDirection = TargetLocation - PawnLocation;
	TargetDirection.Normalize();

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ControlledPawn->GetActorForwardVector(), TargetDirection)));
	
	bResult = Angle < AttackAngle; 
	
	return bResult;
}
