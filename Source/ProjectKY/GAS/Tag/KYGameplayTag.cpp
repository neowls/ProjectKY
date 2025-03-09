#include "KYGameplayTag.h"

// 캐릭터 상태 태그 정의
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Light = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Light"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Heavy = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Heavy"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Upper = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Upper"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Slam = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Slam"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Grab = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Grab"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Blast = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Blast"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Parry = FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Parry"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Attack_Blocked = FGameplayTag::RequestGameplayTag(FName("Character.State.IsBlocked"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Unmovable = FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Unstable = FGameplayTag::RequestGameplayTag(FName("Character.State.Unstable"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Executable = FGameplayTag::RequestGameplayTag(FName("Character.State.Executable"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsDead = FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsGliding = FGameplayTag::RequestGameplayTag(FName("Character.State.IsGliding"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsFocusing = FGameplayTag::RequestGameplayTag(FName("Character.State.IsFocusing"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsSkill = FGameplayTag::RequestGameplayTag(FName("Character.State.IsSkill"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsKnockDown = FGameplayTag::RequestGameplayTag(FName("Character.State.Unstable.IsKnockDown"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsKnockOut = FGameplayTag::RequestGameplayTag(FName("Character.State.Unstable.IsKnockOut"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsStaggered = FGameplayTag::RequestGameplayTag(FName("Character.State.Unstable.IsStaggered"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsAttacking = FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsAttacking"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsDodge = FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsDodge"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsGuard = FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsGuard"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsParry = FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsParry"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::Invincible = FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsRotatable = FGameplayTag::RequestGameplayTag(FName("Character.State.IsRotable"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsFalling = FGameplayTag::RequestGameplayTag(FName("Character.State.IsFalling"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsJumping = FGameplayTag::RequestGameplayTag(FName("Character.State.IsJumping"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsCombat = FGameplayTag::RequestGameplayTag(FName("Character.State.IsCombat"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::IsGuardEnd = FGameplayTag::RequestGameplayTag(FName("Character.State.IsGuardEnd"));
const FGameplayTag UKYGameplayTags::FCharacterStateTags::MoveMontageEnabled = FGameplayTag::RequestGameplayTag(FName("Character.State.MoveMontageEnabled"));

// 자주 사용되는 태그 컨테이너 초기화
const FGameplayTagContainer UKYGameplayTags::FCharacterStateTags::CombatTags = []() {
    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.IsCombat")));
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsAttacking")));
    return Tags;
}();

const FGameplayTagContainer UKYGameplayTags::FCharacterStateTags::MovementBlockTags = []() {
    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Unstable")));
    Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable")));
    return Tags;
}();


// 데이터 태그 정의
const FGameplayTag UKYGameplayTags::FDataTags::Ability = FGameplayTag::RequestGameplayTag(FName("Data.Ability"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Common_Hit_KnockDown = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Common.Hit.KnockDown"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Common_Hit_Staggered = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Common.Hit.Staggered"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Common_Jump = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Common.Jump"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Common_Combat = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Common.Combat"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Common_Dead = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Common.Dead"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Enemy_Attack = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Enemy.Attack"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Enemy_Attack_Light = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Enemy.Attack.Light"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack_Light = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack.Light"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack_Heavy = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack.Heavy"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack_Air = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack.Air"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack_Range = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack.Range"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack_Upper = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack.Upper"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Attack_Special = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Attack.Special"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Dodge = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Dodge"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_DoubleJump = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.DoubleJump"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Execution = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Execution"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Glide = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Glide"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Guard = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Guard"));
const FGameplayTag UKYGameplayTags::FDataTags::Ability_Player_Focus = FGameplayTag::RequestGameplayTag(FName("Data.Ability.Player.Focus"));
const FGameplayTag UKYGameplayTags::FDataTags::Data = FGameplayTag::RequestGameplayTag(FName("Data"));

// 이벤트 태그 정의
const FGameplayTag UKYGameplayTags::FEventTags::Combo_Available = FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.ComboAvailable"));
const FGameplayTag UKYGameplayTags::FEventTags::AttackHit = FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.AttackHitCheck"));
const FGameplayTag UKYGameplayTags::FEventTags::SkillHit = FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.SkillHitCheck"));
const FGameplayTag UKYGameplayTags::FEventTags::Bounty_Experience = FGameplayTag::RequestGameplayTag(FName("Event.Bounty.Experience"));
const FGameplayTag UKYGameplayTags::FEventTags::Hit = FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit"));
const FGameplayTag UKYGameplayTags::FEventTags::LevelUp = FGameplayTag::RequestGameplayTag(FName("Event.Character.LevelUp"));
const FGameplayTag UKYGameplayTags::FEventTags::Execution = FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Execution"));

// 게임플레이 큐 태그 정의
const FGameplayTag UKYGameplayTags::FGameplayCueTags::Character_AttackHit = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"));

// 정적 멤버 초기화
UKYGameplayTags::FCharacterStateTags UKYGameplayTags::CharacterState;
UKYGameplayTags::FDataTags UKYGameplayTags::Data;
UKYGameplayTags::FEventTags UKYGameplayTags::Event;
UKYGameplayTags::FGameplayCueTags UKYGameplayTags::GameplayCue;

void UKYGameplayTags::InitializeTags()
{
    // 이미 모든 태그가 정적 초기화되었음.
    // 필요한 경우 추가 초기화 로직을 구현한다.
} 