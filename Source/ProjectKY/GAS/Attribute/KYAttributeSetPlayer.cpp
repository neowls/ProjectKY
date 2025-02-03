// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetPlayer.h"

#include "GameplayEffectExtension.h"
#include "ProjectKY.h"

UKYAttributeSetPlayer::UKYAttributeSetPlayer() :
	RagePoint(0.0f),
	MaxRagePoint(100.0f),
	SkillPoint(0.0f),
	MaxSkillPoint(99.0f),
	Gold(0.0f),
	MaxGold(999999.0f),
	Experience(0.0f),
	MaxExperience(200.0f)
{

}

void UKYAttributeSetPlayer::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetGoldAttribute())
	{
		float IncomeGold = Data.EvaluatedData.Attribute.GetNumericValue(this);
	}
}

void UKYAttributeSetPlayer::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributeOnChange(Attribute, NewValue);
	if (Attribute == GetRagePointAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxRagePoint());
	}
	else if (Attribute == GetSkillPointAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillPoint());
	}
	else if (Attribute == GetGoldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxGold());
	}
	else if (Attribute == GetExperienceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxExperience());
	}
}

