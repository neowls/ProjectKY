// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerState.h"

#include "AbilitySystemComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"

AKYPlayerState::AKYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetHealth = CreateDefaultSubobject<UKYAttributeSetHealth>(TEXT("AttributeSetHealth"));
}

class UAbilitySystemComponent* AKYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
