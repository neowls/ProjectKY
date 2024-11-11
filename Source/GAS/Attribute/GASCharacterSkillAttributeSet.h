// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASCharacterSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GAS_API UGASCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGASCharacterSkillAttributeSet();

	ATTRIBUTE_ACCESSORS(UGASCharacterSkillAttributeSet, SkillRange);
	ATTRIBUTE_ACCESSORS(UGASCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UGASCharacterSkillAttributeSet, SkillAttackRate);
	ATTRIBUTE_ACCESSORS(UGASCharacterSkillAttributeSet, MaxSkillAttackRate);
	ATTRIBUTE_ACCESSORS(UGASCharacterSkillAttributeSet, SkillEnergy);
	ATTRIBUTE_ACCESSORS(UGASCharacterSkillAttributeSet, MaxSkillEnergy);
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;		

protected:
	UPROPERTY(BlueprintReadOnly, Category="Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange;

	UPROPERTY(BlueprintReadOnly, Category="Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;

	UPROPERTY(BlueprintReadOnly, Category="Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category="Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category="Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy;

	UPROPERTY(BlueprintReadOnly, Category="Skill", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;
};
