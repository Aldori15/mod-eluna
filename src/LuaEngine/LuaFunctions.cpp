/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

extern "C"
{
#include "lua.h"
};

// Eluna
#include "LuaEngine.h"
#include "ElunaEventMgr.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"
#include "ElunaUtility.h"

// Method includes
#include "GlobalMethods.h"
#include "ObjectMethods.h"
#include "WorldObjectMethods.h"
#include "UnitMethods.h"
#include "PlayerMethods.h"
#include "CreatureMethods.h"
#include "GroupMethods.h"
#include "GuildMethods.h"
#include "GameObjectMethods.h"
#include "ElunaQueryMethods.h"
#include "AuraMethods.h"
#include "ItemMethods.h"
#include "WorldPacketMethods.h"
#include "SpellMethods.h"
#include "QuestMethods.h"
#include "MapMethods.h"
#include "CorpseMethods.h"
#include "VehicleMethods.h"
#include "BattleGroundMethods.h"
#include "ChatHandlerMethods.h"
#include "AchievementMethods.h"
#include "ItemTemplateMethods.h"
#include "RollMethods.h"
#include "TicketMethods.h"
#include "SpellInfoMethods.h"
#include "LootMethods.h"

// DBCStores includes
#include "GemPropertiesEntryMethods.h"
#include "SpellEntryMethods.h"

luaL_Reg GlobalMethods[] =
{
    // Hooks
    { "RegisterPacketEvent", &LuaGlobalFunctions::RegisterPacketEvent },
    { "RegisterServerEvent", &LuaGlobalFunctions::RegisterServerEvent },
    { "RegisterPlayerEvent", &LuaGlobalFunctions::RegisterPlayerEvent },
    { "RegisterGuildEvent", &LuaGlobalFunctions::RegisterGuildEvent },
    { "RegisterGroupEvent", &LuaGlobalFunctions::RegisterGroupEvent },
    { "RegisterCreatureEvent", &LuaGlobalFunctions::RegisterCreatureEvent },
    { "RegisterUniqueCreatureEvent", &LuaGlobalFunctions::RegisterUniqueCreatureEvent },
    { "RegisterCreatureGossipEvent", &LuaGlobalFunctions::RegisterCreatureGossipEvent },
    { "RegisterGameObjectEvent", &LuaGlobalFunctions::RegisterGameObjectEvent },
    { "RegisterGameObjectGossipEvent", &LuaGlobalFunctions::RegisterGameObjectGossipEvent },
    { "RegisterItemEvent", &LuaGlobalFunctions::RegisterItemEvent },
    { "RegisterItemGossipEvent", &LuaGlobalFunctions::RegisterItemGossipEvent },
    { "RegisterPlayerGossipEvent", &LuaGlobalFunctions::RegisterPlayerGossipEvent },
    { "RegisterBGEvent", &LuaGlobalFunctions::RegisterBGEvent },
    { "RegisterMapEvent", &LuaGlobalFunctions::RegisterMapEvent },
    { "RegisterInstanceEvent", &LuaGlobalFunctions::RegisterInstanceEvent },
    { "RegisterTicketEvent", &LuaGlobalFunctions::RegisterTicketEvent },
    { "RegisterSpellEvent", &LuaGlobalFunctions::RegisterSpellEvent },

    { "ClearBattleGroundEvents", &LuaGlobalFunctions::ClearBattleGroundEvents },
    { "ClearCreatureEvents", &LuaGlobalFunctions::ClearCreatureEvents },
    { "ClearUniqueCreatureEvents", &LuaGlobalFunctions::ClearUniqueCreatureEvents },
    { "ClearCreatureGossipEvents", &LuaGlobalFunctions::ClearCreatureGossipEvents },
    { "ClearGameObjectEvents", &LuaGlobalFunctions::ClearGameObjectEvents },
    { "ClearGameObjectGossipEvents", &LuaGlobalFunctions::ClearGameObjectGossipEvents },
    { "ClearGroupEvents", &LuaGlobalFunctions::ClearGroupEvents },
    { "ClearGuildEvents", &LuaGlobalFunctions::ClearGuildEvents },
    { "ClearItemEvents", &LuaGlobalFunctions::ClearItemEvents },
    { "ClearItemGossipEvents", &LuaGlobalFunctions::ClearItemGossipEvents },
    { "ClearPacketEvents", &LuaGlobalFunctions::ClearPacketEvents },
    { "ClearPlayerEvents", &LuaGlobalFunctions::ClearPlayerEvents },
    { "ClearPlayerGossipEvents", &LuaGlobalFunctions::ClearPlayerGossipEvents },
    { "ClearServerEvents", &LuaGlobalFunctions::ClearServerEvents },
    { "ClearMapEvents", &LuaGlobalFunctions::ClearMapEvents },
    { "ClearInstanceEvents", &LuaGlobalFunctions::ClearInstanceEvents },
    { "ClearTicketEvents", &LuaGlobalFunctions::ClearTicketEvents },
    { "ClearSpellEvents", &LuaGlobalFunctions::ClearSpellEvents },

    // Getters
    { "GetLuaEngine", &LuaGlobalFunctions::GetLuaEngine },
    { "GetCoreName", &LuaGlobalFunctions::GetCoreName },
    { "GetConfigValue", &LuaGlobalFunctions::GetConfigValue },
    { "GetRealmID", &LuaGlobalFunctions::GetRealmID },
    { "GetCoreVersion", &LuaGlobalFunctions::GetCoreVersion },
    { "GetCoreExpansion", &LuaGlobalFunctions::GetCoreExpansion },
    { "GetStateMap", &LuaGlobalFunctions::GetStateMap },
    { "GetStateMapId", &LuaGlobalFunctions::GetStateMapId },
    { "GetStateInstanceId", &LuaGlobalFunctions::GetStateInstanceId },
    { "GetQuest", &LuaGlobalFunctions::GetQuest },
    { "GetPlayerByGUID", &LuaGlobalFunctions::GetPlayerByGUID },
    { "GetPlayerByName", &LuaGlobalFunctions::GetPlayerByName },
    { "GetGameTime", &LuaGlobalFunctions::GetGameTime },
    { "GetPlayersInWorld", &LuaGlobalFunctions::GetPlayersInWorld },
    { "GetGuildByName", &LuaGlobalFunctions::GetGuildByName },
    { "GetGuildByLeaderGUID", &LuaGlobalFunctions::GetGuildByLeaderGUID },
    { "GetPlayerCount", &LuaGlobalFunctions::GetPlayerCount },
    { "GetPlayerGUID", &LuaGlobalFunctions::GetPlayerGUID },
    { "GetItemGUID", &LuaGlobalFunctions::GetItemGUID },
    { "GetItemTemplate", &LuaGlobalFunctions::GetItemTemplate },
    { "GetObjectGUID", &LuaGlobalFunctions::GetObjectGUID },
    { "GetUnitGUID", &LuaGlobalFunctions::GetUnitGUID },
    { "GetGUIDLow", &LuaGlobalFunctions::GetGUIDLow },
    { "GetGUIDType", &LuaGlobalFunctions::GetGUIDType },
    { "GetGUIDEntry", &LuaGlobalFunctions::GetGUIDEntry },
    { "GetAreaName", &LuaGlobalFunctions::GetAreaName },
    { "GetOwnerHalaa", &LuaGlobalFunctions::GetOwnerHalaa },
    { "bit_not", &LuaGlobalFunctions::bit_not },
    { "bit_xor", &LuaGlobalFunctions::bit_xor },
    { "bit_rshift", &LuaGlobalFunctions::bit_rshift },
    { "bit_lshift", &LuaGlobalFunctions::bit_lshift },
    { "bit_or", &LuaGlobalFunctions::bit_or },
    { "bit_and", &LuaGlobalFunctions::bit_and },
    { "GetItemLink", &LuaGlobalFunctions::GetItemLink },
    { "GetMapById", &LuaGlobalFunctions::GetMapById },
    { "GetCurrTime", &LuaGlobalFunctions::GetCurrTime },
    { "GetTimeDiff", &LuaGlobalFunctions::GetTimeDiff },
    { "PrintInfo", &LuaGlobalFunctions::PrintInfo },
    { "PrintError", &LuaGlobalFunctions::PrintError },
    { "PrintDebug", &LuaGlobalFunctions::PrintDebug },
    { "GetActiveGameEvents", &LuaGlobalFunctions::GetActiveGameEvents },
    { "GetGossipMenuOptionLocale", &LuaGlobalFunctions::GetGossipMenuOptionLocale },
    { "GetMapEntrance", &LuaGlobalFunctions::GetMapEntrance },
    { "GetSpellInfo", &LuaGlobalFunctions::GetSpellInfo },

    // Boolean
    { "IsCompatibilityMode", &LuaGlobalFunctions::IsCompatibilityMode },
    { "IsInventoryPos", &LuaGlobalFunctions::IsInventoryPos },
    { "IsEquipmentPos", &LuaGlobalFunctions::IsEquipmentPos },
    { "IsBankPos", &LuaGlobalFunctions::IsBankPos },
    { "IsBagPos", &LuaGlobalFunctions::IsBagPos },
    { "IsGameEventActive", &LuaGlobalFunctions::IsGameEventActive },

    // Other
    { "ReloadEluna", &LuaGlobalFunctions::ReloadEluna },
    { "RunCommand", &LuaGlobalFunctions::RunCommand },
    { "SendWorldMessage", &LuaGlobalFunctions::SendWorldMessage },
    { "WorldDBQuery", &LuaGlobalFunctions::WorldDBQuery },
    { "WorldDBQueryAsync", &LuaGlobalFunctions::WorldDBQueryAsync },
    { "WorldDBExecute", &LuaGlobalFunctions::WorldDBExecute },
    { "CharDBQuery", &LuaGlobalFunctions::CharDBQuery },
    { "CharDBQueryAsync", &LuaGlobalFunctions::CharDBQueryAsync },
    { "CharDBExecute", &LuaGlobalFunctions::CharDBExecute },
    { "AuthDBQuery", &LuaGlobalFunctions::AuthDBQuery },
    { "AuthDBQueryAsync", &LuaGlobalFunctions::AuthDBQueryAsync },
    { "AuthDBExecute", &LuaGlobalFunctions::AuthDBExecute },
    { "CreateLuaEvent", &LuaGlobalFunctions::CreateLuaEvent },
    { "RemoveEventById", &LuaGlobalFunctions::RemoveEventById },
    { "RemoveEvents", &LuaGlobalFunctions::RemoveEvents },
    { "PerformIngameSpawn", &LuaGlobalFunctions::PerformIngameSpawn },
    { "CreatePacket", &LuaGlobalFunctions::CreatePacket },
    { "AddVendorItem", &LuaGlobalFunctions::AddVendorItem },
    { "VendorRemoveItem", &LuaGlobalFunctions::VendorRemoveItem },
    { "VendorRemoveAllItems", &LuaGlobalFunctions::VendorRemoveAllItems },
    { "Kick", &LuaGlobalFunctions::Kick },
    { "Ban", &LuaGlobalFunctions::Ban },
    { "SaveAllPlayers", &LuaGlobalFunctions::SaveAllPlayers },
    { "SendMail", &LuaGlobalFunctions::SendMail },
    { "AddTaxiPath", &LuaGlobalFunctions::AddTaxiPath },
    { "CreateInt64", &LuaGlobalFunctions::CreateLongLong },
    { "CreateUint64", &LuaGlobalFunctions::CreateULongLong },
    { "StartGameEvent", &LuaGlobalFunctions::StartGameEvent },
    { "StopGameEvent", &LuaGlobalFunctions::StopGameEvent },
    { "HttpRequest", &LuaGlobalFunctions::HttpRequest },
    { "SetOwnerHalaa", &LuaGlobalFunctions::SetOwnerHalaa },
    { "LookupEntry", &LuaGlobalFunctions::LookupEntry },

    { NULL, NULL }
};

ElunaRegister<Object> ObjectMethods[] =
{
    // Getters
    { "GetEntry", &LuaObject::GetEntry },
    { "GetGUID", &LuaObject::GetGUID },
    { "GetGUIDLow", &LuaObject::GetGUIDLow },
    { "GetInt32Value", &LuaObject::GetInt32Value },
    { "GetUInt32Value", &LuaObject::GetUInt32Value },
    { "GetFloatValue", &LuaObject::GetFloatValue },
    { "GetByteValue", &LuaObject::GetByteValue },
    { "GetUInt16Value", &LuaObject::GetUInt16Value },
    { "GetUInt64Value", &LuaObject::GetUInt64Value },
    { "GetScale", &LuaObject::GetScale },
    { "GetTypeId", &LuaObject::GetTypeId },

    // Setters
    { "SetInt32Value", &LuaObject::SetInt32Value },
    { "SetUInt32Value", &LuaObject::SetUInt32Value },
    { "UpdateUInt32Value", &LuaObject::UpdateUInt32Value },
    { "SetFloatValue", &LuaObject::SetFloatValue },
    { "SetByteValue", &LuaObject::SetByteValue },
    { "SetUInt16Value", &LuaObject::SetUInt16Value },
    { "SetInt16Value", &LuaObject::SetInt16Value },
    { "SetUInt64Value", &LuaObject::SetUInt64Value },
    { "SetScale", &LuaObject::SetScale },
    { "SetFlag", &LuaObject::SetFlag },

    // Boolean
    { "IsInWorld", &LuaObject::IsInWorld },
    { "IsPlayer", &LuaObject::IsPlayer },
    { "HasFlag", &LuaObject::HasFlag },

    // Other
    { "ToGameObject", &LuaObject::ToGameObject },
    { "ToUnit", &LuaObject::ToUnit },
    { "ToCreature", &LuaObject::ToCreature },
    { "ToPlayer", &LuaObject::ToPlayer },
    { "ToCorpse", &LuaObject::ToCorpse },
    { "RemoveFlag", &LuaObject::RemoveFlag },

    { NULL, NULL }
};

