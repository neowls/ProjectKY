// Fill out your copyright notice in the Description page of Project Settings.


#include "GASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/GASCharacterAttributeSet.h"
#include "Attribute/GASCharacterSkillAttributeSet.h"

AGASPlayerState::AGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UGASCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UGASCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
