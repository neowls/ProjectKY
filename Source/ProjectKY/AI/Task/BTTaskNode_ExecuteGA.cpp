// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_ExecuteGA.h"
#include "AbilitySystemComponent.h"
#include "AI/KYAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_ExecuteGA::UBTTaskNode_ExecuteGA()
{
	NodeName = "Execute Gameplay Ability";
}

EBTNodeResult::Type UBTTaskNode_ExecuteGA::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_ASC));
	if(ASC)
	{
		ASC->TryActivateAbilityByClass(AbilityToActivate);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
