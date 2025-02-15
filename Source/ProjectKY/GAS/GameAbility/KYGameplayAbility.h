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
	
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnSimpleCompleteCallback();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSimpleInterruptedCallback();
	

	UPROPERTY(EditDefaultsOnly, Category = "Activation")
	bool bPassiveAbility = false;

	
};
