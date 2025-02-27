#pragma once

#include "GameplayTagContainer.h"
#include "KYGameplayTag.generated.h"

UCLASS()
class PROJECTKY_API UKYGameplayTags : public UObject
{
    GENERATED_BODY()
    
public:
    // 초기화 함수 - 게임 시작 시 호출
    static void InitializeTags();
    
    // 캐릭터 상태 태그
    struct FCharacterStateTags
    {
        static const FGameplayTag Attack;
        static const FGameplayTag Attack_Light;
        static const FGameplayTag Attack_Heavy;
        static const FGameplayTag Attack_Upper;
        static const FGameplayTag Attack_Slam;
        static const FGameplayTag Attack_Grab;
        static const FGameplayTag Attack_Blast;
        static const FGameplayTag Attack_Parry;
        
        static const FGameplayTag Attack_Blocked;
        static const FGameplayTag Unmovable;
        static const FGameplayTag Unstable;
        static const FGameplayTag Executable;
        static const FGameplayTag IsDead;
        static const FGameplayTag IsGliding;
        static const FGameplayTag IsFocusing;
        static const FGameplayTag IsSkilling;
        static const FGameplayTag IsKnockDown;
        static const FGameplayTag IsKnockOut;
        static const FGameplayTag IsStaggered;
        static const FGameplayTag IsAttacking;
        static const FGameplayTag IsDash;
        static const FGameplayTag IsGuard;
        static const FGameplayTag IsParry;
        static const FGameplayTag Invincible;
        static const FGameplayTag IsRotable;
        static const FGameplayTag IsFalling;
        static const FGameplayTag IsJumping;
        static const FGameplayTag IsCombat;
        static const FGameplayTag IsGuardEnd;
        static const FGameplayTag MoveMontageEnabled;
        
        // 자주 사용되는 태그 컨테이너
        static const FGameplayTagContainer CombatTags;
        static const FGameplayTagContainer UnmovableTags;
        static const FGameplayTagContainer UnstableTags;
    };
    
    // 데이터 태그
    struct FDataTags
    {
        static const FGameplayTag Ability;
        
        // 공통 어빌리티
        static const FGameplayTag Ability_Common_Hit_KnockDown;
        static const FGameplayTag Ability_Common_Hit_Staggered;
        static const FGameplayTag Ability_Common_Jump;
        
        // 적 어빌리티
        static const FGameplayTag Ability_Enemy_Attack;
        static const FGameplayTag Ability_Enemy_Attack_Light;
        
        // 플레이어 어빌리티
        static const FGameplayTag Ability_Player_Attack;
        static const FGameplayTag Ability_Player_Attack_Light;
        static const FGameplayTag Ability_Player_Attack_Heavy;
        static const FGameplayTag Ability_Player_Attack_Air;
        static const FGameplayTag Ability_Player_Attack_Range;
        static const FGameplayTag Ability_Player_Attack_Upper;
        static const FGameplayTag Ability_Player_Attack_Special;
        static const FGameplayTag Ability_Player_Dash;
        static const FGameplayTag Ability_Player_DoubleJump;
        static const FGameplayTag Ability_Player_Execution;
        static const FGameplayTag Ability_Player_Glide;
        static const FGameplayTag Ability_Player_Guard;
        static const FGameplayTag Ability_Player_Focus;
        
        static const FGameplayTag Data;
    };
    
    // 이벤트 태그
    struct FEventTags
    {
        static const FGameplayTag Combo_Available;
        static const FGameplayTag AttackHit;
        static const FGameplayTag SkillHit;
        static const FGameplayTag Bounty_Experience;
        static const FGameplayTag Hit;
    };
    
    // 게임플레이 큐 태그
    struct FGameplayCueTags
    {
        static const FGameplayTag Character_AttackHit;
    };
    
    // 태그 접근용 정적 멤버
    static FCharacterStateTags CharacterState;
    static FDataTags Data;
    static FEventTags Event;
    static FGameplayCueTags GameplayCue;
};