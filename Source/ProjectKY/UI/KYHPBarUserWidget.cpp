// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYHPBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"

void UKYHPBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetHealth::GetHealthAttribute()).AddUObject(this, &UKYHPBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetHealth::GetMaxHealthAttribute()).AddUObject(this, &UKYHPBarUserWidget::OnMaxHealthChanged);

		const UKYAttributeSetHealth* CurrentAttributeSetHealth = ASC->GetSet<UKYAttributeSetHealth>();
		if (CurrentAttributeSetHealth)
		{
			CurrentHealth = CurrentAttributeSetHealth->GetHealth();
			CurrentMaxHealth = CurrentAttributeSetHealth->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHPBar();
			}
		}
	}
}

void UKYHPBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	if (CurrentHealth < KINDA_SMALL_NUMBER)
	{
		HPProgressBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	UpdateHPBar();
}

void UKYHPBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UKYHPBarUserWidget::UpdateHPBar()
{
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
}