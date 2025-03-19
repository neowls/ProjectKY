// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

class UAbilitySystemComponent* UKYUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}

void UKYUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
		K2_SetAbilitySystemComponent(InOwner);
	}
}

void UKYUserWidget::SetAbilitySystemComponent(UAbilitySystemComponent& InASC)
{
	ASC = InASC;
}

void UKYUserWidget::K2_SetAbilitySystemComponent_Implementation(AActor* InOwner)
{
	ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
}
