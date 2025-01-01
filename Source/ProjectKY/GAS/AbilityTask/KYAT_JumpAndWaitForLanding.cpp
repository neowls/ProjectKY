// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTask/KYAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UKYAT_JumpAndWaitForLanding::UKYAT_JumpAndWaitForLanding()
{
	
}

UKYAT_JumpAndWaitForLanding* UKYAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UKYAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UKYAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UKYAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();
	
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UKYAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UKYAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UKYAT_JumpAndWaitForLanding::OnLandedCallback);
	Super::OnDestroy(bInOwnerFinished);
}

void UKYAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
