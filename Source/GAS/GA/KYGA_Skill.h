// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGameplayAbility.h"
#include "KYGA_Skill.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UKYGA_Skill : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Skill();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY()
	TObjectPtr<class UAnimMontage> ActiveSkillActionMontage;
};
