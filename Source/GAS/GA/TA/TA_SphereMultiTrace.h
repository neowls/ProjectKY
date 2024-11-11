// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/TA_Trace.h"
#include "TA_SphereMultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API ATA_SphereMultiTrace : public ATA_Trace
{
	GENERATED_BODY()

	
protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
