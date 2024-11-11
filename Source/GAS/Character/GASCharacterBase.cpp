#include "Character/GASCharacterBase.h"
#include "GAS.h"
#include "Attribute/GASCharacterAttributeSet.h"

AGASCharacterBase::AGASCharacterBase()
{
	//	Character Mesh Section
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	ASC = nullptr;
}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AGASCharacterBase::OnOutOfHealth()
{
	SetDead();
}

void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASCharacterBase::SetDead()
{
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);		
	}
}

