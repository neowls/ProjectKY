// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GASCharacterBase.h"
#include "GASCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AGASCharacterNonPlayer : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	AGASCharacterNonPlayer();

	virtual void PossessedBy(AController* NewController) override;


protected:
	virtual void SetDead() override;

protected:
	UPROPERTY()
	TObjectPtr<class UGASCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category=GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category=GAS)
	float Level;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGASWidgetComponent> HpBar;
};
