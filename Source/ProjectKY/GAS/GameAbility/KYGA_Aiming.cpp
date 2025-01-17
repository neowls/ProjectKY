// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Aiming.h"
#include "ProjectKY.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/TargetActor/KYTA_Trace.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}

void UKYGA_Aiming::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	
	TargetedActor = Cast<AKYTA_Trace>(GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	
	if(TargetedActor)
	{
		TargetedActor->SetShowDebug(true);
		TargetedActor->TargetDataReadyDelegate.AddUObject(this, &UKYGA_Aiming::OnTargetCompleteCallback);
	}
	
	const FTransform SpawnTransform = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()->GetTransform();
	TargetedActor->FinishSpawning(SpawnTransform);
	
	GetAbilitySystemComponentFromActorInfo()->SpawnedTargetActors.Push(TargetedActor);
	
	TargetedActor->StartTargeting(this);
	
	OnGameplayAbilityCancelled.AddUObject(this, &UKYGA_Aiming::OnSimpleInterruptedCallback);
}

void UKYGA_Aiming::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (TargetedActor)
	{
		TargetedActor->Destroy();
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
		AimAtCursor(*ActorInfo);
		TraceTarget(Handle, *ActorInfo, ActivationInfo);
	}
}


void UKYGA_Aiming::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UKYGA_Aiming::OnTargetCompleteCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	CurrentTargetDataHandle = TargetDataHandle;
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

void UKYGA_Aiming::TraceTarget(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	TargetedActor->ConfirmTargetingAndContinue();
	
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(CurrentTargetDataHandle, 0))
	{
		if(CurrentTargetDataHandle.Data.IsEmpty()) return;

		bIsTargeting = true;
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(CurrentTargetDataHandle, 0);
		FVector TargetLocation  = HitResult.GetActor()->GetActorLocation();
		FVector MouseDirection  = CursorLocation - GetAvatarActorFromActorInfo()->GetActorLocation();
		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct((TargetLocation - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal(), MouseDirection.GetSafeNormal())));
		
		
		if (Angle > TargetingAngleThreshold)
		{
			CurrentTargetDataHandle.Clear();
			bIsTargeting = false;
			return;
		}

		RotateCharacter(TargetLocation);
	}
	else
	{
		CurrentTargetDataHandle.Clear();
		bIsTargeting = false;
	}
		
}

void UKYGA_Aiming::RotateCharacter(FVector& TargetLocation)
{
	FRotator CurrentRotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), TargetLocation);
	LookAtRotation.Pitch = CurrentRotation.Pitch;
	GetAvatarActorFromActorInfo()->SetActorRotation(LookAtRotation);
}