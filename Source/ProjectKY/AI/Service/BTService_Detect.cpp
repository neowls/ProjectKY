// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "AI/KYAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControlledPawn == nullptr) return;
	UWorld* World = ControlledPawn->GetWorld();
	FVector Center = ControlledPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if(World == nullptr) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, ControlledPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DetectRadius),
		Params);
	if(bResult)
	{
		for(auto const& OverlapResult : OverlapResults)
		{
			ACharacter* Character = Cast<ACharacter>(OverlapResult.GetActor());
			if(Character && Character->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AKYAIController::TargetKey, Character);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControlledPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AKYAIController::TargetKey, nullptr);
	}
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
