// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYHUDUserWidget.h"
#include "UI/KYHPBarUserWidget.h"


void UKYHUDUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	
	if (WBPHPBar)
	{
		WBPHPBar->SetAbilitySystemComponent(InOwner);
	}
}
