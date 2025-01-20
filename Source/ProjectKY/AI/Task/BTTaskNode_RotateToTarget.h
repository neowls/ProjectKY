// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTTaskNode_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_RotateToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
