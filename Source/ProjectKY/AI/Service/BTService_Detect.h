// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UBTService_Detect : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_Detect();

protected:
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Service)
	float DetectRadius = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool bShowDebugInfo = true;

private:
	
	UPROPERTY()
	mutable APawn* CachedAIPawn = nullptr;
};