ElunaRegister<WorldObject> WorldObjectMethods[] =
{
    // Getters
    { "GetName", &LuaWorldObject::GetName },
    { "GetMap", &LuaWorldObject::GetMap },
    { "GetPhaseMask", &LuaWorldObject::GetPhaseMask },
    { "SetPhaseMask", &LuaWorldObject::SetPhaseMask },
    { "GetInstanceId", &LuaWorldObject::GetInstanceId },
    { "GetAreaId", &LuaWorldObject::GetAreaId },
    { "GetZoneId", &LuaWorldObject::GetZoneId },
    { "GetMapId", &LuaWorldObject::GetMapId },
    { "GetX", &LuaWorldObject::GetX },
    { "GetY", &LuaWorldObject::GetY },
    { "GetZ", &LuaWorldObject::GetZ },
    { "GetO", &LuaWorldObject::GetO },
    { "GetLocation", &LuaWorldObject::GetLocation },
    { "GetPlayersInRange", &LuaWorldObject::GetPlayersInRange },
    { "GetCreaturesInRange", &LuaWorldObject::GetCreaturesInRange },
    { "GetGameObjectsInRange", &LuaWorldObject::GetGameObjectsInRange },
    { "GetNearestPlayer", &LuaWorldObject::GetNearestPlayer },
    { "GetNearestGameObject", &LuaWorldObject::GetNearestGameObject },
    { "GetNearestCreature", &LuaWorldObject::GetNearestCreature },
    { "GetNearObject", &LuaWorldObject::GetNearObject },
    { "GetNearObjects", &LuaWorldObject::GetNearObjects },
    { "GetDistance", &LuaWorldObject::GetDistance },
    { "GetExactDistance", &LuaWorldObject::GetExactDistance },
    { "GetDistance2d", &LuaWorldObject::GetDistance2d },
    { "GetExactDistance2d", &LuaWorldObject::GetExactDistance2d },
    { "GetRelativePoint", &LuaWorldObject::GetRelativePoint },
    { "GetAngle", &LuaWorldObject::GetAngle },

    // Boolean
    { "IsWithinLoS", &LuaWorldObject::IsWithinLoS },
    { "IsInMap", &LuaWorldObject::IsInMap },
    { "IsWithinDist3d", &LuaWorldObject::IsWithinDist3d },
    { "IsWithinDist2d", &LuaWorldObject::IsWithinDist2d },
    { "IsWithinDist", &LuaWorldObject::IsWithinDist },
    { "IsWithinDistInMap", &LuaWorldObject::IsWithinDistInMap },
    { "IsInRange", &LuaWorldObject::IsInRange },
    { "IsInRange2d", &LuaWorldObject::IsInRange2d },
    { "IsInRange3d", &LuaWorldObject::IsInRange3d },
    { "IsInFront", &LuaWorldObject::IsInFront },
    { "IsInBack", &LuaWorldObject::IsInBack },

    // Other
    { "SummonGameObject", &LuaWorldObject::SummonGameObject },
    { "SpawnCreature", &LuaWorldObject::SpawnCreature },
    { "SendPacket", &LuaWorldObject::SendPacket },
    { "RegisterEvent", &LuaWorldObject::RegisterEvent },
    { "RemoveEventById", &LuaWorldObject::RemoveEventById },
    { "RemoveEvents", &LuaWorldObject::RemoveEvents },
    { "PlayMusic", &LuaWorldObject::PlayMusic },
    { "PlayDirectSound", &LuaWorldObject::PlayDirectSound },
    { "PlayDistanceSound", &LuaWorldObject::PlayDistanceSound },

    { NULL, NULL }
};

ElunaRegister<Unit> UnitMethods[] =
{
    // Getters
    { "GetLevel", &LuaUnit::GetLevel },
    { "GetHealth", &LuaUnit::GetHealth },
    { "GetDisplayId", &LuaUnit::GetDisplayId },
    { "GetNativeDisplayId", &LuaUnit::GetNativeDisplayId },
    { "GetPower", &LuaUnit::GetPower },
    { "GetMaxPower", &LuaUnit::GetMaxPower },
    { "GetPowerType", &LuaUnit::GetPowerType },
    { "GetMaxHealth", &LuaUnit::GetMaxHealth },
    { "GetHealthPct", &LuaUnit::GetHealthPct },
    { "GetPowerPct", &LuaUnit::GetPowerPct },
    { "GetGender", &LuaUnit::GetGender },
    { "GetRace", &LuaUnit::GetRace },
    { "GetClass", &LuaUnit::GetClass },
    { "GetRaceMask", &LuaUnit::GetRaceMask },
    { "GetClassMask", &LuaUnit::GetClassMask },
    { "GetRaceAsString", &LuaUnit::GetRaceAsString },
    { "GetClassAsString", &LuaUnit::GetClassAsString },
    { "GetAura", &LuaUnit::GetAura },
    { "GetFaction", &LuaUnit::GetFaction },
    { "GetCurrentSpell", &LuaUnit::GetCurrentSpell },
    { "GetCreatureType", &LuaUnit::GetCreatureType },
    { "GetMountId", &LuaUnit::GetMountId },
    { "GetOwner", &LuaUnit::GetOwner },
    { "GetFriendlyUnitsInRange", &LuaUnit::GetFriendlyUnitsInRange },
    { "GetUnfriendlyUnitsInRange", &LuaUnit::GetUnfriendlyUnitsInRange },
    { "GetOwnerGUID", &LuaUnit::GetOwnerGUID },
    { "GetCreatorGUID", &LuaUnit::GetCreatorGUID },
    { "GetMinionGUID", &LuaUnit::GetPetGUID },
    { "GetCharmerGUID", &LuaUnit::GetCharmerGUID },
    { "GetCharmGUID", &LuaUnit::GetCharmGUID },
    { "GetPetGUID", &LuaUnit::GetPetGUID },
    { "GetCritterGUID", &LuaUnit::GetCritterGUID },
    { "GetControllerGUID", &LuaUnit::GetControllerGUID },
    { "GetControllerGUIDS", &LuaUnit::GetControllerGUIDS },
    { "GetStandState", &LuaUnit::GetStandState },
    { "GetVictim", &LuaUnit::GetVictim },
    { "GetSpeed", &LuaUnit::GetSpeed },
    { "GetSpeedRate", &LuaUnit::GetSpeedRate },
    { "GetStat", &LuaUnit::GetStat },
    { "GetBaseSpellPower", &LuaUnit::GetBaseSpellPower },
    { "GetVehicleKit", &LuaUnit::GetVehicleKit },
    // {"GetVehicle", &LuaUnit::GetVehicle},                           // :GetVehicle() - UNDOCUMENTED - Gets the Vehicle kit of the vehicle the unit is on
    { "GetMovementType", &LuaUnit::GetMovementType },
    { "GetAttackers", &LuaUnit::GetAttackers },
    { "GetThreat", &LuaUnit::GetThreat },

    // Setters
    { "SetFaction", &LuaUnit::SetFaction },
    { "SetLevel", &LuaUnit::SetLevel },
    { "SetHealth", &LuaUnit::SetHealth },
    { "SetMaxHealth", &LuaUnit::SetMaxHealth },
    { "SetPower", &LuaUnit::SetPower },
    { "SetMaxPower", &LuaUnit::SetMaxPower },
    { "SetPowerType", &LuaUnit::SetPowerType },
    { "SetDisplayId", &LuaUnit::SetDisplayId },
    { "SetNativeDisplayId", &LuaUnit::SetNativeDisplayId },
    { "SetFacing", &LuaUnit::SetFacing },
    { "SetFacingToObject", &LuaUnit::SetFacingToObject },
    { "SetSpeed", &LuaUnit::SetSpeed },
    { "SetSpeedRate", &LuaUnit::SetSpeedRate },
    // {"SetStunned", &LuaUnit::SetStunned},                           // :SetStunned([enable]) - UNDOCUMENTED - Stuns or removes stun
    {"SetRooted", &LuaUnit::SetRooted},
    {"SetConfused", &LuaUnit::SetConfused},
    {"SetFeared", &LuaUnit::SetFeared},
    { "SetPvP", &LuaUnit::SetPvP },
    { "SetFFA", &LuaUnit::SetFFA },
    { "SetSanctuary", &LuaUnit::SetSanctuary },
    // {"SetCanFly", &LuaUnit::SetCanFly},                             // :SetCanFly(apply) - UNDOCUMENTED
    // {"SetVisible", &LuaUnit::SetVisible},                           // :SetVisible(x) - UNDOCUMENTED
    { "SetOwnerGUID", &LuaUnit::SetOwnerGUID },
    { "SetName", &LuaUnit::SetName },
    { "SetSheath", &LuaUnit::SetSheath },
    { "SetCreatorGUID", &LuaUnit::SetCreatorGUID },
    { "SetMinionGUID", &LuaUnit::SetPetGUID },
    { "SetPetGUID", &LuaUnit::SetPetGUID },
    { "SetCritterGUID", &LuaUnit::SetCritterGUID },
    { "SetWaterWalk", &LuaUnit::SetWaterWalk },
    { "SetStandState", &LuaUnit::SetStandState },
    { "SetInCombatWith", &LuaUnit::SetInCombatWith },
    { "ModifyPower", &LuaUnit::ModifyPower },
    { "SetImmuneTo", &LuaUnit::SetImmuneTo },

    // Boolean
    { "IsAlive", &LuaUnit::IsAlive },
    { "IsDead", &LuaUnit::IsDead },
    { "IsDying", &LuaUnit::IsDying },
    { "IsPvPFlagged", &LuaUnit::IsPvPFlagged },
    { "IsInCombat", &LuaUnit::IsInCombat },
    { "IsBanker", &LuaUnit::IsBanker },
    { "IsBattleMaster", &LuaUnit::IsBattleMaster },
    { "IsCharmed", &LuaUnit::IsCharmed },
    { "IsArmorer", &LuaUnit::IsArmorer },
    { "IsAttackingPlayer", &LuaUnit::IsAttackingPlayer },
    { "IsInWater", &LuaUnit::IsInWater },
    { "IsUnderWater", &LuaUnit::IsUnderWater },
    { "IsAuctioneer", &LuaUnit::IsAuctioneer },
    { "IsGuildMaster", &LuaUnit::IsGuildMaster },
    { "IsInnkeeper", &LuaUnit::IsInnkeeper },
    { "IsTrainer", &LuaUnit::IsTrainer },
    { "IsGossip", &LuaUnit::IsGossip },
    { "IsTaxi", &LuaUnit::IsTaxi },
    { "IsSpiritHealer", &LuaUnit::IsSpiritHealer },
    { "IsSpiritGuide", &LuaUnit::IsSpiritGuide },
    { "IsTabardDesigner", &LuaUnit::IsTabardDesigner },
    { "IsServiceProvider", &LuaUnit::IsServiceProvider },
    { "IsSpiritService", &LuaUnit::IsSpiritService },
    { "HealthBelowPct", &LuaUnit::HealthBelowPct },
    { "HealthAbovePct", &LuaUnit::HealthAbovePct },
    { "IsMounted", &LuaUnit::IsMounted },
    { "AttackStop", &LuaUnit::AttackStop },
    { "Attack", &LuaUnit::Attack },
    // {"IsVisible", &LuaUnit::IsVisible},                              // :IsVisible() - UNDOCUMENTED
    // {"IsMoving", &LuaUnit::IsMoving},                                // :IsMoving() - UNDOCUMENTED
    // {"IsFlying", &LuaUnit::IsFlying},                                // :IsFlying() - UNDOCUMENTED
    { "IsStopped", &LuaUnit::IsStopped },
    { "HasUnitState", &LuaUnit::HasUnitState },
    { "IsQuestGiver", &LuaUnit::IsQuestGiver },
    { "IsInAccessiblePlaceFor", &LuaUnit::IsInAccessiblePlaceFor },
    { "IsVendor", &LuaUnit::IsVendor },
    { "IsRooted", &LuaUnit::IsRooted },
    { "IsFullHealth", &LuaUnit::IsFullHealth },
    { "HasAura", &LuaUnit::HasAura },
    { "IsCasting", &LuaUnit::IsCasting },
    { "IsStandState", &LuaUnit::IsStandState },
    { "IsOnVehicle", &LuaUnit::IsOnVehicle },

    // Other
    { "HandleStatModifier", &LuaUnit::HandleStatModifier },
    { "AddAura", &LuaUnit::AddAura },
    { "RemoveAura", &LuaUnit::RemoveAura },
    { "RemoveAllAuras", &LuaUnit::RemoveAllAuras },
    { "RemoveArenaAuras", &LuaUnit::RemoveArenaAuras },
    { "ClearInCombat", &LuaUnit::ClearInCombat },
    { "DeMorph", &LuaUnit::DeMorph },
    { "SendUnitWhisper", &LuaUnit::SendUnitWhisper },
    { "SendUnitEmote", &LuaUnit::SendUnitEmote },
    { "SendUnitSay", &LuaUnit::SendUnitSay },
    { "SendUnitYell", &LuaUnit::SendUnitYell },
    { "CastSpell", &LuaUnit::CastSpell },
    { "CastCustomSpell", &LuaUnit::CastCustomSpell },
    { "CastSpellAoF", &LuaUnit::CastSpellAoF },
    { "Kill", &LuaUnit::Kill },
    { "StopSpellCast", &LuaUnit::StopSpellCast },
    { "InterruptSpell", &LuaUnit::InterruptSpell },
    { "SendChatMessageToPlayer", &LuaUnit::SendChatMessageToPlayer },
    { "PerformEmote", &LuaUnit::PerformEmote },
    { "EmoteState", &LuaUnit::EmoteState },
    { "CountPctFromCurHealth", &LuaUnit::CountPctFromCurHealth },
    { "CountPctFromMaxHealth", &LuaUnit::CountPctFromMaxHealth },
    { "Dismount", &LuaUnit::Dismount },
    { "Mount", &LuaUnit::Mount },
    // {"RestoreDisplayId", &LuaUnit::RestoreDisplayId},                // :RestoreDisplayId() - UNDOCUMENTED
    // {"RestoreFaction", &LuaUnit::RestoreFaction},                    // :RestoreFaction() - UNDOCUMENTED
    // {"RemoveBindSightAuras", &LuaUnit::RemoveBindSightAuras},        // :RemoveBindSightAuras() - UNDOCUMENTED
    // {"RemoveCharmAuras", &LuaUnit::RemoveCharmAuras},                // :RemoveCharmAuras() - UNDOCUMENTED
    { "ClearThreatList", &LuaUnit::ClearThreatList },
    { "GetThreatList", &LuaUnit::GetThreatList },
    { "ClearUnitState", &LuaUnit::ClearUnitState },
    { "AddUnitState", &LuaUnit::AddUnitState },
    // {"DisableMelee", &LuaUnit::DisableMelee},                        // :DisableMelee([disable]) - UNDOCUMENTED - if true, enables
    // {"SummonGuardian", &LuaUnit::SummonGuardian},                    // :SummonGuardian(entry, x, y, z, o[, duration]) - UNDOCUMENTED - summons a guardian to location. Scales with summoner, is friendly to him and guards him.
    { "NearTeleport", &LuaUnit::NearTeleport },
    { "MoveIdle", &LuaUnit::MoveIdle },
    { "MoveRandom", &LuaUnit::MoveRandom },
    { "MoveHome", &LuaUnit::MoveHome },
    { "MoveFollow", &LuaUnit::MoveFollow },
    { "MoveChase", &LuaUnit::MoveChase },
    { "MoveConfused", &LuaUnit::MoveConfused },
    { "MoveFleeing", &LuaUnit::MoveFleeing },
    { "MoveTo", &LuaUnit::MoveTo },
    { "MoveJump", &LuaUnit::MoveJump },
    { "MoveStop", &LuaUnit::MoveStop },
    { "MoveExpire", &LuaUnit::MoveExpire },
    { "MoveClear", &LuaUnit::MoveClear },
    { "DealDamage", &LuaUnit::DealDamage },
    { "DealHeal", &LuaUnit::DealHeal },
    { "AddThreat", &LuaUnit::AddThreat },
    { "ModifyThreatPct", &LuaUnit::ModifyThreatPct },
    { "ClearThreat", &LuaUnit::ClearThreat },
    { "ResetAllThreat", &LuaUnit::ResetAllThreat },

    { NULL, NULL }
};

