#include "KYItem.h"

UKYEquipmentItem::UKYEquipmentItem(): EquipmentType(EKYEquipmentType::None), bIsEquipped(false)
{
}

UKYWeaponItem::UKYWeaponItem(): bIsInHand(false), WeaponSlotIndex(-1), WeaponType(EKYWeaponType::None)
{
}
