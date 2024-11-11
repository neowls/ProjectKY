// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/GASCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Tag/GASGameplayTag.h"

void UGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	 if(ASC)
	 {
		ASC->GetGameplayAttributeValueChangeDelegate(UGASCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UGASHpBarUserWidget::OnHealthChanged);
	 	ASC->GetGameplayAttributeValueChangeDelegate(UGASCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UGASHpBarUserWidget::OnMaxHealthChanged);
	 	ASC->RegisterGameplayTagEvent(GASTAG_CHARACTER_INVINCIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGASHpBarUserWidget::OnInvincibleTagChanged);
	 	const UGASCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UGASCharacterAttributeSet>();

	 	if(CurrentAttributeSet)
	 	{
	 		CurrentHealth = CurrentAttributeSet->GetHealth();
	 		CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

	 		if(CurrentMaxHealth > 0.0f)
	 		{
	 			UpdateHpBar();	 			
	 		}
	 	}
	 }
}

void UGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UGASHpBarUserWidget::OnInvincibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvincibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

void UGASHpBarUserWidget::UpdateHpBar()
{
	if(PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
}
