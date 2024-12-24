#pragma once

#include "GameplayTagContainer.h"

#define KYTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define KYTAG_CHARACTER_UNMOVABLE FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable"))
#define KYTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define KYTAG_CHARACTER_ISGLIDING FGameplayTag::RequestGameplayTag(FName("Character.State.IsGliding"))
#define KYTAG_CHARACTER_ISATTACKING FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsAttacking"))
#define KYTAG_CHARACTER_ISAIMING FGameplayTag::RequestGameplayTag(FName("Character.State.IsAiming"))
#define KYTAG_CHARACTER_ISSKILLING FGameplayTag::RequestGameplayTag(FName("Character.State.IsSkilling"))
#define KYTAG_CHARACTER_ISDASH FGameplayTag::RequestGameplayTag(FName("Character.State.Unmovable.IsDash"))
#define KYTAG_CHARACTER_INVINCIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invincible"))
#define KYTAG_EVENT_COMBO_AVAILABLE FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.ComboAvailable"))
#define KYTAG_EVENT_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("Event.Character.Action.AttackHitCheck"))

#define GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))