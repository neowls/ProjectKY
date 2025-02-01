// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "AI/KYAIController.h"
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

	WeaponComp->SetupAttachment(GetMesh(), TEXT("weapon_r"));

	HPBar = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("Widget"));
	TargetedWidget = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("Targeted"));
	
	HPBar->SetupAttachment(GetMesh());
	TargetedWidget->SetupAttachment(GetMesh());
	
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	TargetedWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	TSubclassOf<UUserWidget> HPBarWidgetClass;
	InitializeClassFinder(HPBarWidgetClass, TEXT("/Game/_Dev/UI/WBP_EnemyHPBar.WBP_EnemyHPBar_C"));
	
	HPBar->SetWidgetClass(HPBarWidgetClass);
	HPBar->SetDrawSize(FVector2D(150.0f, 20.0f));
	

	TSubclassOf<UUserWidget> TargetedWidgetClass;
	InitializeClassFinder(TargetedWidgetClass, TEXT("/Game/_Dev/UI/WBP_TargetPoint.WBP_TargetPoint_C"));
	
	TargetedWidget->SetWidgetClass(TargetedWidgetClass);
	TargetedWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
	TargetedWidget->SetVisibility(false);
}

void AKYCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC->InitAbilityActorInfo(this, this);

	
	AttributeSetEnemy->OnOutOfHealth.AddDynamic(this, &AKYCharacterNonPlayer::OutOfHealth);
	AttributeSetEnemy->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);

	InitializeStatEffect();
	
	GiveStartAbilities();

	ASC->RegisterGameplayTagEvent(KYTAG_CHARACTER_UNSTABLE).AddUObject(this, &ThisClass::OnHitTagChanged);
}

void AKYCharacterNonPlayer::UpdateMotionWarpToTransform_Implementation(FVector InLocation)
{
	
}


void AKYCharacterNonPlayer::SetDead_Implementation()
{
	Super::SetDead();
	
	HPBar->SetVisibility(false);
	
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

void AKYCharacterNonPlayer::DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer,
	float Damage)
{
	Super::DamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
	UpdateHitFlash();
}

void AKYCharacterNonPlayer::OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	AKYAIController* AIController = Cast<AKYAIController>(GetController());
	if (AIController)
	{
		AIController->SetHitStatus(NewCount>0);
	}
}

void AKYCharacterNonPlayer::UpdateTargetedStatus(bool InStatus)
{
	TargetedWidget->SetVisibility(InStatus);
}


