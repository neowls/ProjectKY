// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Execution.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/KYCharacterNonPlayer.h"
#include "Character/KYCharacterPlayer.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "GAS/TargetActor/KYTA_Execution.h"
#include "Library/KYBlueprintFunctionLibrary.h"
#include "Struct/KYStruct.h"

void UKYGA_Execution::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AKYTA_Execution* TargetActor = GetWorld()->SpawnActorDeferred<AKYTA_Execution>(TargetingActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	const FTransform SpawnTransform = GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()->GetTransform();
	TargetActor->FinishSpawning(SpawnTransform);

	TargetActor->TargetDataReadyDelegate.AddUObject(this, &UKYGA_Execution::OnTargetDataReceived);
	GetAbilitySystemComponentFromActorInfo()->SpawnedTargetActors.Push(TargetActor);

	TargetActor->StartTargeting(this);
	TargetActor->ConfirmTargetingAndContinue();

	FString TargetActorName = TargetActor->GetName();
	
	KY_LOG(LogKY, Log, TEXT("Target Actor : %s "), *TargetActorName);

	if (TargetActor == nullptr) EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
}

void UKYGA_Execution::OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	AActor* TargetedActor = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(DataHandle, 0).Last();
	if (TargetedActor)
	{
		AbilityTags.Filter(FGameplayTagContainer(KYTAG_DATA));
		FGameplayTag AbilityTag;
		for (auto& Tag : AbilityTags)
		{
			AbilityTag = Tag;
		}
	
		if (AbilityTag == FGameplayTag())
		{
			KY_LOG(LogKY, Warning, TEXT("This Ability Does Not Have Data Tag."));
			return;
		}
	
		FEventAnimMontageData AnimMontageData = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo())->GetAnimMontageData(AbilityTag);

		if (AnimMontageData.Montage == nullptr) return;
		
		UAbilityTask_PlayMontageAndWait* PMW = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, AnimMontageData.TaskInstanceName, AnimMontageData.Montage, AnimMontageData.PlayRate);
		PMW->OnCompleted.AddDynamic(this, &ThisClass::OnSimpleCompleteCallback);
		AKYCharacterNonPlayer* TargetCharacter = Cast<AKYCharacterNonPlayer>(TargetedActor);

		FRotator LookRotator = UKYBlueprintFunctionLibrary::GetRotationToTarget(TargetedActor->GetActorLocation(), GetAvatarActorFromActorInfo()->GetActorLocation());
		TargetCharacter->SetActorRotation(LookRotator);
		TargetCharacter->PlayExecutedMontage(FName("0"));
		Cast<AKYCharacterPlayer>(GetAvatarActorFromActorInfo())->UpdateMotionWarpToTarget(TargetedActor);
		PMW->ReadyForActivation();
	}
}
