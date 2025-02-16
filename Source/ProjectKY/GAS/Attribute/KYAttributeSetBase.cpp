// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "ProjectKY.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "Library/KYBlueprintFunctionLibrary.h"


UKYAttributeSetBase::UKYAttributeSetBase()  : 
	Health(200.0f),
	MaxHealth(200.0f),
	Shield(0.0f),
	MaxShield(100.0f),
	MoveSpeed(300.0f),
	MaxMoveSpeed(1000.0f),
	AttackSpeed(1.0f),
	MaxAttackSpeed(2.5f),
	StrikingPower(20.0f),
	MaxStrikingPower(1000.0f),
	DefensivePower(10.0f),
	MaxDefensivePower(1000.0f),
	Level(1.0f),
	MaxLevel(99.0f)
{
	
}

void UKYAttributeSetBase::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UKYAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributeOnChange(Attribute, NewValue);
}

void UKYAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if(Data.EvaluatedData.Attribute == GetInDamageAttribute())
	{
		float InDamageDone = GetInDamage();
		SetInDamage(0.0f);	// 메타 어트리뷰트 처리 
		if (InDamageDone > 0.0f)
		{
			
			bool IsFrontAttack = UKYBlueprintFunctionLibrary::GetAngleToTarget(GetActorInfo()->AvatarActor->GetActorLocation(), GetActorInfo()->AvatarActor->GetActorForwardVector(), Data.EffectSpec.GetEffectContext().GetEffectCauser()->GetActorLocation()) < 90.0f;
			
			bool IsParryState = GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISPARRY) && IsFrontAttack;
			bool IsGuardState = GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISGUARD) && IsFrontAttack;
			
			if (OnDamageTaken.IsBound())	// 만약 델리게이트 바인딩 된 게 있다면 실행
			{
				const FGameplayEffectContextHandle& EffectContextHandle = Data.EffectSpec.GetEffectContext();
				AActor* Instigator = EffectContextHandle.GetInstigator();
				AActor* Causer = EffectContextHandle.GetEffectCauser();
				

				float DamageMagnitude = IsParryState || IsGuardState ? -1.0f : Data.EvaluatedData.Magnitude;
				
				OnDamageTaken.Broadcast(Instigator, Causer, Data.EffectSpec.CapturedSourceTags.GetSpecTags(), DamageMagnitude);
			}

			if (IsParryState)
			{
				return;
			}

			if (IsGuardState)
			{
				InDamageDone *= 0.2f;
			}

			if (GetDefensivePower() > 0.0f)
			{
				InDamageDone = FMath::Clamp(InDamageDone - GetDefensivePower(), 0, InDamageDone);
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

void UKYAttributeSetBase::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMoveSpeed());
	}
	else if (Attribute == GetAttackSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAttackSpeed());
	}
	else if (Attribute == GetStrikingPowerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStrikingPower());
	}
	else if (Attribute == GetDefensivePowerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDefensivePower());
	}
}
