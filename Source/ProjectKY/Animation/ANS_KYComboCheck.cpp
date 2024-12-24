// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_KYComboCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectKY.h"

UANS_KYComboCheck::UANS_KYComboCheck()
{
}

void UANS_KYComboCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, ComboGameplayTag);
		}
	}
}

void UANS_KYComboCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, ComboGameplayTag);
		}
	}
}
