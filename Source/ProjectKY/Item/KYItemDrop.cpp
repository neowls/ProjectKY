// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/KYItemDrop.h"

#include "ProjectKY.h"

AKYItemDrop::AKYItemDrop()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	SpawnDirection = {FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 1.0f };
	TargetActor = nullptr;
	InterpSpeed = 12.0f;
	ApplyDistance = 30.0f;
	bIsApplied = false;
	bIsHoldPosition = false;
}

void AKYItemDrop::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKYItemDrop::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TargetActor == nullptr) return;

	if (!bIsApplied)
	{
		if (!bIsHoldPosition) SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetActor->GetActorLocation(), DeltaSeconds, InterpSpeed));


		if(GetDistanceTo(TargetActor) <= ApplyDistance)
		{
			bIsApplied = true;
			ApplyEffectToTarget(1.0f);
			GrantAbilityToTarget();
			Destroy();
		}
	}
}

