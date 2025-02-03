// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KYCharacterBase.h"
#include "Interface/KYTargetableInterface.h"
#include "KYCharacterNonPlayer.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYCharacterNonPlayer : public AKYCharacterBase, public IKYTargetableInterface
{
	GENERATED_BODY()

public:
	AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintNativeEvent)
	void UpdateMotionWarpToTransform(FVector InLocation);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void SetDead() override;
	
	virtual void OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void UpdateTargetedStatus(bool InStatus) override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UKYAttributeSetEnemy> AttributeSetEnemy;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UKYWidgetComponent> HPBar;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UKYWidgetComponent> TargetedWidget;
	
	
};
