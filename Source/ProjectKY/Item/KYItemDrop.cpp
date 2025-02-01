// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/KYItemDrop.h"

AKYItemDrop::AKYItemDrop()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	SpawnDirection = {FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), 1.0f };
	TargetActor = nullptr;
	InterpSpeed = 10.0f;
}

void AKYItemDrop::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AKYItemDrop::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TargetActor == nullptr || bIsApplied) return;
	
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetActor->GetActorLocation(), DeltaSeconds, InterpSpeed));

	if(GetDistanceTo(TargetActor) < 30.0f)
	{
		ApplyEffectToTarget();
		bIsApplied = true;
		Destroy();
	}
}

