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


AKYCharacterBase::AKYCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UKYCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	RootComponent = GetCapsuleComponent();
	
	//	Character Mesh Section
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Game/NO_Cooking/Animation/AnimBaseCharacters/Mannequin_UE4/Meshes/SK_Mannequin_Base.SK_Mannequin_Base"));
	GetMesh()->SetSkeletalMeshAsset(SkeletalMeshRef.Object);
	static ConstructorHelpers::FObjectFinder<USkeleton> SkeletonRef(TEXT("/Game/NO_Cooking/Animation/AnimBaseCharacters/Mannequin_UE4/Meshes/SK_Mannequin_Skeleton_Base.SK_Mannequin_Skeleton_Base"));
	GetMesh()->GetSkeletalMeshAsset()->SetSkeleton(SkeletonRef.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	ASC = nullptr;
	KYCharacterMovement = Cast<UKYCharacterMovementComponent>(GetCharacterMovement());

	WeaponCompRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp_R"));
	WeaponCompRight->SetupAttachment(GetMesh(), TEXT("Weapon_R"));

	WeaponCompLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp_L"));
	WeaponCompLeft->SetupAttachment(GetMesh(), TEXT("Weapon_L"));

	WeaponCompLeft->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponCompBeginOverlap);
	WeaponCompRight->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponCompBeginOverlap);
	
	bIsCombat = false;

	AttackEventData.EventTag = UKYGameplayTags::Event.AttackHit;
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

void AKYCharacterBase::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	static FGameplayTagContainer FilteredGameplayTags(FGameplayTag(UKYGameplayTags::CharacterState.Attack));
	
	DamageEventData.Instigator = DamageCauser;
	DamageEventData.EventMagnitude = Damage;
	
	for (auto& InGameplayTag : GameplayTagContainer.Filter(FilteredGameplayTags))
	{
		DamageEventData.EventTag = InGameplayTag;
		
		if (Damage < 0.0f)
		{
			DamageEventData.EventTag = UKYGameplayTags::CharacterState.Attack_Blocked;
			
			FGameplayEventData EventDataToCauser;
			EventDataToCauser.Instigator = this;
			EventDataToCauser.EventTag = UKYGameplayTags::CharacterState.Attack_Parry;
			EventDataToCauser.EventMagnitude = 1.0f;
			
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(DamageCauser, EventDataToCauser.EventTag, EventDataToCauser);
		}
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, DamageEventData.EventTag, DamageEventData);
	}
}

void AKYCharacterBase::OutOfHealth()
{
	ASC->CancelAllAbilities();
	ASC->ClearAllAbilities();
	ASC->AddLooseGameplayTag(UKYGameplayTags::CharacterState.IsDead);
	SetDead();
}



void AKYCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetDynamicMaterialInstance();
	AttackEventData.Instigator = this;

}

void AKYCharacterBase::GrantStartAbilities()
{
	for (const auto& StartAbility : StartAbilities)	// 기본 어빌리티 부여
	{
		GrantAbility(StartAbility);
	}
}

void AKYCharacterBase::GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass, float Level, bool bAddToTagMap)
{
	if (!IsValid(NewAbilityClass) || !ASC) return;
    
	// 이미 보유 중인 어빌리티인지 확인
	if (ASC->FindAbilitySpecFromClass(NewAbilityClass))	return;
    
	// 어빌리티 기본 객체 가져오기
	UGameplayAbility* AbilityDefaultObject = NewAbilityClass.GetDefaultObject();
	if (!AbilityDefaultObject)	return;
    
	// 어빌리티 스펙 생성 및 부여
	FGameplayAbilitySpec AbilitySpec(NewAbilityClass, Level);
	FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(AbilitySpec);

	// 태그 맵에 추가 (bAddToTagMap이 true이고 데이터 태그가 있는 경우)
	if (bAddToTagMap)
	{
		for (const FGameplayTag& Tag : AbilityDefaultObject->AbilityTags)
		{
			if (Tag.MatchesTag(UKYGameplayTags::Data.Data))
			{
				AbilitiesTagMap.Add(Tag, Handle);
				break;
			}
		}
	}
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

void AKYCharacterBase::InitializeAbilitySystemComponent()
{
	GrantStartAbilities();
	InitializeStatEffect();
	RegisterGameplayEvents();
}

void AKYCharacterBase::RegisterGameplayEvents() 
{
	// Blank
}

void AKYCharacterBase::OnCombatState(const FGameplayTag GameplayTag, int32 Count)
{
	bIsCombat = Count > 0;
	if (bIsCombat)
	{
		UpdateWeaponSocket();
		if (CurrentWeaponType == EWeaponType::DualBlade)
		{
			UpdateWeaponSocket(false, TEXT("Weapon_L"));
		}
	}
}

void AKYCharacterBase::ChangeWeapon(FName WeaponName)
{
	if (!OwnWeaponData.Contains(WeaponName))
	{
		KY_LOG(LogKY, Warning, TEXT("Can't find %s Weapon"), *WeaponName.ToString());
		return;
	}
	UpdateWeaponData(OwnWeaponData.FindRef(WeaponName));
}


void AKYCharacterBase::OnWeaponCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitIgnoreActors.Contains(OtherActor)) return;

	UAbilitySystemComponent* OtherASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor); 
	if (!OtherASC) return;
	
	AttackEventData.Target = OtherActor;
	AttackEventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherActor);
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, UKYGameplayTags::Event.AttackHit, AttackEventData);
	HitIgnoreActors.Add(OtherActor);
}

