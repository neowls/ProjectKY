// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_IsInAttackRange.h"

#include "AI/KYAI.h"
#include "GameFramework/Character.h"
#include "AI/KYAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControlledPawn == nullptr) return false;

	auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if(Target == nullptr) return false;

	bResult = (Target->GetDistanceTo(ControlledPawn) <= 200.0f);
	return bResult;
}
