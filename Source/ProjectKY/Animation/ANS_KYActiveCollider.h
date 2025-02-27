// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_KYActiveCollider.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UANS_KYActiveCollider : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_KYActiveCollider();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRight = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLeft = false;
};
