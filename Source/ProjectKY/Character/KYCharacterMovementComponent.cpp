// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "KYCharacterBase.h"
#include "ProjectKY.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYCharacterMovementComponent::UKYCharacterMovementComponent()
{
	GeneralGravityScale = 2.0f;
	MaxFallingSpeed = 60.0f;
	bNotifyApex = true;
}

void UKYCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	AKYCharacterBase* CharacterBase = Cast<AKYCharacterBase>(GetOwner());
	
	if(CharacterBase)
	{
		if (IsFalling() && !CharacterBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsFalling))
		{
			CharacterBase->GetAbilitySystemComponent()->AddLooseGameplayTag(UKYGameplayTags::CharacterState.IsFalling);
		}
		
		else if (!IsFalling() && CharacterBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsFalling))
		{
			CharacterBase->GetAbilitySystemComponent()->RemoveLooseGameplayTag(UKYGameplayTags::CharacterState.IsFalling);
		}
	}
	// 점프 상태 로깅
	LOG_SCREEN(1, TEXT("JumpCurrentCount: %d, JumpMaxCount: %d, CanJump: %s"),
		CharacterBase->JumpCurrentCount,
		CharacterBase->JumpMaxCount,
		CharacterBase->CanJump() ? TEXT("true") : TEXT("false"));

	LOG_SCREEN(2, TEXT("Movement - Mode: %d, IsFalling: %s, Velocity.Z: %f"),
		(int32)MovementMode,
		IsFalling() ? TEXT("true") : TEXT("false"),
		Velocity.Z);
}
