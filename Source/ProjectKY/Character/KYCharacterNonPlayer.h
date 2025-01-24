// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KYCharacterBase.h"
#include "KYCharacterNonPlayer.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYCharacterNonPlayer : public AKYCharacterBase
{
	GENERATED_BODY()

public:
	AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMotionWarpToTransform(FTransform InTransform);
	
protected:
	virtual void SetDead() override;
	
	virtual void OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
protected:
	UPROPERTY()
	TObjectPtr<class UKYAttributeSetHealth> AttributeSetHealth;

	/*UPROPERTY()
	TObjectPtr<class UKYAttributeSetStance> AttributeSetStance;*/

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UKYWidgetComponent> HPBar;
	
	
};
