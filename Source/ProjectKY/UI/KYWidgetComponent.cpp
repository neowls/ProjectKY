// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYWidgetComponent.h"
#include "UI/KYUserWidget.h"

void UKYWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UKYUserWidget* KYUserWidget = Cast<UKYUserWidget>(GetWidget());
	if (KYUserWidget)
	{
		KYUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
