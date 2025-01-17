// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "KYCharacterMovementComponent.h"
#include "ProjectKY.h"
#include "Components/CapsuleComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
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

UAnimMontage* AKYCharacterBase::GetAnimMontageByTag(FGameplayTag& InHitTag)
{
	if (HitMontages.Contains(InHitTag))
	{
		if (HitMontages[InHitTag])
		{
			return HitMontages[InHitTag];
		}
	}
	else
	{
		KY_LOG(LogKY, Log, TEXT("Can't Find Montage"));
	}
	return nullptr;
}

TSubclassOf<UKYAT_DamageReaction> AKYCharacterBase::GetDamageTask(const FGameplayTag& InAttackTag)
{
	if (GetCharacterMovement()->IsFalling()) return DamageTaskAir[InAttackTag];
	return DamageTaskGround[InAttackTag];
}


void AKYCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void AKYCharacterBase::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	FGameplayEventData EventData;
	EventData.Instigator = DamageCauser;
	EventData.InstigatorTags = GameplayTagContainer; // GE에 담겨진 태그들
	EventData.EventMagnitude = Damage;

	KY_LOG(LogKY, Log, TEXT("Damage Taken"));
	//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, KYTAG_EVENT_HIT, EventData);
}

void AKYCharacterBase::OnStanceEvent(AActor* Causer, const FGameplayTagContainer& GameplayTagContainer, uint8 CurrentStanceState)
{
	FGameplayEventData EventData;
	EventData.Instigator = Causer;
	EventData.InstigatorTags = GameplayTagContainer; // GE에 담겨진 태그들
	EventData.EventMagnitude = CurrentStanceState;

	KY_LOG(LogKY, Log, TEXT("OnStanceEvent"));
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, KYTAG_EVENT_HIT, EventData);
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

