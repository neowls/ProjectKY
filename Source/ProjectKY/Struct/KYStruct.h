#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MediaSource.h"
#include "KYStruct.generated.h"

// 아이템 메인타입
UENUM(BlueprintType)
enum class EKYItemType : uint8
{
	None		UMETA(DisplayName = "None"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Armor		UMETA(DisplayName = "Armor"),
	Usable		UMETA(DisplayName = "Usable"),
	Misc		UMETA(DisplayName = "Misc")
};

// 무기 서브타입
UENUM(BlueprintType)
enum class EKYWeaponSubType : uint8
{
	None        UMETA(DisplayName = "None"),
	Sword       UMETA(DisplayName = "Sword"),
	DualBlade   UMETA(DisplayName = "Dual Blade"),
	GreatSword  UMETA(DisplayName = "Great Sword"),
	Scythe      UMETA(DisplayName = "Scythe"),
	Hammer		UMETA(DisplayName = "Hammer"),
	Spear		UMETA(DisplayName = "Spear")
};

// 방어구 서브타입
UENUM(BlueprintType)
enum class EKYArmorSubType : uint8
{
	None	UMETA(DisplayName = "None"),
	Head	UMETA(DisplayName = "Head"),
	Chest	UMETA(DisplayName = "Chest"),
	Legs	UMETA(DisplayName = "Legs"),
	Hands	UMETA(DisplayName = "Hands"),
	Feet	UMETA(DisplayName = "Feet")
};

// 소모품 서브타입
UENUM(BlueprintType)
enum class EKYUsableSubType : uint8
{
	None   UMETA(DisplayName = "None"),
	Potion UMETA(DisplayName = "Potion"),
	Scroll UMETA(DisplayName = "Scroll"),
	Food   UMETA(DisplayName = "Food")
};

UENUM(BlueprintType)
enum class EKYSlotType : uint8
{
	None        UMETA(DisplayName = "None"),
	Inventory   UMETA(DisplayName = "Inventory"),
	Equipment   UMETA(DisplayName = "Equipment")
};

// 어빌리티 카테고리 Enum
UENUM(BlueprintType)
enum class EKYAbilityCategory : uint8
{
	Common      UMETA(DisplayName = "Common"),
	OneSword    UMETA(DisplayName = "One Sword"),
	DualBlade   UMETA(DisplayName = "Dual Blade"),
	GreatSword  UMETA(DisplayName = "Great Sword")
};

UENUM(BlueprintType)
enum class EKYEquipmentState : uint8
{
	None,
	Inventory,
	Equipped,
	InHand
};

USTRUCT()
struct FKYItemInstanceData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName BaseID = NAME_None;

	UPROPERTY()
	FName InstanceID = NAME_None;

	UPROPERTY()
	bool bIsDirty = false;

	UPROPERTY()
	uint8 Count = 0;

	UPROPERTY()
	uint8 AdditionalSlotIndex = 255;

	UPROPERTY()
	EKYEquipmentState EquipState = EKYEquipmentState::None;


public:
	void SetCount(int32 InCount)
	{
		if (Count != InCount)
		{
			Count = InCount;
			bIsDirty = true;
		}
	}

	void SetEquipState(EKYEquipmentState InEquipState)
	{
		if (EquipState != InEquipState)
		{
			EquipState = InEquipState;
			bIsDirty = true;
		}
	}
	
	void FlushChanges()
	{
		if (bIsDirty)
		{
			bIsDirty = false;
		}
	}

	void SetData(const FKYItemInstanceData& NewInstanceData)
	{
		BaseID = NewInstanceData.BaseID;
		InstanceID = NewInstanceData.InstanceID;
		Count = NewInstanceData.Count;
		AdditionalSlotIndex = NewInstanceData.AdditionalSlotIndex;
		EquipState = NewInstanceData.EquipState;
		bIsDirty = true;
	}

	bool IsDirty() const { return bIsDirty; }
	
	void ClearData()
	{
		BaseID = NAME_None;
		InstanceID = NAME_None;
		Count = 0;
		AdditionalSlotIndex = 255;
		EquipState = EKYEquipmentState::None;
	}

	bool IsEmpty() const
	{
		return InstanceID == NAME_None;
	}
};


