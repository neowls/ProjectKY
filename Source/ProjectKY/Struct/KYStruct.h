#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MediaSource.h"
#include "KYStruct.generated.h"

// 아이템 카테고리
UENUM(BlueprintType)
enum class EKYItemType : uint8
{
	None        UMETA(DisplayName = "None"),
	Usable      UMETA(DisplayName = "Usable Item"),
	Misc        UMETA(DisplayName = "Misc"),
	Head        UMETA(DisplayName = "Head"),
	Chest       UMETA(DisplayName = "Chest"),
	Legs		UMETA(DisplayName = "Legs"),
	Gloves      UMETA(DisplayName = "Gloves"),
	Feet        UMETA(DisplayName = "Feet"),
	Weapon      UMETA(DisplayName = "Weapon")
};

UENUM(BlueprintType)
enum class EKYWeaponType : uint8
{
	None        UMETA(DisplayName = "None"),
	Sword       UMETA(DisplayName = "Sword"),
	DualBlade   UMETA(DisplayName = "Dual Blade"),
	GreatSword  UMETA(DisplayName = "Great Sword"),
	Scythe      UMETA(DisplayName = "Scythe"),
	Hammer		UMETA(DisplayName = "Hammer"),
	Spear		UMETA(DisplayName = "Spear")
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta=(EditConditionHides="ItemType == EKYItemType::Weapon"))
	EKYWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	TSubclassOf<class UGameplayEffect> EffectClass;

	// 인스턴스 데이터
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UKYItem> InstanceData;

	FKYItemData()
	: ItemType(EKYItemType::None)
	, WeaponType(EKYWeaponType::None)
	, Icon(nullptr)
	, InstanceData(nullptr)
	{}

	FKYItemData(EKYItemType InType)
	: ItemType(InType)
	, WeaponType(EKYWeaponType::None)
	, Icon(nullptr)
	, InstanceData(nullptr)
	{}
};

UENUM(BlueprintType)
enum class EKYEquipmentState : uint8
{
	Inventory,
	Equipped,
	InHand
};

UCLASS(Abstract, BlueprintType)
class PROJECTKY_API UKYItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName BaseID = NAME_None;

	UPROPERTY()
	uint8 Count = 0;

	UPROPERTY()
	int8 AdditionalSlotIndex = -1;

	UPROPERTY()
	EKYEquipmentState EquipState = EKYEquipmentState::Inventory;
};

USTRUCT(BlueprintType)
struct FKYInventoryWidgetData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName InstanceID;

	UPROPERTY()
	FText Name;

	UPROPERTY()
	FText Description;

	UPROPERTY()
	EKYItemType Type;

	UPROPERTY()
	int8 AdditionalSlotIndex;

	UPROPERTY()
	uint8 Count;

	UPROPERTY()
	EKYEquipmentState EquipState;

	UPROPERTY()
	UTexture2D* Icon;

	void ClearData()
	{
		InstanceID = NAME_None;
		Name = FText::GetEmpty();
		Description = FText::GetEmpty();
		Type = EKYItemType::None;
		AdditionalSlotIndex = -1;
		Count = 0;
		EquipState = EKYEquipmentState::Inventory;
		Icon = nullptr;
	}

	FKYInventoryWidgetData() :
	InstanceID(NAME_None),
	Name(FText::GetEmpty()),
	Description(FText::GetEmpty()),
	Type(EKYItemType::None),
	AdditionalSlotIndex(-1),
	Count(0),
	EquipState(EKYEquipmentState::Inventory),
	Icon(nullptr) {}

	// 변환 생성자 추가
	FKYInventoryWidgetData(const FKYItemData& ItemData)
		: InstanceID(NAME_None)
		, Name(ItemData.Name)
		, Description(ItemData.Description)
		, Type(ItemData.ItemType)
		, AdditionalSlotIndex(-1)
		, Count(0)
		, EquipState(EKYEquipmentState::Inventory)
		, Icon(ItemData.Icon)
	{
		if (ItemData.InstanceData)
		{
			InstanceID = ItemData.InstanceData->GetFName();
			Count = ItemData.InstanceData->Count;
			AdditionalSlotIndex = ItemData.InstanceData->AdditionalSlotIndex;
			EquipState = ItemData.InstanceData->EquipState;
		}
	}

	void operator=(const FKYItemData& ItemData)
	{
		Name = ItemData.Name;
		Description = ItemData.Description;
		Type = ItemData.ItemType;
		Icon = ItemData.Icon;

		if (ItemData.InstanceData)
		{
			InstanceID = ItemData.InstanceData->GetFName();
			Count = ItemData.InstanceData->Count;
			AdditionalSlotIndex = ItemData.InstanceData->AdditionalSlotIndex;
			EquipState = ItemData.InstanceData->EquipState;
		}
	}
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
		, Type(EKYWeaponType::None) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	TObjectPtr<class UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	EKYWeaponType Type;
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
