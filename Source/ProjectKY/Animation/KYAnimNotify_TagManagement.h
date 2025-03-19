// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/KYAnimNotifyBase.h"
#include "KYAnimNotify_TagManagement.generated.h"

UENUM(BlueprintType)
enum class ETagOperation : uint8
{
	Add,
	Remove,
	SendEvent
};

UCLASS()
class PROJECTKY_API UKYAnimNotify_TagManagement : public UKYAnimNotifyBase
{
	GENERATED_BODY()

protected:
	virtual void ExecuteNotify(AActor* Owner) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	ETagOperation Operation;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer Tags;
};
