// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if(IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
