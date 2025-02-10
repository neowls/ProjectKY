
#include "Item/KYItemObjectBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"

AKYItemObjectBase::AKYItemObjectBase()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
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

void AKYItemObjectBase::ApplyEffectToTarget()
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		 
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectToApply, 1.0f, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			KY_LOG(LogKY, Log, TEXT("Apply Effect"));
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

