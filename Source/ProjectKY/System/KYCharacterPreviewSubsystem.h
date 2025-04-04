// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FCharacterPreviewScene.h" 
#include "KYCharacterPreviewSubsystem.generated.h"


class USkeletalMesh;
class UTextureRenderTarget2D;

// 	캐릭터 프리뷰 서브시스템으로 별도의 프리뷰 월드를 만들어 캐릭터를 생성한 뒤, 이를 캡쳐하여 렌더 타겟 텍스쳐로 반환한다.
UCLASS()
class PROJECTKY_API UKYCharacterPreviewSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void UpdateScene(float DeltaTime);
	
	void SetPreviewMesh(USkeletalMesh* NewMesh, UAnimationAsset* AnimAsset = nullptr);
	void SetCaptureCameraTransform(FTransform& NewTransform);
	UTextureRenderTarget2D* GetPreviewRenderTarget();

private:
	TUniquePtr<FCharacterPreviewScene> PreviewScene;
};
