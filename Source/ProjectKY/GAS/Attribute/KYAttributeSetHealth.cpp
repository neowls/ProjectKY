// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "GameplayEffectExtension.h"
#include "ProjectKY.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYAttributeSetHealth::UKYAttributeSetHealth() : 
	Health(200.0f),
	MaxHealth(200.0f),
	Shield(0.0f),
	MaxShield(100.0f)
{
	
}

void UKYAttributeSetHealth::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
}

void UKYAttributeSetHealth::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if(Data.EvaluatedData.Attribute == GetInDamageAttribute())
	{
		float InDamageDone = GetInDamage();
		SetInDamage(0.0f);	// 메타 어트리뷰트 처리 
		if (InDamageDone > 0.0f)
		{
			if (OnDamageTaken.IsBound())	// 만약 델리게이트 바인딩 된 게 있다면 실행
			{
				const FGameplayEffectContextHandle& EffectContextHandle = Data.EffectSpec.GetEffectContext();
				AActor* Instigator = EffectContextHandle.GetInstigator();
				AActor* Causer = EffectContextHandle.GetEffectCauser();
				
				OnDamageTaken.Broadcast(Instigator, Causer, Data.EffectSpec.CapturedSourceTags.GetSpecTags(), Data.EvaluatedData.Magnitude);
			}

			if (GetShield() > 0.0f)		// 방어막이 있다면 데미지 만큼 방어막 차감
			{
				const float NewShield = GetShield() - InDamageDone;
				InDamageDone -= GetShield();
				SetShield(FMath::Clamp(NewShield, 0.0f, GetMaxShield()));
			}

			if (InDamageDone > 0.0f && GetHealth() > 0.0f)		// 남은 데미지가 있다면 그만큼 체력 차감
			{
				const float NewHealth = GetHealth() - InDamageDone;
				SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			}

			if (GetHealth() <= 0.0f && !bOutOfHealth)		// 현재 사망 처리가 되었는지 확인, 이로 인해 죽은뒤 들어온 데미지로 인한 델리게이트 브로드 캐스팅 방지
			{
				OnOutOfHealth.Broadcast();
			}

			bOutOfHealth = (GetHealth() <= 0.0f); 
		}
	}
}
