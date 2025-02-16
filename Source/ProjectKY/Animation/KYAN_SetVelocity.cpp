// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KYAN_SetVelocity.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UKYAN_SetVelocity::UKYAN_SetVelocity()
{
}

void UKYAN_SetVelocity::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character)
	{
		//Character->GetCharacterMovement()->Velocity = TargetVelocity;
		Character->GetCharacterMovement()->Launch(TargetVelocity);
	}
}
