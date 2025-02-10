// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KYBlueprintFunctionLibrary.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Math|Custom")
	static float GetAngleToTarget(const FVector& OriginLocation, const FVector& OriginForwardVector, const FVector& TargetLocation);

	UFUNCTION(BlueprintPure, Category = "Math|Custom")
	static FRotator GetRotationToTarget(const FVector& OriginLocation, const FVector& TargetLocation);
};
