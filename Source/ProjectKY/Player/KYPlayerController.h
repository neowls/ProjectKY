
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "KYPlayerController.generated.h"

class UKYHUDUserWidget;
class UKYStatusWindowWidget;
class UInputAction;
class UInputMappingContext;

UCLASS()
class PROJECTKY_API AKYPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKYPlayerController();

	FORCEINLINE class UKYHUDUserWidget* GetHUDWidget() const {  return HUDWidget; }

	virtual void SetupWidget();

protected:
	virtual void SetupInputComponent() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	void SetWindowInputContext(const TObjectPtr<UInputMappingContext>& Context);

	// 상태 창 토글 함수
	void ToggleStatusWindow();
    
	// 상태 창 탭 전환 함수
	void SwitchStatusWindowTab(const FInputActionValue& InputActionValue);

	// 상태 창 탭 내부 이동 함수
	void NavigateStatusWindowTab(const FInputActionValue& InputActionValue);
    
	// 상태 창 선택 함수
	void SelectStatusWindow(const FInputActionValue& InputActionValue);
    
	// 상태 창 뒤로가기 함수
	void BackStatusWindow(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UKYHUDUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TObjectPtr<UKYHUDUserWidget> HUDWidget;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UKYStatusWindowWidget> StatusWindowWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UKYStatusWindowWidget> StatusWindowWidget;
	
	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> GameplayContext;

	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WindowContext;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=UI)
	TObjectPtr<UInputAction> WindowAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=UI)
	TObjectPtr<UInputAction> WindowSelectAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=UI)
	TObjectPtr<UInputAction> WindowBackAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=UI)
	TObjectPtr<UInputAction> WindowNavigateAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=UI)
	TObjectPtr<UInputAction> WindowTabSwitchAction;
};


