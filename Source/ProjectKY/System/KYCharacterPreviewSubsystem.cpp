// Fill out your copyright notice in the Description page of Project Settings.


#include "System/KYCharacterPreviewSubsystem.h"
#include "FCharacterPreviewScene.h"

void UKYCharacterPreviewSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PreviewScene = MakeUnique<FCharacterPreviewScene>();
}

void UKYCharacterPreviewSubsystem::Deinitialize()
{
	Super::Deinitialize();
	PreviewScene.Reset();
}

void UKYCharacterPreviewSubsystem::UpdateScene(float DeltaTime)
{
	if (PreviewScene)
	{
		PreviewScene->UpdatePreviewScene(DeltaTime);
	}
}



void UKYCharacterPreviewSubsystem::SetPreviewMesh(USkeletalMesh* NewMesh, UAnimationAsset* AnimAsset)
{
	if (PreviewScene)
	{
		PreviewScene->SetSkeletalMeshAsset(NewMesh, AnimAsset);
	}
}

void UKYCharacterPreviewSubsystem::SetCaptureCameraTransform(FTransform& NewTransform)
{
	if(PreviewScene)
	{
		PreviewScene->SetCameraTransform(NewTransform);
	}
}

UTextureRenderTarget2D* UKYCharacterPreviewSubsystem::GetPreviewRenderTarget()
{
	return PreviewScene ? PreviewScene->GetRenderTarget() : nullptr;
}
