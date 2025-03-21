// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KYAnimInstance.generated.h"

/**
 * 
 */

class AKYCharacterBase;

UCLASS()
class PROJECTKY_API UKYAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKYAnimInstance();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateCharacterMovement();
	virtual void UpdateCharacterState();
	

	TObjectPtr<AKYCharacterBase> Owner;

	//	Velocity Property
	UPROPERTY(BlueprintReadOnly, Category = Velocity)
	FVector WorldVelocity;

	UPROPERTY(BlueprintReadOnly, Category = Velocity)
	FVector LocalVelocity2D;

	UPROPERTY(BlueprintReadOnly, Category = Velocity)
	uint8 bHasVelocity : 1;

	UPROPERTY(BlueprintReadOnly, Category = Velocity)
	float WorldVelocityDirectionAngle;
	
	UPROPERTY(BlueprintReadOnly, Category = Velocity)
	float LocalVelocityDirectionAngle;

	UPROPERTY(BlueprintReadOnly, Category = Velocity)
	float GroundSpeed;
	
	//	Acceleration Data
	UPROPERTY(BlueprintReadOnly, Category = Acceleration)
	FVector LocalAcceleration2D;

	UPROPERTY(BlueprintReadOnly, Category = Acceleration)
	float LocalAccelerationDirectionAngle;

	UPROPERTY(BlueprintReadOnly, Category = Acceleration)
	uint8 bHasAcceleration : 1;


	
	//	Focus Property
	UPROPERTY(BlueprintReadOnly, Category = Focus)
	float FocusYaw;

	UPROPERTY(BlueprintReadOnly, Category = Focus)
	uint8 bIsFocus : 1;

	//	State Property
	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsSprint : 1;

	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsJumping : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsFalling : 1;

	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsGliding : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsOnGround : 1;

	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsKnockDown : 1;

	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsCombat : 1;

	UPROPERTY(BlueprintReadOnly, Category = State)
	uint8 bIsDoubleJump : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = State)
	float TimeToJumpApex;

	
	
	
};