ElunaRegister<Player> PlayerMethods[] =
{
    // Getters
    { "GetSelection", &LuaPlayer::GetSelection },
    { "GetGMRank", &LuaPlayer::GetGMRank },
    { "GetGuildId", &LuaPlayer::GetGuildId },
    { "GetCoinage", &LuaPlayer::GetCoinage },
    { "GetTeam", &LuaPlayer::GetTeam },
    { "GetItemCount", &LuaPlayer::GetItemCount },
    { "GetGroup", &LuaPlayer::GetGroup },
    { "GetGuild", &LuaPlayer::GetGuild },
    { "GetAccountId", &LuaPlayer::GetAccountId },
    { "GetAccountName", &LuaPlayer::GetAccountName },
    { "GetCompletedQuestsCount", &LuaPlayer::GetCompletedQuestsCount },
    { "GetArenaPoints", &LuaPlayer::GetArenaPoints },
    { "GetHonorPoints", &LuaPlayer::GetHonorPoints },
    { "GetLifetimeKills", &LuaPlayer::GetLifetimeKills },
    { "GetPlayerIP", &LuaPlayer::GetPlayerIP },
    { "GetLevelPlayedTime", &LuaPlayer::GetLevelPlayedTime },
    { "GetTotalPlayedTime", &LuaPlayer::GetTotalPlayedTime },
    { "GetItemByPos", &LuaPlayer::GetItemByPos },
    { "GetItemByEntry", &LuaPlayer::GetItemByEntry },
    { "GetItemByGUID", &LuaPlayer::GetItemByGUID },
    { "GetMailCount", &LuaPlayer::GetMailCount },
    { "GetMailItem", &LuaPlayer::GetMailItem },
    { "GetReputation", &LuaPlayer::GetReputation },
    { "GetEquippedItemBySlot", &LuaPlayer::GetEquippedItemBySlot },
    { "GetQuestLevel", &LuaPlayer::GetQuestLevel },
    { "GetChatTag", &LuaPlayer::GetChatTag },
    { "GetRestBonus", &LuaPlayer::GetRestBonus },
    { "GetPhaseMaskForSpawn", &LuaPlayer::GetPhaseMaskForSpawn },
    { "GetAchievementPoints", &LuaPlayer::GetAchievementPoints },
    { "GetCompletedAchievementsCount", &LuaPlayer::GetCompletedAchievementsCount },
    { "GetReqKillOrCastCurrentCount", &LuaPlayer::GetReqKillOrCastCurrentCount },
    { "GetQuestStatus", &LuaPlayer::GetQuestStatus },
    { "GetInGameTime", &LuaPlayer::GetInGameTime },
    { "GetComboPoints", &LuaPlayer::GetComboPoints },
    { "GetComboTarget", &LuaPlayer::GetComboTarget },
    { "GetGuildName", &LuaPlayer::GetGuildName },
    { "GetFreeTalentPoints", &LuaPlayer::GetFreeTalentPoints },
    { "GetActiveSpec", &LuaPlayer::GetActiveSpec },
    { "GetSpecsCount", &LuaPlayer::GetSpecsCount },
    { "GetSpellCooldownDelay", &LuaPlayer::GetSpellCooldownDelay },
    { "GetGuildRank", &LuaPlayer::GetGuildRank },
    { "GetDifficulty", &LuaPlayer::GetDifficulty },
    { "GetHealthBonusFromStamina", &LuaPlayer::GetHealthBonusFromStamina },
    { "GetManaBonusFromIntellect", &LuaPlayer::GetManaBonusFromIntellect },
    { "GetMaxSkillValue", &LuaPlayer::GetMaxSkillValue },
    { "GetPureMaxSkillValue", &LuaPlayer::GetPureMaxSkillValue },
    { "GetSkillValue", &LuaPlayer::GetSkillValue },
    { "GetBaseSkillValue", &LuaPlayer::GetBaseSkillValue },
    { "GetPureSkillValue", &LuaPlayer::GetPureSkillValue },
    { "GetSkillPermBonusValue", &LuaPlayer::GetSkillPermBonusValue },
    { "GetSkillTempBonusValue", &LuaPlayer::GetSkillTempBonusValue },
    { "GetReputationRank", &LuaPlayer::GetReputationRank },
    { "GetDrunkValue", &LuaPlayer::GetDrunkValue },
    { "GetBattlegroundId", &LuaPlayer::GetBattlegroundId },
    { "GetBattlegroundTypeId", &LuaPlayer::GetBattlegroundTypeId },
    { "GetXP", &LuaPlayer::GetXP },
    { "GetXPRestBonus", &LuaPlayer::GetXPRestBonus },
    { "GetGroupInvite", &LuaPlayer::GetGroupInvite },
    { "GetSubGroup", &LuaPlayer::GetSubGroup },
    { "GetNextRandomRaidMember", &LuaPlayer::GetNextRandomRaidMember },
    { "GetOriginalGroup", &LuaPlayer::GetOriginalGroup },
    { "GetOriginalSubGroup", &LuaPlayer::GetOriginalSubGroup },
    { "GetChampioningFaction", &LuaPlayer::GetChampioningFaction },
    { "GetLatency", &LuaPlayer::GetLatency },
    // {"GetRecruiterId", &LuaPlayer::GetRecruiterId},                            // :GetRecruiterId() - UNDOCUMENTED - Returns player's recruiter's ID
    { "GetDbLocaleIndex", &LuaPlayer::GetDbLocaleIndex },
    { "GetDbcLocale", &LuaPlayer::GetDbcLocale },
    { "GetCorpse", &LuaPlayer::GetCorpse },
    { "GetGossipTextId", &LuaPlayer::GetGossipTextId },
    { "GetQuestRewardStatus", &LuaPlayer::GetQuestRewardStatus },
    { "GetShieldBlockValue", &LuaPlayer::GetShieldBlockValue },
    { "GetPlayerSettingValue", &LuaPlayer::GetPlayerSettingValue },
    { "GetTrader", &LuaPlayer::GetTrader },
    { "GetBonusTalentCount", &LuaPlayer::GetBonusTalentCount },
    { "GetKnownTaxiNodes", &LuaPlayer::GetKnownTaxiNodes },

    // Setters
    { "AdvanceSkillsToMax", &LuaPlayer::AdvanceSkillsToMax },
    { "AdvanceSkill", &LuaPlayer::AdvanceSkill },
    { "AdvanceAllSkills", &LuaPlayer::AdvanceAllSkills },
    { "AddLifetimeKills", &LuaPlayer::AddLifetimeKills },
    { "SetCoinage", &LuaPlayer::SetCoinage },
    { "SetKnownTitle", &LuaPlayer::SetKnownTitle },
    { "UnsetKnownTitle", &LuaPlayer::UnsetKnownTitle },
    { "SetBindPoint", &LuaPlayer::SetBindPoint },
    { "SetArenaPoints", &LuaPlayer::SetArenaPoints },
    { "SetHonorPoints", &LuaPlayer::SetHonorPoints },
    { "SetSpellPower", &LuaPlayer::SetSpellPower },
    { "SetLifetimeKills", &LuaPlayer::SetLifetimeKills },
    { "SetGameMaster", &LuaPlayer::SetGameMaster },
    { "SetGMChat", &LuaPlayer::SetGMChat },
    { "SetKnownTaxiNodes", &LuaPlayer::SetKnownTaxiNodes },
    { "SetTaxiCheat", &LuaPlayer::SetTaxiCheat },
    { "SetGMVisible", &LuaPlayer::SetGMVisible },
    { "SetPvPDeath", &LuaPlayer::SetPvPDeath },
    { "SetAcceptWhispers", &LuaPlayer::SetAcceptWhispers },
    { "SetRestBonus", &LuaPlayer::SetRestBonus },
    { "SetQuestStatus", &LuaPlayer::SetQuestStatus },
    { "SetReputation", &LuaPlayer::SetReputation },
    { "SetFreeTalentPoints", &LuaPlayer::SetFreeTalentPoints },
    { "SetGuildRank", &LuaPlayer::SetGuildRank },
    // {"SetMovement", &LuaPlayer::SetMovement},                  // :SetMovement(type) - UNDOCUMENTED - Sets player's movement type
    { "SetSkill", &LuaPlayer::SetSkill },
    { "SetFactionForRace", &LuaPlayer::SetFactionForRace },
    { "SetDrunkValue", &LuaPlayer::SetDrunkValue },
    { "SetAtLoginFlag", &LuaPlayer::SetAtLoginFlag },
    { "SetPlayerLock", &LuaPlayer::SetPlayerLock },
    { "SetGender", &LuaPlayer::SetGender },
    { "SetSheath", &LuaPlayer::SetSheath },
    { "SetBonusTalentCount", &LuaPlayer::SetBonusTalentCount },
    { "AddBonusTalent", &LuaPlayer::AddBonusTalent },
    { "RemoveBonusTalent", &LuaPlayer::RemoveBonusTalent },
    { "GetHomebind", &LuaPlayer::GetHomebind },
    { "GetSpells", &LuaPlayer::GetSpells },

    // Boolean
    { "HasTankSpec", &LuaPlayer::HasTankSpec },
    { "HasMeleeSpec", &LuaPlayer::HasMeleeSpec },
    { "HasCasterSpec", &LuaPlayer::HasCasterSpec },
    { "HasHealSpec", &LuaPlayer::HasHealSpec },
    { "IsInGroup", &LuaPlayer::IsInGroup },
    { "IsInGuild", &LuaPlayer::IsInGuild },
    { "IsGM", &LuaPlayer::IsGM },
    { "IsImmuneToDamage", &LuaPlayer::IsImmuneToDamage },
    { "IsAlliance", &LuaPlayer::IsAlliance },
    { "IsHorde", &LuaPlayer::IsHorde },
    { "HasTitle", &LuaPlayer::HasTitle },
    { "HasItem", &LuaPlayer::HasItem },
    { "Teleport", &LuaPlayer::Teleport },
    { "AddItem", &LuaPlayer::AddItem },
    { "IsInArenaTeam", &LuaPlayer::IsInArenaTeam },
    { "CanRewardQuest", &LuaPlayer::CanRewardQuest },
    { "CanCompleteRepeatableQuest", &LuaPlayer::CanCompleteRepeatableQuest },
    { "CanCompleteQuest", &LuaPlayer::CanCompleteQuest },
    { "CanEquipItem", &LuaPlayer::CanEquipItem },
    { "IsFalling", &LuaPlayer::IsFalling },
    { "ToggleAFK", &LuaPlayer::ToggleAFK },
    { "ToggleDND", &LuaPlayer::ToggleDND },
    { "IsAFK", &LuaPlayer::IsAFK },
    { "IsDND", &LuaPlayer::IsDND },
    { "IsAcceptingWhispers", &LuaPlayer::IsAcceptingWhispers },
    { "IsGMChat", &LuaPlayer::IsGMChat },
    { "IsTaxiCheater", &LuaPlayer::IsTaxiCheater },
    { "IsGMVisible", &LuaPlayer::IsGMVisible },
    { "HasQuest", &LuaPlayer::HasQuest },
    { "InBattlegroundQueue", &LuaPlayer::InBattlegroundQueue },
    // {"IsImmuneToEnvironmentalDamage", &LuaPlayer::IsImmuneToEnvironmentalDamage},        // :IsImmuneToEnvironmentalDamage() - UNDOCUMENTED - Returns true if the player is immune to environmental damage
    { "CanSpeak", &LuaPlayer::CanSpeak },
    { "HasAtLoginFlag", &LuaPlayer::HasAtLoginFlag },
    // {"InRandomLfgDungeon", &LuaPlayer::InRandomLfgDungeon},                              // :InRandomLfgDungeon() - UNDOCUMENTED - Returns true if the player is in a random LFG dungeon
    // {"HasPendingBind", &LuaPlayer::HasPendingBind},                                      // :HasPendingBind() - UNDOCUMENTED - Returns true if the player has a pending instance bind
    { "HasAchieved", &LuaPlayer::HasAchieved },
    { "GetAchievementCriteriaProgress", &LuaPlayer::GetAchievementCriteriaProgress },
    { "SetAchievement", &LuaPlayer::SetAchievement },
    { "CanUninviteFromGroup", &LuaPlayer::CanUninviteFromGroup },
    { "IsRested", &LuaPlayer::IsRested },
    // {"CanFlyInZone", &LuaPlayer::CanFlyInZone},                                          // :CanFlyInZone(mapid, zone) - UNDOCUMENTED - Returns true if the player can fly in the area
    // {"IsNeverVisible", &LuaPlayer::IsNeverVisible},                                      // :IsNeverVisible() - UNDOCUMENTED - Returns true if the player is never visible
    { "IsVisibleForPlayer", &LuaPlayer::IsVisibleForPlayer },
    // {"IsUsingLfg", &LuaPlayer::IsUsingLfg},                                              // :IsUsingLfg() - UNDOCUMENTED - Returns true if the player is using LFG
    { "HasQuestForItem", &LuaPlayer::HasQuestForItem },
    { "HasQuestForGO", &LuaPlayer::HasQuestForGO },
    { "CanShareQuest", &LuaPlayer::CanShareQuest },
    // {"HasReceivedQuestReward", &LuaPlayer::HasReceivedQuestReward},                      // :HasReceivedQuestReward(entry) - UNDOCUMENTED - Returns true if the player has recieved the quest's reward
    { "HasTalent", &LuaPlayer::HasTalent },
    { "IsInSameGroupWith", &LuaPlayer::IsInSameGroupWith },
    { "IsInSameRaidWith", &LuaPlayer::IsInSameRaidWith },
    { "IsGroupVisibleFor", &LuaPlayer::IsGroupVisibleFor },
    { "HasSkill", &LuaPlayer::HasSkill },
    { "IsHonorOrXPTarget", &LuaPlayer::IsHonorOrXPTarget },
    { "CanParry", &LuaPlayer::CanParry },
    { "CanBlock", &LuaPlayer::CanBlock },
    { "CanTitanGrip", &LuaPlayer::CanTitanGrip },
    { "InBattleground", &LuaPlayer::InBattleground },
    { "InArena", &LuaPlayer::InArena },
    // {"IsOutdoorPvPActive", &LuaPlayer::IsOutdoorPvPActive},                              // :IsOutdoorPvPActive() - UNDOCUMENTED - Returns true if the player is outdoor pvp active
    // {"IsARecruiter", &LuaPlayer::IsARecruiter},                                          // :IsARecruiter() - UNDOCUMENTED - Returns true if the player is a recruiter
    { "CanUseItem", &LuaPlayer::CanUseItem },
    { "HasSpell", &LuaPlayer::HasSpell },
    { "HasSpellCooldown", &LuaPlayer::HasSpellCooldown },
    { "IsInWater", &LuaPlayer::IsInWater },
    { "CanFly", &LuaPlayer::CanFly },
    { "IsMoving", &LuaPlayer::IsMoving },
    { "IsFlying", &LuaPlayer::IsFlying },

    // Gossip
    { "GossipMenuAddItem", &LuaPlayer::GossipMenuAddItem },
    { "GossipSendMenu", &LuaPlayer::GossipSendMenu },
    { "GossipComplete", &LuaPlayer::GossipComplete },
    { "GossipClearMenu", &LuaPlayer::GossipClearMenu },

    // Other
    { "SendBroadcastMessage", &LuaPlayer::SendBroadcastMessage },
    { "SendAreaTriggerMessage", &LuaPlayer::SendAreaTriggerMessage },
    { "SendNotification", &LuaPlayer::SendNotification },
    { "SendPacket", &LuaPlayer::SendPacket },
    { "SendAddonMessage", &LuaPlayer::SendAddonMessage },
    { "ModifyMoney", &LuaPlayer::ModifyMoney },
    { "LearnSpell", &LuaPlayer::LearnSpell },
    { "LearnTalent", &LuaPlayer::LearnTalent },

    { "RunCommand", &LuaPlayer::RunCommand },
    { "SetGlyph", &LuaPlayer::SetGlyph },
    { "GetGlyph", &LuaPlayer::GetGlyph },
    { "RemoveArenaSpellCooldowns", &LuaPlayer::RemoveArenaSpellCooldowns },
    { "RemoveItem", &LuaPlayer::RemoveItem },
    { "RemoveLifetimeKills", &LuaPlayer::RemoveLifetimeKills },
    { "ResurrectPlayer", &LuaPlayer::ResurrectPlayer },
    { "EquipItem", &LuaPlayer::EquipItem },
    { "ResetSpellCooldown", &LuaPlayer::ResetSpellCooldown },
    { "ResetTypeCooldowns", &LuaPlayer::ResetTypeCooldowns },
    { "ResetAllCooldowns", &LuaPlayer::ResetAllCooldowns },
    { "GiveXP", &LuaPlayer::GiveXP },                                                       // :GiveXP(xp[, victim, pureXP, triggerHook]) - UNDOCUMENTED - Gives XP to the player. If pure is false, bonuses are count in. If triggerHook is false, GiveXp hook is not triggered.
    // {"RemovePet", &LuaPlayer::RemovePet},                                                // :RemovePet([mode, returnreagent]) - UNDOCUMENTED - Removes the player's pet. Mode determines if the pet is saved and how
    // {"SummonPet", &LuaPlayer::SummonPet},                                              // :SummonPet(entry, x, y, z, o, petType, despwtime) - Summons a pet for the player
    { "Say", &LuaPlayer::Say },
    { "Yell", &LuaPlayer::Yell },
    { "TextEmote", &LuaPlayer::TextEmote },
    { "Whisper", &LuaPlayer::Whisper },
    { "CompleteQuest", &LuaPlayer::CompleteQuest },
    { "IncompleteQuest", &LuaPlayer::IncompleteQuest },
    { "FailQuest", &LuaPlayer::FailQuest },
    { "AddQuest", &LuaPlayer::AddQuest },
    { "RemoveQuest", &LuaPlayer::RemoveQuest },
    // {"RemoveActiveQuest", &LuaPlayer::RemoveActiveQuest},                                // :RemoveActiveQuest(entry) - UNDOCUMENTED - Removes an active quest
    // {"RemoveRewardedQuest", &LuaPlayer::RemoveRewardedQuest},                            // :RemoveRewardedQuest(entry) - UNDOCUMENTED - Removes a rewarded quest
    { "AreaExploredOrEventHappens", &LuaPlayer::AreaExploredOrEventHappens },
    { "GroupEventHappens", &LuaPlayer::GroupEventHappens },
    { "KilledMonsterCredit", &LuaPlayer::KilledMonsterCredit },
    // {"KilledPlayerCredit", &LuaPlayer::KilledPlayerCredit},                              // :KilledPlayerCredit() - UNDOCUMENTED - Satisfies a player kill for the player
    // {"KillGOCredit", &LuaPlayer::KillGOCredit},                                          // :KillGOCredit(GOEntry[, GUID]) - UNDOCUMENTED - Credits the player for destroying a GO, guid is optional
    { "TalkedToCreature", &LuaPlayer::TalkedToCreature },
    { "ResetPetTalents", &LuaPlayer::ResetPetTalents },
    { "AddComboPoints", &LuaPlayer::AddComboPoints },
    // {"GainSpellComboPoints", &LuaPlayer::GainSpellComboPoints},                          // :GainSpellComboPoints(amount) - UNDOCUMENTED - Player gains spell combo points
    { "ClearComboPoints", &LuaPlayer::ClearComboPoints },
    { "RemoveSpell", &LuaPlayer::RemoveSpell },
    { "ResetTalents", &LuaPlayer::ResetTalents },
    { "ResetTalentsCost", &LuaPlayer::ResetTalentsCost },
    // {"AddTalent", &LuaPlayer::AddTalent},                                                // :AddTalent(spellid, spec, learning) - UNDOCUMENTED - Adds a talent spell for the player to given spec
    { "RemoveFromGroup", &LuaPlayer::RemoveFromGroup },
    { "KillPlayer", &LuaPlayer::KillPlayer },
    { "DurabilityLossAll", &LuaPlayer::DurabilityLossAll },
    { "DurabilityLoss", &LuaPlayer::DurabilityLoss },
    { "DurabilityPointsLoss", &LuaPlayer::DurabilityPointsLoss },
    { "DurabilityPointsLossAll", &LuaPlayer::DurabilityPointsLossAll },
    { "DurabilityPointLossForEquipSlot", &LuaPlayer::DurabilityPointLossForEquipSlot },
    { "DurabilityRepairAll", &LuaPlayer::DurabilityRepairAll },
    { "DurabilityRepair", &LuaPlayer::DurabilityRepair },
    { "ModifyHonorPoints", &LuaPlayer::ModifyHonorPoints },
    { "ModifyArenaPoints", &LuaPlayer::ModifyArenaPoints },
    { "LeaveBattleground", &LuaPlayer::LeaveBattleground },
    // {"BindToInstance", &LuaPlayer::BindToInstance},                                      // :BindToInstance() - UNDOCUMENTED - Binds the player to the current instance
    { "UnbindInstance", &LuaPlayer::UnbindInstance },
    { "UnbindAllInstances", &LuaPlayer::UnbindAllInstances },
    { "RemoveFromBattlegroundRaid", &LuaPlayer::RemoveFromBattlegroundRaid },
    { "ResetAchievements", &LuaPlayer::ResetAchievements },
    { "KickPlayer", &LuaPlayer::KickPlayer },
    { "LogoutPlayer", &LuaPlayer::LogoutPlayer },
    { "SendTrainerList", &LuaPlayer::SendTrainerList },
    { "SendListInventory", &LuaPlayer::SendListInventory },
    { "SendShowBank", &LuaPlayer::SendShowBank },
    { "SendTabardVendorActivate", &LuaPlayer::SendTabardVendorActivate },
    { "SendSpiritResurrect", &LuaPlayer::SendSpiritResurrect },
    { "SendTaxiMenu", &LuaPlayer::SendTaxiMenu },
    { "SendUpdateWorldState", &LuaPlayer::SendUpdateWorldState },
    { "RewardQuest", &LuaPlayer::RewardQuest },
    { "SendAuctionMenu", &LuaPlayer::SendAuctionMenu },
    { "SendShowMailBox", &LuaPlayer::SendShowMailBox },
    { "StartTaxi", &LuaPlayer::StartTaxi },
    { "GossipSendPOI", &LuaPlayer::GossipSendPOI },
    { "GossipAddQuests", &LuaPlayer::GossipAddQuests },
    { "SendQuestTemplate", &LuaPlayer::SendQuestTemplate },
    { "SpawnBones", &LuaPlayer::SpawnBones },
    { "RemovedInsignia", &LuaPlayer::RemovedInsignia },
    { "SendGuildInvite", &LuaPlayer::SendGuildInvite },
    { "Mute", &LuaPlayer::Mute },
    { "SummonPlayer", &LuaPlayer::SummonPlayer },
    { "SaveToDB", &LuaPlayer::SaveToDB },
    { "GroupInvite", &LuaPlayer::GroupInvite },
    { "GroupCreate", &LuaPlayer::GroupCreate },
    { "SendCinematicStart", &LuaPlayer::SendCinematicStart },
    { "SendMovieStart", &LuaPlayer::SendMovieStart },
    { "UpdatePlayerSetting", &LuaPlayer::UpdatePlayerSetting },
    { "TeleportTo", &LuaPlayer::TeleportTo },

    { NULL, NULL }
};

