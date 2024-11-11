// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/TA_SphereMultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Attribute/GASCharacterSkillAttributeSet.h"

FGameplayAbilityTargetDataHandle ATA_SphereMultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if(!ASC)
	{
		GAS_LOG(LogGAS, Error, TEXT("ASC not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UGASCharacterSkillAttributeSet* SkillAttributeSet = ASC->GetSet<UGASCharacterSkillAttributeSet>();
	if(!SkillAttributeSet)
	{
		GAS_LOG(LogGAS, Error, TEXT("SkillAttributeSet not Found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> OverlapResults;
	const float SkillRadius = SkillAttributeSet->GetSkillRange();

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ATA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for(const FOverlapResult& OverlapResult : OverlapResults)
	{
		AActor* HitActor = OverlapResult.OverlapObjectHandle.FetchActor<AActor>();
		if(HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}
	
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG
	if(bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}
#endif
	

	return FGameplayAbilityTargetDataHandle(ActorsData);
	
}
