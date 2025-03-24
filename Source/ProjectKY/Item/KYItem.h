#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Struct/KYStruct.h"
#include "KYItem.generated.h"

class UAbilitySystemComponent;

class AKYPlayerState;

UCLASS(Abstract, BlueprintType)
class PROJECTKY_API UKYItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

};

// 장비 아이템 클래스
UCLASS(Blueprintable)
class PROJECTKY_API UKYEquipmentItem : public UKYItem
{
	GENERATED_BODY()

public:
	UKYEquipmentItem();
	
	// 장비 관련 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment")
	EKYEquipmentType EquipmentType;
	
	// 장비 상태
	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	bool bIsEquipped;

	// 장착 소켓 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FName AttachSocketName;

	// 장비 메시 경로
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	FString MeshPath;
};

// 무기 아이템 클래스
UCLASS(Blueprintable)
class PROJECTKY_API UKYWeaponItem : public UKYEquipmentItem
{
	GENERATED_BODY()

public:
	UKYWeaponItem();

	// 무기 상태
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsInHand;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int32 WeaponSlotIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EKYWeaponType WeaponType;
};

// 소비 아이템 클래스
UCLASS(Blueprintable)
class PROJECTKY_API UKYConsumableItem : public UKYItem
{
	GENERATED_BODY()

};
