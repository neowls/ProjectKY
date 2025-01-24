// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "KYHUDUserWidget.generated.h"

class UKYHPBarUserWidget;
/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYHUDUserWidget : public UKYUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UKYHPBarUserWidget> WBPHPBar;
};
