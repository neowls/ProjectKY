// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"

AKYPlayerState::AKYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetPlayer = CreateDefaultSubobject<UKYAttributeSetPlayer>(TEXT("AttributeSetPlayer"));
}


class UAbilitySystemComponent* AKYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
