// Fill out your copyright notice in the Description page of Project Settings.


#include "KYGA_Damage.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/KYCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Tag/KYGameplayTag.h"


UKYGA_Damage::UKYGA_Damage()
{
	MomentumStrength = 1200.0f;
}


void UKYGA_Damage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo == nullptr || ActorInfo->AvatarActor == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KYTAG_EVENT_HIT, nullptr, false,  false);
	EventTask->EventReceived.AddDynamic(this, &UKYGA_Damage::HitEventCallBack);
	EventTask->ReadyForActivation();
}

void UKYGA_Damage::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}


void UKYGA_Damage::ApplyHitReactionMomentum(FGameplayTag& InHitTag, AKYCharacterBase* Character, const FVector& InstigatorLocation)
{
	
}

void UKYGA_Damage::HitEventCallBack(FGameplayEventData Payload)
{
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo());
	if (Character == nullptr) return;

	if (Payload.ContextHandle.GetHitResult())
	{
		KY_LOG(LogKY, Log, TEXT("Hit Result"));
	}
	else
	{
		KY_LOG(LogKY, Log, TEXT("No Hit Result"));
	}

	FVector HitDirection = Payload.Instigator.Get()->GetActorLocation() - Character->GetActorLocation();
	HitDirection.Normalize();
	
	Character->SetActorRotation(HitDirection.Rotation());

	FGameplayTag ReactionTag;
	for (auto& Tag : Payload.InstigatorTags)
	{
		KY_LOG(LogKY, Log, TEXT("GE Tag : %s"), *Tag.GetTagName().ToString());
		ReactionTag = Tag;
	}

	if (ReactionTag.MatchesTag(KYTAG_CHARACTER_ATTACK) && Payload.EventMagnitude > 1.0f)
	{
		HitReaction(Character,  ReactionTag);
	}
	
	//UAnimMontage* MontageToPlay = Character->GetAnimMontageByTag();
	
	
	/*UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("HitReaction"), MontageToPlay);
	MontageTask->OnCompleted.AddDynamic(this, &UKYGA_Damage::OnHitMontageCallback);
	MontageTask->OnInterrupted.AddDynamic(this, &UKYGA_Damage::OnHitMontageCallback);
	
	MontageTask->ReadyForActivation();*/
	
}

void UKYGA_Damage::HitReaction(const ACharacter* InCharacter, const FGameplayTag& InReactionTag) const
{
	FVector NewDirection;

	if (InReactionTag.MatchesTagExact(KYTAG_CHARACTER_ATTACK_HEAVY))
	{
		NewDirection = -InCharacter->GetActorForwardVector();
		NewDirection.Z = 0.2f;
	}
	else if(InReactionTag.MatchesTagExact(KYTAG_CHARACTER_ATTACK_UPPER))
	{
		NewDirection = FVector::UpVector;
	}
	else if(InReactionTag.MatchesTagExact(KYTAG_CHARACTER_ATTACK_SLAM))
	{
		NewDirection = FVector::DownVector;
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Matches Nothing."));
		return;
	}
	
	InCharacter->GetCharacterMovement()->Launch(NewDirection * MomentumStrength);
}

void UKYGA_Damage::OnHitMontageCallback()
{
	KY_LOG(LogKY, Log, TEXT("Hit Montage Call Back"));
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISSTAGGERED))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(KYTAG_CHARACTER_ISSTAGGERED, 2);
	}
}

void UKYGA_Damage::OnLandedCallback(const FHitResult& Hit)
{
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo());
	if (Character == nullptr) return;
	Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(KYTAG_CHARACTER_ISKNOCKDOWN);
}
