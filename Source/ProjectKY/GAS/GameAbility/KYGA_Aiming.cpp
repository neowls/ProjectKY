// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Aiming.h"
#include "ProjectKY.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/KYCharacterBase.h"
#include "GAS/TargetActor/KYTA_Trace.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/KYTargetableInterface.h"
#include "Kismet/KismetMathLibrary.h"

UKYGA_Aiming::UKYGA_Aiming()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetActorClass = AGameplayAbilityTargetActor::StaticClass();
	TargetingRange = 1000.0f;
	TargetingRadius = 100.0f;
	TargetingAngleThreshold = 25.0f;
	bIsTargeting = false;
	TargetedActor = nullptr;
	bPassiveAbility = false;
	bInputAbility = true;
}

bool UKYGA_Aiming::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (Cast<AKYCharacterBase>(ActorInfo->AvatarActor)->GetCurrentWeaponType() == EKYWeaponType::None) Result = false;

	return Result;
}

void UKYGA_Aiming::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if(TargetedActor == nullptr)
	{
		TargetedActor = Cast<AKYTA_Trace>(GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		const FTransform SpawnTransform = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()->GetTransform();
		TargetedActor->SetShowDebug(bShowDebug);
		TargetedActor->FinishSpawning(SpawnTransform);
		TargetedActor->TargetDataReadyDelegate.AddUObject(this, &UKYGA_Aiming::OnTargetCompleteCallback);
		GetAbilitySystemComponentFromActorInfo()->SpawnedTargetActors.Push(TargetedActor);
	}
	
	TargetedActor->StartTargeting(this);
	
	OnGameplayAbilityCancelled.AddUObject(this, &UKYGA_Aiming::OnSimpleInterruptedCallback);
}

void UKYGA_Aiming::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (TargetedActor)
	{
		if(!CurrentTargetDataHandle.Data.IsEmpty())
		{
			SetTargetUIStatus(false);
		}
		CurrentTargetDataHandle.Clear();
		bIsTargeting = false;
	}
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void UKYGA_Aiming::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		
		AimAtCursor(*ActorInfo);
		TargetedActor->ConfirmTargetingAndContinue();
		CheckCurrentTarget();
		TargetedActor->SetShowDebug(true);
	}
}


void UKYGA_Aiming::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	OnSimpleCompleteCallback();
}

void UKYGA_Aiming::OnTargetCompleteCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if(TargetDataHandle != CurrentTargetDataHandle && CurrentTargetDataHandle.IsValid(0)) SetTargetUIStatus(false);
	CurrentTargetDataHandle = TargetDataHandle;
	SetTargetUIStatus(true);
}


void UKYGA_Aiming::AimAtCursor(const FGameplayAbilityActorInfo& ActorInfo)
{
	APlayerController* PlayerController = Cast<APlayerController>(ActorInfo.PlayerController);
	if (PlayerController != nullptr)
	{
		FHitResult CursorHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
		CursorLocation = CursorHitResult.Location;
		if(!bIsTargeting) RotateCharacter(CursorLocation);
	}
}


void UKYGA_Aiming::CheckCurrentTarget()
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(CurrentTargetDataHandle, 0))
	{
		bIsTargeting = true;
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(CurrentTargetDataHandle, 0);
		FVector TargetLocation  = HitResult.GetActor()->GetActorLocation();
		FVector MouseDirection  = CursorLocation - GetAvatarActorFromActorInfo()->GetActorLocation();
		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct((TargetLocation - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal(), MouseDirection.GetSafeNormal())));
		
		if (Angle > TargetingAngleThreshold)
		{
			SetTargetUIStatus(false);
			CurrentTargetDataHandle.Clear();
			bIsTargeting = false;
			return;
		}
		
		RotateCharacter(TargetLocation);
	}
}


void UKYGA_Aiming::SetTargetUIStatus(bool InStatus)
{
	if (CurrentTargetDataHandle.Data[0].IsValid() && CurrentTargetDataHandle.Data[0].Get()->GetActors()[0].IsValid())
	{
		IKYTargetableInterface* CurrentTarget = Cast<IKYTargetableInterface>(CurrentTargetDataHandle.Data[0].Get()->GetActors()[0].Get());
		if (CurrentTarget)
		{
			CurrentTarget->UpdateTargetedStatus(InStatus);
		}
	}
}


void UKYGA_Aiming::RotateCharacter(FVector& TargetLocation)
{
	FRotator CurrentRotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), TargetLocation);
	LookAtRotation.Pitch = CurrentRotation.Pitch;
	GetAvatarActorFromActorInfo()->SetActorRotation(LookAtRotation);
}
