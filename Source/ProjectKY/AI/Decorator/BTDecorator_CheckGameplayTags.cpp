// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_CheckGameplayTags.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "AI/KYAI.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_CheckGameplayTags::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = Cast<UAbilitySystemComponent>(BlackboardComp->GetValueAsObject(BBKEY_ASC));
	if (ASC == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Find ASC"));
		return false;
	}

	if (ASC->HasAnyMatchingGameplayTags(GameplayTags))
	{
		return true;
	}

	return false;
}
