// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KYAnimNotifyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYAnimNotifyBase : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	bool ShouldExecuteInWorld(USkeletalMeshComponent* MeshComp) const;

	virtual void ExecuteNotify(AActor* Owner) PURE_VIRTUAL(UKYAnimNotifyBase::ExecuteNotify, );
};
