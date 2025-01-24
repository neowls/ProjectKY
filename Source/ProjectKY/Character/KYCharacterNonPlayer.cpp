// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "AI/KYAIController.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "UI/KYWidgetComponent.h"
#include "UI/KYUserWidget.h"

AKYCharacterNonPlayer::AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetHealth = CreateDefaultSubobject<UKYAttributeSetHealth>(TEXT("AttributeSetHealth"));
	//AttributeSetStance = CreateDefaultSubobject<UKYAttributeSetStance>(TEXT("AttributeSetStance"));
	AIControllerClass = AKYAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPBar = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("Widget"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	TSubclassOf<UUserWidget> HPBarWidgetClass;
	InitializeClassFinder(HPBarWidgetClass, TEXT("/Game/_Dev/UI/WBP_EnemyHPBar.WBP_EnemyHPBar_C"));
	
	HPBar->SetWidgetClass(HPBarWidgetClass);
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	HPBar->SetDrawSize(FVector2D(150.0f, 20.0f));
	HPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TargetedWidget = CreateDefaultSubobject<UKYWidgetComponent>(TEXT("Targeted"));
	TargetedWidget->SetupAttachment(GetMesh());
	TargetedWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	TSubclassOf<UUserWidget> TargetedWidgetClass;
	InitializeClassFinder(TargetedWidgetClass, TEXT("/Game/_Dev/UI/WBP_TargetPoint.WBP_TargetPoint_C"));
	
	TargetedWidget->SetWidgetClass(TargetedWidgetClass);
	TargetedWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetedWidget->SetDrawSize(FVector2D(50.0f, 50.0f));
	TargetedWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetedWidget->SetVisibility(false);
}

void AKYCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC->InitAbilityActorInfo(this, this);

	
	AttributeSetHealth->OnOutOfHealth.AddDynamic(this, &AKYCharacterNonPlayer::OutOfHealth);
	AttributeSetHealth->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);
	
	//AttributeSetStance->OnStanceChange.AddDynamic(this, &ThisClass::OnStanceEvent);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, 1.0f, EffectContextHandle); // 이펙트 부여
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
	
	GiveStartAbilities();

	ASC->RegisterGameplayTagEvent(KYTAG_CHARACTER_UNSTABLE).AddUObject(this, &ThisClass::OnHitTagChanged);
}

void AKYCharacterNonPlayer::UpdateMotionWarpToTransform_Implementation(FTransform InTransform)
{
	
}


void AKYCharacterNonPlayer::SetDead()
{
	Super::SetDead();
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


