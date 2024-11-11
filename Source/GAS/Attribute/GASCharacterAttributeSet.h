// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GASCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

/**
 * 
 */
UCLASS()
class GAS_API UGASCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGASCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UGASCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;						// 어트리뷰트 변경 전에 호출
	// virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;	// 어트리뷰트 변경 후에 호출
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;							// 게임 플레이 이펙트 적용 전에 호출
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;						// 게임 플레이 이펙트 적용 후에 호출

	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category="Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, Category="Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	bool bOutOfHealth = false;
	
};
