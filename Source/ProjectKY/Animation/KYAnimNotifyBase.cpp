// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KYAnimNotifyBase.h"

void UKYAnimNotifyBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ShouldExecuteInWorld(MeshComp))
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			ExecuteNotify(Owner);
		}
	}
}

bool UKYAnimNotifyBase::ShouldExecuteInWorld(USkeletalMeshComponent* MeshComp) const
{
	return MeshComp && MeshComp->GetWorld()->WorldType != EWorldType::Type::EditorPreview;
}
