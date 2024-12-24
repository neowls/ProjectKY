// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTask/KYAT_Trace.h"
#include "GAS/TargetActor/KYTA_Trace.h"
#include "AbilitySystemComponent.h"


UKYAT_Trace::UKYAT_Trace()
{
	
}

UKYAT_Trace* UKYAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AKYTA_Trace> TargetActorClass)
{
	UKYAT_Trace* NewTask = NewAbilityTask<UKYAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UKYAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UKYAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	if(SpawnedTargetACtor)
	{
		SpawnedTargetACtor->Destroy();
	}
}

void UKYAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetACtor = Cast<AKYTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if(SpawnedTargetACtor)
	{
		SpawnedTargetACtor->SetShowDebug(true);
		SpawnedTargetACtor->TargetDataReadyDelegate.AddUObject(this, &UKYAT_Trace::OnTargetDataReadyCallback);
	}
}

void UKYAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if(ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetACtor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetACtor);
		SpawnedTargetACtor->StartTargeting(Ability);
		SpawnedTargetACtor->ConfirmTargeting();
	}
}

void UKYAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnTraceComplete.Broadcast(DataHandle);
	}
	EndTask();
}
