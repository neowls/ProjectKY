// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_RotateToTarget.h"

#include "AI/KYAIController.h"
#include "AI/KYAI.h"
#include "ProjectKY.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_RotateToTarget::UBTTaskNode_RotateToTarget()
{
	NodeName = "RotateToTarget";
}

EBTNodeResult::Type UBTTaskNode_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControlledPawn == nullptr)
	{
		KY_LOG(LogKY, Log, TEXT("Can't find Controlled Pawn"));
		return Result = EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	FVector TargetDirection = Target->GetActorLocation() - ControlledPawn->GetActorLocation();
	FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetDirection.Rotation(), GetWorld()->GetDeltaSeconds(), 25.0f);
	
	ControlledPawn->SetActorRotation(NewRotation);

	Result = EBTNodeResult::Succeeded;
	
	return Result;
}
