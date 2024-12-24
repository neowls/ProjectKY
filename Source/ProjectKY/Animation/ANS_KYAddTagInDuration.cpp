// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_KYAddTagInDuration.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectKY.h"

UANS_KYAddTagInDuration::UANS_KYAddTagInDuration()
{
	
}

void UANS_KYAddTagInDuration::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, AddedGameplayTag);
		}
	}
}

void UANS_KYAddTagInDuration::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, AddedGameplayTag);
		}
	}
}
