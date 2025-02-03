// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/KYBlueprintFunctionLibrary.h"

float UKYBlueprintFunctionLibrary::GetAngleToTarget(const FVector& OriginLocation, const FVector& OriginForwardVector, const FVector& TargetLocation)
{
	FVector Direction = (TargetLocation - OriginLocation).GetSafeNormal();
	
	float AngleRadians = FMath::Acos(OriginForwardVector.Dot(Direction));
	
	return FMath::RadiansToDegrees(AngleRadians);
}
