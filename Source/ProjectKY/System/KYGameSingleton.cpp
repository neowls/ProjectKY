// Fill out your copyright notice in the Description page of Project Settings.


#include "System/KYGameSingleton.h"

#include "DataTableEditorUtils.h"
#include "ProjectKY.h"

UKYGameSingleton::UKYGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableRef(TEXT("/Game/_Dev/DataTable/DT_MeleeWeapon.DT_MeleeWeapon"));
	if (WeaponDataTableRef.Object != nullptr)
	{
		WeaponDataTable = WeaponDataTableRef.Object;
		check(WeaponDataTable->GetRowMap().Num() > 0);
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

FWeaponData UKYGameSingleton::GetWeaponDataByRowName(const FName& InName) const
{
	static const FString ContextString(TEXT("Weapon Data Table Context"));
	FWeaponData* RowData = WeaponDataTable->FindRow<FWeaponData>(InName, ContextString);;
	if (!RowData)
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Find Weapon With This Name : %s"), *InName.ToString());
		return FWeaponData();
	}
	return *RowData;
}
