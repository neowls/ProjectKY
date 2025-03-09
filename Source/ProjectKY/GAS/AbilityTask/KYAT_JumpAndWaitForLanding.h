// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "KYAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForApexDelegate);

/**
 * 
 */

UCLASS()
class PROJECTKY_API UKYAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public:
	UKYAT_JumpAndWaitForLanding();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding",	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UKYAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForApexDelegate OnReachApex;

protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
	void OnReachApexCallback();
};
