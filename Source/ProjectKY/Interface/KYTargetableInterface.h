// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KYTargetableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UKYTargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTKY_API IKYTargetableInterface
{
	GENERATED_BODY()

public:
	virtual void UpdateTargetedStatus(bool InStatus) = 0;
};
