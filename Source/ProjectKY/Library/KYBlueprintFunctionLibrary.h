// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KYBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Math|Custom")
	static float GetAngleFromTarget(const FVector& OriginLocation, const FVector& OriginForwardVector, const FVector& TargetLocation);
	
	UFUNCTION(BlueprintPure, Category = "Math|Custom")
	static float GetAngleFromDirection(const FVector& TargetDirection, const FVector& OriginDirectionVector);

	UFUNCTION(BlueprintPure, Category = "Math|Custom")
	static float GetRadianFromDirection(const FVector& TargetDirection, const FVector& OriginDirectionVector);
	
	UFUNCTION(BlueprintPure, Category = "Math|Custom")
	static FRotator GetRotationToTarget(const FVector& OriginLocation, const FVector& TargetLocation);
};
