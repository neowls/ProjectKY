// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "KYAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDamageTakenEvent, AActor*, DamageInstigator, AActor*, DamageCauser, const FGameplayTagContainer&, GameplayTagContainer, float, DamageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);


UCLASS()
class PROJECTKY_API UKYAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UKYAttributeSetBase();

	
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, Health);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxHealth);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, Shield);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxShield);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxMoveSpeed);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, AttackSpeed);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxAttackSpeed);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, StrikingPower);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxStrikingPower);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, DefensivePower);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxDefensivePower);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, Level);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, MaxLevel);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetBase, InDamage);
	
	mutable FDamageTakenEvent OnDamageTaken;
	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxHealth;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Shield;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxShield;

	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MoveSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxMoveSpeed;

	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData AttackSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxAttackSpeed;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData StrikingPower;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxStrikingPower;


	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DefensivePower;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxDefensivePower;

	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Level;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxLevel;
	

	UPROPERTY(BlueprintReadOnly, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData InDamage;

	bool bOutOfHealth = false; 

protected:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const;
};
