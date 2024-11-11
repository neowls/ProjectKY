// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/GASCharacterAttributeSet.h"
#include "GAS.h"
#include "GameplayEffectExtension.h"
#include "Tag/GASGameplayTag.h"

UGASCharacterAttributeSet::UGASCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.0f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Damage(0.0f)
	
{
	InitHealth(GetMaxHealth());
}

void UGASCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if(Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
		
	}
}

bool UGASCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if(!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if(Data.EvaluatedData.Magnitude > 0.0f)
		{
			if(Data.Target.HasMatchingGameplayTag(GASTAG_CHARACTER_INVINCIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}
	return true;
}

void UGASCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const float MinimumHealth = 0.0f;
	
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		GAS_LOG(LogGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		GAS_LOG(LogGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(GASTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}

/*
void UGASCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if(Attribute == GetHealthAttribute())
	{
		GAS_LOG(LogGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}
*/
