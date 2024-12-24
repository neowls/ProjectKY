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

	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, Category="Custom Movement")
	float AirAttackHangGravityScale;

	UPROPERTY(EditAnywhere, Category="Custom Movement")
	float GeneralGravityScale;

	bool ClearVelocityZ = false;
};
