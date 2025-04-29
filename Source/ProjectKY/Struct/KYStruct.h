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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	EKYArmorSubType ArmorType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta=(EditCondition="ItemType == EKYItemType::Weapon", EditConditionHides))
	EKYWeaponSubType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Basic")
	TSubclassOf<class UGameplayEffect> EffectClass;

	// 인스턴스 데이터
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UKYItem> InstanceData;

	FKYItemData()
	: Name(FText::GetEmpty())
	, Description(FText::GetEmpty())
	, ItemType(EKYItemType::None)
	, ArmorType(EKYArmorSubType::None)
	, WeaponType(EKYWeaponSubType::None)
	, Icon(nullptr)
	, InstanceData(nullptr)
	{}

	FKYItemData(const FText& InName, const FText& InDescription, const EKYItemType InItemType, const EKYArmorSubType InArmorType, const EKYWeaponSubType InWeaponType, const TObjectPtr<UTexture2D>& InIcon, const TObjectPtr<UKYItem>& InInstanceData)
	: Name(InName)
	, Description(InDescription)
	, ItemType(InItemType)
	, ArmorType(InArmorType)
	, WeaponType(InWeaponType)
	, Icon(InIcon)
	, InstanceData(InInstanceData)
	{}
};

UENUM(BlueprintType)
enum class EKYEquipmentState : uint8
{
	Inventory,
	Equipped,
	InHand
};

UCLASS(BlueprintType)
class PROJECTKY_API UKYItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName BaseID = NAME_None;

	UPROPERTY()
	uint8 Count = 0;

	UPROPERTY()
	uint8 AdditionalSlotIndex = 255;

	UPROPERTY()
	EKYEquipmentState EquipState = EKYEquipmentState::Inventory;
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
