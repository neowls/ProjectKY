// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KYCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UKYCharacterMovementComponent();
	
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
};
