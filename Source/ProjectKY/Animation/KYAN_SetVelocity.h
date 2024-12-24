// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KYAN_SetVelocity.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYAN_SetVelocity : public UAnimNotify
{
	GENERATED_BODY()

public:
	UKYAN_SetVelocity();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


	UPROPERTY(EditAnywhere)
	FVector TargetVelocity;
};
