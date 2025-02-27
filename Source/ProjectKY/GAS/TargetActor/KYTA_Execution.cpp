// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/KYTA_Execution.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GAS/Tag/KYGameplayTag.h"

void AKYTA_Execution::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AKYTA_Execution::ConfirmTargetingAndContinue()
{
	AActor* TargetActor = FindExecutableTarget();
	if (TargetActor)
	{
		FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetActor);

		TargetDataReadyDelegate.Broadcast(TargetDataHandle);
	}
	else
	{
		CancelTargeting();
	}
}

AActor* AKYTA_Execution::FindExecutableTarget()
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if(!ASC)
	{
		KY_LOG(LogKY, Error, TEXT("ASC not Found!"));
		return nullptr;
	}
	
	FVector Location = Character->GetActorLocation();
	float SearchRadius = 300.0f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UGASTA_Trace), false, Character);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_ENEMY);

	KY_LOG(LogKY, Log, TEXT("Find Target"));
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(OverlapResults, Location, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(SearchRadius), Params);
	if (!bHit) return nullptr;

	AActor* ClosestTarget = nullptr;
	float MinDistance = 2000.0f;

	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	
	DrawDebugSphere(GetWorld(), Location, SearchRadius, 20, DrawColor);

	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (AActor* HitActor = OverlapResult.GetActor())
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if (TargetASC && TargetASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.Executable))
			{
				float Distance = FVector::Dist(Location, HitActor->GetActorLocation());
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestTarget = HitActor;
				}
			}
		}
	}
	return ClosestTarget;
}
