// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterNonPlayer.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "AI/KYAIController.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "GAS/Attribute/KYAttributeSetStance.h"
#include "GAS/Tag/KYGameplayTag.h"

AKYCharacterNonPlayer::AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetHealth = CreateDefaultSubobject<UKYAttributeSetHealth>(TEXT("AttributeSetHealth"));
	AttributeSetStance = CreateDefaultSubobject<UKYAttributeSetStance>(TEXT("AttributeSetStance"));
	AIControllerClass = AKYAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AKYCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC->InitAbilityActorInfo(this, this);

	
	AttributeSetHealth->OnOutOfHealth.AddDynamic(this, &AKYCharacterNonPlayer::OutOfHealth);
	AttributeSetHealth->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);
	
	AttributeSetStance->OnStanceChange.AddDynamic(this, &ThisClass::OnStanceEvent);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, 1.0f, EffectContextHandle); // 이펙트 부여
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
	
	GiveStartAbilities();

	ASC->RegisterGameplayTagEvent(KYTAG_CHARACTER_ISINACTIVE).AddUObject(this, &ThisClass::OnHitTagChanged);
}


void AKYCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	AKYAIController* AIController = Cast<AKYAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
		KY_LOG(LogKY, Log, TEXT("Stop AI"));
	}

	FVector ImpulseDirection = GetActorRotation().Vector() * -1.0f;
	ImpulseDirection.Normalize();

	float ImpulseStrength = 2000.0f;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse);
	
	
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
	[&]()
	{
		Destroy();
	}
	), 2.0f, false);
}


void AKYCharacterNonPlayer::OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	AKYAIController* AIController = Cast<AKYAIController>(GetController());
	if (AIController)
	{
		AIController->SetHitStatus(NewCount>0);
	}
}

