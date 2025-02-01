// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "KYWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UKYWidgetComponent();

protected:
	virtual void InitWidget() override;

	virtual void InitializeComponent() override;
	
};
