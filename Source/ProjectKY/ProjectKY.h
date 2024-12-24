// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_ENEMY ECollisionChannel::ECC_GameTraceChannel1

// 실행되는 함수의 클라 및 서버 정보를 가져오는 매크로
#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))
#define LOG_NETMODEINFO_GAMEPLAYABILITY ((GetAvatarActorFromActorInfo()->GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetAvatarActorFromActorInfo()->GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))
#define LOG_NETMODEINFO_ABILITYTASK ((Ability->GetAvatarActorFromActorInfo()->GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((Ability->GetAvatarActorFromActorInfo()->GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")))


// 넷모드의 Local 및 Remote의 Role을 가져오는 매크로
#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_LOCALROLEINFO_GAMEPLAYABILITY *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetAvatarActorFromActorInfo()->GetLocalRole()))
#define LOG_LOCALROLEINFO_ABILITYTASK *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), Ability->GetAvatarActorFromActorInfo()->GetLocalRole()))

#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_REMOTEROLEINFO_GAMEPLAYABILITY *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), GetAvatarActorFromActorInfo()->GetRemoteRole()))
#define LOG_REMOTEROLEINFO_ABILITYTASK *(UEnum::GetValueAsString (TEXT("Engine.ENetRole"), Ability->GetAvatarActorFromActorInfo()->GetRemoteRole()))


// 호출되는 함수의 정보를 가져오는 매크로
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)


#define KY_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOG_SCREEN(Key, Format, ...) GEngine->AddOnScreenDebugMessage(Key, 5.f, FColor::Orange, FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogKY, Log, All);

template<typename T>
static void InitializeObjectFinder(TObjectPtr<T>& Ref, const FString& Path)
{
	ConstructorHelpers::FObjectFinder<T> Finder(*Path);
	if (Finder.Succeeded()) {
		Ref = Finder.Object;
	}
}

template<typename T>
static void InitializeObjectFinder(T& Ref, const FString& Path)
{
	ConstructorHelpers::FObjectFinder<T> Finder(*Path);
	if (Finder.Succeeded()) {
		Ref = Finder.Object;
	}
}


/*
template<typename T>
static void InitializeClassFinder(TSubclassOf<T>& Ref, const FString& Path)
{
	ConstructorHelpers::FClassFinder<T> Finder(*Path);
	if (Finder.Succeeded()) {
		Ref = Finder.Class;
	}
}
*/
