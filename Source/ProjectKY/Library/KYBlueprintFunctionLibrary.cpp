// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/KYBlueprintFunctionLibrary.h"


float UKYBlueprintFunctionLibrary::GetAngleFromTarget(const FVector& OriginLocation, const FVector& OriginForwardVector, const FVector& TargetLocation)
{
	FVector Direction = (TargetLocation - OriginLocation).GetSafeNormal();
	float AngleRadians = FMath::Acos(OriginForwardVector.Dot(Direction));
	
	return FMath::RadiansToDegrees(AngleRadians);
}

float UKYBlueprintFunctionLibrary::GetAngleFromDirection(const FVector& TargetDirection, const FVector& OriginDirectionVector)
{
	return FMath::RadiansToDegrees(GetRadianFromDirection(TargetDirection, OriginDirectionVector));
}

float UKYBlueprintFunctionLibrary::GetRadianFromDirection(const FVector& TargetDirection, const FVector& OriginDirectionVector)
{
	return FMath::Acos(OriginDirectionVector.Dot(TargetDirection));
}

FRotator UKYBlueprintFunctionLibrary::GetRotationToTarget(const FVector& OriginLocation, const FVector& TargetLocation)
{
	return (TargetLocation - OriginLocation).GetSafeNormal().Rotation();
}
