// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Attack : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
	void AttackHitCheckCallback(FGameplayEventData Payload);

	UPROPERTY(EditAnywhere)
	FGameplayTag AttackType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> TargetHitEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere)
	float AttackSpeed;
};
