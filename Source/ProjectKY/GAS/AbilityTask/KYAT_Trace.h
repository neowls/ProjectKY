// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "KYAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYAT_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UKYAT_Trace();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace",	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UKYAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AKYTA_Trace> TargetActorClass);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnTraceComplete;


protected:
	UPROPERTY()
	TSubclassOf<class AKYTA_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AKYTA_Trace> SpawnedTargetACtor;
};