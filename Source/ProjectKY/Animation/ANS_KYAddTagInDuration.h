// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_KYAddTagInDuration.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UANS_KYAddTagInDuration : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_KYAddTagInDuration();

	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AddedGameplayTag;
	
};
