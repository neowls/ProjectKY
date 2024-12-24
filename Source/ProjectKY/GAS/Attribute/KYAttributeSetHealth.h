// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "KYAttributeSetHealth.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FDamageTakenEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCause*/, const FGameplayTagContainer& /*GameplayTagContainer*/, float /*Damage*/)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

UCLASS()
class PROJECTKY_API UKYAttributeSetHealth : public UKYAttributeSetBase
{
	GENERATED_BODY()

public:
	UKYAttributeSetHealth();

	ATTRIBUTE_ACCESSORS(UKYAttributeSetHealth, Health);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetHealth, MaxHealth);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetHealth, Shield);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetHealth, MaxShield);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetHealth, InDamage);
	
	mutable FDamageTakenEvent OnDamageTaken;
	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Health;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxHealth;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Shield;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxShield;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData InDamage;

	bool bOutOfHealth = false; 

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
