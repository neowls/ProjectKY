// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYVideoWidget.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "ProjectKY.h"
#include "Components/Image.h"


void UKYVideoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!MediaPlayer)
	{
		MediaPlayer = NewObject<UMediaPlayer>(this);
		MediaPlayer->SetLooping(IsLooping);
		
	}

	if (!MediaTexture)
	{
		MediaTexture = NewObject<UMediaTexture>(this);
		MediaTexture->SetMediaPlayer(MediaPlayer);
		MediaTexture->AutoClear = false;
		MediaTexture->UpdateResource();
	}

	// 동적 머티리얼 생성 및 설정
	if (UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(VideoMaterial, this))
	{
		DynamicMaterial->SetTextureParameterValue(FName("MediaTexture"), MediaTexture);
		if (VideoImage) VideoImage->SetBrushFromMaterial(DynamicMaterial);
		else
		{
			VideoImage = NewObject<UImage>(this);
			VideoImage->SetBrushFromMaterial(DynamicMaterial);
		}
	}
    
}

void UKYVideoWidget::SetVideo(UMediaSource* MediaSource)
{
	if (!MediaPlayer) return;


	// 현재 재생 중인 비디오 중지
	MediaPlayer->Close();
	
    
	// 미디어 플레이어에 소스 설정 및 재생
	MediaPlayer->OpenSource(MediaSource);
}

void UKYVideoWidget::PlayVideo()
{
	if (MediaPlayer)
	{
		MediaPlayer->Play();
		KY_LOG(LogKY, Log, TEXT("Video Play"));
	}
}

void UKYVideoWidget::PauseVideo()
{
	if (MediaPlayer)
	{
		MediaPlayer->Pause();
	}
}

void UKYVideoWidget::StopVideo()
{
	if (MediaPlayer)
	{
		MediaPlayer->Close();
	}
}
