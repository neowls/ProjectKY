#pragma once

class FPreviewScene;
class USkeletalMeshComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;


class FCharacterPreviewScene
{
	
public:
	FCharacterPreviewScene();
	~FCharacterPreviewScene();

	void SetSkeletalMeshAsset(USkeletalMesh* NewMesh, UAnimationAsset* AnimAsset = nullptr);
	void SetCameraTransform(FTransform& NewTransform);
	void UpdatePreviewScene(float DeltaTime);
	
	UTextureRenderTarget2D* GetRenderTarget() const;

private:
	TUniquePtr<FPreviewScene> PreviewScene;
	USkeletalMeshComponent* PreviewMesh = nullptr;
	USceneCaptureComponent2D* CaptureComponent = nullptr;
	UTextureRenderTarget2D* RenderTarget = nullptr;
};
