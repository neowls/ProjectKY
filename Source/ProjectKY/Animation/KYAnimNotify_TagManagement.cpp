// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/KYAnimNotify_TagManagement.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UKYAnimNotify_TagManagement::ExecuteNotify(AActor* Owner)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		switch (Operation) {
		case ETagOperation::Add:
			ASC->AddLooseGameplayTags(Tags);
			break;
		case ETagOperation::Remove:
			ASC->RemoveLooseGameplayTags(Tags);
			break;
		case ETagOperation::SendEvent:
			for (const FGameplayTag& Tag : Tags)
			{
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, Tag, FGameplayEventData());
			}
			break;
		}
	}
}