ElunaRegister<Creature> CreatureMethods[] =
{
    // Getters
    { "GetAITarget", &LuaCreature::GetAITarget },
    { "GetAITargets", &LuaCreature::GetAITargets },
    { "GetAITargetsCount", &LuaCreature::GetAITargetsCount },
    { "GetHomePosition", &LuaCreature::GetHomePosition },
    { "GetCorpseDelay", &LuaCreature::GetCorpseDelay },
    { "GetCreatureSpellCooldownDelay", &LuaCreature::GetCreatureSpellCooldownDelay },
    { "GetScriptId", &LuaCreature::GetScriptId },
    { "GetAIName", &LuaCreature::GetAIName },
    { "GetScriptName", &LuaCreature::GetScriptName },
    { "GetAggroRange", &LuaCreature::GetAggroRange },
    { "GetDefaultMovementType", &LuaCreature::GetDefaultMovementType },
    { "GetRespawnDelay", &LuaCreature::GetRespawnDelay },
    { "GetWanderRadius", &LuaCreature::GetWanderRadius },
    { "GetCurrentWaypointId", &LuaCreature::GetCurrentWaypointId },
    { "GetCreatureSpawnId", &LuaCreature::GetCreatureSpawnId },
    { "GetWaypointPath", &LuaCreature::GetWaypointPath },
    { "GetLootMode", &LuaCreature::GetLootMode },
    { "GetLootRecipient", &LuaCreature::GetLootRecipient },
    { "GetLootRecipientGroup", &LuaCreature::GetLootRecipientGroup },
    { "GetNPCFlags", &LuaCreature::GetNPCFlags },
    { "GetUnitFlags", &LuaCreature::GetUnitFlags },
    { "GetUnitFlagsTwo", &LuaCreature::GetUnitFlagsTwo },
    { "GetExtraFlags", &LuaCreature::GetExtraFlags },
    { "GetRank", &LuaCreature::GetRank },
    { "GetShieldBlockValue", &LuaCreature::GetShieldBlockValue },
    { "GetDBTableGUIDLow", &LuaCreature::GetDBTableGUIDLow },
    { "GetCreatureFamily", &LuaCreature::GetCreatureFamily },
    { "GetReactState", &LuaCreature::GetReactState },
    { "GetLoot", &LuaCreature::GetLoot },
    { "AllLootRemoved", &LuaCreature::AllLootRemoved },

    // Setters
    { "SetRegeneratingHealth", &LuaCreature::SetRegeneratingHealth },
    { "SetHover", &LuaCreature::SetHover },
    { "SetDisableGravity", &LuaCreature::SetDisableGravity },
    { "SetAggroEnabled", &LuaCreature::SetAggroEnabled },
    { "SetCorpseDelay", &LuaCreature::SetCorpseDelay },
    { "SetNoCallAssistance", &LuaCreature::SetNoCallAssistance },
    { "SetNoSearchAssistance", &LuaCreature::SetNoSearchAssistance },
    { "SetDefaultMovementType", &LuaCreature::SetDefaultMovementType },
    { "SetRespawnDelay", &LuaCreature::SetRespawnDelay },
    { "SetWanderRadius", &LuaCreature::SetWanderRadius },
    { "SetInCombatWithZone", &LuaCreature::SetInCombatWithZone },
    { "SetDisableReputationGain", &LuaCreature::SetDisableReputationGain },
    { "SetLootMode", &LuaCreature::SetLootMode },
    { "SetNPCFlags", &LuaCreature::SetNPCFlags },
    { "SetUnitFlags", &LuaCreature::SetUnitFlags },
    { "SetUnitFlagsTwo", &LuaCreature::SetUnitFlagsTwo },
    { "SetReactState", &LuaCreature::SetReactState },
    { "SetDeathState", &LuaCreature::SetDeathState },
    { "SetWalk", &LuaCreature::SetWalk },
    { "SetHomePosition", &LuaCreature::SetHomePosition },
    { "SetEquipmentSlots", &LuaCreature::SetEquipmentSlots },

    // Boolean
    { "IsRegeneratingHealth", &LuaCreature::IsRegeneratingHealth },
    { "IsDungeonBoss", &LuaCreature::IsDungeonBoss },
    { "IsWorldBoss", &LuaCreature::IsWorldBoss },
    { "IsRacialLeader", &LuaCreature::IsRacialLeader },
    { "IsCivilian", &LuaCreature::IsCivilian },
    { "IsTrigger", &LuaCreature::IsTrigger },
    { "IsGuard", &LuaCreature::IsGuard },
    { "IsElite", &LuaCreature::IsElite },
    { "IsInEvadeMode", &LuaCreature::IsInEvadeMode },
    { "HasCategoryCooldown", &LuaCreature::HasCategoryCooldown },
    { "CanWalk", &LuaCreature::CanWalk },
    { "CanSwim", &LuaCreature::CanSwim },
    { "CanAggro", &LuaCreature::CanAggro },
    { "CanStartAttack", &LuaCreature::CanStartAttack },
    { "HasSearchedAssistance", &LuaCreature::HasSearchedAssistance },
    { "IsTappedBy", &LuaCreature::IsTappedBy },
    { "HasLootRecipient", &LuaCreature::HasLootRecipient },
    { "CanAssistTo", &LuaCreature::CanAssistTo },
    { "IsTargetableForAttack", &LuaCreature::IsTargetableForAttack },
    { "CanCompleteQuest", &LuaCreature::CanCompleteQuest },
    { "IsReputationGainDisabled", &LuaCreature::IsReputationGainDisabled },
    { "IsDamageEnoughForLootingAndReward", &LuaCreature::IsDamageEnoughForLootingAndReward },
    { "HasLootMode", &LuaCreature::HasLootMode },
    { "HasSpell", &LuaCreature::HasSpell },
    { "HasQuest", &LuaCreature::HasQuest },
    { "HasSpellCooldown", &LuaCreature::HasSpellCooldown },
    { "CanFly", &LuaCreature::CanFly },

    // Other
    { "FleeToGetAssistance", &LuaCreature::FleeToGetAssistance },
    { "CallForHelp", &LuaCreature::CallForHelp },
    { "CallAssistance", &LuaCreature::CallAssistance },
    { "RemoveCorpse", &LuaCreature::RemoveCorpse },
    { "DespawnOrUnsummon", &LuaCreature::DespawnOrUnsummon },
    { "Respawn", &LuaCreature::Respawn },
    { "AttackStart", &LuaCreature::AttackStart },
    { "AddLootMode", &LuaCreature::AddLootMode },
    { "ResetLootMode", &LuaCreature::ResetLootMode },
    { "RemoveLootMode", &LuaCreature::RemoveLootMode },
    { "SaveToDB", &LuaCreature::SaveToDB },
    { "SelectVictim", &LuaCreature::SelectVictim },
    { "MoveWaypoint", &LuaCreature::MoveWaypoint },
    { "UpdateEntry", &LuaCreature::UpdateEntry },

    { NULL, NULL }
};

