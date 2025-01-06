// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsDamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTDecorator_IsDamageTaken : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsDamageTaken();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
