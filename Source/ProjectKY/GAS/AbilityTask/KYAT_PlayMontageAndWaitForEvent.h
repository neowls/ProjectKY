// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AbilityTask/KYAbilityTask.h"
#include "Struct/KYStruct.h"
#include "KYAT_PlayMontageAndWaitForEvent.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKYPlayMontageAndWaitForEventDelegate, FGameplayEventData, Payload);

/**
 * 이 태스크는 PlayMontageAndWait 과 WaitForEvent 를 하나의 태스크로 결합하여 근접 콤보와 같은 여러 유형의 발동을 감지할 수 있습니다.
 * 이 코드의 대부분은 두 어빌리티 태스크 중 하나에서 복사한 것입니다.
 * 게임별 태스크를 만들 때 예시로 참고하기 좋은 태스크입니다.
 * 각 게임마다 원하는 작업을 수행하기 위한 게임별 태스크 세트가 있을 것으로 예상됩니다.
 */

UCLASS()
class PROJECTKY_API UKYAT_PlayMontageAndWaitForEvent : public UKYAbilityTask
{
	GENERATED_BODY()

public:
	UKYAT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FKYPlayMontageAndWaitForEventDelegate OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FKYPlayMontageAndWaitForEventDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FKYPlayMontageAndWaitForEventDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FKYPlayMontageAndWaitForEventDelegate OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FKYPlayMontageAndWaitForEventDelegate EventReceived;

	/**
	 * 몽타주를 재생하고 종료될 때까지 기다립니다. 이벤트 태그와 일치하는 게임플레이 이벤트가 발생하면(또는 이벤트 태그가 비어 있으면) 태그와 이벤트 데이터와 함께 EventReceived 델리게이트가 실행됩니다.
	 * StopWhenAbilityEnds가 참이면 어빌리티가 정상적으로 종료되면 이 몽타주가 중단됩니다. 어빌리티가 명시적으로 취소되면 항상 중지됩니다.
	 * 정상 실행 시에는 몽타주가 블렌딩될 때 OnBlendOut이 호출되고, 재생이 완전히 끝나면 OnCompleted가 호출됩니다.
	 * 다른 몽타주가 이를 덮어쓰면 OnInterrupted가 호출되고, 어빌리티 또는 작업이 취소되면 OnCancelled가 호출됩니다.
	 *
	 * @param TaskInstanceName 나중에 쿼리를 위해 이 태스크의 이름을 재정의하도록 설정합니다.
	 * @param MontageToPlay 캐릭터에 재생할 몽타주
	 * @param StartSection 비어있지 않다면, 해당 이름의 몽타주 섹션부터 재생합니다.
	 * @param EventTag 이 태그와 일치하는 모든 게임플레이 이벤트는 EventReceived 콜백을 활성화합니다. 비어 있으면 모든 이벤트가 콜백됩니다.
	 * @param Rate 몽타주를 더 빠르게 또는 더 느리게 재생하려면 이것을 변경합니다.
	 * @param bStopWhenAbilityEnds 참이면 어빌리티가 정상적으로 종료되면 이 몽타주가 중단됩니다. 어빌리티가 명시적으로 취소되면 항상 중지됩니다.
	 * @param AnimRootMotionTranslationScale 애니메이션 루트 모션의 크기를 수정하려면 변경하거나 완전히 차단하려면 0으로 설정합니다.
	 */

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UKYAT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(
		UGameplayAbility* OwningAbility,
		FEventAnimMontageData PlayEventMontageData,
		bool bStopWhenAbilityEnds = true,
		bool OnlyMatchExact = false,
		float AnimRootMotionTranslationScale = 1.f);

private:

	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	FGameplayTag EventTag;
	
	UPROPERTY()
	float Rate;
	
	UPROPERTY()
	FName StartSection;
	
	UPROPERTY()
	float AnimRootMotionTranslationScale;
	
	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	bool OnlyMatchExact;
	

	/** 어빌리티가 몽타주를 재생 중인지 확인하고 해당 몽타주를 중지하며, 몽타주가 중지된 경우 true를 반환하고 그렇지 않은 경우 false를 반환합니다. */
	bool StopPlayingMontage();
	
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void OnGameplayEvent(const FGameplayEventData* Payload);
	void OnGameplayEventContainerCallback(FGameplayTag MatchingTag, const FGameplayEventData* Payload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
	
};
