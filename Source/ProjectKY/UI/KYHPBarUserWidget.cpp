// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYHPBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "GAS/Attribute/KYAttributeSetBase.h"

void UKYHPBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (ASC)
	{
		if (OnHealthChangedDelegateHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetBase::GetHealthAttribute()).Remove(OnHealthChangedDelegateHandle);
		}
		if (OnMaxHealthChangedDelegateHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetBase::GetMaxHealthAttribute()).Remove(OnMaxHealthChangedDelegateHandle);
		}
	}

	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		OnHealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetBase::GetHealthAttribute()).AddUObject(this, &UKYHPBarUserWidget::OnHealthChanged);
		OnMaxHealthChangedDelegateHandle = ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetBase::GetMaxHealthAttribute()).AddUObject(this, &UKYHPBarUserWidget::OnMaxHealthChanged);

		const UKYAttributeSetBase* CurrentAttributeSetBase = ASC->GetSet<UKYAttributeSetBase>();
		if (CurrentAttributeSetBase)
		{
			CurrentHealth = CurrentAttributeSetBase->GetHealth();
			CurrentMaxHealth = CurrentAttributeSetBase->GetMaxHealth();

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