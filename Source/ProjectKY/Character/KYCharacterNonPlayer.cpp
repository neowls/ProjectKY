// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterNonPlayer.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"

#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "GAS/Tag/KYGameplayTag.h"

AKYCharacterNonPlayer::AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetHealth = CreateDefaultSubobject<UKYAttributeSetHealth>(TEXT("AttributeSetHealth"));
}

void AKYCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC->InitAbilityActorInfo(this, this);
	AttributeSetHealth->OnOutOfHealth.AddDynamic(this, &AKYCharacterNonPlayer::OutOfHealth);
	AttributeSetHealth->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, 1.0f, EffectContextHandle); // 이펙트 부여
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}

	GiveStartAbilities();
}


void AKYCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
	[&]()
	{
		Destroy();
	}
	), 2.0f, false);
}


void AKYCharacterNonPlayer::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer,
	float Damage)
{
	Super::DamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);

	FGameplayEventData EventData;
	EventData.Instigator = DamageCauser;
	EventData.InstigatorTags = GameplayTagContainer;
	EventData.EventMagnitude = Damage;
	
	for (const auto& iter : GameplayTagContainer)
	{
		if (iter.IsValid())
		{
			KY_LOG(LogKY, Log, TEXT("Tag Name : %s"), *iter.GetTagName().ToString());
		}
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, KYTAG_EVENT_HIT, EventData);
}
