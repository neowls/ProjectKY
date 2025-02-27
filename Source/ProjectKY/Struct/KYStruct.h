#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "KYStruct.generated.h"

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
