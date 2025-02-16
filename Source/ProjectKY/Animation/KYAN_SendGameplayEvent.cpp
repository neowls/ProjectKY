// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KYAN_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UKYAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetWorld()->WorldType != EWorldType::Type::EditorPreview)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());
		}
	}
}
