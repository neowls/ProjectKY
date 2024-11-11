// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/AT_Trace.h"
#include "GA/TA/TA_Trace.h"
#include "AbilitySystemComponent.h"

UAT_Trace::UAT_Trace()
{
	
}

UAT_Trace* UAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ATA_Trace> TargetActorClass)
{
	UAT_Trace* NewTask = NewAbilityTask<UAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	if(SpawnedTargetACtor)
	{
		SpawnedTargetACtor->Destroy();
	}
}

void UAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetACtor = Cast<ATA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if(SpawnedTargetACtor)
	{
		SpawnedTargetACtor->SetShowDebug(true);
		SpawnedTargetACtor->TargetDataReadyDelegate.AddUObject(this, &UAT_Trace::OnTargetDataReadyCallback);
	}
}

void UAT_Trace::FinalizeTargetActor()
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

void UAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnTraceComplete.Broadcast(DataHandle);
	}
	EndTask();
}
