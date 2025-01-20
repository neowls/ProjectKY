// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "GAS/Attribute/KYAttributeSetStance.h"

AKYPlayerState::AKYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetHealth = CreateDefaultSubobject<UKYAttributeSetHealth>(TEXT("AttributeSetHealth"));
	AttributeSetStance = CreateDefaultSubobject<UKYAttributeSetStance>(TEXT("AttributeSetStance"));
}

class UAbilitySystemComponent* AKYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
