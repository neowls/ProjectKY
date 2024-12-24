// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_MainAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_MainAttack : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_MainAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	FName GetNextSection();

	UFUNCTION()
	void StartNextCombo();

	UFUNCTION()
	void AttackHitCheckCallback(FGameplayEventData Payload);
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> GroundAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AirAttackMontage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> TargetHitEffect;

	uint8 CurrentCombo = 0;

	bool HasNextComboInput = false;
};
