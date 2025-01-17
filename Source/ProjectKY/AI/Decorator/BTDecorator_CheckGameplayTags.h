// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_CheckGameplayTagsOnActor.h"
#include "BTDecorator_CheckGameplayTags.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTDecorator_CheckGameplayTags : public UBTDecorator_CheckGameplayTagsOnActor
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
