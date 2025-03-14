// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "KYAttributeSetPlayer.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelUpDelegate);

UCLASS()
class PROJECTKY_API UKYAttributeSetPlayer : public UKYAttributeSetBase
{
	GENERATED_BODY()

public:
	UKYAttributeSetPlayer();

	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, Rage);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, MaxRage);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, InRage);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, SkillPoint);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, MaxSkillPoint);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, Gold);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, MaxGold);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, Experience);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, MaxExperience);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetPlayer, InExperience);
	

	mutable FLevelUpDelegate OnLevelUp;

	UPROPERTY()
	mutable UCurveTable* PlayerLevelCurveTable;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Rage;

	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxRage;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData InRage;

	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData SkillPoint;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxSkillPoint;

	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Gold;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxGold;


	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Experience;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxExperience;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData InExperience;

protected:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual float GetAttributeValueFromCurveTable(FName CurveName, float InValue, float InLevel);
};