ElunaRegister<GameObject> GameObjectMethods[] =
{
    // Getters
    { "GetDisplayId", &LuaGameObject::GetDisplayId },
    { "GetGoState", &LuaGameObject::GetGoState },
    { "GetLootState", &LuaGameObject::GetLootState },
    { "GetLootRecipient", &LuaGameObject::GetLootRecipient },
    { "GetLootRecipientGroup", &LuaGameObject::GetLootRecipientGroup },
    { "GetDBTableGUIDLow", &LuaGameObject::GetDBTableGUIDLow },

    // Setters
    { "SetGoState", &LuaGameObject::SetGoState },
    { "SetLootState", &LuaGameObject::SetLootState },
    { "SetRespawnTime", &LuaGameObject::SetRespawnTime },
    { "SetRespawnDelay", &LuaGameObject::SetRespawnDelay },

    // Boolean
    { "IsTransport", &LuaGameObject::IsTransport },
    // {"IsDestructible", &LuaGameObject::IsDestructible},    // :IsDestructible() - UNDOCUMENTED
    { "IsActive", &LuaGameObject::IsActive },
    { "HasQuest", &LuaGameObject::HasQuest },
    { "IsSpawned", &LuaGameObject::IsSpawned },

    // Other
    { "RemoveFromWorld", &LuaGameObject::RemoveFromWorld },
    { "UseDoorOrButton", &LuaGameObject::UseDoorOrButton },
    { "Despawn", &LuaGameObject::Despawn },
    { "Respawn", &LuaGameObject::Respawn },
    { "SaveToDB", &LuaGameObject::SaveToDB },
    { "AddLoot", &LuaGameObject::AddLoot },

    { NULL, NULL }
};

ElunaRegister<Item> ItemMethods[] =
{
    // Getters
    { "GetOwnerGUID", &LuaItem::GetOwnerGUID },
    { "GetOwner", &LuaItem::GetOwner },
    { "GetCount", &LuaItem::GetCount },
    { "GetMaxStackCount", &LuaItem::GetMaxStackCount },
    { "GetSlot", &LuaItem::GetSlot },
    { "GetBagSlot", &LuaItem::GetBagSlot },
    { "GetEnchantmentId", &LuaItem::GetEnchantmentId },
    { "GetSpellId", &LuaItem::GetSpellId },
    { "GetSpellTrigger", &LuaItem::GetSpellTrigger },
    { "GetItemLink", &LuaItem::GetItemLink },
    { "GetClass", &LuaItem::GetClass },
    { "GetSubClass", &LuaItem::GetSubClass },
    { "GetName", &LuaItem::GetName },
    { "GetDisplayId", &LuaItem::GetDisplayId },
    { "GetQuality", &LuaItem::GetQuality },
    { "GetBuyCount", &LuaItem::GetBuyCount },
    { "GetBuyPrice", &LuaItem::GetBuyPrice },
    { "GetSellPrice", &LuaItem::GetSellPrice },
    { "GetInventoryType", &LuaItem::GetInventoryType },
    { "GetAllowableClass", &LuaItem::GetAllowableClass },
    { "GetAllowableRace", &LuaItem::GetAllowableRace },
    { "GetItemLevel", &LuaItem::GetItemLevel },
    { "GetRequiredLevel", &LuaItem::GetRequiredLevel },
    { "GetStatsCount", &LuaItem::GetStatsCount },
    { "GetRandomProperty", &LuaItem::GetRandomProperty },
    { "GetRandomSuffix", &LuaItem::GetRandomSuffix },
    { "GetItemSet", &LuaItem::GetItemSet },
    { "GetBagSize", &LuaItem::GetBagSize },
    { "GetItemTemplate", &LuaItem::GetItemTemplate },

    // Setters
    { "SetOwner", &LuaItem::SetOwner },
    { "SetBinding", &LuaItem::SetBinding },
    { "SetCount", &LuaItem::SetCount },
    { "SetRandomProperty", &LuaItem::SetRandomProperty },
    { "SetRandomSuffix", &LuaItem::SetRandomSuffix },

    // Boolean
    { "IsSoulBound", &LuaItem::IsSoulBound },
    { "IsBoundAccountWide", &LuaItem::IsBoundAccountWide },
    { "IsBoundByEnchant", &LuaItem::IsBoundByEnchant },
    { "IsNotBoundToPlayer", &LuaItem::IsNotBoundToPlayer },
    { "IsLocked", &LuaItem::IsLocked },
    { "IsBag", &LuaItem::IsBag },
    { "IsCurrencyToken", &LuaItem::IsCurrencyToken },
    { "IsNotEmptyBag", &LuaItem::IsNotEmptyBag },
    { "IsBroken", &LuaItem::IsBroken },
    { "CanBeTraded", &LuaItem::CanBeTraded },
    { "IsInTrade", &LuaItem::IsInTrade },
    { "IsInBag", &LuaItem::IsInBag },
    { "IsEquipped", &LuaItem::IsEquipped },
    { "HasQuest", &LuaItem::HasQuest },
    { "IsPotion", &LuaItem::IsPotion },
    { "IsWeaponVellum", &LuaItem::IsWeaponVellum },
    { "IsArmorVellum", &LuaItem::IsArmorVellum },
    { "IsConjuredConsumable", &LuaItem::IsConjuredConsumable },
    //{"IsRefundExpired", &LuaItem::IsRefundExpired},               // :IsRefundExpired() - UNDOCUMENTED - Returns true if the item's refund time has expired
    { "SetEnchantment", &LuaItem::SetEnchantment },
    { "ClearEnchantment", &LuaItem::ClearEnchantment },

    // Other
    { "SaveToDB", &LuaItem::SaveToDB },

    { NULL, NULL }
};

ElunaRegister<ItemTemplate> ItemTemplateMethods[] =
{
    { "GetItemId", &LuaItemTemplate::GetItemId },
    { "GetClass", &LuaItemTemplate::GetClass },
    { "GetSubClass", &LuaItemTemplate::GetSubClass },
    { "GetName", &LuaItemTemplate::GetName },
    { "GetDisplayId", &LuaItemTemplate::GetDisplayId },
    { "GetQuality", &LuaItemTemplate::GetQuality },
    { "GetFlags", &LuaItemTemplate::GetFlags },
    { "GetExtraFlags", &LuaItemTemplate::GetExtraFlags },
    { "GetBuyCount", &LuaItemTemplate::GetBuyCount },
    { "GetBuyPrice", &LuaItemTemplate::GetBuyPrice },
    { "GetSellPrice", &LuaItemTemplate::GetSellPrice },
    { "GetInventoryType", &LuaItemTemplate::GetInventoryType },
    { "GetAllowableClass", &LuaItemTemplate::GetAllowableClass },
    { "GetAllowableRace", &LuaItemTemplate::GetAllowableRace },
    { "GetItemLevel", &LuaItemTemplate::GetItemLevel },
    { "GetRequiredLevel", &LuaItemTemplate::GetRequiredLevel },
    { "GetIcon", &LuaItemTemplate::GetIcon },
    { NULL, NULL }
};

