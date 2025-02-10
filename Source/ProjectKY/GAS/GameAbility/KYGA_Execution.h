// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGameplayAbility.h"
#include "KYGA_Execution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Execution : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AKYTA_Execution> TargetingActorClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle);
	
};
