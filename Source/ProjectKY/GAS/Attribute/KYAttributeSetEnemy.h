// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "KYAttributeSetEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYAttributeSetEnemy : public UKYAttributeSetBase
{
	GENERATED_BODY()

public:
	UKYAttributeSetEnemy();

	ATTRIBUTE_ACCESSORS(UKYAttributeSetEnemy, DropGold);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetEnemy, MaxDropGold);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetEnemy, DropExperience)
	ATTRIBUTE_ACCESSORS(UKYAttributeSetEnemy, MaxDropExperience)

protected:
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DropGold;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxDropGold;

	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DropExperience;
	
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxDropExperience;


protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
};
