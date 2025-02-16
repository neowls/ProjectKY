// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetEnemy.h"

#include "GAS/Tag/KYGameplayTag.h"

UKYAttributeSetEnemy::UKYAttributeSetEnemy() :
	DropGold(10.0f),
	MaxDropGold(1000.0f),
	DropExperience(10.0f),
	MaxDropExperience(10000.0f)
{
	
}

void UKYAttributeSetEnemy::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributeOnChange(Attribute, NewValue);
	if (Attribute == GetDropGoldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDropGold());
	}
	else if (Attribute == GetDropExperienceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDropExperience());
	}
}

void UKYAttributeSetEnemy::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		float InHealthRatio = GetHealth() / GetMaxHealth();
		
		if (InHealthRatio <= 0.2f && !GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_EXECUTABLE))
		{
			GetOwningAbilitySystemComponent()->AddLooseGameplayTag(KYTAG_CHARACTER_EXECUTABLE);
		}
	}
}
