// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/KYItemInteract.h"

#include "UI/KYWidgetComponent.h"

AKYItemInteract::AKYItemInteract()
{
	InteractWidget = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("InteractableWidget"));
	InteractWidget->SetupAttachment(RootComponent);
}

void AKYItemInteract::InteractEvent_Implementation(AActor* Causer, float Magnitude)
{
	SetTargetActor(Causer);
	/* More Interact Function */
	OnInteractEvent.AddDynamic(this, &ThisClass::ApplyEffectToTarget);
	OnInteractEvent.AddDynamic(this, &ThisClass::GrantAbilityToTarget);
}

void AKYItemInteract::SetInteractableStatus_Implementation(bool InStatus)
{
	
}
