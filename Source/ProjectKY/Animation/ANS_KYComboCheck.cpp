// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_KYComboCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/Tag/KYGameplayTag.h"

UANS_KYComboCheck::UANS_KYComboCheck()
{
}

void UANS_KYComboCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp && MeshComp->GetWorld()->WorldType != EWorldType::Type::EditorPreview)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
			ASC->AddLooseGameplayTag(UKYGameplayTags::Event.Combo_Available);
		}
	}
}

void UANS_KYComboCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetWorld()->WorldType != EWorldType::Type::EditorPreview)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
			ASC->RemoveLooseGameplayTag(UKYGameplayTags::Event.Combo_Available);
		}
	}
}
