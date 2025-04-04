#include "FCharacterPreviewScene.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ProjectKY.h"
#include "PreviewScene.h"


FCharacterPreviewScene::FCharacterPreviewScene()
{
	PreviewScene = MakeUnique<FPreviewScene>(FPreviewScene::ConstructionValues());
	
	PreviewMesh = NewObject<USkeletalMeshComponent>();
	
	PreviewMesh->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	PreviewMesh->bCastDynamicShadow = false;
	PreviewScene->AddComponent(PreviewMesh, FTransform::Identity);

	
	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->RenderTargetFormat = RTF_RGBA16f;
	RenderTarget->ClearColor = FLinearColor::Transparent;
	RenderTarget->InitAutoFormat(512,830);
	
	CaptureComponent = NewObject<USceneCaptureComponent2D>();
	CaptureComponent->TextureTarget = RenderTarget;
	CaptureComponent->CaptureSource = SCS_SceneColorHDR;
	CaptureComponent->FOVAngle = 30.0f;
	CaptureComponent->bCaptureEveryFrame = false;
	CaptureComponent->bAlwaysPersistRenderingState = true; 

	FTransform CamTransform(FRotator(-5.0f, -90, 0.f), FVector(0.f, 250.f, 120.f));
	PreviewScene->AddComponent(CaptureComponent, CamTransform);
	
}

FCharacterPreviewScene::~FCharacterPreviewScene()
{
	PreviewMesh = nullptr;
	CaptureComponent = nullptr;
	RenderTarget = nullptr;
}

void FCharacterPreviewScene::SetSkeletalMeshAsset(USkeletalMesh* NewMesh, UAnimationAsset* AnimAsset)
{
	if (PreviewMesh)
	{
		PreviewMesh->SetSkeletalMesh(NewMesh);
		PreviewMesh->SetAnimation(AnimAsset);
		PreviewMesh->Play(true);
	}
}

void FCharacterPreviewScene::SetCameraTransform(FTransform& NewTransform) // 프리뷰 카메라의 트랜스폼을 설정한다.
{
	if (CaptureComponent)
	{
		CaptureComponent->SetRelativeTransform(NewTransform);
	}
}


void FCharacterPreviewScene::UpdatePreviewScene(float DeltaTime) // 실시간으로 캐릭터 메쉬와 움직임을 캡쳐한다.
{
	if (PreviewMesh)
	{
		PreviewMesh->TickComponent(DeltaTime, LEVELTICK_All, nullptr);
	}
	if (CaptureComponent)
	{
		CaptureComponent->CaptureScene();
	}
}

UTextureRenderTarget2D* FCharacterPreviewScene::GetRenderTarget() const
{
	return RenderTarget;
}
