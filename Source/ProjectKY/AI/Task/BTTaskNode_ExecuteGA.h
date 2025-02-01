// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AbilitySystemComponent.h"
#include "BTTaskNode_ExecuteGA.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTTaskNode_ExecuteGA : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_ExecuteGA();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 실행할 어빌리티 클래스
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	TSubclassOf<UGameplayAbility> AbilityToActivate;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	float AttackDistanceOffset = 30.0f;
};
