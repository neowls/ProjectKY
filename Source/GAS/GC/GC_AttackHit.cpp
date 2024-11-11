// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/GC_AttackHit.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UGC_AttackHit::UGC_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BloodRef(TEXT("/Script/Engine.ParticleSystem'/Game/_Assets/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if(BloodRef.Object)
	{
		BloodParticleSystem = BloodRef.Object;
	}
}

bool UGC_AttackHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if(HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyTarget, BloodParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
		UGameplayStatics::SpawnEmitterAtLocation(MyTarget, ImpactParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
		UGameplayStatics::SpawnSoundAtLocation(MyTarget, HitSound, HitResult->ImpactPoint);
	}
	else
	{
		for(const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if(TargetActor.Get())
			{
				UGameplayStatics::SpawnEmitterAtLocation(MyTarget, BloodParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
				UGameplayStatics::SpawnEmitterAtLocation(MyTarget, ImpactParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}
	return false;
}
 