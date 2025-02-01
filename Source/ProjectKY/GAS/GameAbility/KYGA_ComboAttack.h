// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_Attack.h"
#include "KYGA_ComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_ComboAttack : public UKYGA_Attack
{
	GENERATED_BODY()

public:
	UKYGA_ComboAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void InputCallback();

	void InputCallback_Implementation();


private:
	void StartNextCombo();
	
	FName GetNextSection();
	
	uint8 MaxCombo = 1;
	
	bool HasNextComboInput = false;
};
