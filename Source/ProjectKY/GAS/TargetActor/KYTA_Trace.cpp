// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/KYTA_Trace.h"

#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectKY.h"
#include "Kismet/KismetSystemLibrary.h"


AKYTA_Trace::AKYTA_Trace()
{
	
}

void AKYTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AKYTA_Trace::ConfirmTargetingAndContinue()
{
	if(SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AKYTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if(!ASC)
	{
		KY_LOG(LogKY, Error, TEXT("ASC not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	FHitResult HitResult;

	constexpr float TraceRange = 1000.0f;
	constexpr float TraceRadius = 100.0f;
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UGASTA_Trace), false, Character);
	
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * TraceRange;
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_ENEMY);
	bool HitDetected = false;
	
	if (bShapeTrace)
	{
		HitDetected = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(TraceRadius), Params);
	}
	else
	{
		HitDetected = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, Params);
	}
	
	FGameplayAbilityTargetDataHandle DataHandle;

	
	if(HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);			
	}

#if ENABLE_DRAW_DEBUG
	if(bShowDebug)
	{
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		if (bShapeTrace)
		{
			FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
			float CapsuleHalfHeight = TraceRange * 0.5f;
			DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, TraceRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor);
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, DrawColor);
		}
	}
#endif
	return DataHandle;
}
