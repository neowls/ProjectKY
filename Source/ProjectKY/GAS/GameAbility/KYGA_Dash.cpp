// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Dash.h"
#include "ProjectKY.h"
#include "Components/CapsuleComponent.h"
#include "Character/KYCharacterPlayer.h"
#include "Library/KYBlueprintFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UKYGA_Dash::UKYGA_Dash()
{
	DashSpeed = 1500.0f;
}

void UKYGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AKYCharacterPlayer* Character = Cast<AKYCharacterPlayer>(ActorInfo->AvatarActor);
	if (Character)
	{
		PrepareDash(Character); // 캐릭터 충돌 무시 
		
		float ForwardAngle = UKYBlueprintFunctionLibrary::GetAngleFromDirection(Character->GetInputDirection(), Character->GetRotationOffset().RotateVector(Character->GetActorForwardVector()));
		
		if (ForwardAngle <= 45.0f) // 전방 회피
		{
			return;
		}
		if (ForwardAngle >= 125.0f) // 후방 회피
		{
			MontageJumpToSection(TEXT("Backward"));
		}
		else // 이후 좌,우로 구분
		{
			float RightAngle = UKYBlueprintFunctionLibrary::GetAngleFromDirection(Character->GetInputDirection(), Character->GetRotationOffset().RotateVector(Character->GetActorRightVector()));
			RightAngle -= 90.0f; 
			if (RightAngle < 0.0f)
			{
				MontageJumpToSection(TEXT("Right"));
			}
			else
			{
				MontageJumpToSection(TEXT("Left"));
			}
		}
	}
}

void UKYGA_Dash::PrepareDash(ACharacter* Character)
{
	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return;
	
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_ENEMY, ECR_Ignore);
}

void UKYGA_Dash::RestoreDash(ACharacter* Character)
{
	if (!Character) return;

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent) return;
	
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_ENEMY, ECR_Block);
}

void UKYGA_Dash::OnSimpleInterruptedCallback_Implementation()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	RestoreDash(Character);
	Super::OnSimpleInterruptedCallback_Implementation();
}

void UKYGA_Dash::OnSimpleCompleteCallback_Implementation()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	RestoreDash(Character);
	Super::OnSimpleCompleteCallback_Implementation();
}