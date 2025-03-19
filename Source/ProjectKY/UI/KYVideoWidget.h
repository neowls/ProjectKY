// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "KYVideoWidget.generated.h"

class UMediaSource;
/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYVideoWidget : public UKYUserWidget
{
	GENERATED_BODY()

public:
	// 비디오 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void SetVideo(UMediaSource* MediaSource);
	
	// 비디오 재생 제어 함수
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void PlayVideo();
    
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void PauseVideo();
    
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void StopVideo();
	
protected:
	virtual void NativeConstruct() override;

	// 비디오 관련 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = "Media")
	TObjectPtr<class UMediaPlayer> MediaPlayer;
    
	UPROPERTY(EditDefaultsOnly, Category = "Media")
	TObjectPtr<class UMediaTexture> MediaTexture;
	
	// 비디오 재생용 머티리얼
	UPROPERTY(EditDefaultsOnly, Category = "Media")
	TObjectPtr<class UMaterialInterface> VideoMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Media")
	bool IsLooping = true;
	
	// UI 컴포넌트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> VideoImage;
	
};
