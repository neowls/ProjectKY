// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KYAN_RemoveSelectTags.h"
#include "AbilitySystemBlueprintLibrary.h"


void UKYAN_RemoveSelectTags::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, SelectTags);
		}
	}
}
