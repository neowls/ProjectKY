// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_CheckGameplayTagsASC.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagAssetInterface.h"
#include "ProjectKY.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Character/KYCharacterBase.h"


UBTDecorator_CheckGameplayTagsASC::UBTDecorator_CheckGameplayTagsASC()
{
	
}

bool UBTDecorator_CheckGameplayTagsASC::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}
	
	if (AKYCharacterBase* CharacterBase = Cast<AKYCharacterBase>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorToCheck.GetSelectedKeyID())))
	{
		switch (TagsToMatch)
		{
		case EGameplayContainerMatchType::All:
			return CharacterBase->GetAbilitySystemComponent()->HasAllMatchingGameplayTags(GameplayTags);

		case EGameplayContainerMatchType::Any:
			return CharacterBase->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(GameplayTags);

		default:
			{
				UE_LOG(LogBehaviorTree, Warning, TEXT("Invalid value for TagsToMatch (EGameplayContainerMatchType) %d.  Should only be Any or All."), static_cast<int32>(TagsToMatch));
				return false;
			}
		}
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't find Character"));
		return false;
	}
	
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
