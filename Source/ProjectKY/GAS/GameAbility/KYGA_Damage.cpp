// Fill out your copyright notice in the Description page of Project Settings.


#include "KYGA_Damage.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Tag/KYGameplayTag.h"


UKYGA_Damage::UKYGA_Damage()
{
	KnockbackStrength = 100.0f;
	KnockbackDuration = 1.0f;
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

void UKYGA_Damage::HitEventCallBack(FGameplayEventData Payload)
{

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	UAnimMontage* MontageToPlay = nullptr;
	
	if (Payload.InstigatorTags.HasTagExact(KYTAG_CHARACTER_ATTACK_LIGHT))
	{
		if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISFALLING))
		{
			MontageToPlay = AirKnockBackMontage;
		}
		else
		{
			MontageToPlay = HitMontage[GetHitDirection(Payload.Instigator)];
		}
	}

	else if (Payload.InstigatorTags.HasTagExact(KYTAG_CHARACTER_ATTACK_HEAVY))
	{
		if (Character != nullptr)
		{
			FVector KnockBackDirection = Payload.Instigator->GetActorLocation() - Character->GetActorLocation();
			KnockBackDirection.Normalize();
			
			FRotator KnockBackRotation = KnockBackDirection.Rotation();
			KnockBackRotation.Pitch = 0.0f;
			
			Character->SetActorRotation(KnockBackRotation);
			
			MontageToPlay = KnockBackMontage;
		}
	}
	
	else if(Payload.InstigatorTags.HasTagExact(KYTAG_CHARACTER_ATTACK_UPPER))
	{
		if (Character != nullptr)
		{
			FVector KnockBackDirection = Payload.Instigator->GetActorLocation() - Character->GetActorLocation();
			KnockBackDirection.Normalize();
			
			FRotator KnockBackRotation = KnockBackDirection.Rotation();
			KnockBackRotation.Pitch = 0.0f;
			
			Character->SetActorRotation(KnockBackRotation);
			
			Character->GetCharacterMovement()->Launch(FVector::UpVector * 1200.0f);
			MontageToPlay = AirKnockBackMontage;
		}
	}

	else if (Payload.InstigatorTags.HasTagExact(KYTAG_CHARACTER_ATTACK_SLAM))
	{
		Character->GetCharacterMovement()->Launch(FVector::DownVector * KnockbackStrength);
		MontageToPlay = AirKnockBackMontage;
	}

	if (MontageToPlay == nullptr) return;
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("HitReaction"), MontageToPlay);
	MontageTask->ReadyForActivation();
}

void UKYGA_Damage::ApplyMomentum()
{
	
}

EHitDirection UKYGA_Damage::GetHitDirection(const AActor* HitCauser)
{
	AActor* HitActor = GetOwningActorFromActorInfo();
	FVector DirectionVector = HitCauser->GetActorLocation() - HitActor->GetActorLocation();
	
	float Angle = FMath::UnwindRadians(FMath::Atan2(DirectionVector.Y, DirectionVector.X) - 
									  FMath::Atan2(HitActor->GetActorForwardVector().Y, HitActor->GetActorForwardVector().X)); 

	constexpr float Angle45Rad = FMath::DegreesToRadians(45.0f);
	constexpr float Angle135Rad = FMath::DegreesToRadians(135.0f);

	if (Angle >= Angle45Rad && Angle < Angle135Rad)
	{
		return EHitDirection::Right;
	}
	if (Angle >= -Angle135Rad && Angle < -Angle45Rad)
	{
		return EHitDirection::Left;
	}
	if (Angle >= Angle135Rad || Angle <= -Angle135Rad)
	{
		return EHitDirection::Backward;
	}

	return EHitDirection::Forward; 
}
