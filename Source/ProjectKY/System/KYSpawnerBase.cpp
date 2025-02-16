
#include "System/KYSpawnerBase.h"
#include "NavigationSystem.h"
#include "ProjectKY.h"
#include "Character/KYCharacterNonPlayer.h"
#include "Components/SphereComponent.h"


AKYSpawnerBase::AKYSpawnerBase()
{
	SpawnArea = CreateDefaultSubobject<USphereComponent>("SpawnArea");
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetSphereRadius(50.0f);
}

void AKYSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnCharacter();
}

void AKYSpawnerBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    FlushPersistentDebugLines(GetWorld());
    if (bDebugSpawn)
    {
        // 스폰 지점 재계산
        FVector Origin = SpawnArea->GetComponentLocation();
        float Radius = SpawnArea->GetScaledSphereRadius();
    
        DebugSpawnPoints = GeneratePoissonDiskSamples(Origin, Radius, MinSpawnDistance, CenterOffset, 30);

        // 디버그 드로잉
        for (const FVector& SpawnPoint : DebugSpawnPoints)
        {
            DrawDebugSphere(GetWorld(), SpawnPoint, 10.0f, 12, FColor::Green, true, -1.0f);
        }

        // 중앙 제거 반경을 빨간색 원으로 표시
        DrawDebugCircle(GetWorld(), Origin, CenterOffset, 32, FColor::Red, true, -1.0f, 0, 5.0f, FVector(1,0,0), FVector(0,1,0));
    }
}


TArray<FVector> AKYSpawnerBase::GeneratePoissonDiskSamples(FVector Origin, float Radius, float MinDistance, float MinCenterOffset, int MaxTries)
{
    TArray<FVector> Points;
    TArray<FVector> ActiveList;

    float SafeRadius = Radius * 0.9f;

    if (MinCenterOffset >= SafeRadius)
    {
        UE_LOG(LogTemp, Warning, TEXT("MinCenterOffset is greater than or equal to SafeRadius. Adjusting SafeRadius."));
        SafeRadius = MinCenterOffset * 1.1f;
    }

    // 첫 번째 랜덤 점 생성
    float InitialAngle = FMath::RandRange(0.0f, 2.0f * PI);
    float InitialRadius = FMath::RandRange(0.0f, SafeRadius);
    FVector2D RandomOffset = FVector2D(FMath::Cos(InitialAngle), FMath::Sin(InitialAngle)) * InitialRadius;
    FVector FirstPoint = FVector(Origin.X + RandomOffset.X, Origin.Y + RandomOffset.Y, Origin.Z);

    Points.Add(FirstPoint);
    ActiveList.Add(FirstPoint);

    while (ActiveList.Num() > 0 && Points.Num() < SpawnAmount * 2) // 여유 공간 확보를 위해 2배 크기로 생성
    {
        int32 RandomIndex = FMath::RandRange(0, ActiveList.Num() - 1);
        FVector ActivePoint = ActiveList[RandomIndex];
        bool bFoundValidPoint = false;

        for (int32 i = 0; i < MaxTries; i++)
        {
            float SampleAngle = FMath::RandRange(0.0f, 2.0f * PI);
            float SampleRadius = FMath::RandRange(MinDistance, 2 * MinDistance);
            FVector2D RandomDir = FVector2D(FMath::Cos(SampleAngle), FMath::Sin(SampleAngle)) * SampleRadius;
            FVector NewPoint = FVector(ActivePoint.X + RandomDir.X, ActivePoint.Y + RandomDir.Y, Origin.Z);

            if (FVector::DistSquared(Origin, NewPoint) > SafeRadius * SafeRadius)
            {
                continue;
            }

            bool bValid = true;
            for (const FVector& ExistingPoint : Points)
            {
                if (FVector::DistSquared(NewPoint, ExistingPoint) < MinDistance * MinDistance)
                {
                    bValid = false;
                    break;
                }
            }

            UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
            if (NavSystem)
            {
                FNavLocation NavLocation;
                if (!NavSystem->ProjectPointToNavigation(NewPoint, NavLocation))
                {
                    bValid = false;
                }
            }
            else
            {
                NewPoint.Z = Origin.Z;
            }

            if (bValid)
            {
                Points.Add(NewPoint);
                ActiveList.Add(NewPoint);
                bFoundValidPoint = true;
                break;
            }
        }

        if (!bFoundValidPoint)
        {
            ActiveList.RemoveAt(RandomIndex);
        }
    }

    // 중앙 반경 내 점 제거
    Points = Points.FilterByPredicate([&](const FVector& Point)
    {
        return FVector::DistSquared(Origin, Point) >= MinCenterOffset * MinCenterOffset;
    });

    // 개수가 부족할 경우 추가 생성 (무한 루프 방지 추가)
    int32 MaxAdditionalTries = 100; // 최대 반복 횟수 설정
    int32 TryCount = 0; 

    while (Points.Num() < SpawnAmount && TryCount < MaxAdditionalTries)
    {
        TryCount++; // 반복 횟수 증가

        float SampleAngle = FMath::RandRange(0.0f, 2.0f * PI);
        float SampleRadius = FMath::RandRange(MinDistance, SafeRadius);
        FVector2D RandomDir = FVector2D(FMath::Cos(SampleAngle), FMath::Sin(SampleAngle)) * SampleRadius;
        FVector NewPoint = FVector(Origin.X + RandomDir.X, Origin.Y + RandomDir.Y, Origin.Z);

        if (FVector::DistSquared(Origin, NewPoint) < MinCenterOffset * MinCenterOffset)
        {
            continue; // 중앙 제거 반경 내 생성 방지
        }

        bool bValid = true;
        for (const FVector& ExistingPoint : Points)
        {
            if (FVector::DistSquared(NewPoint, ExistingPoint) < MinDistance * MinDistance)
            {
                bValid = false;
                break;
            }
        }

        if (bValid)
        {
            Points.Add(NewPoint);
        }
    }

    // 무한 루프 방지 확인
    if (TryCount >= MaxAdditionalTries)
    {
        KY_LOG(LogKY, Warning, TEXT("Failed to generate enough spawn points. 목표 스폰 포인트 개수를 생성하는데 실패하였습니다."));
    }

    return Points;
}


void AKYSpawnerBase::SpawnCharacter()
{
	if (SpawnType == nullptr) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector Origin = SpawnArea->GetComponentLocation();
	float Radius = SpawnArea->GetScaledSphereRadius();

	TArray<FVector> SpawnLocations = GeneratePoissonDiskSamples(Origin, Radius, MinSpawnDistance, CenterOffset, 50);
    int CurrentSpawnCount = 0;
	for (const FVector& SpawnLocation : SpawnLocations)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;

		World->SpawnActor<AKYCharacterNonPlayer>(SpawnType, SpawnLocation, SpawnRotation);
	    CurrentSpawnCount++;
	    if (CurrentSpawnCount >= SpawnAmount) return;
	}
}