ElunaRegister<Aura> AuraMethods[] =
{
    // Getters
    { "GetCaster", &LuaAura::GetCaster },
    { "GetCasterGUID", &LuaAura::GetCasterGUID },
    { "GetCasterLevel", &LuaAura::GetCasterLevel },
    { "GetDuration", &LuaAura::GetDuration },
    { "GetMaxDuration", &LuaAura::GetMaxDuration },
    { "GetAuraId", &LuaAura::GetAuraId },
    { "GetStackAmount", &LuaAura::GetStackAmount },
    { "GetOwner", &LuaAura::GetOwner },

    // Setters
    { "SetDuration", &LuaAura::SetDuration },
    { "SetMaxDuration", &LuaAura::SetMaxDuration },
    { "SetStackAmount", &LuaAura::SetStackAmount },

    // Other
    { "Remove", &LuaAura::Remove },

    { NULL, NULL }
};

ElunaRegister<Spell> SpellMethods[] =
{
    // Getters
    { "GetCaster", &LuaSpell::GetCaster },
    { "GetCastTime", &LuaSpell::GetCastTime },
    { "GetEntry", &LuaSpell::GetEntry },
    { "GetDuration", &LuaSpell::GetDuration },
    { "GetPowerCost", &LuaSpell::GetPowerCost },
    { "GetReagentCost", &LuaSpell::GetReagentCost },
    { "GetTargetDest", &LuaSpell::GetTargetDest },
    { "GetTarget", &LuaSpell::GetTarget },

    // Setters
    { "SetAutoRepeat", &LuaSpell::SetAutoRepeat },

    // Boolean
    { "IsAutoRepeat", &LuaSpell::IsAutoRepeat },

    // Other
    { "Cancel", &LuaSpell::Cancel },
    { "Cast", &LuaSpell::Cast },
    { "Finish", &LuaSpell::Finish },

    { NULL, NULL }
};

ElunaRegister<Quest> QuestMethods[] =
{
    // Getters
    { "GetId", &LuaQuest::GetId },
    { "GetLevel", &LuaQuest::GetLevel },
    // {"GetMaxLevel", &LuaQuest::GetMaxLevel},                   // :GetMaxLevel() - UNDOCUMENTED - Returns the quest's max level
    { "GetMinLevel", &LuaQuest::GetMinLevel },
    { "GetNextQuestId", &LuaQuest::GetNextQuestId },
    { "GetPrevQuestId", &LuaQuest::GetPrevQuestId },
    { "GetNextQuestInChain", &LuaQuest::GetNextQuestInChain },
    { "GetFlags", &LuaQuest::GetFlags },
    { "GetType", &LuaQuest::GetType },

    // Boolean
    { "HasFlag", &LuaQuest::HasFlag },
    { "IsDaily", &LuaQuest::IsDaily },
    { "IsRepeatable", &LuaQuest::IsRepeatable },

    { NULL, NULL }
};

ElunaRegister<Group> GroupMethods[] =
{
    // Getters
    { "GetMembers", &LuaGroup::GetMembers },
    { "GetLeaderGUID", &LuaGroup::GetLeaderGUID },
    { "GetGUID", &LuaGroup::GetGUID },
    { "GetMemberGroup", &LuaGroup::GetMemberGroup },
    { "GetMemberGUID", &LuaGroup::GetMemberGUID },
    { "GetMembersCount", &LuaGroup::GetMembersCount },
    { "GetGroupType", &LuaGroup::GetGroupType },

    // Setters
    { "SetLeader", &LuaGroup::SetLeader },
    { "SetMembersGroup", &LuaGroup::SetMembersGroup },
    { "SetTargetIcon", &LuaGroup::SetTargetIcon },
    { "SetMemberFlag", &LuaGroup::SetMemberFlag },

    // Boolean
    { "IsLeader", &LuaGroup::IsLeader },
    { "AddMember", &LuaGroup::AddMember },
    { "RemoveMember", &LuaGroup::RemoveMember },
    { "Disband", &LuaGroup::Disband },
    { "IsFull", &LuaGroup::IsFull },
    { "IsLFGGroup", &LuaGroup::IsLFGGroup },
    { "IsRaidGroup", &LuaGroup::IsRaidGroup },
    { "IsBGGroup", &LuaGroup::IsBGGroup },
    // {"IsBFGroup", &LuaGroup::IsBFGroup},                       // :IsBFGroup() - UNDOCUMENTED - Returns true if the group is a battlefield group
    { "IsMember", &LuaGroup::IsMember },
    { "IsAssistant", &LuaGroup::IsAssistant },
    { "SameSubGroup", &LuaGroup::SameSubGroup },
    { "HasFreeSlotSubGroup", &LuaGroup::HasFreeSlotSubGroup },

    // Other
    { "SendPacket", &LuaGroup::SendPacket },
    // {"ConvertToLFG", &LuaGroup::ConvertToLFG},                 // :ConvertToLFG() - UNDOCUMENTED - Converts the group to an LFG group
    { "ConvertToRaid", &LuaGroup::ConvertToRaid },

    { NULL, NULL }
};

ElunaRegister<Guild> GuildMethods[] =
{
    // Getters
    { "GetMembers", &LuaGuild::GetMembers },
    { "GetLeader", &LuaGuild::GetLeader },
    { "GetLeaderGUID", &LuaGuild::GetLeaderGUID },
    { "GetId", &LuaGuild::GetId },
    { "GetName", &LuaGuild::GetName },
    { "GetMOTD", &LuaGuild::GetMOTD },
    { "GetInfo", &LuaGuild::GetInfo },
    { "GetMemberCount", &LuaGuild::GetMemberCount },
    { "GetCreatedDate", &LuaGuild::GetCreatedDate },
    { "GetTotalBankMoney", &LuaGuild::GetTotalBankMoney },

    // Setters
    { "SetBankTabText", &LuaGuild::SetBankTabText },
    { "SetMemberRank", &LuaGuild::SetMemberRank },
    { "SetLeader", &LuaGuild::SetLeader },
    { "SetName", &LuaGuild::SetName },

    // Other
    { "SendPacket", &LuaGuild::SendPacket },
    { "SendPacketToRanked", &LuaGuild::SendPacketToRanked },
    { "Disband", &LuaGuild::Disband },
    { "AddMember", &LuaGuild::AddMember },
    { "DeleteMember", &LuaGuild::DeleteMember },
    { "SendMessage", &LuaGuild::SendMessage },
    { "UpdateMemberData", &LuaGuild::UpdateMemberData },
    { "MassInviteToEvent", &LuaGuild::MassInviteToEvent },
    { "SwapItems", &LuaGuild::SwapItems },
    { "SwapItemsWithInventory", &LuaGuild::SwapItemsWithInventory },
    { "ResetTimes", &LuaGuild::ResetTimes },
    { "ModifyBankMoney", &LuaGuild::ModifyBankMoney },

    { NULL, NULL }
};

ElunaRegister<Vehicle> VehicleMethods[] =
{
    // Getters
    { "GetOwner", &LuaVehicle::GetOwner },
    { "GetEntry", &LuaVehicle::GetEntry },
    { "GetPassenger", &LuaVehicle::GetPassenger },

    // Boolean
    { "IsOnBoard", &LuaVehicle::IsOnBoard },

    // Other
    { "AddPassenger", &LuaVehicle::AddPassenger },
    { "RemovePassenger", &LuaVehicle::RemovePassenger },

    { NULL, NULL }
};

ElunaRegister<ElunaQuery> QueryMethods[] =
{
    // Getters
    { "GetColumnCount", &LuaQuery::GetColumnCount },
    { "GetRowCount", &LuaQuery::GetRowCount },
    { "GetRow", &LuaQuery::GetRow },
    { "GetBool", &LuaQuery::GetBool },
    { "GetUInt8", &LuaQuery::GetUInt8 },
    { "GetUInt16", &LuaQuery::GetUInt16 },
    { "GetUInt32", &LuaQuery::GetUInt32 },
    { "GetUInt64", &LuaQuery::GetUInt64 },
    { "GetInt8", &LuaQuery::GetInt8 },
    { "GetInt16", &LuaQuery::GetInt16 },
    { "GetInt32", &LuaQuery::GetInt32 },
    { "GetInt64", &LuaQuery::GetInt64 },
    { "GetFloat", &LuaQuery::GetFloat },
    { "GetDouble", &LuaQuery::GetDouble },
    { "GetString", &LuaQuery::GetString },

    // Boolean
    { "NextRow", &LuaQuery::NextRow },
    { "IsNull", &LuaQuery::IsNull },

    { NULL, NULL }
};

ElunaRegister<WorldPacket> PacketMethods[] =
{
    // Getters
    { "GetOpcode", &LuaPacket::GetOpcode },
    { "GetSize", &LuaPacket::GetSize },

    // Setters
    { "SetOpcode", &LuaPacket::SetOpcode },

    // Readers
    { "ReadByte", &LuaPacket::ReadByte },
    { "ReadUByte", &LuaPacket::ReadUByte },
    { "ReadShort", &LuaPacket::ReadShort },
    { "ReadUShort", &LuaPacket::ReadUShort },
    { "ReadLong", &LuaPacket::ReadLong },
    { "ReadULong", &LuaPacket::ReadULong },
    { "ReadGUID", &LuaPacket::ReadGUID },
    { "ReadPackedGUID", &LuaPacket::ReadPackedGUID },
    { "ReadString", &LuaPacket::ReadString },
    { "ReadFloat", &LuaPacket::ReadFloat },
    { "ReadDouble", &LuaPacket::ReadDouble },

    // Writers
    { "WriteByte", &LuaPacket::WriteByte },
    { "WriteUByte", &LuaPacket::WriteUByte },
    { "WriteShort", &LuaPacket::WriteShort },
    { "WriteUShort", &LuaPacket::WriteUShort },
    { "WriteLong", &LuaPacket::WriteLong },
    { "WriteULong", &LuaPacket::WriteULong },
    { "WriteGUID", &LuaPacket::WriteGUID },
    { "WriteString", &LuaPacket::WriteString },
    { "WriteFloat", &LuaPacket::WriteFloat },
    { "WriteDouble", &LuaPacket::WriteDouble },

    { NULL, NULL }
};

ElunaRegister<Map> MapMethods[] =
{
    // Getters
    { "GetName", &LuaMap::GetName },
    { "GetDifficulty", &LuaMap::GetDifficulty },
    { "GetInstanceId", &LuaMap::GetInstanceId },
    { "GetInstanceData", &LuaMap::GetInstanceData },
    { "GetPlayerCount", &LuaMap::GetPlayerCount },
    { "GetPlayers", &LuaMap::GetPlayers },
    { "GetMapId", &LuaMap::GetMapId },
    { "GetAreaId", &LuaMap::GetAreaId },
    { "GetHeight", &LuaMap::GetHeight },
    { "GetWorldObject", &LuaMap::GetWorldObject },
    { "GetCreatures", &LuaMap::GetCreatures },
    { "GetCreaturesByAreaId", &LuaMap::GetCreaturesByAreaId },


    // Setters
    { "SetWeather", &LuaMap::SetWeather },

    // Boolean
    { "IsArena", &LuaMap::IsArena },
    { "IsBattleground", &LuaMap::IsBattleground },
    { "IsDungeon", &LuaMap::IsDungeon },
    { "IsEmpty", &LuaMap::IsEmpty },
    { "IsHeroic", &LuaMap::IsHeroic },
    { "IsRaid", &LuaMap::IsRaid },

    // Other
    { "SaveInstanceData", &LuaMap::SaveInstanceData },

    { NULL, NULL }
};

ElunaRegister<Corpse> CorpseMethods[] =
{
    // Getters
    { "GetOwnerGUID", &LuaCorpse::GetOwnerGUID },
    { "GetGhostTime", &LuaCorpse::GetGhostTime },
    { "GetType", &LuaCorpse::GetType },

    // Other
    { "ResetGhostTime", &LuaCorpse::ResetGhostTime },
    { "SaveToDB", &LuaCorpse::SaveToDB },

    { NULL, NULL }
};

ElunaRegister<AuctionHouseEntry> AuctionMethods[] =
{
    { NULL, NULL }
};

ElunaRegister<BattleGround> BattleGroundMethods[] =
{
    // Getters
    { "GetName", &LuaBattleGround::GetName },
    { "GetAlivePlayersCountByTeam", &LuaBattleGround::GetAlivePlayersCountByTeam },
    { "GetMap", &LuaBattleGround::GetMap },
    { "GetBonusHonorFromKillCount", &LuaBattleGround::GetBonusHonorFromKillCount },
    { "GetEndTime", &LuaBattleGround::GetEndTime },
    { "GetFreeSlotsForTeam", &LuaBattleGround::GetFreeSlotsForTeam },
    { "GetInstanceId", &LuaBattleGround::GetInstanceId },
    { "GetMapId", &LuaBattleGround::GetMapId },
    { "GetTypeId", &LuaBattleGround::GetTypeId },
    { "GetMaxLevel", &LuaBattleGround::GetMaxLevel },
    { "GetMinLevel", &LuaBattleGround::GetMinLevel },
    { "GetMaxPlayers", &LuaBattleGround::GetMaxPlayers },
    { "GetMinPlayers", &LuaBattleGround::GetMinPlayers },
    { "GetMaxPlayersPerTeam", &LuaBattleGround::GetMaxPlayersPerTeam },
    { "GetMinPlayersPerTeam", &LuaBattleGround::GetMinPlayersPerTeam },
    { "GetWinner", &LuaBattleGround::GetWinner },
    { "GetStatus", &LuaBattleGround::GetStatus },

    { NULL, NULL }
};

