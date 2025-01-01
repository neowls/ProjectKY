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

	
}

class UAbilitySystemComponent* AKYCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}


void AKYCharacterBase::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	
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
	PlayAnimMontage(DeathMontage);
}

void AKYCharacterBase::GiveStartAbilities()
{
	for (const auto& StartAbility : StartAbilities)	// 기본 어빌리티 부여
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}
