// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/KYAttributeSetBase.h"


UKYAttributeSetBase::UKYAttributeSetBase()
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

void UKYAttributeSetBase::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	
}
