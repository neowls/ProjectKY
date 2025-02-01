// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYWidgetComponent.h"
#include "UI/KYUserWidget.h"

UKYWidgetComponent::UKYWidgetComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
}

void UKYWidgetComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UKYWidgetComponent::InitWidget()
{
	Super::InitWidget();
	

	UKYUserWidget* KYUserWidget = Cast<UKYUserWidget>(GetWidget());
	if (KYUserWidget)
	{
		KYUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}