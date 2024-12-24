// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Dash : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Dash();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AirDashMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> GroundDashMontage;
};