void AKYCharacterBase::UpgradeAbility(FGameplayTag& InGameplayTag)
{
	if (AbilitiesTagMap.Contains(InGameplayTag))
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilitiesTagMap.FindRef(InGameplayTag));
		if (Spec)
		{
			if (UKYGameplayAbility* UpgradedAbility = Cast<UKYGameplayAbility>(Spec->Ability))
			{
				Spec->Level += 1.0f;
				UpgradedAbility->OnAbilityLevelUp.Broadcast();
			}
			KY_LOG(LogKY, Log, TEXT("%s Ability is Upgraded."), *Spec->Ability.GetName());
		}
	}
}

void AKYCharacterBase::UpdateWeaponData_Implementation(FWeaponData NewWeapon)
{
	if (OwnWeaponAnimAssets.Contains(NewWeapon.Type))
	{
		WeaponCompLeft->SetStaticMesh(nullptr);
		WeaponCompRight->SetStaticMesh(nullptr);;
		
		CurrentWeaponType = NewWeapon.Type;
		WeaponCompRight->SetStaticMesh(NewWeapon.Mesh);
		if (CurrentWeaponType == EWeaponType::DualBlade)
		{
			WeaponCompLeft->SetStaticMesh(NewWeapon.Mesh);
		}
		WeaponCompRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		GetMesh()->LinkAnimClassLayers(OwnWeaponAnimAssets.FindRef(CurrentWeaponType)->WeaponAnimClass);
		
		switch (CurrentWeaponType)
		{
			case EWeaponType::None:
				break;
			case EWeaponType::Sword:
				UpdateWeaponSocket(true, TEXT("Weapon_OneSword"));
				break;
			case EWeaponType::GreatSword:
				UpdateWeaponSocket(true, TEXT("Weapon_GreatSword"));
				break;
			case EWeaponType::DualBlade:
				UpdateWeaponSocket(true, TEXT("Weapon_DualBlade_R"));
				UpdateWeaponSocket(false, TEXT("Weapon_DualBlade_L"));
				break;
			case EWeaponType::Scythe:
				break;
			case EWeaponType::Hammer:
				break;
			case EWeaponType::Spear:
				break;
		}
		OnWeaponAnimSetChanged.Broadcast();
	}
}

void AKYCharacterBase::UpdateWeaponSocket(bool IsRight, FName TargetSocket)
{
	if (IsRight) WeaponCompRight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TargetSocket);
	else
	{
		WeaponCompLeft->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TargetSocket);
	}
}


void AKYCharacterBase::SetDead_Implementation()
{
	bBlockInput = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadPawn"));
	StopAnimMontage();

	if (MeshDynamicMaterialInstance)    	GetMesh()->SetMaterial(0, MeshDynamicMaterialInstance);
	if (WeaponLeftDynamicMaterialInstance)	WeaponCompLeft->SetMaterial(0, WeaponLeftDynamicMaterialInstance);
	if (WeaponRightDynamicMaterialInstance)	WeaponCompRight->SetMaterial(0, WeaponRightDynamicMaterialInstance);
}

void AKYCharacterBase::SetDynamicMaterialInstance()
{
	if (IsValid(GetMesh()))								MeshDynamicMaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0);
	if (IsValid(WeaponCompRight->GetStaticMesh()))	WeaponRightDynamicMaterialInstance = WeaponCompRight->CreateDynamicMaterialInstance(0);
	if (IsValid(WeaponCompLeft->GetStaticMesh()))	WeaponLeftDynamicMaterialInstance = WeaponCompLeft->CreateDynamicMaterialInstance(0);
}
