// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYUserWidget.h"
#include "Components/Button.h"
#include "Interface/KYInputHandlerInterface.h"
#include "KYButtonWidget.generated.h"

// 버튼 클릭 이벤트를 위한 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClickedWithIndexSignature, int32, ButtonIndex);

UCLASS()
class PROJECTKY_API UKYButtonWidget : public UKYUserWidget
{
	GENERATED_BODY()

public:
	UKYButtonWidget();
	
	// 인덱스 설정/가져오기
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonIndex(int32 InIndex);
    
	UFUNCTION(BlueprintPure, Category = "Button")
	int32 GetButtonIndex() const { return ButtonIndex; }
    
	// 선택 상태 설정/가져오기
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetIsSelected(bool bInIsSelected);
    
	UFUNCTION(BlueprintPure, Category = "Button")
	bool GetIsSelected() const { return bIsSelected; }

	// 텍스트 설정
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(const FText& InText);

	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonStyle(const FButtonStyle& InStyle);

	UFUNCTION(BlueprintCallable, Category = "Button")
	FButtonStyle GetButtonStyle() { return Button->GetStyle(); }
	
	// 버튼 클릭 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonClickedWithIndexSignature OnButtonClickedWithIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	FText DefaultButtonText;
	
protected:
	// 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void HandleButtonClicked();

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	// 버튼 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	int32 ButtonIndex;
    
	// 선택 상태
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	bool bIsSelected;
	
	// 텍스트 블록
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;

	UPROPERTY(Transient)
	FButtonStyle DefaultButtonStyle;
};
