#pragma once

#include "GameplayTagContainer.h"

#define KYTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

#define KYTAG_CHARACTER_ATTACK_LIGHT FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Light"))
#define KYTAG_CHARACTER_ATTACK_HEAVY FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Heavy"))
#define KYTAG_CHARACTER_ATTACK_UPPER FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Upper"))
#define KYTAG_CHARACTER_ATTACK_SLAM FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Slam"))
#define KYTAG_CHARACTER_ATTACK_GRAB FGameplayTag::RequestGameplayTag(FName("Character.State.Attack.Grab"))

#define KYTAG_CHARACTER_UNMOVABLE FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable"))
#define KYTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define KYTAG_CHARACTER_ISGLIDING FGameplayTag::RequestGameplayTag(FName("Character.State.IsGliding"))
#define KYTAG_CHARACTER_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsAttacking"))
#define KYTAG_CHARACTER_ISAIMING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAiming"))
#define KYTAG_CHARACTER_ISSKILLING FGameplayTag::RequestGameplayTag(FName("Character.State.IsSkilling"))
#define KYTAG_CHARACTER_ISDASH FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsDash"))
#define KYTAG_CHARACTER_INVINCIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"))
#define KYTAG_CHARACTER_ROTABLE FGameplayTag::RequestGameplayTag(FName("Character.State.IsRotable"))
#define KYTAG_CHARACTER_ISFALLING FGameplayTag::RequestGameplayTag(FName("Character.State.IsFalling"))

#define KYTAG_EVENT_COMBO_AVAILABLE FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.ComboAvailable"))
#define KYTAG_EVENT_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.AttackHitCheck"))

#define KYTAG_EVENT_HIT FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit"))
#define KYTAG_EVENT_HIT_STAGGER FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit.Stagger"))
#define KYTAG_EVENT_HIT_LAUNCH FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit.Launch"))
#define KYTAG_EVENT_HIT_KNOCKBACK FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit.Knockback"))
#define KYTAG_EVENT_HIT_SLAM FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit.Slam"))
#define KYTAG_EVENT_HIT_SNATCH FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.Hit.Snatch"))

#define GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))