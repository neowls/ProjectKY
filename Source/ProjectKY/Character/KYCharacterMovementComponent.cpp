// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "KYCharacterBase.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYCharacterMovementComponent::UKYCharacterMovementComponent()
{
	GeneralGravityScale = 2.0f;
	MaxFallingSpeed = 60.0f;
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
}