ElunaRegister<ChatHandler> ChatHandlerMethods[] =
{
    { "SendSysMessage", &LuaChatHandler::SendSysMessage },
    { "IsConsole", &LuaChatHandler::IsConsole },
    { "GetPlayer", &LuaChatHandler::GetPlayer },
    { "SendGlobalSysMessage", &LuaChatHandler::SendGlobalSysMessage },
    { "SendGlobalGMSysMessage", &LuaChatHandler::SendGlobalGMSysMessage },
    { "HasLowerSecurity", &LuaChatHandler::HasLowerSecurity },
    { "HasLowerSecurityAccount", &LuaChatHandler::HasLowerSecurityAccount },
    { "GetSelectedPlayer", &LuaChatHandler::GetSelectedPlayer },
    { "GetSelectedCreature", &LuaChatHandler::GetSelectedCreature },
    { "GetSelectedUnit", &LuaChatHandler::GetSelectedUnit },
    { "GetSelectedObject", &LuaChatHandler::GetSelectedObject },
    { "GetSelectedPlayerOrSelf", &LuaChatHandler::GetSelectedPlayerOrSelf },
    { "IsAvailable", &LuaChatHandler::IsAvailable },
    { "HasSentErrorMessage", &LuaChatHandler::HasSentErrorMessage },

    { NULL, NULL }
};

ElunaRegister<AchievementEntry> AchievementMethods[] =
{
    { "GetId", &LuaAchievement::GetId },
    { "GetName", &LuaAchievement::GetName },

    { NULL, NULL }
};

ElunaRegister<Roll> RollMethods[] =
{
    { "GetItemGUID", &LuaRoll::GetItemGUID },
    { "GetItemId", &LuaRoll::GetItemId },
    { "GetItemRandomPropId", &LuaRoll::GetItemRandomPropId },
    { "GetItemRandomSuffix", &LuaRoll::GetItemRandomSuffix },
    { "GetItemCount", &LuaRoll::GetItemCount },
    { "GetPlayerVote", &LuaRoll::GetPlayerVote },
    { "GetPlayerVoteGUIDs", &LuaRoll::GetPlayerVoteGUIDs },
    { "GetTotalPlayersRolling", &LuaRoll::GetTotalPlayersRolling },
    { "GetTotalNeed", &LuaRoll::GetTotalNeed },
    { "GetTotalGreed", &LuaRoll::GetTotalGreed },
    { "GetTotalPass", &LuaRoll::GetTotalPass },
    { "GetItemSlot", &LuaRoll::GetItemSlot },
    { "GetRollVoteMask", &LuaRoll::GetRollVoteMask },

    { NULL, NULL }
};

ElunaRegister<Loot> LootMethods[] =
{
    // Get
    { "GetMoney", &LuaLoot::GetMoney },
    { "GetItems", &LuaLoot::GetItems },
    { "GetUnlootedCount", &LuaLoot::GetUnlootedCount },

    // Set
    { "AddItem", &LuaLoot::AddItem },
    { "RemoveItem", &LuaLoot::RemoveItem },
    { "SetMoney", &LuaLoot::SetMoney },
    { "SetUnlootedCount", &LuaLoot::SetUnlootedCount },
    { "UpdateItemIndex", &LuaLoot::UpdateItemIndex },
    { "SetItemLooted", &LuaLoot::SetItemLooted },

    // Boolean
    { "IsLooted", &LuaLoot::IsLooted },
    { "HasItem", &LuaLoot::HasItem },
    { "Clear", &LuaLoot::Clear },

    { NULL, NULL }
};

ElunaRegister<GmTicket> TicketMethods[] =
{
    { "IsClosed", &LuaTicket::IsClosed },
    { "IsCompleted", &LuaTicket::IsCompleted },
    { "IsFromPlayer", &LuaTicket::IsFromPlayer },
    { "IsAssigned", &LuaTicket::IsAssigned },
    { "IsAssignedTo", &LuaTicket::IsAssignedTo },
    { "IsAssignedNotTo", &LuaTicket::IsAssignedNotTo },

    { "GetId", &LuaTicket::GetId },
    { "GetPlayer", &LuaTicket::GetPlayer },
    { "GetPlayerName", &LuaTicket::GetPlayerName },
    { "GetMessage", &LuaTicket::GetMessage },
    { "GetAssignedPlayer", &LuaTicket::GetAssignedPlayer },
    { "GetAssignedToGUID", &LuaTicket::GetAssignedToGUID },
    { "GetLastModifiedTime", &LuaTicket::GetLastModifiedTime },
    { "GetResponse", &LuaTicket::GetResponse },
    { "GetChatLog", &LuaTicket::GetChatLog },

    { "SetAssignedTo", &LuaTicket::SetAssignedTo },
    { "SetResolvedBy", &LuaTicket::SetResolvedBy },
    { "SetCompleted", &LuaTicket::SetCompleted },
    { "SetMessage", &LuaTicket::SetMessage },
    { "SetComment", &LuaTicket::SetComment },
    { "SetViewed", &LuaTicket::SetViewed },
    { "SetUnassigned", &LuaTicket::SetUnassigned },
    { "SetPosition", &LuaTicket::SetPosition },
    { "AppendResponse", &LuaTicket::AppendResponse },
    { "DeleteResponse", &LuaTicket::DeleteResponse },
  
    { NULL, NULL }
};

ElunaRegister<SpellInfo> SpellInfoMethods[] =
{
    // Getters
    { "GetAttributes", &LuaSpellInfo::GetAttributes },
    { "GetCategory", &LuaSpellInfo::GetCategory },
    { "GetName", &LuaSpellInfo::GetName },
    { "CheckShapeshift", &LuaSpellInfo::CheckShapeshift },
    { "CheckLocation", &LuaSpellInfo::CheckLocation },
    { "CheckTarget", &LuaSpellInfo::CheckTarget },
    { "CheckExplicitTarget", &LuaSpellInfo::CheckExplicitTarget },
    { "CheckTargetCreatureType", &LuaSpellInfo::CheckTargetCreatureType },
    { "CheckTargetCreatureType", &LuaSpellInfo::CheckTargetCreatureType },
    { "GetSchoolMask", &LuaSpellInfo::GetSchoolMask },
    { "GetAllEffectsMechanicMask", &LuaSpellInfo::GetAllEffectsMechanicMask },
    { "GetEffectMechanicMask", &LuaSpellInfo::GetEffectMechanicMask },
    { "GetSpellMechanicMaskByEffectMask", &LuaSpellInfo::GetSpellMechanicMaskByEffectMask },
    { "GetEffectMechanic", &LuaSpellInfo::GetEffectMechanic },
    { "GetDispelMask", &LuaSpellInfo::GetDispelMask },
    { "GetExplicitTargetMask", &LuaSpellInfo::GetExplicitTargetMask },
    { "GetAuraState", &LuaSpellInfo::GetAuraState },
    { "GetSpellSpecific", &LuaSpellInfo::GetSpellSpecific },

    // Setters

    // Boolean
    { "HasAreaAuraEffect", &LuaSpellInfo::HasAreaAuraEffect },
    { "HasAttribute", &LuaSpellInfo::HasAttribute },
    { "HasAura", &LuaSpellInfo::HasAura },
    { "HasEffect", &LuaSpellInfo::HasEffect },

    { "IsAbilityLearnedWithProfession", &LuaSpellInfo::IsAbilityLearnedWithProfession },
    { "IsAbilityOfSkillType", &LuaSpellInfo::IsAbilityOfSkillType },
    { "IsAffectingArea", &LuaSpellInfo::IsAffectingArea },
    { "IsAllowingDeadTarget", &LuaSpellInfo::IsAllowingDeadTarget },
    { "IsAutocastable", &LuaSpellInfo::IsAutocastable },
    { "IsAutoRepeatRangedSpell", &LuaSpellInfo::IsAutoRepeatRangedSpell },
    { "IsBreakingStealth", &LuaSpellInfo::IsBreakingStealth },
    { "IsChanneled", &LuaSpellInfo::IsChanneled },
    { "IsCooldownStartedOnEvent", &LuaSpellInfo::IsCooldownStartedOnEvent },
    { "IsDeathPersistent", &LuaSpellInfo::IsDeathPersistent },
    { "IsExplicitDiscovery", &LuaSpellInfo::IsExplicitDiscovery },
    { "IsLootCrafting", &LuaSpellInfo::IsLootCrafting },
    { "IsMultiSlotAura", &LuaSpellInfo::IsMultiSlotAura },
    { "IsPassive", &LuaSpellInfo::IsPassive },
    { "IsPassiveStackableWithRanks", &LuaSpellInfo::IsPassiveStackableWithRanks },
    { "IsPositive", &LuaSpellInfo::IsPositive },
    { "IsPositiveEffect", &LuaSpellInfo::IsPositiveEffect },
    { "IsPrimaryProfession", &LuaSpellInfo::IsPrimaryProfession },
    { "IsPrimaryProfessionFirstRank", &LuaSpellInfo::IsPrimaryProfessionFirstRank },
    { "IsProfession", &LuaSpellInfo::IsProfession },
    { "IsProfessionOrRiding", &LuaSpellInfo::IsProfessionOrRiding },
    { "IsRangedWeaponSpell", &LuaSpellInfo::IsRangedWeaponSpell },
    { "IsRequiringDeadTarget", &LuaSpellInfo::IsRequiringDeadTarget },
    { "IsStackableWithRanks", &LuaSpellInfo::IsStackableWithRanks },
    { "IsTargetingArea", &LuaSpellInfo::IsTargetingArea },
    { "IsAffectedBySpellMods", &LuaSpellInfo::IsAffectedBySpellMods },
    /* { "IsAffectedBySpellMod", &LuaSpellInfo::IsAffectedBySpellMod }, */
    { "CanPierceImmuneAura", &LuaSpellInfo::CanPierceImmuneAura },
    { "CanDispelAura", &LuaSpellInfo::CanDispelAura },
    { "IsSingleTarget", &LuaSpellInfo::IsSingleTarget },
    { "IsAuraExclusiveBySpecificWith", &LuaSpellInfo::IsAuraExclusiveBySpecificWith },
    { "IsAuraExclusiveBySpecificPerCasterWith", &LuaSpellInfo::IsAuraExclusiveBySpecificPerCasterWith },
    { "CanBeUsedInCombat", &LuaSpellInfo::CanBeUsedInCombat },

    { "NeedsComboPoints", &LuaSpellInfo::NeedsComboPoints },
    { "NeedsExplicitUnitTarget", &LuaSpellInfo::NeedsExplicitUnitTarget },
    { "NeedsToBeTriggeredByCaster", &LuaSpellInfo::NeedsToBeTriggeredByCaster },

    { NULL, NULL }
};

ElunaRegister<GemPropertiesEntry> GemPropertiesEntryMethods[] =
{
    // Getters
    { "GetId", &LuaGemPropertiesEntry::GetId },
    { "GetSpellItemEnchantement", &LuaGemPropertiesEntry::GetSpellItemEnchantement },

    { NULL, NULL }
};

