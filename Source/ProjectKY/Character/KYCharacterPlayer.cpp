// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ProjectKY.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Attribute/KYAttributeSetHealth.h"
#include "GAS/Attribute/KYAttributeSetStance.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "Player/KYPlayerState.h"


AKYCharacterPlayer::AKYCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitializeObjectFinder(LookAction, TEXT("/Game/_Dev/Input/IA_Look.IA_Look"));
	InitializeObjectFinder(MoveAction, TEXT("/Game/_Dev/Input/IA_Move.IA_Move"));
	InitializeObjectFinder(WalkAction, TEXT("/Game/_Dev/Input/IA_Walk.IA_Walk"));
	InitializeObjectFinder(SprintAction, TEXT("/Game/_Dev/Input/IA_Sprint.IA_Sprint"));
	InitializeObjectFinder(DashAction, TEXT("/Game/_Dev/Input/IA_Dash.IA_Dash"));
	InitializeObjectFinder(JumpAction, TEXT("/Game/_Dev/Input/IA_Jump.IA_Jump"));
	InitializeObjectFinder(InteractAction, TEXT("/Game/_Dev/Input/IA_Interact.IA_Interact"));
	InitializeObjectFinder(MainAttackAction, TEXT("/Game/_Dev/Input/IA_MainAttack.IA_MainAttack"));
	InitializeObjectFinder(SkillAttackAction, TEXT("/Game/_Dev/Input/IA_SubAttack.IA_SubAttack"));
	InitializeObjectFinder(UpperAttackAction, TEXT("/Game/_Dev/Input/IA_UpperAttack.IA_UpperAttack"));
	InitializeObjectFinder(SkillAction, TEXT("/Game/_Dev/Input/IA_Skill.IA_Skill"));
	InitializeObjectFinder(DefaultContext, TEXT("/Game/_Dev/Input/IMC_Player.IMC_Player"));

	TObjectPtr<USkeletalMesh> CharacterMesh = nullptr;

	InitializeObjectFinder(CharacterMesh, TEXT("/Game/NO_Cooking/Animation/GreatSword/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	
	GetMesh()->SetSkeletalMesh(CharacterMesh);

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 캐릭터가 회전한다.
	

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;

	SpringArmComp->TargetArmLength = 800;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void AKYCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AKYPlayerState* PS = GetPlayerState<AKYPlayerState>();
	if(PS)
	{
		ASC = PS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(PS, this);
		const UKYAttributeSetHealth* AttributeSetHealth = ASC->GetSet<UKYAttributeSetHealth>();
		if(AttributeSetHealth)
		{
			AttributeSetHealth->OnOutOfHealth.AddDynamic(this, &ThisClass::OutOfHealth);	// 사망 델리게이트 바인딩
			AttributeSetHealth->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);
		}

		const UKYAttributeSetStance* AttributeSetStance = ASC->GetSet<UKYAttributeSetStance>();
		if (AttributeSetStance)
		{
			AttributeSetStance->OnStanceChange.AddDynamic(this, &ThisClass::OnStanceEvent);
		}

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, 1.0f, EffectContextHandle); // 이펙트 부여
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
		
		GiveStartAbilities();
		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitySystem"));	// 어빌리티 시스템 디버깅
	}
}

void AKYCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CastChecked<APlayerController>(GetController())->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if(DefaultContext)
		{
			Subsystem->AddMappingContext(DefaultContext, 0);
		}
	}
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Rotate);
	
	SetupGASInputComponent();
}


void AKYCharacterPlayer::SetupGASInputComponent()
{
	if(IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, 1);
		EnhancedInputComponent->BindAction(MainAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 2);	// 하나는 지상, 하나는 공중 공격 어빌리티
		EnhancedInputComponent->BindAction(MainAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 3);
		EnhancedInputComponent->BindAction(UpperAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 4);
		EnhancedInputComponent->BindAction(SkillAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 5);
		EnhancedInputComponent->BindAction(DashAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 6);
	}
}

void AKYCharacterPlayer::GiveStartAbilities()
{
	Super::GiveStartAbilities();
	for (const auto& StartInputAbility : StartInputAbilities)		// 입력 어빌리티 부여
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = StartInputAbility.Key;
		ASC->GiveAbility(StartSpec);
	}
}


void AKYCharacterPlayer::Move(const FInputActionValue& Value)
{
	if(ASC->HasMatchingGameplayTag(KYTAG_CHARACTER_UNMOVABLE)) return;	// 해당 태그 부착시 캐릭터 이동 제한
	if(GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr)) GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);	// 재생중인 몽타주가 있다면 중단한다.
	
	FVector2D MovementVector = Value.Get<FVector2d>(); // X, Y 입력 벡터 저장
	
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SizeSquared();		// 이동 속도 제곱 연산
	if(MovementVectorSizeSquared > 1.0f)	// 조이 스틱 사용시 1.0f 아래의 미세값들을 위한 연산
	{
		MovementVector.Normalize(); // 이동 벡터 정규화
	}
	
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);	// 이동 속도 제곱근 연산
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);	// 이동 방향 벡터 연산
	
	AddMovementInput(MoveDirection, MovementVectorSize);
}


void AKYCharacterPlayer::Rotate(const FInputActionValue& Value)		// 태그가 있는 동안 입력 방향으로 캐릭터가 회전한다.
{
	if(!ASC->HasMatchingGameplayTag(KYTAG_CHARACTER_ROTABLE)) return;
	FVector2D MovementVector = Value.Get<FVector2d>();

	FVector Direction = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	if(!MovementVector.IsNearlyZero())
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 25.0f);
		SetActorRotation(NewRotation);
	}
}


void AKYCharacterPlayer::GASInputPressed(int32 InputId)		// 입력시 어빌리티 InputID에 맞게 발동시킨다.
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if(Spec)
	{
		Spec->InputPressed = true;
		if(Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AKYCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if(Spec)
	{
		Spec->InputPressed = false;
		if(Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AKYCharacterPlayer::SetDead()
{
	Super::SetDead();
	PlayAnimMontage(DeathMontage);
}

