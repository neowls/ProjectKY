// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "KYCharacterBase.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYCharacterMovementComponent::UKYCharacterMovementComponent()
{
	AirAttackHangGravityScale = 0.1f;
	GeneralGravityScale = 1.0f;
}

void UKYCharacterMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	AKYCharacterBase* CharacterBase = Cast<AKYCharacterBase>(GetOwner());
	
	if(CharacterBase)
	{
		if (CharacterBase->GetAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISGLIDING))
		{
			GravityScale = AirAttackHangGravityScale;
		}
		else
		{
			GravityScale = GeneralGravityScale;
		}
	}
}

