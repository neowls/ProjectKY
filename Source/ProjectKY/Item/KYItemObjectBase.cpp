
#include "Item/KYItemObjectBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"

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
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		 
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectToApply, Magnitude, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
		else
		{
			KY_LOG(LogKY, Log, TEXT("EffectSpecHandle Is Not Valid"));
		}
	}
	else
	{
		KY_LOG(LogKY, Log, TEXT("Can't Find TargetActor"));
	}
}

