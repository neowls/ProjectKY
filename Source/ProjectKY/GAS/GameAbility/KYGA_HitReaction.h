// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_HitReaction.generated.h"


class UKYAT_DamageReaction;
class AKYCharacterBase;

UCLASS()
class PROJECTKY_API UKYGA_HitReaction : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_HitReaction();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void HitEventCallBack(FGameplayEventData Payload);

	UFUNCTION()
	void ApplyHitReactionMomentum(ACharacter* InCharacter, const FGameplayTag& InReactionTag) const;

	UFUNCTION()
	void OnLandedCallback(const FHitResult& Result);
	
	UFUNCTION()
	void PlayStanceMontage(UAnimMontage* MontageToPlay);

	UPROPERTY(EditAnywhere)
	float MomentumStrength;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> ResetGameplayEffect;
	
};

