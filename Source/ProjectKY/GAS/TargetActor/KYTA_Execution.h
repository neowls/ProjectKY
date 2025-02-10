// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "KYTA_Execution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYTA_Execution : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

	
public:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

private:
	AActor* FindExecutableTarget();
	
};
