// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_KYAttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"

UANS_KYAttackHitCheck::UANS_KYAttackHitCheck()
{
	ComboAttackLevel = 1.0f;
}

FString UANS_KYAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UANS_KYAttackHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = ComboAttackLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, TriggerGameplayTag, PayloadData);
		}
	}
}

void UANS_KYAttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
