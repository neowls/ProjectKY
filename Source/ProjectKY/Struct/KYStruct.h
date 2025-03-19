#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MediaSource.h"
#include "KYStruct.generated.h"

// 장비 슬롯 타입
UENUM(BlueprintType)
enum class EKYEquipmentSlot : uint8
{
	Head,
	Chest,
	Pants,
	Gloves,
	Shoes
};

// 무기 슬롯 타입
UENUM(BlueprintType)
enum class EKYWeaponSlot : uint8
{
	Primary,
	Secondary,
	Tertiary
};

// 아이템 카테고리
UENUM(BlueprintType)
enum class EKYItemCategory : uint8
{
	Weapon,
	Ammo,
	Equipment,
	Consumable
};

UENUM(BlueprintType)
enum class EKYSlotType : uint8
{
	None,
	// 인벤토리
	Inventory,
	// 장비
	Head,
	Chest,
	Pants,
	Gloves,
	Shoes,
	// 무기
	Primary,
	Secondary,
	Tertiary
};

// 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FKYItemData
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKYItemCategory Category;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKYSlotType EquipSlotType;

	// 기본 생성자
	FKYItemData()
		: ItemID(NAME_None)
		, ItemName(FText::GetEmpty())
		, ItemDescription(FText::GetEmpty())
		, ItemIcon(nullptr)
		, Category(EKYItemCategory::Equipment)
		, EquipSlotType(EKYSlotType::None)
	{
	}
};

// 무기 타입
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Sword,
	GreatSword,
	DualBlade,
	Scythe,
	Hammer,
	Spear
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
		, Type(EWeaponType::None) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	TObjectPtr<class UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon Info")
	EWeaponType Type;
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
