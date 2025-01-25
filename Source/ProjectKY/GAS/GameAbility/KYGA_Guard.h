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

protected:

	void OnSimpleEventReceivedCallback(FGameplayEventData Payload) override;
};
