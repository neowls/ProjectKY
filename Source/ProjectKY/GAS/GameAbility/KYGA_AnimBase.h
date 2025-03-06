// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "Struct/KYStruct.h"
#include "KYGA_AnimBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_AnimBase : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_AnimBase();
	
	UFUNCTION()
	virtual void OnAnimSetChangeCallback(); 

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
	UFUNCTION(BlueprintNativeEvent)
	void OnSimpleEventReceivedCallback(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void OnSimpleCompleteEventCallback(FGameplayEventData Payload);

	UFUNCTION()
	virtual void OnSimpleInterruptEventCallback(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void PlayAnimMontageTask();
	
	UPROPERTY()
	FEventAnimMontageData AnimMontageData;

	UPROPERTY()
	FGameplayTag DataTag;
	
};


