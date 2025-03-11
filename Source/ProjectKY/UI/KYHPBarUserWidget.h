// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "GameplayEffectTypes.h"
#include "KYHPBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYHPBarUserWidget : public UKYUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHPBar();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HPProgressBar;
	
	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

private:
	FDelegateHandle OnHealthChangedDelegateHandle;
	FDelegateHandle OnMaxHealthChangedDelegateHandle;
	
};