ElunaRegister<SpellEntry> SpellEntryMethods[] =
{
    // Getters
    { "GetId", &LuaSpellEntry::GetId },
    { "GetCategory", &LuaSpellEntry::GetCategory },
    { "GetDispel", &LuaSpellEntry::GetDispel },
    { "GetMechanic", &LuaSpellEntry::GetMechanic },
    { "GetAttributes", &LuaSpellEntry::GetAttributes },
    { "GetAttributesEx", &LuaSpellEntry::GetAttributesEx },
    { "GetAttributesEx2", &LuaSpellEntry::GetAttributesEx2 },
    { "GetAttributesEx3", &LuaSpellEntry::GetAttributesEx3 },
    { "GetAttributesEx4", &LuaSpellEntry::GetAttributesEx4 },
    { "GetAttributesEx5", &LuaSpellEntry::GetAttributesEx5 },
    { "GetAttributesEx6", &LuaSpellEntry::GetAttributesEx6 },
    { "GetAttributesEx7", &LuaSpellEntry::GetAttributesEx7 },
    { "GetStances", &LuaSpellEntry::GetStances },
    { "GetStancesNot", &LuaSpellEntry::GetStancesNot },
    { "GetTargets", &LuaSpellEntry::GetTargets },
    { "GetTargetCreatureType", &LuaSpellEntry::GetTargetCreatureType },
    { "GetRequiresSpellFocus", &LuaSpellEntry::GetRequiresSpellFocus },
    { "GetFacingCasterFlags", &LuaSpellEntry::GetFacingCasterFlags },
    { "GetCasterAuraState", &LuaSpellEntry::GetCasterAuraState },
    { "GetTargetAuraState", &LuaSpellEntry::GetTargetAuraState },
    { "GetCasterAuraStateNot", &LuaSpellEntry::GetCasterAuraStateNot },
    { "GetTargetAuraStateNot", &LuaSpellEntry::GetTargetAuraStateNot },
    { "GetCasterAuraSpell", &LuaSpellEntry::GetCasterAuraSpell },
    { "GetTargetAuraSpell", &LuaSpellEntry::GetTargetAuraSpell },
    { "GetExcludeCasterAuraSpell", &LuaSpellEntry::GetExcludeCasterAuraSpell },
    { "GetExcludeTargetAuraSpell", &LuaSpellEntry::GetExcludeTargetAuraSpell },
    { "GetCastingTimeIndex", &LuaSpellEntry::GetCastingTimeIndex },
    { "GetRecoveryTime", &LuaSpellEntry::GetRecoveryTime },
    { "GetCategoryRecoveryTime", &LuaSpellEntry::GetCategoryRecoveryTime },
    { "GetInterruptFlags", &LuaSpellEntry::GetInterruptFlags },
    { "GetAuraInterruptFlags", &LuaSpellEntry::GetAuraInterruptFlags },
    { "GetChannelInterruptFlags", &LuaSpellEntry::GetChannelInterruptFlags },
    { "GetProcFlags", &LuaSpellEntry::GetProcFlags },
    { "GetProcChance", &LuaSpellEntry::GetProcChance },
    { "GetProcCharges", &LuaSpellEntry::GetProcCharges },
    { "GetMaxLevel", &LuaSpellEntry::GetMaxLevel },
    { "GetBaseLevel", &LuaSpellEntry::GetBaseLevel },
    { "GetSpellLevel", &LuaSpellEntry::GetSpellLevel },
    { "GetDurationIndex", &LuaSpellEntry::GetDurationIndex },
    { "GetPowerType", &LuaSpellEntry::GetPowerType },
    { "GetManaCost", &LuaSpellEntry::GetManaCost },
    { "GetManaCostPerlevel", &LuaSpellEntry::GetManaCostPerlevel },
    { "GetManaPerSecond", &LuaSpellEntry::GetManaPerSecond },
    { "GetManaPerSecondPerLevel", &LuaSpellEntry::GetManaPerSecondPerLevel },
    { "GetRangeIndex", &LuaSpellEntry::GetRangeIndex },
    { "GetSpeed", &LuaSpellEntry::GetSpeed },
    { "GetStackAmount", &LuaSpellEntry::GetStackAmount },
    { "GetTotem", &LuaSpellEntry::GetTotem },
    { "GetReagent", &LuaSpellEntry::GetReagent },
    { "GetReagentCount", &LuaSpellEntry::GetReagentCount },
    { "GetEquippedItemClass", &LuaSpellEntry::GetEquippedItemClass },
    { "GetEquippedItemSubClassMask", &LuaSpellEntry::GetEquippedItemSubClassMask },
    { "GetEquippedItemInventoryTypeMask", &LuaSpellEntry::GetEquippedItemInventoryTypeMask },
    { "GetEffect", &LuaSpellEntry::GetEffect },
    { "GetEffectDieSides", &LuaSpellEntry::GetEffectDieSides },
    { "GetEffectRealPointsPerLevel", &LuaSpellEntry::GetEffectRealPointsPerLevel },
    { "GetEffectBasePoints", &LuaSpellEntry::GetEffectBasePoints },
    { "GetEffectMechanic", &LuaSpellEntry::GetEffectMechanic },
    { "GetEffectImplicitTargetA", &LuaSpellEntry::GetEffectImplicitTargetA },
    { "GetEffectImplicitTargetB", &LuaSpellEntry::GetEffectImplicitTargetB },
    { "GetEffectRadiusIndex", &LuaSpellEntry::GetEffectRadiusIndex },
    { "GetEffectApplyAuraName", &LuaSpellEntry::GetEffectApplyAuraName },
    { "GetEffectAmplitude", &LuaSpellEntry::GetEffectAmplitude },
    { "GetEffectValueMultiplier", &LuaSpellEntry::GetEffectValueMultiplier },
    { "GetEffectChainTarget", &LuaSpellEntry::GetEffectChainTarget },
    { "GetEffectItemType", &LuaSpellEntry::GetEffectItemType },
    { "GetEffectMiscValue", &LuaSpellEntry::GetEffectMiscValue },
    { "GetEffectMiscValueB", &LuaSpellEntry::GetEffectMiscValueB },
    { "GetEffectTriggerSpell", &LuaSpellEntry::GetEffectTriggerSpell },
    { "GetEffectPointsPerComboPoint", &LuaSpellEntry::GetEffectPointsPerComboPoint },
    { "GetEffectSpellClassMask", &LuaSpellEntry::GetEffectSpellClassMask },
    { "GetSpellVisual", &LuaSpellEntry::GetSpellVisual },
    { "GetSpellIconID", &LuaSpellEntry::GetSpellIconID },
    { "GetActiveIconID", &LuaSpellEntry::GetActiveIconID },
    { "GetSpellPriority", &LuaSpellEntry::GetSpellPriority },
    { "GetSpellName", &LuaSpellEntry::GetSpellName },
    { "GetRank", &LuaSpellEntry::GetRank },
    { "GetManaCostPercentage", &LuaSpellEntry::GetManaCostPercentage },
    { "GetStartRecoveryCategory", &LuaSpellEntry::GetStartRecoveryCategory },
    { "GetStartRecoveryTime", &LuaSpellEntry::GetStartRecoveryTime },
    { "GetMaxTargetLevel", &LuaSpellEntry::GetMaxTargetLevel },
    { "GetSpellFamilyName", &LuaSpellEntry::GetSpellFamilyName },
    { "GetSpellFamilyFlags", &LuaSpellEntry::GetSpellFamilyFlags },
    { "GetMaxAffectedTargets", &LuaSpellEntry::GetMaxAffectedTargets },
    { "GetDmgClass", &LuaSpellEntry::GetDmgClass },
    { "GetPreventionType", &LuaSpellEntry::GetPreventionType },
    { "GetEffectDamageMultiplier", &LuaSpellEntry::GetEffectDamageMultiplier },
    { "GetTotemCategory", &LuaSpellEntry::GetTotemCategory },
    { "GetAreaGroupId", &LuaSpellEntry::GetAreaGroupId },
    { "GetSchoolMask", &LuaSpellEntry::GetSchoolMask },
    { "GetRuneCostID", &LuaSpellEntry::GetRuneCostID },
    { "GetEffectBonusMultiplier", &LuaSpellEntry::GetEffectBonusMultiplier },

    { NULL, NULL }
};

// fix compile error about accessing vehicle destructor
template<> int ElunaTemplate<Vehicle>::CollectGarbage(lua_State* L)
{
    ASSERT(!manageMemory);

    // Get object pointer (and check type, no error)
    ElunaObject* obj = Eluna::CHECKOBJ<ElunaObject>(L, 1, false);
    delete obj;
    return 0;
}

// Template by Mud from http://stackoverflow.com/questions/4484437/lua-integer-type/4485511#4485511
template<> int ElunaTemplate<unsigned long long>::Add(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) + Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Substract(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) - Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Multiply(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) * Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Divide(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) / Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Mod(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) % Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
// template<> int ElunaTemplate<unsigned long long>::UnaryMinus(lua_State* L) { Eluna::Push(L, -Eluna::CHECKVAL<unsigned long long>(L, 1)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Equal(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) == Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Less(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) < Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::LessOrEqual(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<unsigned long long>(L, 1) <= Eluna::CHECKVAL<unsigned long long>(L, 2)); return 1; }
template<> int ElunaTemplate<unsigned long long>::Pow(lua_State* L)
{
    Eluna::Push(L, static_cast<unsigned long long>(powl(static_cast<long double>(Eluna::CHECKVAL<unsigned long long>(L, 1)), static_cast<long double>(Eluna::CHECKVAL<unsigned long long>(L, 2)))));
    return 1;
}
template<> int ElunaTemplate<unsigned long long>::ToString(lua_State* L)
{
    unsigned long long l = Eluna::CHECKVAL<unsigned long long>(L, 1);
    std::ostringstream ss;
    ss << l;
    Eluna::Push(L, ss.str());
    return 1;
}

template<> int ElunaTemplate<long long>::Add(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) + Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::Substract(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) - Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::Multiply(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) * Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::Divide(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) / Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::Mod(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) % Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::UnaryMinus(lua_State* L) { Eluna::Push(L, -Eluna::CHECKVAL<long long>(L, 1)); return 1; }
template<> int ElunaTemplate<long long>::Equal(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) == Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::Less(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) < Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::LessOrEqual(lua_State* L) { Eluna::Push(L, Eluna::CHECKVAL<long long>(L, 1) <= Eluna::CHECKVAL<long long>(L, 2)); return 1; }
template<> int ElunaTemplate<long long>::Pow(lua_State* L)
{
    Eluna::Push(L, static_cast<long long>(powl(static_cast<long double>(Eluna::CHECKVAL<long long>(L, 1)), static_cast<long double>(Eluna::CHECKVAL<long long>(L, 2)))));
    return 1;
}
template<> int ElunaTemplate<long long>::ToString(lua_State* L)
{
    long long l = Eluna::CHECKVAL<long long>(L, 1);
    std::ostringstream ss;
    ss << l;
    Eluna::Push(L, ss.str());
    return 1;
}

void RegisterFunctions(Eluna* E)
{
    ElunaGlobal::SetMethods(E, GlobalMethods);

    ElunaTemplate<Object>::Register(E, "Object");
    ElunaTemplate<Object>::SetMethods(E, ObjectMethods);

    ElunaTemplate<WorldObject>::Register(E, "WorldObject");
    ElunaTemplate<WorldObject>::SetMethods(E, ObjectMethods);
    ElunaTemplate<WorldObject>::SetMethods(E, WorldObjectMethods);

    ElunaTemplate<Unit>::Register(E, "Unit");
    ElunaTemplate<Unit>::SetMethods(E, ObjectMethods);
    ElunaTemplate<Unit>::SetMethods(E, WorldObjectMethods);
    ElunaTemplate<Unit>::SetMethods(E, UnitMethods);

    ElunaTemplate<Player>::Register(E, "Player");
    ElunaTemplate<Player>::SetMethods(E, ObjectMethods);
    ElunaTemplate<Player>::SetMethods(E, WorldObjectMethods);
    ElunaTemplate<Player>::SetMethods(E, UnitMethods);
    ElunaTemplate<Player>::SetMethods(E, PlayerMethods);

    ElunaTemplate<Creature>::Register(E, "Creature");
    ElunaTemplate<Creature>::SetMethods(E, ObjectMethods);
    ElunaTemplate<Creature>::SetMethods(E, WorldObjectMethods);
    ElunaTemplate<Creature>::SetMethods(E, UnitMethods);
    ElunaTemplate<Creature>::SetMethods(E, CreatureMethods);

    ElunaTemplate<GameObject>::Register(E, "GameObject");
    ElunaTemplate<GameObject>::SetMethods(E, ObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(E, WorldObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(E, GameObjectMethods);

    ElunaTemplate<Corpse>::Register(E, "Corpse");
    ElunaTemplate<Corpse>::SetMethods(E, ObjectMethods);
    ElunaTemplate<Corpse>::SetMethods(E, WorldObjectMethods);
    ElunaTemplate<Corpse>::SetMethods(E, CorpseMethods);

    ElunaTemplate<Item>::Register(E, "Item");
    ElunaTemplate<Item>::SetMethods(E, ObjectMethods);
    ElunaTemplate<Item>::SetMethods(E, ItemMethods);

    ElunaTemplate<ItemTemplate>::Register(E, "ItemTemplate");
    ElunaTemplate<ItemTemplate>::SetMethods(E, ItemTemplateMethods);

    ElunaTemplate<Vehicle>::Register(E, "Vehicle");
    ElunaTemplate<Vehicle>::SetMethods(E, VehicleMethods);

    ElunaTemplate<Group>::Register(E, "Group");
    ElunaTemplate<Group>::SetMethods(E, GroupMethods);

    ElunaTemplate<Guild>::Register(E, "Guild");
    ElunaTemplate<Guild>::SetMethods(E, GuildMethods);

    ElunaTemplate<Aura>::Register(E, "Aura");
    ElunaTemplate<Aura>::SetMethods(E, AuraMethods);

    ElunaTemplate<Spell>::Register(E, "Spell");
    ElunaTemplate<Spell>::SetMethods(E, SpellMethods);

    ElunaTemplate<Quest>::Register(E, "Quest");
    ElunaTemplate<Quest>::SetMethods(E, QuestMethods);

    ElunaTemplate<Map>::Register(E, "Map");
    ElunaTemplate<Map>::SetMethods(E, MapMethods);

    ElunaTemplate<AuctionHouseEntry>::Register(E, "AuctionHouseEntry");
    ElunaTemplate<AuctionHouseEntry>::SetMethods(E, AuctionMethods);

    ElunaTemplate<BattleGround>::Register(E, "BattleGround");
    ElunaTemplate<BattleGround>::SetMethods(E, BattleGroundMethods);

    ElunaTemplate<ChatHandler>::Register(E, "ChatHandler");
    ElunaTemplate<ChatHandler>::SetMethods(E, ChatHandlerMethods);

    ElunaTemplate<WorldPacket>::Register(E, "WorldPacket", true);
    ElunaTemplate<WorldPacket>::SetMethods(E, PacketMethods);

    ElunaTemplate<ElunaQuery>::Register(E, "ElunaQuery", true);
    ElunaTemplate<ElunaQuery>::SetMethods(E, QueryMethods);

    ElunaTemplate<AchievementEntry>::Register(E, "AchievementEntry");
    ElunaTemplate<AchievementEntry>::SetMethods(E, AchievementMethods);

    ElunaTemplate<Roll>::Register(E, "Roll");
    ElunaTemplate<Roll>::SetMethods(E, RollMethods);

    ElunaTemplate<Loot>::Register(E, "Loot");
    ElunaTemplate<Loot>::SetMethods(E, LootMethods);

    ElunaTemplate<GmTicket>::Register(E, "Ticket");
    ElunaTemplate<GmTicket>::SetMethods(E, TicketMethods);
  
    ElunaTemplate<SpellInfo>::Register(E, "SpellInfo");
    ElunaTemplate<SpellInfo>::SetMethods(E, SpellInfoMethods);

    ElunaTemplate<GemPropertiesEntry>::Register(E, "GemPropertiesEntry");
    ElunaTemplate<GemPropertiesEntry>::SetMethods(E, GemPropertiesEntryMethods);

    ElunaTemplate<SpellEntry>::Register(E, "SpellEntry");
    ElunaTemplate<SpellEntry>::SetMethods(E, SpellEntryMethods);

    ElunaTemplate<long long>::Register(E, "long long", true);

    ElunaTemplate<unsigned long long>::Register(E, "unsigned long long", true);
}
