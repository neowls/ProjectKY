
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "KYItemObjectBase.generated.h"

class UKYGameplayAbility;

UCLASS()
class PROJECTKY_API AKYItemObjectBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKYItemObjectBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(float Magnitude = 1.0f);

	UFUNCTION(BlueprintCallable)
	void GrantAbilityToTarget(float Magnitude = 1.0f);

	UFUNCTION(BlueprintImplementableEvent)
	void AbilityGranted();

	UFUNCTION(BlueprintImplementableEvent)
	void EffectGranted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetItem();

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
	TArray<TSubclassOf<class UGameplayEffect>> ApplyEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=GAS)
	TArray<TSubclassOf<UKYGameplayAbility>> GrantAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GAS)
	TObjectPtr<AActor> TargetActor;
};
