
#include "Item/KYItemObjectBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Character/KYCharacterPlayer.h"

AKYItemObjectBase::AKYItemObjectBase()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

class UAbilitySystemComponent* AKYItemObjectBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AKYItemObjectBase::SetTargetActor_Implementation(AActor* InTargetActor)
{
	TargetActor = InTargetActor;
}

void AKYItemObjectBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this, this);;
}

void AKYItemObjectBase::ApplyEffectToTarget(float Magnitude)
{
	if (ApplyEffects.IsEmpty())
	{
		KY_LOG(LogKY, Warning, TEXT("Effects To Apply Is Empty"));
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		for(auto& ApplyEffect : ApplyEffects)
		{
			FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(ApplyEffect, Magnitude, EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
				EffectGranted();
			}
			else
			{
				KY_LOG(LogKY, Warning, TEXT("EffectSpecHandle Is Not Valid"));
			}
		}
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Find TargetActor"));
	}
}

void AKYItemObjectBase::GrantAbilityToTarget(float Magnitude)
{
	if (GrantAbilities.IsEmpty()) return;
	
	AKYCharacterPlayer* CharacterPlayer = Cast<AKYCharacterPlayer>(TargetActor);
	if (CharacterPlayer)
	{
		for (auto& Ability : GrantAbilities)
		{
			CharacterPlayer->GrantAbility(Ability, Magnitude);
			AbilityGranted();
			KY_LOG(LogKY, Log, TEXT("Grant Ability"));
		}
	}
}

