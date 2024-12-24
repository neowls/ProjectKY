// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterBase.h"

#include "KYCharacterMovementComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"

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
	SetDead();	
}

void AKYCharacterBase::SetDead()
{
	// Progress Base Character Death 
}
