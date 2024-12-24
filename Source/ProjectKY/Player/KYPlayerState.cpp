// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerState.h"

#include "AbilitySystemComponent.h"

AKYPlayerState::AKYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	
}

class UAbilitySystemComponent* AKYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
