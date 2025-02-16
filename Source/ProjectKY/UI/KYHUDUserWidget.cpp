// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYHUDUserWidget.h"

#include "KYRageBarUserWidget.h"
#include "ProjectKY.h"
#include "UI/KYHPBarUserWidget.h"


void UKYHUDUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	KY_LOG(LogKY, Log, TEXT("Begin"));
	
	if (WBPHPBar)
	{
		WBPHPBar->SetAbilitySystemComponent(InOwner);
	}
	if (WBP_PlayerRageBar)
	{
		WBP_PlayerRageBar->SetAbilitySystemComponent(InOwner);
	}
}
