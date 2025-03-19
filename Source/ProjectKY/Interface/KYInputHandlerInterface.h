// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KYInputHandlerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UKYInputHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTKY_API IKYInputHandlerInterface
{
	GENERATED_BODY()

public:
	// 네비게이션 입력 처리
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	bool HandleNavigationInput(float AxisX, float AxisY);
    
	// 확인 입력 처리
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	bool HandleConfirmInput();
    
	// 취소 입력 처리
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	bool HandleCancelInput();
    
	// 탭 입력 처리
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	bool HandleWindowInput();
    
	// 탭 전환 입력 처리
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Input")
	bool HandleTabSwitchInput(int32 TabIndex);
	
};
