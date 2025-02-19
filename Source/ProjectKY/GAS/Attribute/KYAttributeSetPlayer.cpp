// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetPlayer.h"

#include "GameplayEffectExtension.h"
#include "ProjectKY.h"

UKYAttributeSetPlayer::UKYAttributeSetPlayer() :
	Rage(0.0f),
	MaxRage(100.0f),
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

	if (Data.EvaluatedData.Attribute == GetInRageAttribute())
	{
		float InRageDone = GetInRage();
		SetInRage(0.0f);
		if (InRageDone > 0.0f)
		{
			const float NewRage = InRageDone + GetRage();
			SetRage(NewRage);
		}
		else
		{
			const float NewRage = GetRage() - InRageDone;
			SetRage(FMath::Clamp(NewRage, 0.0f, GetMaxRage()));
		}
	}

	if (Data.EvaluatedData.Attribute == GetInExperienceAttribute())
	{
		float InExperienceDone = GetInExperience();
		float InLevel = GetLevel();
		float NextLevelMaxExperience = GetMaxExperience();
		float NewMaxHealth = GetMaxHealth();
		float NewStrikingPower = GetStrikingPower();
		float NewDefensivePower = GetDefensivePower();
		float NewMaxRage = GetMaxRage();
		float NewSkillPoint = GetSkillPoint();
		
		SetInExperience(0.0f);
		if (InExperienceDone > 0.0f)
		{
			InExperienceDone += GetExperience();
			const bool bLevelUp = InExperienceDone >= NextLevelMaxExperience;;
			
			while (InExperienceDone >= NextLevelMaxExperience)
			{
				InExperienceDone -= NextLevelMaxExperience;
				InLevel += 1;
				NewSkillPoint += 1;

				if (PlayerLevelCurveTable)
				{
					SetMaxExperience(GetAttributeValueFromCurveTable(TEXT("MaxExperience"), NextLevelMaxExperience, InLevel));
				}
			}

			KY_LOG(LogKY, Log, TEXT("Current Exp : %f, Max Exp : %f"), InExperienceDone, NextLevelMaxExperience);
			if (bLevelUp && PlayerLevelCurveTable)
			{
				SetMaxHealth(GetAttributeValueFromCurveTable(TEXT("MaxHealth"), NewMaxHealth, InLevel));
				SetStrikingPower(GetAttributeValueFromCurveTable(TEXT("StrikingPower"), NewStrikingPower, InLevel));
				SetDefensivePower(GetAttributeValueFromCurveTable(TEXT("DefensivePower"), NewDefensivePower, InLevel));
				SetMaxRage(GetAttributeValueFromCurveTable(TEXT("MaxRage"), NewMaxRage, InLevel));
				SetLevel(InLevel);
				SetSkillPoint(NewSkillPoint);

				OnLevelUp.Broadcast();
			}
			
			SetExperience(InExperienceDone);
		}
	}
}

void UKYAttributeSetPlayer::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributeOnChange(Attribute, NewValue);

	if (Attribute == GetSkillPointAttribute())
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

float UKYAttributeSetPlayer::GetAttributeValueFromCurveTable(FName CurveName, float InValue, float InLevel)
{
	static const FString ContextString(TEXT("Level Up"));
	if (FRealCurve* TargetCurve = PlayerLevelCurveTable->FindCurve(CurveName, ContextString, true))
	{
		return TargetCurve->Eval(InLevel);
	}
	return InValue;
}

