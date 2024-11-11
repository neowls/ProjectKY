// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Player/GASPlayerState.h"
#include "GAS.h"
#include "Attribute/GASCharacterAttributeSet.h"
#include "Tag/GASGameplayTag.h"


AGASCharacterPlayer::AGASCharacterPlayer()
{
	InitializeObjectFinder(LookAction, TEXT("/Game/_Dev/Input/IA_Look.IA_Look"));
	InitializeObjectFinder(MoveAction, TEXT("/Game/_Dev/Input/IA_Move.IA_Move"));
	InitializeObjectFinder(CrouchAction, TEXT("/Game/_Dev/Input/IA_Crouch.IA_Crouch"));
	InitializeObjectFinder(WalkAction, TEXT("/Game/_Dev/Input/IA_Walk.IA_Walk"));
	InitializeObjectFinder(SprintAction, TEXT("/Game/_Dev/Input/IA_Sprint.IA_Sprint"));
	InitializeObjectFinder(JumpAction, TEXT("/Game/_Dev/Input/IA_Jump.IA_Jump"));
	InitializeObjectFinder(InteractAction, TEXT("/Game/_Dev/Input/IA_Interact.IA_Interact"));
	InitializeObjectFinder(MainAttackAction, TEXT("/Game/_Dev/Input/IA_MainAttack.IA_MainAttack"));
	InitializeObjectFinder(SubAttackAction, TEXT("/Game/_Dev/Input/IA_SubAttack.IA_SubAttack"));
	InitializeObjectFinder(SkillAction, TEXT("/Game/_Dev/Input/IA_Skill.IA_Skill"));
	InitializeObjectFinder(DefaultContext, TEXT("/Game/_Dev/Input/IMC_Player.IMC_Player"));
	InitializeObjectFinder(MainAttackMontage, TEXT("/Game/_Assets/ParagonKhaimera/Characters/Heroes/Khaimera/Animations/Montage/Melee_Combo_Montage.Melee_Combo_Montage"));
	
}

void AGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Look();
}

void AGASCharacterPlayer::Move(const FInputActionValue& Value)
{
	if(ASC->GetOwnedGameplayTags().HasTag(GASTAG_CHARACTER_ISATTACKING) || ASC->GetOwnedGameplayTags().HasTag(GASTAG_CHARACTER_ISSKILLING)) return;
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

void AGASCharacterPlayer::Look() // 캐릭터 방향 전환 TODO: 추후 변경
{
	FVector WorldMouseLocation,	WorldMouseDirection;
	if(GetLocalViewingPlayerController()->DeprojectMousePositionToWorld(WorldMouseLocation, WorldMouseDirection))
	{
		FVector CharacterLocation = GetActorLocation();

		float Distance = FVector::DotProduct((WorldMouseLocation - CharacterLocation), FVector::UpVector) / FVector::DotProduct(WorldMouseDirection, FVector::UpVector);
		FVector IntersectionPoint = WorldMouseLocation - (WorldMouseDirection * Distance);
		
		FRotator TargetRotation = (IntersectionPoint - CharacterLocation).Rotation();
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;
		LOG_SCREEN(0, TEXT("Yaw : %f"), TargetRotation.Yaw);
		
		GetController()->SetControlRotation(TargetRotation);
	}
}

void AGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AGASPlayerState* GASPS = GetPlayerState<AGASPlayerState>();
	if(GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);
		const UGASCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UGASCharacterAttributeSet>();
		if(CurrentAttributeSet)
		{
			CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		}
		for(const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for(const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
			
		}
		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AGASCharacterPlayer::Move);

	SetupGASInputComponent();
}

void AGASCharacterPlayer::SetupGASInputComponent()
{
	if(IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered, this, &AGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed, this, &AGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(MainAttackAction,ETriggerEvent::Triggered, this, &AGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(SkillAction,ETriggerEvent::Triggered, this, &AGASCharacterPlayer::GASInputPressed, 2);
	}
}

void AGASCharacterPlayer::GASInputPressed(int32 InputId)
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

void AGASCharacterPlayer::GASInputReleased(int32 InputId)
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