// 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FKYItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 기본 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	EKYItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic", meta=(EditCondition="ItemType == EKYItemType::Armor", EditConditionHides))
	EKYArmorSubType ArmorType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta=(EditCondition="ItemType == EKYItemType::Weapon", EditConditionHides))
	EKYWeaponSubType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	TSubclassOf<class UGameplayEffect> EffectClass;

	// 인스턴스 데이터
	UPROPERTY()
	FKYItemInstanceData InstanceData;

	bool IsEmpty() const { return InstanceData.IsEmpty(); }

	void SetData(const FKYItemData& NewData)
	{
		Name = NewData.Name;
		Description = NewData.Description;
		ItemType = NewData.ItemType;
		ArmorType = NewData.ArmorType;
		WeaponType = NewData.WeaponType;
		Icon = NewData.Icon;
		InstanceData = NewData.InstanceData;
	}

	void ClearData()
	{
		Name = FText::GetEmpty();
		Description = FText::GetEmpty();
		ItemType = EKYItemType::None;
		ArmorType = EKYArmorSubType::None;
		WeaponType = EKYWeaponSubType::None;
		Icon = nullptr;
		InstanceData.ClearData();
	}

	bool operator==(const FKYItemData& InData) const
	{
		return	InstanceData.Count == InData.InstanceData.Count &&
				InstanceData.EquipState == InData.InstanceData.EquipState &&
				InstanceData.AdditionalSlotIndex == InData.InstanceData.AdditionalSlotIndex;
	};

	FKYItemData()
	: Name(FText::GetEmpty())
	, Description(FText::GetEmpty())
	, ItemType(EKYItemType::None)
	, ArmorType(EKYArmorSubType::None)
	, WeaponType(EKYWeaponSubType::None)
	, Icon(nullptr)
	, InstanceData(FKYItemInstanceData())
	{}

	FKYItemData(const FText& InName, const FText& InDescription, const EKYItemType InItemType, const EKYArmorSubType InArmorType, const EKYWeaponSubType InWeaponType, const TObjectPtr<UTexture2D>& InIcon, const FKYItemInstanceData& InInstanceData)
	: Name(InName)
	, Description(InDescription)
	, ItemType(InItemType)
	, ArmorType(InArmorType)
	, WeaponType(InWeaponType)
	, Icon(InIcon)
	, InstanceData(InInstanceData)
	{}
};

// 무기 데이터
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FWeaponData()
		: Mesh(nullptr)
		, AttackSpeed(1.0f)
		, Damage(0.0f)
		, Type(EKYWeaponSubType::None) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	TObjectPtr<class UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	EKYWeaponSubType Type;
};

// 어빌리티 데이터 구조체
USTRUCT(BlueprintType)
struct FKYAbilityData : public FTableRowBase
{
	GENERATED_BODY()
    
	// 어빌리티 ID (개발자 색인용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FName AbilityID;
    
	// 어빌리티 이름 (실제 표시되는 이름)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FText AbilityName;
    
	// 어빌리티 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FText AbilityDescription;
    
	// 어빌리티 커맨드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FText AbilityCommand;
    
	// 어빌리티 카테고리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	EKYAbilityCategory AbilityCategory;
    
	// 어빌리티 프리뷰 미디어 소스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TObjectPtr<class UMediaSource> AbilityPreviewMediaSource;
    
	// 기본 생성자
	FKYAbilityData()
		: AbilityID(NAME_None)
		, AbilityName(FText::GetEmpty())
		, AbilityDescription(FText::GetEmpty())
		, AbilityCommand(FText::GetEmpty())
		, AbilityCategory(EKYAbilityCategory::Common)
		, AbilityPreviewMediaSource(nullptr)
	{
	}
};

// 어빌리티 태그 데이터
USTRUCT(BlueprintType)
struct FGameplayAbilityDataTag : public FTableRowBase
{
	GENERATED_BODY()

public:

	FGameplayAbilityDataTag()
	: AbilityClass(nullptr)
	, AbilityTag(FGameplayTag())
	{}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay Info")
	TSubclassOf<class UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay Info")
	FGameplayTag AbilityTag;
};

// 애니메이션 몽타주 데이터
USTRUCT(BlueprintType)
struct FEventAnimMontageData
{
	GENERATED_BODY()

public:

	FEventAnimMontageData()
		: Montage(nullptr)
		, EventTag(FGameplayTag())
		, TaskInstanceName(NAME_None)
		, StartSectionName(NAME_None)
		, PlayRate(1.0f)
		, IsUseEvent(false)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	TObjectPtr<class UAnimMontage> Montage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info", meta=(EditCondition="IsUseEvent", EditConditionHides))
	FGameplayTag EventTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	FName TaskInstanceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	FName StartSectionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	float PlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	bool IsUseEvent;
};
