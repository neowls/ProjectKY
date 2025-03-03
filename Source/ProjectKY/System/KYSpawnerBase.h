
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KYSpawnerBase.generated.h"

UCLASS()
class PROJECTKY_API AKYSpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AKYSpawnerBase();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	TArray<FVector> GeneratePoissonDiskSamples(FVector Origin, float Radius, float MinDistance, float MinCenterOffSet, int MaxTries);

	UFUNCTION(BlueprintCallable)
	void SpawnCharacter();


protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Components")
	TObjectPtr<class USphereComponent> SpawnArea;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<class AKYCharacterNonPlayer> SpawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn")
	uint8 SpawnAmount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn")
	uint8 SpawnLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn")
	bool bSpawnMultiple = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn")
	float MinSpawnDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn")
	float CenterOffset = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn",meta = (EditCondition = "bSpawnMultiple", EditConditionHides))
	uint8 MultipleSpawnCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Spawn",meta = (EditCondition = "bSpawnMultiple", EditConditionHides))
	float MultipleSpawnRate = 10.0f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Spawn")
	TArray<FVector> DebugSpawnPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bDebugSpawn = true;
};
