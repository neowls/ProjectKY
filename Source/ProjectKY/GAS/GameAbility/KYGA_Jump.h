// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_AnimBase.h"
#include "KYGA_Jump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Jump : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Jump();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);

	UFUNCTION()
	void OnApexCallback();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Jump", meta=(AllowPrivateAccess=true))
	int32 JumpMaxCount = 1;
};
