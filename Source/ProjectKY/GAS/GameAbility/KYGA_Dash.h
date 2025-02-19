// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_AnimBase.h"
#include "KYGA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Dash : public UKYGA_AnimBase
{
	GENERATED_BODY()

public:
	UKYGA_Dash();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void OnSimpleInterruptedCallback_Implementation() override;

	virtual void OnSimpleCompleteCallback_Implementation() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	float DashSpeed;

private:
	void PrepareDash(ACharacter* Character);
	void RestoreDash(ACharacter* Character);
};
