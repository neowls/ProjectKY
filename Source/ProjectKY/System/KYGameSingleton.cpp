// Fill out your copyright notice in the Description page of Project Settings.


#include "System/KYGameSingleton.h"
#include "ProjectKY.h"
#include "Data/KYAbilityDataAsset.h"

UKYGameSingleton::UKYGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/_Dev/DataTable/DT_AbilityByTag.DT_AbilityByTag"));
	if (DataTableRef.Object != nullptr)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);
		LoadAbilityDataFromTable(DataTable);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableRef(TEXT("/Game/_Dev/DataTable/DT_MeleeWeapon.DT_MeleeWeapon"));
	if (WeaponDataTableRef.Object != nullptr)
	{
		WeaponDataTable = WeaponDataTableRef.Object;
		check(WeaponDataTable->GetRowMap().Num() > 0);
		LoadWeaponDataFromTable();
	}

	static ConstructorHelpers::FObjectFinder<UKYAbilityDataAsset> DataMapRef(TEXT("/Game/_Dev/DataAsset/DA_PlayerAbilityClassData.DA_PlayerAbilityClassData"));
	if (DataMapRef.Object != nullptr)
	{
		AbilityNameAsset = DataMapRef.Object;
	}
}

UKYGameSingleton& UKYGameSingleton::Get()
{
	UKYGameSingleton* Singleton = CastChecked<UKYGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	KY_LOG(LogKY, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UKYGameSingleton>();
}

UKYGameSingleton* UKYGameSingleton::GetSingleton()
{
	UKYGameSingleton* Singleton = CastChecked<UKYGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return Singleton;
	}

	KY_LOG(LogKY, Error, TEXT("Invalid Game Singleton"));
	return NewObject<UKYGameSingleton>();
}


void UKYGameSingleton::LoadAbilityDataFromTable(const UDataTable* InDataTable)
{
	if (InDataTable)
	{
		
		AbilityTagMap.Empty();

		TArray<FGameplayAbilityDataTag*> AllRows;
		InDataTable->GetAllRows(TEXT("UKYAbilityManager"), AllRows);

		for(auto* Row : AllRows)
		{
			if(Row && Row->AbilityTag.IsValid())
			{
				if(AbilityTagMap.Contains(Row->AbilityTag))
				{
					KY_LOG(LogKY, Warning, TEXT("Duplicated Ability Tag detected: %s"), *Row->AbilityTag.ToString());
					continue;
				}
				AbilityTagMap.Add(Row->AbilityTag, *Row);
			}
		}
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Failed to Load Ability Data Table"));
	}
}

void UKYGameSingleton::LoadWeaponDataFromTable()
{
	static const FString ContextString(TEXT("Weapon Data Table Context"));
	TArray<FName> RowNames = WeaponDataTable->GetRowNames();
	
	for (auto& RowName : RowNames)
	{
		FWeaponData* RowData = WeaponDataTable->FindRow<FWeaponData>(RowName,ContextString);
		if (RowData)
		{
			WeaponDataMap.Add(RowName, *RowData);
		}
	}
}

FGameplayAbilityDataTag UKYGameSingleton::GetAbilityByTag(const FGameplayTag& AbilityTag) const
{
	if (const auto FoundRow = AbilityTagMap.Find(AbilityTag))
	{
		return *FoundRow;
	}

	KY_LOG(LogKY, Warning, TEXT("Can't Find Ability With This Tag : %s"), *AbilityTag.ToString());
	return FGameplayAbilityDataTag();
}

FWeaponData UKYGameSingleton::GetWeaponDataByRowName(const FName& InName) const
{
	if (WeaponDataMap.Contains(InName))
	{
		return WeaponDataMap.FindRef(InName);
	}
	
	KY_LOG(LogKY, Warning, TEXT("Can't Find Weapon With This Name : %s"), *InName.ToString());
	return FWeaponData();
}

TSubclassOf<UGameplayAbility> UKYGameSingleton::GetAbilityByName(const FName& InName)
{
	
	if (AbilityNameAsset)
	{
		if (AbilityNameAsset->AbilityNameMap.Contains(InName))
		{
			return AbilityNameAsset->AbilityNameMap.FindRef(InName);
		}
		else
		{
			KY_LOG(LogKY, Warning, TEXT("Can't Find Ability With This Name : %s"), *InName.ToString());
			return nullptr;
		}
	}
	KY_LOG(LogKY, Error, TEXT("Ability Name Asset InValid!"));
	return nullptr;
}

