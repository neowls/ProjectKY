// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/TA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Attribute/GASCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS.h"


ATA_Trace::ATA_Trace()
{
	
}

void ATA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ATA_Trace::ConfirmTargetingAndContinue()
{
	if(SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ATA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if(!ASC)
	{
		GAS_LOG(LogGAS, Error, TEXT("ASC not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UGASCharacterAttributeSet* AttributeSet = ASC->GetSet<UGASCharacterAttributeSet>();
	if(!AttributeSet)
	{
		GAS_LOG(LogGAS, Error, TEXT("AttributeSet not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	//FHitResult OutHitResult;
	TArray<FHitResult> OutHitResults;
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UGASTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	//bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_Pawn ,FCollisionShape::MakeSphere(AttackRadius), Params);
	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_Pawn ,FCollisionShape::MakeSphere(AttackRadius), Params);
	

	FGameplayAbilityTargetDataHandle DataHandle;
	if(HitDetected)
	{
		for(auto Iter : OutHitResults)
		{
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(Iter);
			DataHandle.Add(TargetData);			
		}
	}

#if ENABLE_DRAW_DEBUG
	if(bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif
	return DataHandle;
}
