#pragma once

#include <cstdint>
#include "common/Data.h"

namespace Offset
{
	inline void Set(const char* key, uint64_t value)
	{
		GameData.Offset[key] = value;
	}

	inline void Sever_Init()
	{
		GameData.Offset.clear();

		// Static offsets only. Network/file fetching has been removed on purpose.
		// Update these values directly in this header when the game updates.

		// Core world pointers
		Set("UWorld", 0x1249AC38);
		Set("XenuineDecrypt", 0x109A3228);
		Set("GNames", 0x1272E560);
		Set("ChunkSize", 0x3E4C);

		// FName / object id decode
		Set("ObjID", 0x20);
		Set("ObjID2", 0x1C);
		Set("DecryptNameIndexRor", 1);
		Set("DecryptNameIndexXorKey1", 0x07360F24);
		Set("DecryptNameIndexXorKey2", 0xB621EC05);
		Set("DecryptNameIndexRval", 23);
		Set("DecryptNameIndexSval", 25);
		Set("DecryptNameIndex2Ror", 1);
		Set("DecryptNameIndex2XorKey1", 0x221A03AC);
		Set("DecryptNameIndex2XorKey2", 0x12082DD7);
		Set("DecryptNameIndex2Rval", 17);
		Set("DecryptNameIndex2Sval", 31);

		// World / actor traversal
		Set("CurrentLevel", 0x800);
		Set("GameInstance", 0x3B0);
		Set("LocalPlayer", 0xF0);
		Set("Actors", 0x38);
		Set("PlayerController", 0x38);
		Set("AcknowledgedPawn", 0x4A8);
		Set("PlayerCameraManager", 0x4D0);
		Set("RootComponent", 0x308);
		Set("Mesh", 0x4A0);
		Set("StaticMesh", 0xAE8);
		Set("ComponentToWorld", 0x320);
		Set("ComponentLocation", 0x330);
		Set("LastUpdateVelocity", 0x23C);

		// Camera
		Set("CameraCacheFOV", 0xA2C);
		Set("CameraCacheLocation", 0xA30);
		Set("CameraCacheRotation", 0xA10);

		// Player
		Set("GroggyHealth", 0x1530);
		Set("LastTeamNum", 0x2B18);
		Set("CharacterName", 0x1E00);
		Set("SpectatedCount", 0x11BC);

		// Weapon / recoil
		Set("WeaponProcessor", 0x968);
		Set("EquippedWeapons", 0x208);
		Set("CurrentWeaponIndex", 0x319);
		Set("WeaponTrajectoryData", 0x11A8);
		Set("TrajectoryConfig", 0x108);
		Set("AnimScriptInstance", 0xE30);
		Set("ControlRotation_CP", 0x654);
		Set("RecoilADSRotation_CP", 0x824);
		Set("LeanLeftAlpha_CP", 0x69C);
		Set("LeanRightAlpha_CP", 0x6A0);

		// Vehicle / render / physics
		Set("VehicleRiderComponent", 0x20E0);
		Set("LastVehiclePawn", 0x270);
		Set("ReplicatedMovement", 0xD0);
		Set("SeatIndex", 0x230);
		Set("LastSubmitTime", 0x758);
		Set("LastRenderTimeOnScreen", 0x75C);
		Set("bAlwaysCreatePhysicsState", 0x488);

		// Health-related values from the current reference are unknown/zeroed.
		Set("HeaFlag", 0x0);
		Set("Health1", 0x0);
		Set("Health2", 0x0);
		Set("Health3", 0x0);
		Set("Health4", 0x0);
		Set("Health5", 0x0);
		Set("Health6", 0x0);
		Set("DecryptedHealthOffsets0", 0x0);
		Set("DecryptedHealthOffsets1", 0x0);
		Set("DecryptedHealthOffsets2", 0x0);
		Set("DecryptedHealthOffsets3", 0x0);
		Set("DecryptedHealthOffsets4", 0x0);
		Set("DecryptedHealthOffsets5", 0x0);
		Set("DecryptedHealthOffsets6", 0x0);
		Set("DecryptedHealthOffsets7", 0x0);
		Set("DecryptedHealthOffsets8", 0x0);
		Set("DecryptedHealthOffsets9", 0x0);
		Set("DecryptedHealthOffsets10", 0x0);
		Set("DecryptedHealthOffsets11", 0x0);
		Set("DecryptedHealthOffsets12", 0x0);
		Set("DecryptedHealthOffsets13", 0x0);
		Set("DecryptedHealthOffsets14", 0x0);
		Set("DecryptedHealthOffsets15", 0x0);
		Set("Health", GameData.Offset["Health4"]);
		Set("bEncryptedHealth", GameData.Offset["Health"] + 0x15);
		Set("EncryptedHealthOffset", GameData.Offset["Health"] + 0x14);
		Set("DecryptedHealthOffset", GameData.Offset["Health"] + 0x10);

		// Keep optional keys present so older call sites resolve to a stable default.
		Set("GNamesPtr", 0x0);
		Set("TimeSeconds", 0x0);
		Set("WorldToMap", 0x0);
		Set("CharacterMovement", 0x0);
		Set("PlayerInput", 0x0);
		Set("InputYawScale", 0x0);
		Set("bShowMouseCursor", 0x0);
		Set("MyHUD", 0x0);
		Set("InputAxisProperties", 0x0);
		Set("GameState", 0x0);
		Set("SafetyZoneRadius", 0x0);
		Set("SafetyZonePosition", 0x0);
		Set("BlueZoneRadius", 0x0);
		Set("BlueZonePosition", 0x0);
		Set("PlayerArray", 0x0);
		Set("ViewTarget", 0x0);
		Set("PlayerState", 0x0);
		Set("CharacterState", 0x0);
		Set("PreEvalPawnState", 0x0);
		Set("Eyes", 0x0);
		Set("AimOffsets", 0x0);
		Set("PlayerStatistics", 0x0);
		Set("DamageDealtOnEnemy", 0x0);
		Set("PartnerLevel", 0x0);
		Set("TeamNumber", 0x0);
		Set("PlayerName", 0x0);
		Set("PlayerStatusType", 0x0);
		Set("SquadMemberIndex", 0x0);
		Set("AccountId", 0x0);
		Set("SurvivalTier", 0x0);
		Set("SurvivalLevel", 0x0);
		Set("CharacterClanInfo", 0x0);
		Set("VehicleMovement", 0x0);
		Set("Wheels", 0x0);
		Set("WheelLocation", 0x0);
		Set("DampingRate", 0x0);
		Set("ShapeRadius", 0x0);
		Set("WidgetStateMap", 0x0);
		Set("TrainingMapGrid", 0x0);
		Set("MapGrid_Map", 0x0);
		Set("LayoutData", 0x0);
		Set("Visibility", 0x0);
		Set("Slot", 0x0);
		Set("Offsets", 0x0);
		Set("Alignment", 0x0);
		Set("BlockInputWidgetList", 0x0);
		Set("SelectMinimapSizeIndex", 0x0);
		Set("ItemID", 0x0);
		Set("ItemTable", 0x0);
		Set("ItemPackageItems", 0x0);
		Set("DroppedItemGroup", 0x0);
		Set("DroppedItem", 0x0);
		Set("DroppedItemGroupUItem", 0x0);
		Set("TimeTillExplosion", 0x0);
		Set("ExplodeState", 0x0);
		Set("CurrentAmmoData", 0x0);
		Set("TrajectoryGravityZ", 0x0);
		Set("BallisticCurve", 0x0);
		Set("FloatCurves", 0x0);
		Set("Keys", 0x0);
		Set("WeaponConfig_WeaponClass", 0x0);
		Set("Mesh3P", 0x0);
		Set("FiringAttachPoint", 0x0);
		Set("AttachedStaticComponentMap", 0x0);
		Set("AttachedItems", 0x0);
		Set("WeaponAttachmentData", 0x0);
		Set("ScopingAttachPoint", 0x0);
		Set("ElapsedCookingTime", 0x0);
		Set("bIsScoping_CP", 0x0);
		Set("bIsReloading_CP", 0x0);
		Set("StaticSockets", 0x0);
		Set("StaticSocketName", 0x0);
		Set("StaticRelativeLocation", 0x0);
		Set("StaticRelativeRotation", 0x0);
		Set("StaticRelativeScale", 0x0);
		Set("VehicleFuel", 0x0);
		Set("VehicleCommonComponent", 0x0);
		Set("FloatingComponent", 0x0);
		Set("VehicleFuelMax", 0x0);
		Set("VehicleHealth", 0x0);
		Set("VehicleHealthMax", 0x0);
		Set("InventoryFacade", 0x0);
		Set("Inventory", 0x0);
		Set("InventoryItems", 0x0);
		Set("InventoryItemCount", 0x0);
		Set("InventoryItemTagItemCount", 0x0);
		Set("MouseX", 0x0);
		Set("MouseY", 0x0);
		Set("NumAliveTeams", 0x0);
		Set("AntiCheatCharacterSyncManager", 0x0);
	}
}
