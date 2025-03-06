// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KYGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGameplayAbility();
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void TryActivatePassiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	bool GetIsInputAbility() const { return bInputAbility;}
	
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnSimpleCompleteCallback();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSimpleInterruptedCallback();
	

	UPROPERTY(EditDefaultsOnly, Category ="Activation")
	bool bPassiveAbility = false;

	UPROPERTY(EditDefaultsOnly, Category ="Activation")
	bool bInputAbility = false;

	UPROPERTY(EditDefaultsOnly, Category="Activation")
	bool bIsCombatAbility = false;
	
	UPROPERTY()
	FTimerHandle CombatTagTimerHandle;

private:
	void UpdateCombatStateTag();
	
};
