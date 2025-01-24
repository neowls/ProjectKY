// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "KYCharacterMovementComponent.h"
#include "ProjectKY.h"
#include "Components/CapsuleComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
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
	WeaponComp->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	WeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AKYCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

UAnimMontage* AKYCharacterBase::GetAnimMontageByTag(FGameplayTag InTag)
{
	if (HitMontageMap.Find(InTag))
	{
		KY_LOG(LogKY, Warning, TEXT("Find Matched Montage, Tag : %s"), *InTag.GetTagName().ToString());
		return HitMontageMap[InTag];
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Find '%s' Tag Montage"), *InTag.GetTagName().ToString());
	}
	return nullptr;
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
		FGameplayEventData EventData;
		EventData.Instigator = DamageCauser;
		EventData.EventTag = InGameplayTag;
		EventData.EventMagnitude = Damage;

		KY_LOG(LogKY, Log, TEXT("InGameplayTag Before Send : %s"), *InGameplayTag.GetTagName().ToString());

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, InGameplayTag, EventData);
	}
}

/*
void AKYCharacterBase::OnStanceEvent(AActor* Causer, const FGameplayTagContainer& GameplayTagContainer, uint8 CurrentStanceState)
{
	FGameplayEventData EventData;
	EventData.Instigator = Causer;
	EventData.InstigatorTags = GameplayTagContainer;
	EventData.EventMagnitude = CurrentStanceState;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, KYTAG_EVENT_HIT, EventData);
}
*/

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

