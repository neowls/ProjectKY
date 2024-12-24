// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KYCharacterBase.h"
#include "KYCharacterNonPlayer.generated.h"

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

	virtual void DamageTaken(AActor* Instigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage) override;

protected:
	virtual void SetDead() override;

protected:
	UPROPERTY()
	TObjectPtr<class UKYAttributeSetHealth> AttributeSetHealth;

	UPROPERTY()
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	//UPROPERTY()
};
