// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_Damage.generated.h"


class UKYAT_DamageReaction;
class AKYCharacterBase;

UCLASS()
class PROJECTKY_API UKYGA_Damage : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Damage();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void HitEventCallBack(FGameplayEventData Payload);

	UFUNCTION()
	void HitReaction(const ACharacter* InCharacter, const FGameplayTag& InReactionTag) const;
	
	UFUNCTION()
	void OnHitMontageCallback();

	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
	
	UFUNCTION()
	void ApplyHitReactionMomentum(FGameplayTag& InHitTag,AKYCharacterBase* Character, const FVector& InstigatorLocation);
	3
	UPROPERTY(EditAnywhere)
	float MomentumStrength;
	
};

