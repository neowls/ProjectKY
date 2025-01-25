// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_AnimBase.h"
#include "KYGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Attack : public UKYGA_AnimBase
{
	GENERATED_BODY()

public:
	UKYGA_Attack();
	
protected:
	virtual void OnSimpleEventReceivedCallback(FGameplayEventData Payload) override;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> AttackGameplayEffect;
	
	uint8 CurrentAttackIndex;
};