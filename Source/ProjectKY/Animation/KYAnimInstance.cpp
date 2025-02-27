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
	bIsDoubleJump = false;
	bIsFalling = false;
	bIsGliding = false;
	bIsOnGround = true;
	bIsFocus = false;
	bIsKnockDown = false;
	bIsCombat = false;

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
	
	GroundSpeed = WorldVelocity2D.Size();
	bHasVelocity = WorldVelocity2D.SquaredLength() > KINDA_SMALL_NUMBER;
	
	WorldVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldVelocity2D, WorldRotation);
	LocalVelocity2D = WorldRotation.UnrotateVector(WorldVelocity2D);
	
	if(bHasVelocity)
	{
		LocalVelocityDirectionAngle = UKismetAnimationLibrary::CalculateDirection(LocalVelocity2D, WorldRotation);
	}
	
	const FVector WorldAcceleration2D = Owner->GetCharacterMovement()->GetCurrentAcceleration() * FVector(1.0f, 1.0f, 0.0f);
	LocalAcceleration2D = WorldRotation.UnrotateVector(WorldAcceleration2D);
	bHasAcceleration = LocalAcceleration2D.SquaredLength() > KINDA_SMALL_NUMBER;
	
	if(bHasAcceleration)
	{
		LocalAccelerationDirectionAngle = UKismetAnimationLibrary::CalculateDirection(WorldAcceleration2D, WorldRotation);
	}
}

void UKYAnimInstance::UpdateCharacterState()
{
	UCharacterMovementComponent* CharacterMovementComponent = Owner->GetCharacterMovement();
	UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();
	
	bIsOnGround = CharacterMovementComponent->IsMovingOnGround();
	bIsFalling = CharacterMovementComponent->IsFalling();
	
	bIsJumping = bIsFalling && WorldVelocity.Z > 0.f;
	bIsDoubleJump = Owner->JumpCurrentCount > 1 && bIsJumping;
	bIsCombat = Owner->GetIsCombat();
	TimeToJumpApex = bIsJumping ? -WorldVelocity.Z / CharacterMovementComponent->GetGravityZ() : 0.f;

	bIsFocus = ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsFocusing);
	bIsGliding = ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsGliding);
	bIsKnockDown = ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsKnockDown);
	//bIsSprint = Get Sprint Logic
}
