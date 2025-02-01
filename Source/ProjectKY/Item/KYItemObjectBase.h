
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "KYItemObjectBase.generated.h"

UCLASS()
class PROJECTKY_API AKYItemObjectBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKYItemObjectBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTargetActor(AActor* InTargetActor);

protected:
	virtual void PostInitializeComponents() override;


protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Mesh)
	TObjectPtr<class UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GAS)
	TSubclassOf<class UGameplayEffect> EffectToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GAS)
	TObjectPtr<AActor> TargetActor;
};
