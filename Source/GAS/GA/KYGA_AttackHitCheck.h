// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGameplayAbility.h"
#include "KYGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UKYGA_AttackHitCheck : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTargetCompleteCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	

	UPROPERTY(EditAnywhere, Category="GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category="GAS")
	TSubclassOf<class UGameplayEffect> AttackBuffEffect;

	float CurrentLevel = 0;

	UPROPERTY(EditAnywhere, Category="GAS")
	TSubclassOf<class AKYTA_Trace> TargetActorClass;

};
