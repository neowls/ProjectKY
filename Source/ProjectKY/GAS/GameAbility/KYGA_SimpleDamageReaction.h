// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_AnimBase.h"
#include "KYGA_SimpleDamageReaction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_SimpleDamageReaction : public UKYGA_AnimBase
{
	GENERATED_BODY()

public:
	UKYGA_SimpleDamageReaction();

	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
