// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/GASCharacterSkillAttributeSet.h"

UGASCharacterSkillAttributeSet::UGASCharacterSkillAttributeSet() :
	SkillRange(400.0f),
	MaxSkillRange(800.f),
	SkillAttackRate(100.0f),
	MaxSkillAttackRate(300.0f),
	SkillEnergy(100.0f),
	MaxSkillEnergy(100.0f)
{
	
}

void UGASCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetSkillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if(Attribute == GetSkillAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillAttackRate());
	}
}
