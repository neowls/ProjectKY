// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_Damage.generated.h"

class UKYDA_HitReaction;

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};


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
	void ApplyMomentum();

	UFUNCTION()
	EHitDirection GetHitDirection(const AActor* HitCauser);

	UPROPERTY(EditAnywhere)
	TMap<EHitDirection, TObjectPtr<UAnimMontage>> HitMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> KnockBackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AirKnockBackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage", meta=(AllowPrivateAccess=true))
	float KnockbackStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage", meta=(AllowPrivateAccess=true))
	float KnockbackDuration;
};
