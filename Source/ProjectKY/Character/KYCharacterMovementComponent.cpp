// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "KYCharacterBase.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYCharacterMovementComponent::UKYCharacterMovementComponent()
{
	GeneralGravityScale = 2.0f;
	MaxFallingSpeed = 10.0f;
}

void UKYCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	AKYCharacterBase* CharacterBase = Cast<AKYCharacterBase>(GetOwner());
	
	if(CharacterBase)
	{
		if (IsFalling() && !CharacterBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISFALLING))
		{
			CharacterBase->GetAbilitySystemComponent()->AddLooseGameplayTag(KYTAG_CHARACTER_ISFALLING);
		}
		else if (!IsFalling() && CharacterBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISFALLING))
		{
			CharacterBase->GetAbilitySystemComponent()->RemoveLooseGameplayTag(KYTAG_CHARACTER_ISFALLING);
		}
		
		if (CharacterBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISGLIDING))
		{
			Velocity.Z = FMath::Clamp(Velocity.Z, -MaxFallingSpeed, 1200.0f);
		}
	}
}
