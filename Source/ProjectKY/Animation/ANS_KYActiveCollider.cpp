// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_KYActiveCollider.h"

#include "Character/KYCharacterBase.h"

UANS_KYActiveCollider::UANS_KYActiveCollider()
{
}

void UANS_KYActiveCollider::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		if (bIsRight) Character->SetWeaponCollisionState(EWeaponCollisionState::RightOn);
		if (bIsLeft) Character->SetWeaponCollisionState(EWeaponCollisionState::LeftOn);
	}
}

void UANS_KYActiveCollider::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(MeshComp->GetOwner());
	if (Character)
	{
		if (bIsRight) Character->SetWeaponCollisionState(EWeaponCollisionState::RightOff);
		if (bIsLeft) Character->SetWeaponCollisionState(EWeaponCollisionState::LeftOff);
	}
}
