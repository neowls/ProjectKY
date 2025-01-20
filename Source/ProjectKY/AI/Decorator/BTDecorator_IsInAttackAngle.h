// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackAngle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTDecorator_IsInAttackAngle : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInAttackAngle();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
	UPROPERTY(EditAnywhere)
	float AttackAngle;
	
};
