// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_Guard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Guard : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Guard();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnGuardEventCallback(FGameplayEventData Payload);

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* GuardMontage;
	
};
