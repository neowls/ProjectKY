// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_IsDamageTaken.h"

UBTDecorator_IsDamageTaken::UBTDecorator_IsDamageTaken()
{
	
}

bool UBTDecorator_IsDamageTaken::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//GetAIOwner()
	
	return Result;
}
