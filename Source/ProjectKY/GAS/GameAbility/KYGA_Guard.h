// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_AnimBase.h"
#include "KYGA_Guard.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Guard : public UKYGA_AnimBase
{
	GENERATED_BODY()

public:
	UKYGA_Guard();

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:

	virtual void OnSimpleEventReceivedCallback_Implementation(FGameplayEventData Payload) override;
};
