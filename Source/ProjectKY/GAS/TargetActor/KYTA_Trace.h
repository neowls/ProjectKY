// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "KYTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AKYTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	
	virtual void ConfirmTargetingAndContinue() override;

	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
