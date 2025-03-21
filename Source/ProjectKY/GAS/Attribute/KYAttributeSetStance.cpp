// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetStance.h"
#include "GameplayEffectExtension.h"
#include "ProjectKY.h"

UKYAttributeSetStance::UKYAttributeSetStance() :
	Stance(0.f),
	MaxStance(300.0f),
	MinStaggerStance(50.0f),
	MaxStaggerStance(200.0f)
{
}

void UKYAttributeSetStance::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStance());
	}
}

void UKYAttributeSetStance::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetIncomingStanceAttribute())
	{
		float IncomingStanceDone = GetIncomingStance();
		SetIncomingStance(0.0f);
		if(IncomingStanceDone > 0.0f)
		{
			const float NewStance = GetStance() + IncomingStanceDone;
			SetStance(NewStance);
			if (OnStanceChange.IsBound())
			{
				const FGameplayEffectContextHandle& EffectContextHandle = Data.EffectSpec.GetEffectContext();
				AActor* Causer = EffectContextHandle.GetEffectCauser();

				
				OnStanceChange.Broadcast(Causer, Data.EffectSpec.CapturedSourceTags.GetSpecTags(), GetCurrentStanceState(IncomingStanceDone));
			}
		}
	}
}

uint8 UKYAttributeSetStance::GetCurrentStanceState(float IncomingStanceValue) const
{
	return GetStance() == GetMaxStance() ? 2 : GetStance() >= GetMaxStaggerStance() ? 1 : IncomingStanceValue >= GetMinStaggerStance() ? 1 : 0;
}
