// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_BPGABase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTTask_BPGABase : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	// 실행할 어빌리티 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability")
	TSubclassOf<class UGameplayAbility> AbilityToActivate;
	
};

