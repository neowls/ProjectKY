// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "GameplayEffectTypes.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateWidgetStyle.h"
#include "KYRageBarUserWidget.generated.h"

class USlateBrushAsset;
/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYRageBarUserWidget : public UKYUserWidget
{
	GENERATED_BODY()
                    
public:

	UKYRageBarUserWidget();
	
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	virtual void OnRageChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxRageChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateRageBar();
	void CreateWidgets();

	
protected:
	float CurrentRage = 0.0f;
	float CurrentMaxRage = 0.1f;
	int CurrentCell = 0;
	int MaxCell = 5;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= UI )
	float RagePerCell = 20.0f;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ProgressBarBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ImageBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= UI)
	FSlateBrush ImageBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= UI)
	FProgressBarStyle ProgressBarStyle;
	
};
