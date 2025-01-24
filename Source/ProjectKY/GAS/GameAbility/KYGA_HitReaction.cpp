// Fill out your copyright notice in the Description page of Project Settings.


#include "KYGA_HitReaction.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/KYCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Tag/KYGameplayTag.h"


UKYGA_HitReaction::UKYGA_HitReaction()
{
	MomentumStrength = 1000.0f;
}


void UKYGA_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo == nullptr || ActorInfo->AvatarActor == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KYTAG_EVENT_HIT, nullptr, false,  false);
	EventTask->EventReceived.AddDynamic(this, &UKYGA_HitReaction::HitEventCallBack);
	EventTask->ReadyForActivation();
}


void UKYGA_HitReaction::HitEventCallBack(FGameplayEventData Payload)
{
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo());
	if (Character == nullptr) return;
	
	FVector HitDirection = Payload.Instigator.Get()->GetActorLocation() - Character->GetActorLocation();
	HitDirection.Normalize();


	FRotator HitRotation = HitDirection.Rotation();
	HitRotation.Pitch = 0.0f;
	
	Character->SetActorRotation(HitRotation);
	

	FGameplayTag ReactionTag;
	for (auto& Tag : Payload.InstigatorTags)
	{
		ReactionTag = Tag;
	}

	/*if (ReactionTag.MatchesTag(KYTAG_CHARACTER_ATTACK))
	{
		if (Payload.EventMagnitude > 0.0f)
		{
			PlayStanceMontage(Character->GetAnimMontageByTag(Payload.EventMagnitude - 1.0f));
			if (Payload.EventMagnitude > 1.0f)
			{
				ApplyHitReactionMomentum(Character,  ReactionTag);
				
				if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISSTAGGERED))
					UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Character, FGameplayTagContainer(KYTAG_CHARACTER_ISSTAGGERED));
				if(!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISKNOCKOUT))
					UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Character, FGameplayTagContainer(KYTAG_CHARACTER_ISKNOCKOUT));
			}
			else
			{
				if(!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISSTAGGERED))
					UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Character, FGameplayTagContainer(KYTAG_CHARACTER_ISSTAGGERED));
			}
		}
	}*/
}

void UKYGA_HitReaction::ApplyHitReactionMomentum(ACharacter* InCharacter, const FGameplayTag& InReactionTag) const
{
	FVector NewDirection;

	if (InReactionTag.MatchesTagExact(KYTAG_CHARACTER_ATTACK_HEAVY))
	{
		NewDirection = -InCharacter->GetActorForwardVector();
		NewDirection.Z = 0.2f;
		InCharacter->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	}
	else if(InReactionTag.MatchesTagExact(KYTAG_CHARACTER_ATTACK_UPPER))
	{
		NewDirection = FVector::UpVector;
		InCharacter->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	}
	else if(InReactionTag.MatchesTagExact(KYTAG_CHARACTER_ATTACK_SLAM))
	{
		NewDirection = FVector::DownVector;
	}
	else
	{
		NewDirection = FVector::UpVector * 0.3;
	}
	
	InCharacter->GetCharacterMovement()->Launch(NewDirection * MomentumStrength);
}

void UKYGA_HitReaction::OnLandedCallback(const FHitResult& Result)
{
	BP_ApplyGameplayEffectToOwner(ResetGameplayEffect);
	
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo());
	if (Character == nullptr) return;
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISSTAGGERED) || GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISKNOCKOUT) )
	{
		FGameplayTagContainer TempReactionTags;
		TempReactionTags.AddTag(KYTAG_CHARACTER_ISSTAGGERED);
		TempReactionTags.AddTag(KYTAG_CHARACTER_ISKNOCKOUT);
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Character, TempReactionTags);
	}
	Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);
}

void UKYGA_HitReaction::PlayStanceMontage(UAnimMontage* MontageToPlay)
{
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("HitReaction"), MontageToPlay);
	MontageTask->ReadyForActivation();
	KY_LOG(LogKY, Log, TEXT("Play Montage : %s"), *MontageToPlay->GetName());
}
