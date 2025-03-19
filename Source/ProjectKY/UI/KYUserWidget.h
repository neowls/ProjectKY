// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "KYUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual void SetAbilitySystemComponent(UAbilitySystemComponent& InASC);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void K2_SetAbilitySystemComponent(AActor* InOwner);

	UFUNCTION(BlueprintPure)
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;
};
