// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"

#include "GAS/Attribute/KYAttributeSetHealth.h"

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
	AttributeSetHealth->OnDamageTaken.AddUFunction(this, FName("DamageTaken"));

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, 1.0f, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AKYCharacterNonPlayer::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	Super::DamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
	EHitDirection HitDirection = EHitDirection::Forward;
	FVector DirectionVector = DamageCauser->GetActorLocation() - GetActorLocation();
	
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), DirectionVector.GetSafeNormal())));
	
	FVector LocalCrossAngle = GetActorRotation().RotateVector(FVector::CrossProduct(GetActorForwardVector(), DirectionVector));
	
	if (LocalCrossAngle.Y > 0.0f)
	{
		Angle = -Angle;	
	}
	
	if (Angle >= 45.0f && Angle < 135.0f)
	{
		// 오른쪽
		HitDirection = EHitDirection::Right;
	}
	else if (Angle >= 135.0f || Angle < -135.0f)
	{
		// 뒤
		HitDirection = EHitDirection::Backward;
	}
	else if (Angle >= -135.0f && Angle < -45.0f)
	{
		// 왼쪽
		HitDirection = EHitDirection::Left;
	}

	if (HitMontage[HitDirection] != nullptr) PlayAnimMontage(HitMontage[HitDirection]);
	KY_LOG(LogKY, Log, TEXT("Hit Action"));
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
