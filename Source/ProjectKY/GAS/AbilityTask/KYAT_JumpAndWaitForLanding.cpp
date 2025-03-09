// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTask/KYAT_JumpAndWaitForLanding.h"

#include "ProjectKY.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (Character)
	Character->LandedDelegate.AddDynamic(this, &UKYAT_JumpAndWaitForLanding::OnLandedCallback);

	if(!Character->OnReachedJumpApex.IsAlreadyBound(this, &UKYAT_JumpAndWaitForLanding::OnReachApexCallback))
	Character->OnReachedJumpApex.AddDynamic(this, &UKYAT_JumpAndWaitForLanding::OnReachApexCallback);
	
	Character->Jump();
	SetWaitingOnAvatar();
}

void UKYAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UKYAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->OnReachedJumpApex.RemoveDynamic(this, &UKYAT_JumpAndWaitForLanding::OnReachApexCallback);
}

void UKYAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}

void UKYAT_JumpAndWaitForLanding::OnReachApexCallback()
{
	KY_LOG(LogKY, Log, TEXT("APEX"));
	OnReachApex.Broadcast();
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnReachApex.Broadcast();
	}
}