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
	InitializeObjectFinder(SubAttackAction, TEXT("/Game/_Dev/Input/IA_SubAttack.IA_SubAttack"));
	InitializeObjectFinder(UpperAttackAction, TEXT("/Game/_Dev/Input/IA_UpperAttack.IA_UpperAttack"));
	InitializeObjectFinder(SkillAction, TEXT("/Game/_Dev/Input/IA_Skill.IA_Skill"));
	InitializeObjectFinder(DefaultContext, TEXT("/Game/_Dev/Input/IMC_Player.IMC_Player"));

	TObjectPtr<USkeletalMesh> CharacterMesh = nullptr;

	InitializeObjectFinder(CharacterMesh, TEXT("/Game/NO_Cooking/Animation/GreatSword/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	
	GetMesh()->SetSkeletalMesh(CharacterMesh);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponComp->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	WeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

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
		}

		for (const auto& StartAbility : StartAbilities)	// 기본 어빌리티 부여
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)		// 입력 어빌리티 부여
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
			
		}

		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitySystem"));
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
	
	SetupGASInputComponent();
}


void AKYCharacterPlayer::SetupGASInputComponent()
{
	if(IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(MainAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(DashAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 3);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, 3);
		EnhancedInputComponent->BindAction(SubAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 4);
		EnhancedInputComponent->BindAction(UpperAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, 5);
	}
}

void AKYCharacterPlayer::Move(const FInputActionValue& Value)
{
	if(ASC->HasMatchingGameplayTag(KYTAG_CHARACTER_UNMOVABLE)) return;
	if(GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr)) GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
	FVector2D MovementVector = Value.Get<FVector2d>();

	//float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if(MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
	}
	
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	
	AddMovementInput(MoveDirection, MovementVectorSize);
	
}

void AKYCharacterPlayer::Rotate(const FInputActionValue& Value)
{
	AActor* Actor = nullptr;
	
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

