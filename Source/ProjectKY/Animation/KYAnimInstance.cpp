// Fill out your copyright notice in the Description page of Project Settings.


#include "KYAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "Character/KYCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "ProjectKY.h"

UKYAnimInstance::UKYAnimInstance()
{
	bHasAcceleration = false;
	bHasVelocity = false;
	bIsSprint = false;
	bIsJumping = false;
	bIsFalling = false;
	bIsGliding = false;
	bIsOnGround = true;
	bIsFocus = false;
	bIsKnockDown = false;

	GroundSpeed = 0.f;
	
	FocusYaw = 0.f;
	TimeToJumpApex = 0.f;
	
	LocalVelocityDirectionAngle = 0.f;
	WorldVelocityDirectionAngle = 0.f;
	
	LocalAccelerationDirectionAngle = 0.f;
}


void UKYAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Owner = Cast<AKYCharacterBase>(TryGetPawnOwner());
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't cast Pawn to Character"));
	}
}

void UKYAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if(Owner == nullptr) return;
	UpdateCharacterMovement();
	UpdateCharacterState();
}

void UKYAnimInstance::UpdateCharacterMovement()
{
	const FRotator WorldRotation = Owner->GetActorRotation();
	
	WorldVelocity = Owner->GetVelocity();
	const FVector WorldVelocity2D = FVector(WorldVelocity.X, WorldVelocity.Y, 0.0f);

	WorldVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);
	
	LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(LocalVelocity2D, WorldRotation);
	GroundSpeed = WorldVelocity2D.Size();
	bHasVelocity = !UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::VSizeXYSquared(LocalVelocity2D), 0.0f);

	const FVector WorldAcceleration2D = Owner->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
	LocalAcceleration2D = WorldRotation.UnrotateVector(WorldAcceleration2D);
	LocalAccelerationDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldAcceleration2D, WorldRotation);
	bHasAcceleration = !UKismetMathLibrary::NearlyEqual_FloatFloat(LocalAcceleration2D.SizeSquared2D(), 0.0);

	
}

void UKYAnimInstance::UpdateCharacterState()
{
	UCharacterMovementComponent* CharacterMovementComponent = Owner->GetCharacterMovement();
	
	bIsOnGround = CharacterMovementComponent->IsMovingOnGround();
	bIsKnockDown = Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISKNOCKDOWN);
	bIsFalling = CharacterMovementComponent->IsFalling();
	bIsJumping = bIsFalling && WorldVelocity.Z > 0.f;
	bIsGliding = Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISGLIDING);
	TimeToJumpApex = bIsJumping ? -WorldVelocity.Z / CharacterMovementComponent->GetGravityZ() : 0.f;

	bIsFocus = Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISFOCUS);
	//bIsSprint = Get Sprint Logic
}
