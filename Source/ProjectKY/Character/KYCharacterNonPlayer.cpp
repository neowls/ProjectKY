// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterNonPlayer.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "AI/KYAIController.h"
#include "Components/CapsuleComponent.h"
#include "GAS/Attribute/KYAttributeSetEnemy.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "UI/KYWidgetComponent.h"
#include "UI/KYUserWidget.h"

AKYCharacterNonPlayer::AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetEnemy = CreateDefaultSubobject<UKYAttributeSetEnemy>(TEXT("AttributeSetEnemy"));
	AIControllerClass = AKYAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	HPBar = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("Widget"));
	TargetedWidget = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("Targeted"));
	
	HPBar->SetupAttachment(GetMesh());
	TargetedWidget->SetupAttachment(GetMesh(), TEXT("pelvis"));

	HPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetedWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	TSubclassOf<UUserWidget> HPBarWidgetClass;
	InitializeClassFinder(HPBarWidgetClass, TEXT("/Game/_Dev/UI/WBP/WBP_EnemyHPBar.WBP_EnemyHPBar_C"));
	
	HPBar->SetWidgetClass(HPBarWidgetClass);
	HPBar->SetDrawSize(FVector2D(150.0f, 20.0f));
	

	TSubclassOf<UUserWidget> TargetedWidgetClass;
	InitializeClassFinder(TargetedWidgetClass, TEXT("/Game/_Dev/UI/WBP/WBP_TargetPoint.WBP_TargetPoint_C"));
	
	TargetedWidget->SetWidgetClass(TargetedWidgetClass);
	TargetedWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
	TargetedWidget->SetVisibility(false);

	WeaponCompLeft->SetCollisionProfileName(FName("EnemyWeapon"));
	WeaponCompRight->SetCollisionProfileName(FName("EnemyWeapon"));

	WeaponCompLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCompRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}


void AKYCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ASC->InitAbilityActorInfo(this, this);

	CurrentWeaponType = OwnWeaponAnimAssets.begin().Key();
	
	AttributeSetEnemy->OnOutOfHealth.AddDynamic(this, &ThisClass::OutOfHealth);
	AttributeSetEnemy->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);
	AttributeSetEnemy->DropBountyEffect = DropBountyEffect;
	AttributeSetEnemy->InitDropGold(10.0f);
	InitializeAbilitySystemComponent();

	OnWeaponAnimSetChanged.Broadcast();
}

void AKYCharacterNonPlayer::RegisterGameplayEvents()
{
	ASC->RegisterGameplayTagEvent(UKYGameplayTags::CharacterState.Unstable).AddUObject(this, &ThisClass::OnHitTagChanged);
	ASC->RegisterGameplayTagEvent(UKYGameplayTags::CharacterState.Executable).AddUObject(this, &ThisClass::OnExecutableTagChanged);
	bIsCombat = true;
}

bool AKYCharacterNonPlayer::ExecuteGameplayAbilityFromClass(TSubclassOf<UGameplayAbility> InAbilityClass)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(InAbilityClass);
	return ASC->TryActivateAbility(Spec->Handle);
}

void AKYCharacterNonPlayer::PlayExecutedMontage(FName SectionName, UAnimMontage* MontageToPlay)
{
	HPBar->Deactivate();
	HPBar->SetHiddenInGame(true);
	TargetedWidget->Deactivate();
	TargetedWidget->SetHiddenInGame(true);
	
	GetMesh()->GetAnimInstance()->StopAllMontages(0.01f);
	GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay);
	if (SectionName != TEXT("0")) GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName);
	
	GetCapsuleComponent()->SetCollisionProfileName("IgnoreOnlyPawn");
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKYCharacterNonPlayer::OnExecutableTagChanged(FGameplayTag GameplayTag, int Count)
{
	OnExecutableState(Count > 0);
}

void AKYCharacterNonPlayer::GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass, float Level, bool bAddToTagMap)
{
	return Super::GrantAbility(NewAbilityClass, Level, bAddToTagMap);
}

void AKYCharacterNonPlayer::OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	AKYAIController* AIController = Cast<AKYAIController>(GetController());
	if (AIController)
	{
		AIController->SetHitStatus(NewCount>0);
		
	}
}

void AKYCharacterNonPlayer::OutOfHealth()
{
	Super::OutOfHealth();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
		
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DropBountyEffect, AttributeSetEnemy->GetLevel(), EffectContextHandle); // 이펙트 부여
	if (EffectSpecHandle.IsValid())
	{
		if (AActor* TargetActor = GetController<AKYAIController>()->GetTargetActor())
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AKYCharacterNonPlayer::UpdateTargetedStatus(bool InStatus)
{
	TargetedWidget->SetVisibility(InStatus);
}

void AKYCharacterNonPlayer::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer,
	float Damage)
{
	Super::DamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
	UpdateHitFlash();
}

void AKYCharacterNonPlayer::SetDead_Implementation()
{
	Super::SetDead_Implementation();
	
	HPBar->Deactivate();
	HPBar->SetHiddenInGame(true);
	TargetedWidget->Deactivate();
	TargetedWidget->SetHiddenInGame(true);
	

	AKYAIController* AIController = Cast<AKYAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
		KY_LOG(LogKY, Log, TEXT("Stop AI"));
	}

	FVector ImpulseDirection = GetActorRotation().Vector() * -1.0f;
	ImpulseDirection.Normalize();

	float ImpulseStrength = 2000.0f;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;
	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse);
	
	
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
	[&]()
	{
		Destroy();
	}
	), 2.0f, false);
}

void AKYCharacterNonPlayer::DropBountyItem()
{
	
}




