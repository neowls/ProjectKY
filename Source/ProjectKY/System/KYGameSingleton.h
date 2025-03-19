// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "UObject/NoExportTypes.h"
#include "Struct/KYStruct.h"
#include "KYGameSingleton.generated.h"

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
	FWeaponData GetWeaponDataByRowName(const FName& InName) const;

	TArray<FKYAbilityData*> AbilityDataRows();

private:
	UPROPERTY()
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> AbilityDataTable;
};


