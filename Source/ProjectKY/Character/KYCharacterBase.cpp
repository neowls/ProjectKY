// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "KYCharacterMovementComponent.h"
#include "ProjectKY.h"
#include "Components/CapsuleComponent.h"
#include "Data/KYCharacterBaseAsset.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "GAS/GameAbility/KYGA_SimpleDamageReaction.h"
#include "GAS/Tag/KYGameplayTag.h"

// Sets default values
AKYCharacterBase::AKYCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKYCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//	Character Mesh Section
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	ASC = nullptr;
	KYCharacterMovement = Cast<UKYCharacterMovementComponent>(GetCharacterMovement());

	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AKYCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

FEventAnimMontageData AKYCharacterBase::GetAnimMontageData(FGameplayTag InGameplayTag)
{
	return AnimMontageAsset->GetEventAnimMontageData(InGameplayTag);
}

void AKYCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void AKYCharacterBase::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	FGameplayTagContainer FilteredGameplayTags(FGameplayTag(KYTAG_CHARACTER_ATTACK));
	
	for (auto& InGameplayTag : GameplayTagContainer.Filter(FilteredGameplayTags))
	{
		FGameplayEventData EventDataToReceiver;
		EventDataToReceiver.Instigator = DamageCauser;
		EventDataToReceiver.EventTag = InGameplayTag;
		EventDataToReceiver.EventMagnitude = Damage;

		KY_LOG(LogKY, Log, TEXT("Damage : %f"), Damage);
		if (Damage < 0.0f)
		{
			KY_LOG(LogKY, Log, TEXT("Attack Parry"));
			
			FGameplayEventData EventDataToCauser;
			EventDataToCauser.Instigator = this;
			EventDataToCauser.EventTag = KYTAG_CHARACTER_ATTACK_PARRY;
			EventDataToCauser.EventMagnitude = 1.0f;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageCauser, EventDataToCauser.EventTag, EventDataToCauser);
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, InGameplayTag, EventDataToReceiver);
	}
}


void AKYCharacterBase::OutOfHealth()
{
	ASC->CancelAllAbilities();
	ASC->ClearAllAbilities();
	ASC->AddLooseGameplayTag(KYTAG_CHARACTER_ISDEAD);
	SetDead();
}

void AKYCharacterBase::SetDead()
{
	bBlockInput = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadPawn"));
	StopAnimMontage();
}

void AKYCharacterBase::GiveStartAbilities()
{
	for (const auto& StartAbility : StartAbilities)	// 기본 어빌리티 부여
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AKYCharacterBase::InitializeStatEffect()
{
	for (auto& InitEffect : InitStatEffect)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		
		
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitEffect, 1.0f, EffectContextHandle); // 이펙트 부여
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

