// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Struct/KYStruct.h"
#include "KYGameSingleton.generated.h"

class UKYAbilityDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UKYGameSingleton();
	
	static UKYGameSingleton& Get();

	UFUNCTION(blueprintCallable, Category = "Singleton")
	static UKYGameSingleton* GetSingleton();
	
	UFUNCTION(BlueprintCallable,  Category = "Ability System")
	FGameplayAbilityDataTag GetAbilityByTag(const FGameplayTag& AbilityTag) const;

	UFUNCTION(BlueprintCallable,  Category = "Ability System")
	TSubclassOf<UGameplayAbility> GetAbilityByName(const FName& InName);

	UFUNCTION(BlueprintCallable,  Category = "Ability System")
	FWeaponData GetWeaponDataByRowName(const FName& InName) const;

private:
	UPROPERTY()
	TMap<FGameplayTag, FGameplayAbilityDataTag> AbilityTagMap;

	UPROPERTY()
	TMap<FName, FWeaponData> WeaponDataMap;

	UPROPERTY()
	TObjectPtr<UKYAbilityDataAsset> AbilityNameAsset;
	
	void LoadAbilityDataFromTable(const UDataTable* InDataTable);

	void LoadWeaponDataFromTable();

	UPROPERTY()
	TObjectPtr<UDataTable> WeaponDataTable;
};


