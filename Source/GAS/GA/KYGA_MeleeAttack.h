// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGameplayAbility.h"
#include "KYGA_MeleeAttack.generated.h"


/**
 * 
 */
UCLASS()
class GAS_API UKYGA_MeleeAttack : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_MeleeAttack();

	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	FName GetNextSection();

	void StartComboTimer();
	
	void CheckComboInput();

	void StartNextCombo();

	void CloseComboEffectiveTime();
	
protected:
	UPROPERTY()
	TObjectPtr<class UGASComboActionData> CurrentComboData;
	
	uint8 CurrentCombo = 0;
	FTimerHandle ComboEffectiveTimerHandle;
	FTimerHandle ComboAvailableTimerHandle;
	bool HasNextComboInput = false;
	bool IsComboAvailable = false;
	
};

