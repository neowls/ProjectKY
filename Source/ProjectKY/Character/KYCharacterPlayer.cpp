// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KYCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "Player/KYPlayerState.h"
#include "ProjectKY.h"
#include "Components/CapsuleComponent.h"


AKYCharacterPlayer::AKYCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InitializeObjectFinder(LookAction, TEXT("/Game/_Dev/Input/IA_Look.IA_Look"));
	InitializeObjectFinder(MoveAction, TEXT("/Game/_Dev/Input/IA_Move.IA_Move"));
	InitializeObjectFinder(DodgeAction, TEXT("/Game/_Dev/Input/IA_Dash.IA_Dash"));
	InitializeObjectFinder(GuardAction, TEXT("/Game/_Dev/Input/IA_Guard.IA_Guard"));
	InitializeObjectFinder(JumpAction, TEXT("/Game/_Dev/Input/IA_Jump.IA_Jump"));
	InitializeObjectFinder(GlideAction, TEXT("/Game/_Dev/Input/IA_Glide.IA_Glide"));
	InitializeObjectFinder(InteractAction, TEXT("/Game/_Dev/Input/IA_Interact.IA_Interact"));
	InitializeObjectFinder(LightAttackAction, TEXT("/Game/_Dev/Input/IA_LightAttack.IA_LightAttack"));
	InitializeObjectFinder(HeavyAttackAction, TEXT("/Game/_Dev/Input/IA_HeavyAttack.IA_HeavyAttack"));
	InitializeObjectFinder(SkillAttackAction, TEXT("/Game/_Dev/Input/IA_SkillAttack.IA_SkillAttack"));
	InitializeObjectFinder(UpperAttackAction, TEXT("/Game/_Dev/Input/IA_UpperAttack.IA_UpperAttack"));
	InitializeObjectFinder(RangeAttackAction, TEXT("/Game/_Dev/Input/IA_RangeAttack.IA_RangeAttack"));
	InitializeObjectFinder(ChangeWeaponAction, TEXT("/Game/_Dev/Input/IA_ChangeWeapon.IA_ChangeWeapon"));
	InitializeObjectFinder(DefaultContext, TEXT("/Game/_Dev/Input/IMC_Player.IMC_Player"));
	
	InitializeObjectFinder(PlayerLevelCurveTable, TEXT("/Game/_Dev/DataAsset/PlayerLevelStatTable.PlayerLevelStatTable"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 캐릭터가 회전한다.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	
	ItemTriggerComp = CreateDefaultSubobject<USphereComponent>(TEXT("DropCollectTrigger"));
	InteractTriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractTrigger"));
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	WingComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	ItemTriggerComp->SetupAttachment(RootComponent);
	InteractTriggerComp->SetupAttachment(RootComponent);
	WingComp->SetupAttachment(GetMesh(), TEXT("Wing"));

	ItemTriggerComp->SetSphereRadius(250.0f);
	ItemTriggerComp->SetCollisionProfileName(TEXT("ItemCollectTrigger"));
	
	InteractTriggerComp->SetBoxExtent(FVector(100.0f, 48.0f, 64.0f));
	InteractTriggerComp->SetRelativeLocation(FVector(140.0f, 0.0f, 0.0f));
	InteractTriggerComp->SetCollisionProfileName(TEXT("InteractTrigger"));

	
	WingComp->SetRelativeScale3D(FVector(1.5f, 1.3f, 1.3f));
	WingComp->SetComponentTickEnabled(false);
	WingComp->SetEnableGravity(false);
	WingComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WingComp->SetGenerateOverlapEvents(false);
	WingComp->SetVisibility(false);
	
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->TargetArmLength = 500;
	SpringArmComp->SetRelativeRotation(FRotator(-50.0f, RotationOffset.Yaw, 0.0f));
	CameraComp->SetupAttachment(SpringArmComp);


	WeaponCompLeft->SetCollisionProfileName(FName("PlayerWeapon"));
	WeaponCompRight->SetCollisionProfileName(FName("PlayerWeapon"));

	WeaponCompLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCompRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKYCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AKYPlayerState* PS = GetPlayerState<AKYPlayerState>();
	if(PS)
	{
		ASC = PS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(PS, this);
		if(const UKYAttributeSetPlayer* AttributeSetPlayer = ASC->GetSet<UKYAttributeSetPlayer>())
		{
			AttributeSetPlayer->OnOutOfHealth.AddDynamic(this, &ThisClass::OutOfHealth);	// 사망 델리게이트 바인딩
			AttributeSetPlayer->OnDamageTaken.AddDynamic(this, &ThisClass::DamageTaken);
			
			AttributeSetPlayer->PlayerLevelCurveTable = PlayerLevelCurveTable;
		}

		InitializeAbilitySystemComponent();
		SetupGASInputComponent();

		ASC->RegisterGameplayTagEvent(UKYGameplayTags::CharacterState.IsCombat).AddUObject(this, &ThisClass::OnCombatState);

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		if (bShowGASDebug)
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
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::InteractObject);
	EnhancedInputComponent->BindAction(ChangeWeaponAction, ETriggerEvent::Triggered, this, &ThisClass::ChangeWeaponWithType);
	
	SetupGASInputComponent();
}

void AKYCharacterPlayer::SetupGASInputComponent()
{
	if(IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Common_Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, UKYGameplayTags::Data.Ability_Common_Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_DoubleJump);
		EnhancedInputComponent->BindAction(GlideAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Glide);
		EnhancedInputComponent->BindAction(GlideAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, UKYGameplayTags::Data.Ability_Player_Glide);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Focus);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, UKYGameplayTags::Data.Ability_Player_Focus);
		EnhancedInputComponent->BindAction(LightAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Attack_Light);
		EnhancedInputComponent->BindAction(LightAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Attack_Air); // Air Attack Ability
		EnhancedInputComponent->BindAction(HeavyAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Attack_Heavy);
		EnhancedInputComponent->BindAction(UpperAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Attack_Upper);
		EnhancedInputComponent->BindAction(RangeAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Attack_Range);
		EnhancedInputComponent->BindAction(SkillAttackAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Attack_Special);
		EnhancedInputComponent->BindAction(DodgeAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Dodge);
		EnhancedInputComponent->BindAction(GuardAction,ETriggerEvent::Triggered, this, &ThisClass::GASInputPressed, UKYGameplayTags::Data.Ability_Player_Guard);
		EnhancedInputComponent->BindAction(GuardAction,ETriggerEvent::Completed, this, &ThisClass::GASInputReleased, UKYGameplayTags::Data.Ability_Player_Guard);
	}
}

