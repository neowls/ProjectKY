// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KYGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UKYGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UKYGameplayAbility();


protected:
	UFUNCTION()
	virtual void OnCompleteCallback();

	UFUNCTION()
	virtual void OnInterruptedCallback();
};
