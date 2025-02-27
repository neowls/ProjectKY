// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "KYCharacterMovementComponent.h"
#include "ProjectKY.h"
#include "Components/CapsuleComponent.h"
#include "Data/KYCharacterWeaponAnimAsset.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "GAS/GameAbility/KYGA_SimpleDamageReaction.h"
#include "GAS/Tag/KYGameplayTag.h"


// Sets default values
AKYCharacterBase::AKYCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKYCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	//	Character Mesh Section
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	ASC = nullptr;
	KYCharacterMovement = Cast<UKYCharacterMovementComponent>(GetCharacterMovement());

	WeaponCompRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_R"));
	WeaponCompRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCompRight->SetupAttachment(GetMesh(), TEXT("Weapon_R"));

	WeaponCompLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_L"));
	WeaponCompLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCompLeft->SetupAttachment(GetMesh(), TEXT("Weapon_L"));
	
	bIsCombat = false;
}

UAbilitySystemComponent* AKYCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

FEventAnimMontageData AKYCharacterBase::GetAnimMontageData(FGameplayTag InGameplayTag)
{
	if (!OwnWeaponAnimAssets.Contains(CurrentWeaponType))
	{
		KY_LOG(LogKY, Warning, TEXT("Anim Asset Is Empty"));
		return FEventAnimMontageData();
	}
	return OwnWeaponAnimAssets.FindRef(CurrentWeaponType)->GetEventAnimMontageData(InGameplayTag);
}

void AKYCharacterBase::SetWeaponCollisionState(EWeaponCollisionState InState)
{
	switch (InState)
	{
	case EWeaponCollisionState::LeftOn:
		WeaponCompLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
		
	case EWeaponCollisionState::LeftOff:
		WeaponCompLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ClearIgnoreActors();
		break;
		
	case EWeaponCollisionState::RightOn:
		WeaponCompRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
		
	case EWeaponCollisionState::RightOff:
		WeaponCompRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ClearIgnoreActors();
		break;
	}
}

void AKYCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void AKYCharacterBase::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	static FGameplayTagContainer FilteredGameplayTags(FGameplayTag(UKYGameplayTags::CharacterState.Attack));
	
	FGameplayEventData EventDataToReceiver;
	EventDataToReceiver.Instigator = DamageCauser;
	EventDataToReceiver.EventMagnitude = Damage;
	
	for (auto& InGameplayTag : GameplayTagContainer.Filter(FilteredGameplayTags))
	{
		EventDataToReceiver.EventTag = InGameplayTag;
		
		if (Damage < 0.0f)
		{
			EventDataToReceiver.EventTag = UKYGameplayTags::CharacterState.Attack_Blocked;
			if(ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsParry))
			{
				FGameplayEventData EventDataToCauser;
				EventDataToCauser.Instigator = this;
				EventDataToCauser.EventTag = UKYGameplayTags::CharacterState.Attack_Parry;
				EventDataToCauser.EventMagnitude = 1.0f;
				
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageCauser, EventDataToCauser.EventTag, EventDataToCauser);
				
			}
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventDataToReceiver.EventTag, EventDataToReceiver);
	}
	
	UpdateHitFlash();
}


void AKYCharacterBase::OutOfHealth()
{
	ASC->CancelAllAbilities();
	ASC->ClearAllAbilities();
	ASC->AddLooseGameplayTag(UKYGameplayTags::CharacterState.IsDead);
	SetDead();
}

void AKYCharacterBase::SetDead()
{
	bBlockInput = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadPawn"));
	StopAnimMontage();
}

void AKYCharacterBase::GrantStartAbilities()
{
	for (const auto& StartAbility : StartAbilities)	// 기본 어빌리티 부여
	{
		GrantAbility(StartAbility);
	}
}

void AKYCharacterBase::GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass)
{
	if (!IsValid(NewAbilityClass)) return;
	FGameplayAbilitySpec StartSpec(NewAbilityClass);
	ASC->GiveAbility(StartSpec);
}

void AKYCharacterBase::InitializeStatEffect()
{
	for (auto& InitEffect : InitStatEffect)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		
		
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitEffect, 1.0f, EffectContextHandle); // 이펙트 부여
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AKYCharacterBase::OnCombatState(const FGameplayTag GameplayTag, int32 Count)
{
	bIsCombat = Count > 0;
}

void AKYCharacterBase::UpdateWeaponData_Implementation(FWeaponData NewWeapon)
{
	if (OwnWeaponAnimAssets.Contains(NewWeapon.Type))
	{
		WeaponCompLeft->SetStaticMesh(nullptr);
		WeaponCompRight->SetStaticMesh(nullptr);;
		
		CurrentWeaponType = NewWeapon.Type;
		WeaponCompRight->SetStaticMesh(NewWeapon.Mesh);
		if (CurrentWeaponType == EWeaponType::DualBlade) WeaponCompLeft->SetStaticMesh(NewWeapon.Mesh);
		UpdateWeaponSocket();
		
		GetMesh()->LinkAnimClassLayers(OwnWeaponAnimAssets.FindRef(CurrentWeaponType)->WeaponAnimClass);
	}
}

void AKYCharacterBase::UpdateWeaponSocket()
{
	FTransform SkeletalSocketTransformRight = GetMesh()->GetSocketTransform(TEXT("Weapon_R"), RTS_World);
	FTransform StaticSocketTransformRight = WeaponCompRight->GetSocketTransform(TEXT("WeaponSocket"), RTS_World);
	FTransform RelativeTransformRight = StaticSocketTransformRight.GetRelativeTransform(SkeletalSocketTransformRight);

	WeaponCompRight->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("Weapon_R"));
	WeaponCompRight->SetRelativeTransform(RelativeTransformRight);

	if (CurrentWeaponType == EWeaponType::DualBlade)
	{
		FTransform SkeletalSocketTransformLeft = GetMesh()->GetSocketTransform(TEXT("Weapon_L"), RTS_World);
		FTransform StaticSocketTransformLeft = WeaponCompLeft->GetSocketTransform(TEXT("WeaponSocket"), RTS_World);
		FTransform RelativeTransformLeft = StaticSocketTransformLeft.GetRelativeTransform(SkeletalSocketTransformLeft);

		WeaponCompLeft->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("Weapon_L"));
		WeaponCompLeft->SetRelativeTransform(RelativeTransformLeft);
	}
}
