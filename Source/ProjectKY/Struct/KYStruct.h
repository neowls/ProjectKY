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
	Equipment   UMETA(DisplayName = "Equipment"),
	Misc        UMETA(DisplayName = "Misc")
};

UENUM(BlueprintType)
enum class EKYSlotType : uint8
{
	None        UMETA(DisplayName = "None"),
	Inventory   UMETA(DisplayName = "Inventory"),
	Equipment   UMETA(DisplayName = "Equipment")
};

// 장비 타입
UENUM(BlueprintType)
enum class EKYEquipmentType : uint8
{
	None        UMETA(DisplayName = "None"),
	Head        UMETA(DisplayName = "Head"),
	Chest       UMETA(DisplayName = "Chest"),
	Legs		UMETA(DisplayName = "Legs"),
	Gloves      UMETA(DisplayName = "Gloves"),
	Feet        UMETA(DisplayName = "Feet"),
	Weapon		UMETA(DisplayName = "Weapon")
};

// 무기 타입
UENUM(BlueprintType)
enum class EKYWeaponType : uint8
{
	None        UMETA(DisplayName = "None"),
	Sword       UMETA(DisplayName = "Sword"),
	GreatSword  UMETA(DisplayName = "GreatSword"),
	DualBlade   UMETA(DisplayName = "DualBlade"),
	Scythe      UMETA(DisplayName = "Scythe"),
	Spear       UMETA(DisplayName = "Spear"),
	Hammer      UMETA(DisplayName = "Hammer")
};

// 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FKYItemData
{
	GENERATED_BODY()
	
	// 아이템 데이터 테이블 참조용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	FName ItemID;
	
	
	// 기본 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	EKYItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	TSoftObjectPtr<UTexture2D> Icon;

	// 수량 (중첩 가능한 아이템의 경우)
	UPROPERTY(BlueprintReadOnly, Category = "Basic")
	int32 Count;

	// 아이템 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic")
	bool bIsStackable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Basic", meta = (EditConditionHides = "bIsStackable"))
	int32 MaxStackCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic")
	TSubclassOf<class UGameplayEffect> EffectClass;

	// 상세 정보 Asset 참조
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UKYItem> SubData;

	FKYItemData()
		: ItemType(EKYItemType::None)
		, Count(1)
		, bIsStackable(false)
		, MaxStackCount(1)
	{}
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