void AKYCharacterPlayer::GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass, float Level, bool bAddToTagMap)
{
	return Super::GrantAbility(NewAbilityClass, Level,true);
}

FVector AKYCharacterPlayer::GetInputDirection() const
{
	if (GetCharacterMovement()->GetCurrentAcceleration().IsZero())
	{
		return GetActorForwardVector();
	}
	return RotationOffset.RotateVector(GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal2D());
}

void AKYCharacterPlayer::AddWeaponData(const FName& WeaponName, const FWeaponData& InWeaponData)
{
	Super::AddWeaponData(WeaponName, InWeaponData);
	KY_LOG(LogKY, Log, TEXT("Equipped Weapon Num : %d"), EquippedWeapon.Num());
	if (EquippedWeapon.Num() < 3)
	{
		EquippedWeapon.Add(WeaponName);
	}
}

void AKYCharacterPlayer::OnCombatState(const FGameplayTag GameplayTag, int32 Count)
{
	Super::OnCombatState(GameplayTag, Count);
	if (Count < 1)
	{
		PlayAnimMontage(GetAnimMontageData(UKYGameplayTags::Data.Ability_Common_Combat).Montage, 1.0f, TEXT("NonCombat"));
	}
}

void AKYCharacterPlayer::Move(const FInputActionValue& Value)
{
	if(ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.Unmovable) || ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.Unstable)) return;	// 해당 태그 부착시 캐릭터 이동 제한
	
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
	MoveDirection = RotationOffset.RotateVector(MoveDirection);

	bool bIsPlayingMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr);
	if(bIsPlayingMontage && !ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.MoveMontageEnabled) && ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsCombat)) // 재생중인 몽타주가 있다면 중단한다.
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);	
	}
	
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AKYCharacterPlayer::Rotate(const FInputActionValue& Value)		// 태그가 있는 동안 입력 방향으로 캐릭터가 회전한다.
{
	if(!ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsRotatable)) return;
	FVector2D MovementVector = Value.Get<FVector2d>();

	FVector Direction = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	Direction = RotationOffset.RotateVector(Direction);
	
	if(!MovementVector.IsNearlyZero())
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = Direction.Rotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 25.0f);
		SetActorRotation(NewRotation);
	}
}

void AKYCharacterPlayer::ChangeWeaponWithType(const FInputActionValue& Value)
{
	if(ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsCombat)) return;
	float AxisValue = Value.Get<float>();
	int32 WeaponIndex = FMath::RoundToInt(AxisValue);
	
	if(EquippedWeapon.IsValidIndex(WeaponIndex - 1))
	{
		ChangeWeapon(EquippedWeapon[WeaponIndex - 1]);
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

void AKYCharacterPlayer::GASInputPressed(FGameplayTag InputTag)
{
	if (!AbilitiesTagMap.Contains(InputTag)) return;
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilitiesTagMap[InputTag]);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AKYCharacterPlayer::GASInputReleased(FGameplayTag InputTag)
{
	if (!AbilitiesTagMap.Contains(InputTag)) return;
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(AbilitiesTagMap[InputTag]);
	if(Spec)
	{
		Spec->InputPressed = false;
		if(Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}


void AKYCharacterPlayer::SetDead_Implementation()
{
	Super::SetDead_Implementation();
	UAnimMontage* MontageToPlay = GetAnimMontageData(UKYGameplayTags::Data.Ability_Common_Dead).Montage;
	if (MontageToPlay) PlayAnimMontage(MontageToPlay);
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Montage Data Is Empty!"));
	}
}
