/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef GLOBALMETHODS_H
#define GLOBALMETHODS_H

#include "BindingMap.h"
#include "ElunaDBCRegistry.h"

#include "BanMgr.h"
#include "GameTime.h"
#include "SharedDefines.h"
#include "OutdoorPvPMgr.h"
#include "../../../../src/server/scripts/OutdoorPvP/OutdoorPvPNA.h"


enum BanMode
{
    BAN_ACCOUNT = 1,
    BAN_CHARACTER = 2,
    BAN_IP = 3
};

/***
 * These functions can be used anywhere at any time, including at start-up.
 */
namespace LuaGlobalFunctions
{
    /**
     * Returns Lua engine's name.
     *
     * Always returns "ElunaEngine" on Eluna.
     *
     * @return string engineName
     */
    int GetLuaEngine(lua_State* L)
    {
        Eluna::Push(L, "ElunaEngine");
        return 1;
    }

    /**
     * Returns emulator's name.
     *
     * The result will be either `MaNGOS`, `cMaNGOS`, or `TrinityCore`.
     *
     * @return string coreName
     */
    int GetCoreName(lua_State* L)
    {
        Eluna::Push(L, CORE_NAME);
        return 1;
    }

    /**
     * Returns config value as a string.
     *
     * @param string name : name of the value
     * @return string value
     */
    int GetConfigValue(lua_State* L)
    {
        // The key we want from the config file.
        const char* key = Eluna::CHECKVAL<const char*>(L, 1);

        // Check if any of the arguments are missing.
        if (!key)
            return 0;

		// Get config value and tell Eluna to push it.
        auto optionFound = sConfigMgr->GetOption<std::string>(key, "", false);
        Eluna::Push(L, optionFound);
        return 1;
    }

    /**
     * Returns emulator .conf RealmID
     *
     * - for MaNGOS returns the realmID as it is stored in the core.
     * - for TrinityCore returns the realmID as it is in the conf file.
     * @return uint32 realm ID
     */
    int GetRealmID(lua_State* L)
    {
        Eluna::Push(L, sConfigMgr->GetOption<uint32>("RealmID", 1));
        return 1;
    }

    /**
     * Returns emulator version
     *
     * - For TrinityCore returns the date of the last revision, e.g. `2015-08-26 22:53:12 +0300`
     * - For cMaNGOS returns the date and time of the last revision, e.g. `2015-09-06 13:18:50`
     * - for MaNGOS returns the version number as string, e.g. `21000`
     *
     * @return string version
     */
    int GetCoreVersion(lua_State* L)
    {
        Eluna::Push(L, CORE_VERSION);
        return 1;
    }

    /**
     * Returns emulator's supported expansion.
     *
     * Expansion is 0 for pre-TBC, 1 for TBC, 2 for WotLK, and 3 for Cataclysm.
     *
     * @return int32 expansion
     */
    int GetCoreExpansion(lua_State* L)
    {
        Eluna::Push(L, 2);
        return 1;
    }
    
    /**
     * Returns the [Map] pointer of the Lua state. Returns null for the "World" state. 
     *
     * @return [Map] map
     */
    int GetStateMap(lua_State* L)
    {
        // Until AC supports multistate, this will always return nil
        Eluna::Push(L);
        return 1;
    }

    /**
     * Returns the map ID of the Lua state. Returns -1 for the "World" state.
     *
     * @return int32 mapId
     */
    int GetStateMapId(lua_State* L)
    {
        // Until AC supports multistate, this will always return -1
        Eluna::Push(L, -1);
        return 1;
    }

    /**
     * Returns the instance ID of the Lua state. Returns 0 for continent maps and the world state.
     *
     * @return uint32 instanceId
     */
    int GetStateInstanceId(lua_State* L)
    {
        // Until AC supports multistate, this will always return 0
        Eluna::Push(L, 0);
        return 1;
    }

    /**
     * Returns [Quest] template
     *
     * @param uint32 questId : [Quest] entry ID
     * @return [Quest] quest
     */
    int GetQuest(lua_State* L)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 1);

        Eluna::Push(L, eObjectMgr->GetQuestTemplate(questId));
        return 1;
    }

    /**
     * Finds and Returns [Player] by guid if found
     *
     * @param ObjectGuid guid : guid of the [Player], you can get it with [Object:GetGUID]
     * @return [Player] player
     */
    int GetPlayerByGUID(lua_State* L)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);
        Eluna::Push(L, eObjectAccessor()FindPlayer(guid));
        return 1;
    }

    /**
     * Finds and Returns [Player] by name if found
     *
     * @param string name : name of the [Player]
     * @return [Player] player
     */
    int GetPlayerByName(lua_State* L)
    {
        const char* name = Eluna::CHECKVAL<const char*>(L, 1);
        Eluna::Push(L, eObjectAccessor()FindPlayerByName(name));
        return 1;
    }

    /**
     * Returns game time in seconds
     *
     * @return uint32 time
     */
    int GetGameTime(lua_State* L)
    {
        Eluna::Push(L, GameTime::GetGameTime().count());
        return 1;
    }

    /**
     * Returns a table with all the current [Player]s in the world
     *
     * Does not return players that may be teleporting or otherwise not on any map.
     *
     *     enum TeamId
     *     {
     *         TEAM_ALLIANCE = 0,
     *         TEAM_HORDE = 1,
     *         TEAM_NEUTRAL = 2
     *     };
     *
     * @param [TeamId] team = TEAM_NEUTRAL : optional check team of the [Player], Alliance, Horde or Neutral (All)
     * @param bool onlyGM = false : optional check if GM only
     * @return table worldPlayers
     */
    int GetPlayersInWorld(lua_State* L)
    {
        uint32 team = Eluna::CHECKVAL<uint32>(L, 1, TEAM_NEUTRAL);
        bool onlyGM = Eluna::CHECKVAL<bool>(L, 2, false);

        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        {
            std::shared_lock<std::shared_mutex> lock(*HashMapHolder<Player>::GetLock());
            const HashMapHolder<Player>::MapType& m = eObjectAccessor()GetPlayers();
            for (HashMapHolder<Player>::MapType::const_iterator it = m.begin(); it != m.end(); ++it)
            {
                if (Player* player = it->second)
                {
                    if (!player->IsInWorld())
                        continue;

                    if ((team == TEAM_NEUTRAL || player->GetTeamId() == team) && (!onlyGM || player->IsGameMaster()))
                    {
                        Eluna::Push(L, player);
                        lua_rawseti(L, tbl, ++i);
                    }
                }
            }
        }

        lua_settop(L, tbl); // push table to top of stack
        return 1;
    }

    /**
     * Returns a [Guild] by name.
     *
     * @param string name
     * @return [Guild] guild : the Guild, or `nil` if it doesn't exist
     */
    int GetGuildByName(lua_State* L)
    {
        const char* name = Eluna::CHECKVAL<const char*>(L, 1);
        Eluna::Push(L, eGuildMgr->GetGuildByName(name));
        return 1;
    }

    /**
     * Returns a [Map] by ID.
     *
     * @param uint32 mapId : see [Map.dbc](https://github.com/cmangos/issues/wiki/Map.dbc)
     * @param uint32 instanceId = 0 : required if the map is an instance, otherwise don't pass anything
     * @return [Map] map : the Map, or `nil` if it doesn't exist
     */
    int GetMapById(lua_State* L)
    {
        uint32 mapid = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 instance = Eluna::CHECKVAL<uint32>(L, 2, 0);

        Eluna::Push(L, eMapMgr->FindMap(mapid, instance));
        return 1;
    }

    /**
     * Returns [Guild] by the leader's GUID
     *
     * @param ObjectGuid guid : the guid of a [Guild] leader
     * @return [Guild] guild, or `nil` if it doesn't exist
     */
    int GetGuildByLeaderGUID(lua_State* L)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);

        Eluna::Push(L, eGuildMgr->GetGuildByLeader(guid));
        return 1;
    }

    /**
     * Returns the amount of [Player]s in the world.
     *
     * @return uint32 count
     */
    int GetPlayerCount(lua_State* L)
    {
        Eluna::Push(L, eWorldSessionMgr->GetActiveSessionCount());
        return 1;
    }

    /**
     * Builds a [Player]'s GUID
     *
     * [Player] GUID consist of low GUID and type ID
     *
     * [Player] and [Creature] for example can have the same low GUID but not GUID.
     *
     * @param uint32 lowguid : low GUID of the [Player]
     * @return ObjectGuid guid
     */
    int GetPlayerGUID(lua_State* L)
    {
        uint32 lowguid = Eluna::CHECKVAL<uint32>(L, 1);
        Eluna::Push(L, MAKE_NEW_GUID(lowguid, 0, HIGHGUID_PLAYER));
        return 1;
    }

    /**
     * Builds an [Item]'s GUID.
     *
     * [Item] GUID consist of low GUID and type ID
     * [Player] and [Item] for example can have the same low GUID but not GUID.
     *
     * @param uint32 lowguid : low GUID of the [Item]
     * @return ObjectGuid guid
     */
    int GetItemGUID(lua_State* L)
    {
        uint32 lowguid = Eluna::CHECKVAL<uint32>(L, 1);
        Eluna::Push(L, MAKE_NEW_GUID(lowguid, 0, HIGHGUID_ITEM));
        return 1;
    }

    /**
    * Returns the [ItemTemplate] for the specified item ID.  The ItemTemplate contains all static data about an item, such as name, quality, stats, required level, and more.
    *
    * @param uint32 itemID : the item entry ID from `item_template` to look up
    * @return [ItemTemplate] itemTemplate
    */
    int GetItemTemplate(lua_State* L)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
        Eluna::Push(L, eObjectMgr->GetItemTemplate(entry));
        return 1;
    }

    /**
     * Builds a [GameObject]'s GUID.
     *
     * A GameObject's GUID consist of entry ID, low GUID and type ID
     *
     * A [Player] and GameObject for example can have the same low GUID but not GUID.
     *
     * @param uint32 lowguid : low GUID of the [GameObject]
     * @param uint32 entry : entry ID of the [GameObject]
     * @return ObjectGuid guid
     */
    int GetObjectGUID(lua_State* L)
    {
        uint32 lowguid = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, MAKE_NEW_GUID(lowguid, entry, HIGHGUID_GAMEOBJECT));
        return 1;
    }

    /**
     * Builds a [Creature]'s GUID.
     *
     * [Creature] GUID consist of entry ID, low GUID and type ID
     *
     * [Player] and [Creature] for example can have the same low GUID but not GUID.
     *
     * @param uint32 lowguid : low GUID of the [Creature]
     * @param uint32 entry : entry ID of the [Creature]
     * @return ObjectGuid guid
     */
    int GetUnitGUID(lua_State* L)
    {
        uint32 lowguid = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, MAKE_NEW_GUID(lowguid, entry, HIGHGUID_UNIT));
        return 1;
    }

    /**
     * Returns the low GUID from a GUID.
     *
     * A GUID consists of a low GUID, type ID, and possibly an entry ID depending on the type ID.
     *
     * Low GUID is an ID to distinct the objects of the same type.
     *
     * [Player] and [Creature] for example can have the same low GUID but not GUID.
     *
     * On TrinityCore all low GUIDs are different for all objects of the same type.
     * For example creatures in instances are assigned new GUIDs when the Map is created.
     *
     * On MaNGOS and cMaNGOS low GUIDs are unique only on the same map.
     * For example creatures in instances use the same low GUID assigned for that spawn in the database.
     * This is why to identify a creature you have to know the instanceId and low GUID. See [Map:GetIntstanceId]
     *
     * @param ObjectGuid guid : GUID of an [Object]
     * @return uint32 lowguid : low GUID of the [Object]
     */
    int GetGUIDLow(lua_State* L)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);

        Eluna::Push(L, guid.GetCounter());
        return 1;
    }

    /**
     * Returns an chat link for an [Item].
     *
     *     enum LocaleConstant
     *     {
     *         LOCALE_enUS = 0,
     *         LOCALE_koKR = 1,
     *         LOCALE_frFR = 2,
     *         LOCALE_deDE = 3,
     *         LOCALE_zhCN = 4,
     *         LOCALE_zhTW = 5,
     *         LOCALE_esES = 6,
     *         LOCALE_esMX = 7,
     *         LOCALE_ruRU = 8
     *     };
     *
     * @param uint32 entry : entry ID of an [Item]
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the [Item] name in
     * @return string itemLink
     */
    int GetItemLink(lua_State* L)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
        uint8 locale = Eluna::CHECKVAL<uint8>(L, 2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(L, 2, "valid LocaleConstant expected");

        const ItemTemplate* temp = eObjectMgr->GetItemTemplate(entry);
        if (!temp)
            return luaL_argerror(L, 1, "valid ItemEntry expected");

        std::string name = temp->Name1;
        if (ItemLocale const* il = eObjectMgr->GetItemLocale(entry))
            ObjectMgr::GetLocaleString(il->Name, static_cast<LocaleConstant>(locale), name);

        std::ostringstream oss;
        oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
            "|Hitem:" << entry << ":0:" <<
            "0:0:0:0:" <<
            "0:0:0:0|h[" << name << "]|h|r";

        Eluna::Push(L, oss.str());
        return 1;
    }

    /**
     * Returns the type ID from a GUID.
     *
     * Type ID is different for each type ([Player], [Creature], [GameObject], etc.).
     *
     * GUID consist of entry ID, low GUID, and type ID.
     *
     * @param ObjectGuid guid : GUID of an [Object]
     * @return int32 typeId : type ID of the [Object]
     */
    int GetGUIDType(lua_State* L)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);
        Eluna::Push(L, static_cast<int>(guid.GetHigh()));
        return 1;
    }

    /**
     * Returns the entry ID from a GUID.
     *
     * GUID consist of entry ID, low GUID, and type ID.
     *
     * @param ObjectGuid guid : GUID of an [Creature] or [GameObject]
     * @return uint32 entry : entry ID, or `0` if `guid` is not a [Creature] or [GameObject]
     */
    int GetGUIDEntry(lua_State* L)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);
        Eluna::Push(L, guid.GetEntry());
        return 1;
    }

    /**
     * Returns the area or zone's name.
     *
     *     enum LocaleConstant
     *     {
     *         LOCALE_enUS = 0,
     *         LOCALE_koKR = 1,
     *         LOCALE_frFR = 2,
     *         LOCALE_deDE = 3,
     *         LOCALE_zhCN = 4,
     *         LOCALE_zhTW = 5,
     *         LOCALE_esES = 6,
     *         LOCALE_esMX = 7,
     *         LOCALE_ruRU = 8
     *     };
     *
     * @param uint32 areaOrZoneId : area ID or zone ID
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the name in
     * @return string areaOrZoneName
     */
    int GetAreaName(lua_State* L)
    {
        uint32 areaOrZoneId = Eluna::CHECKVAL<uint32>(L, 1);
        uint8 locale = Eluna::CHECKVAL<uint8>(L, 2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(L, 2, "valid LocaleConstant expected");

        AreaTableEntry const* areaEntry = sAreaTableStore.LookupEntry(areaOrZoneId);

        if (!areaEntry)
            return luaL_argerror(L, 1, "valid Area or Zone ID expected");

        Eluna::Push(L, areaEntry->area_name[locale]);
        return 1;
    }

    /**
     * Returns the currently active game events.
     *
     * @return table activeEvents
     */
    int GetActiveGameEvents(lua_State* L)
    {
        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 counter = 1;
        GameEventMgr::ActiveEvents const& activeEvents = eGameEventMgr->GetActiveEventList();

        for (GameEventMgr::ActiveEvents::const_iterator i = activeEvents.begin(); i != activeEvents.end(); ++i)
        {
            Eluna::Push(L, *i);
            lua_rawseti(L, tbl, counter);

            counter++;
        }

        lua_settop(L, tbl);
        return 1;
    }

    static int RegisterEntryHelper(lua_State* L, int regtype)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 ev = Eluna::CHECKVAL<uint32>(L, 2);
        luaL_checktype(L, 3, LUA_TFUNCTION);
        uint32 shots = Eluna::CHECKVAL<uint32>(L, 4, 0);

        lua_pushvalue(L, 3);
        int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (functionRef >= 0)
            return Eluna::GetEluna(L)->Register(L, regtype, id, ObjectGuid(), 0, ev, functionRef, shots);
        else
            luaL_argerror(L, 3, "unable to make a ref to function");
        return 0;
    }

    static int RegisterEventHelper(lua_State* L, int regtype)
    {
        uint32 ev = Eluna::CHECKVAL<uint32>(L, 1);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        uint32 shots = Eluna::CHECKVAL<uint32>(L, 3, 0);

        lua_pushvalue(L, 2);
        int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (functionRef >= 0)
            return Eluna::GetEluna(L)->Register(L, regtype, 0, ObjectGuid(), 0, ev, functionRef, shots);
        else
            luaL_argerror(L, 2, "unable to make a ref to function");
        return 0;
    }

    static int RegisterUniqueHelper(lua_State* L, int regtype)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);
        uint32 instanceId = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 ev = Eluna::CHECKVAL<uint32>(L, 3);
        luaL_checktype(L, 4, LUA_TFUNCTION);
        uint32 shots = Eluna::CHECKVAL<uint32>(L, 5, 0);

        lua_pushvalue(L, 4);
        int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (functionRef >= 0)
            return Eluna::GetEluna(L)->Register(L, regtype, 0, guid, instanceId, ev, functionRef, shots);
        else
            luaL_argerror(L, 4, "unable to make a ref to function");
        return 0;
    }

    /**
     * Registers a server event handler.
     *
     *     enum ServerEvents
     *     {
     *         // Server
     *         SERVER_EVENT_ON_NETWORK_START           =     1,       // Not Implemented
     *         SERVER_EVENT_ON_NETWORK_STOP            =     2,       // Not Implemented
     *         SERVER_EVENT_ON_SOCKET_OPEN             =     3,       // Not Implemented
     *         SERVER_EVENT_ON_SOCKET_CLOSE            =     4,       // Not Implemented
     *         SERVER_EVENT_ON_PACKET_RECEIVE          =     5,       // (event, packet, player) - Player only if accessible. Can return false, newPacket
     *         SERVER_EVENT_ON_PACKET_RECEIVE_UNKNOWN  =     6,       // Not Implemented
     *         SERVER_EVENT_ON_PACKET_SEND             =     7,       // (event, packet, player) - Player only if accessible. Can return false, newPacket
     *
     *         // World
     *         WORLD_EVENT_ON_OPEN_STATE_CHANGE        =     8,        // (event, open) - Needs core support on Mangos
     *         WORLD_EVENT_ON_CONFIG_LOAD              =     9,        // (event, reload)
     *         // UNUSED                               =     10,
     *         WORLD_EVENT_ON_SHUTDOWN_INIT            =     11,       // (event, code, mask)
     *         WORLD_EVENT_ON_SHUTDOWN_CANCEL          =     12,       // (event)
     *         WORLD_EVENT_ON_UPDATE                   =     13,       // (event, diff)
     *         WORLD_EVENT_ON_STARTUP                  =     14,       // (event)
     *         WORLD_EVENT_ON_SHUTDOWN                 =     15,       // (event)
     *
     *         // Eluna
     *         ELUNA_EVENT_ON_LUA_STATE_CLOSE          =     16,       // (event) - triggers just before shutting down eluna (on shutdown and restart)
     *
     *         // Map
     *         MAP_EVENT_ON_CREATE                     =     17,       // (event, map)
     *         MAP_EVENT_ON_DESTROY                    =     18,       // (event, map)
     *         MAP_EVENT_ON_GRID_LOAD                  =     19,       // Not Implemented
     *         MAP_EVENT_ON_GRID_UNLOAD                =     20,       // Not Implemented
     *         MAP_EVENT_ON_PLAYER_ENTER               =     21,       // (event, map, player)
     *         MAP_EVENT_ON_PLAYER_LEAVE               =     22,       // (event, map, player)
     *         MAP_EVENT_ON_UPDATE                     =     23,       // (event, map, diff)
     *
     *         // Area trigger
     *         TRIGGER_EVENT_ON_TRIGGER                =     24,       // (event, player, triggerId) - Can return true
     *
     *         // Weather
     *         WEATHER_EVENT_ON_CHANGE                 =     25,       // (event, zoneId, state, grade)
     *
     *         // Auction house
     *         AUCTION_EVENT_ON_ADD                    =     26,       // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
     *         AUCTION_EVENT_ON_REMOVE                 =     27,       // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
     *         AUCTION_EVENT_ON_SUCCESSFUL             =     28,       // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
     *         AUCTION_EVENT_ON_EXPIRE                 =     29,       // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
     *
     *         // AddOns
     *         ADDON_EVENT_ON_MESSAGE                  =     30,       // (event, sender, type, prefix, msg, target) - target can be nil/whisper_target/guild/group/channel. Can return false
     *
     *         WORLD_EVENT_ON_DELETE_CREATURE          =     31,       // (event, creature)
     *         WORLD_EVENT_ON_DELETE_GAMEOBJECT        =     32,       // (event, gameobject)
     *
     *         // Eluna
     *         ELUNA_EVENT_ON_LUA_STATE_OPEN           =     33,       // (event) - triggers after all scripts are loaded
     *
     *         GAME_EVENT_START                        =     34,       // (event, gameeventid)
     *         GAME_EVENT_STOP                         =     35,       // (event, gameeventid)
     *     };
     *
     * @proto cancel = (event, function)
     * @proto cancel = (event, function, shots)
     *
     * @param uint32 event : server event ID, refer to ServerEvents above
     * @param function function : function that will be called when the event occurs
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterServerEvent(lua_State* L)
    {
        return RegisterEventHelper(L, Hooks::REGTYPE_SERVER);
    }

    /**
     * Registers a [Player] event handler.
     *
     * <pre>
     * enum PlayerEvents
     * {
     *     PLAYER_EVENT_ON_CHARACTER_CREATE        =     1,        // (event, player)
     *     PLAYER_EVENT_ON_CHARACTER_DELETE        =     2,        // (event, guid)
     *     PLAYER_EVENT_ON_LOGIN                   =     3,        // (event, player)
     *     PLAYER_EVENT_ON_LOGOUT                  =     4,        // (event, player)
     *     PLAYER_EVENT_ON_SPELL_CAST              =     5,        // (event, player, spell, skipCheck)
     *     PLAYER_EVENT_ON_KILL_PLAYER             =     6,        // (event, killer, killed)
     *     PLAYER_EVENT_ON_KILL_CREATURE           =     7,        // (event, killer, killed)
     *     PLAYER_EVENT_ON_KILLED_BY_CREATURE      =     8,        // (event, killer, killed)
     *     PLAYER_EVENT_ON_DUEL_REQUEST            =     9,        // (event, target, challenger)
     *     PLAYER_EVENT_ON_DUEL_START              =     10,       // (event, player1, player2)
     *     PLAYER_EVENT_ON_DUEL_END                =     11,       // (event, winner, loser, type)
     *     PLAYER_EVENT_ON_GIVE_XP                 =     12,       // (event, player, amount, victim, source) - Can return new XP amount
     *     PLAYER_EVENT_ON_LEVEL_CHANGE            =     13,       // (event, player, oldLevel)
     *     PLAYER_EVENT_ON_MONEY_CHANGE            =     14,       // (event, player, amount) - Can return new money amount
     *     PLAYER_EVENT_ON_REPUTATION_CHANGE       =     15,       // (event, player, factionId, standing, incremental) - Can return new standing -> if standing == -1, it will prevent default action (rep gain)
     *     PLAYER_EVENT_ON_TALENTS_CHANGE          =     16,       // (event, player, points)
     *     PLAYER_EVENT_ON_TALENTS_RESET           =     17,       // (event, player, noCost)
     *     PLAYER_EVENT_ON_CHAT                    =     18,       // (event, player, msg, Type, lang) - Can return false, newMessage
     *     PLAYER_EVENT_ON_WHISPER                 =     19,       // (event, player, msg, Type, lang, receiver) - Can return false, newMessage
     *     PLAYER_EVENT_ON_GROUP_CHAT              =     20,       // (event, player, msg, Type, lang, group) - Can return false, newMessage
     *     PLAYER_EVENT_ON_GUILD_CHAT              =     21,       // (event, player, msg, Type, lang, guild) - Can return false, newMessage
     *     PLAYER_EVENT_ON_CHANNEL_CHAT            =     22,       // (event, player, msg, Type, lang, channel) - channel is negative for custom channels. Can return false, newMessage
     *     PLAYER_EVENT_ON_EMOTE                   =     23,       // (event, player, emote) - Not triggered on any known emote
     *     PLAYER_EVENT_ON_TEXT_EMOTE              =     24,       // (event, player, textEmote, emoteNum, guid)
     *     PLAYER_EVENT_ON_SAVE                    =     25,       // (event, player)
     *     PLAYER_EVENT_ON_BIND_TO_INSTANCE        =     26,       // (event, player, difficulty, mapid, permanent)
     *     PLAYER_EVENT_ON_UPDATE_ZONE             =     27,       // (event, player, newZone, newArea)
     *     PLAYER_EVENT_ON_MAP_CHANGE              =     28,       // (event, player)
     *
     *     // Custom
     *     PLAYER_EVENT_ON_EQUIP                   =     29,       // (event, player, item, bag, slot)
     *     PLAYER_EVENT_ON_FIRST_LOGIN             =     30,       // (event, player)
     *     PLAYER_EVENT_ON_CAN_USE_ITEM            =     31,       // (event, player, itemEntry) - Can return InventoryResult enum value
     *     PLAYER_EVENT_ON_LOOT_ITEM               =     32,       // (event, player, item, count)
     *     PLAYER_EVENT_ON_ENTER_COMBAT            =     33,       // (event, player, enemy)
     *     PLAYER_EVENT_ON_LEAVE_COMBAT            =     34,       // (event, player)
     *     PLAYER_EVENT_ON_REPOP                   =     35,       // (event, player)
     *     PLAYER_EVENT_ON_RESURRECT               =     36,       // (event, player)
     *     PLAYER_EVENT_ON_LOOT_MONEY              =     37,       // (event, player, amount)
     *     PLAYER_EVENT_ON_QUEST_ABANDON           =     38,       // (event, player, questId)
     *     PLAYER_EVENT_ON_LEARN_TALENTS           =     39,       // (event, player, talentId, talentRank, spellid)
     *     // UNUSED                               =     40,       // (event, player)
     *     // UNUSED                               =     41,       // (event, player)
     *     PLAYER_EVENT_ON_COMMAND                 =     42,       // (event, player, command, chatHandler) - player is nil if command used from console. Can return false
     *     PLAYER_EVENT_ON_PET_ADDED_TO_WORLD      =     43,       // (event, player, pet)
     *     PLAYER_EVENT_ON_LEARN_SPELL             =     44,       // (event, player, spellId)
     *     PLAYER_EVENT_ON_ACHIEVEMENT_COMPLETE    =     45,       // (event, player, achievement)
     *     PLAYER_EVENT_ON_FFAPVP_CHANGE           =     46,       // (event, player, hasFfaPvp)
     *     PLAYER_EVENT_ON_UPDATE_AREA             =     47,       // (event, player, oldArea, newArea)
     *     PLAYER_EVENT_ON_CAN_INIT_TRADE          =     48,       // (event, player, target) - Can return false to prevent the trade
     *     PLAYER_EVENT_ON_CAN_SEND_MAIL           =     49,       // (event, player, receiverGuid, mailbox, subject, body, money, cod, item) - Can return false to prevent sending the mail
     *     PLAYER_EVENT_ON_CAN_JOIN_LFG            =     50,       // (event, player, roles, dungeons, comment) - Can return false to prevent queueing
     *     PLAYER_EVENT_ON_QUEST_REWARD_ITEM       =     51,       //  (event, player, item, count)
     *     PLAYER_EVENT_ON_CREATE_ITEM             =     52,       //  (event, player, item, count)
     *     PLAYER_EVENT_ON_STORE_NEW_ITEM          =     53,       //  (event, player, item, count)
     *     PLAYER_EVENT_ON_COMPLETE_QUEST          =     54,       // (event, player, quest)
     *     PLAYER_EVENT_ON_CAN_GROUP_INVITE        =     55,       // (event, player, memberName) - Can return false to prevent inviting
     *     PLAYER_EVENT_ON_GROUP_ROLL_REWARD_ITEM  =     56,       // (event, player, item, count, voteType, roll)
     *     PLAYER_EVENT_ON_BG_DESERTION            =     57,       // (event, player, type)
     *     PLAYER_EVENT_ON_PET_KILL                =     58,       // (event, player, killer)
     *     PLAYER_EVENT_ON_CAN_RESURRECT           =     59,       // (event, player)
     *     PLAYER_EVENT_ON_CAN_UPDATE_SKILL        =     60,       // (event, player, skill_id) -- Can return true or false
     *     PLAYER_EVENT_ON_BEFORE_UPDATE_SKILL     =     61,       // (event, player, skill_id, value, max, step) -- Can return new amount
     *     PLAYER_EVENT_ON_UPDATE_SKILL            =     62,       // (event, player, skill_id, value, max, step, new_value)
     * };
     * </pre>
     *
     * @proto cancel = (event, function)
     * @proto cancel = (event, function, shots)
     *
     * @param uint32 event : [Player] event Id, refer to PlayerEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterPlayerEvent(lua_State* L)
    {
        return RegisterEventHelper(L, Hooks::REGTYPE_PLAYER);
    }

    /**
     * Registers a [Guild] event handler.
     *
     * <pre>
     * enum GuildEvents
     * {
     *     // Guild
     *     GUILD_EVENT_ON_ADD_MEMBER               =     1,       // (event, guild, player, rank)
     *     GUILD_EVENT_ON_REMOVE_MEMBER            =     2,       // (event, guild, player, isDisbanding)
     *     GUILD_EVENT_ON_MOTD_CHANGE              =     3,       // (event, guild, newMotd)
     *     GUILD_EVENT_ON_INFO_CHANGE              =     4,       // (event, guild, newInfo)
     *     GUILD_EVENT_ON_CREATE                   =     5,       // (event, guild, leader, name)  // Not on TC
     *     GUILD_EVENT_ON_DISBAND                  =     6,       // (event, guild)
     *     GUILD_EVENT_ON_MONEY_WITHDRAW           =     7,       // (event, guild, player, amount, isRepair) - Can return new money amount
     *     GUILD_EVENT_ON_MONEY_DEPOSIT            =     8,       // (event, guild, player, amount) - Can return new money amount
     *     GUILD_EVENT_ON_ITEM_MOVE                =     9,       // (event, guild, player, item, isSrcBank, srcContainer, srcSlotId, isDestBank, destContainer, destSlotId)   // TODO
     *     GUILD_EVENT_ON_EVENT                    =     10,      // (event, guild, eventType, plrGUIDLow1, plrGUIDLow2, newRank)  // TODO
     *     GUILD_EVENT_ON_BANK_EVENT               =     11,      // (event, guild, eventType, tabId, playerGUIDLow, itemOrMoney, itemStackCount, destTabId)
     *
     *     GUILD_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (event, function)
     * @proto cancel = (event, function, shots)
     *
     * @param uint32 event : [Guild] event Id, refer to GuildEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterGuildEvent(lua_State* L)
    {
        return RegisterEventHelper(L, Hooks::REGTYPE_GUILD);
    }

    /**
     * Registers a [Group] event handler.
     *
     * <pre>
     * enum GroupEvents
     * {
     *     // Group
     *     GROUP_EVENT_ON_MEMBER_ADD               =     1,       // (event, group, guid)
     *     GROUP_EVENT_ON_MEMBER_INVITE            =     2,       // (event, group, guid)
     *     GROUP_EVENT_ON_MEMBER_REMOVE            =     3,       // (event, group, guid, method, kicker, reason)
     *     GROUP_EVENT_ON_LEADER_CHANGE            =     4,       // (event, group, newLeaderGuid, oldLeaderGuid)
     *     GROUP_EVENT_ON_DISBAND                  =     5,       // (event, group)
     *     GROUP_EVENT_ON_CREATE                   =     6,       // (event, group, leaderGuid, groupType)
     *
     *     GROUP_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (event, function)
     * @proto cancel = (event, function, shots)
     *
     * @param uint32 event : [Group] event Id, refer to GroupEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterGroupEvent(lua_State* L)
    {
        return RegisterEventHelper(L, Hooks::REGTYPE_GROUP);
    }

    /**
     * Registers a [BattleGround] event handler.
     *
     * <pre>
     * enum BGEvents
     * {
     *     BG_EVENT_ON_START                               = 1,    // (event, bg, bgId, instanceId)
     *     BG_EVENT_ON_END                                 = 2,    // (event, bg, bgId, instanceId, winner)
     *     BG_EVENT_ON_CREATE                              = 3,    // (event, bg, bgId, instanceId)
     *     BG_EVENT_ON_PRE_DESTROY                         = 4,    // (event, bg, bgId, instanceId)
     *     BG_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (event, function)
     * @proto cancel = (event, function, shots)
     *
     * @param uint32 event : [BattleGround] event Id, refer to BGEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterBGEvent(lua_State* L)
    {
        return RegisterEventHelper(L, Hooks::REGTYPE_BG);
    }

    /**
     * Registers a [WorldPacket] event handler.
     *
     * <pre>
     * enum PacketEvents
     * {
     *     PACKET_EVENT_ON_PACKET_RECEIVE          =     5,       // (event, packet, player) - Player only if accessible. Can return false, newPacket
     *     PACKET_EVENT_ON_PACKET_RECEIVE_UNKNOWN  =     6,       // Not Implemented
     *     PACKET_EVENT_ON_PACKET_SEND             =     7,       // (event, packet, player) - Player only if accessible. Can return false, newPacket
     *
     *     PACKET_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : opcode
     * @param uint32 event : packet event Id, refer to PacketEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterPacketEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_PACKET);
    }

    /**
     * Registers a [Creature] gossip event handler.
     *
     * <pre>
     * enum GossipEvents
     * {
     *     GOSSIP_EVENT_ON_HELLO                           = 1,    // (event, player, object) - Object is the Creature/GameObject/Item. Can return false to do default action. For item gossip can return false to stop spell casting.
     *     GOSSIP_EVENT_ON_SELECT                          = 2,    // (event, player, object, sender, intid, code, menu_id) - Object is the Creature/GameObject/Item/Player, menu_id is only for player gossip. Can return false to do default action.
     *     GOSSIP_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : [Creature] entry Id
     * @param uint32 event : [Creature] gossip event Id, refer to GossipEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterCreatureGossipEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_CREATURE_GOSSIP);
    }

    /**
     * Registers a [GameObject] gossip event handler.
     *
     * <pre>
     * enum GossipEvents
     * {
     *     GOSSIP_EVENT_ON_HELLO                           = 1,    // (event, player, object) - Object is the Creature/GameObject/Item. Can return false to do default action. For item gossip can return false to stop spell casting.
     *     GOSSIP_EVENT_ON_SELECT                          = 2,    // (event, player, object, sender, intid, code, menu_id) - Object is the Creature/GameObject/Item/Player, menu_id is only for player gossip. Can return false to do default action.
     *     GOSSIP_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : [GameObject] entry Id
     * @param uint32 event : [GameObject] gossip event Id, refer to GossipEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterGameObjectGossipEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_GAMEOBJECT_GOSSIP);
    }

    /**
     * Registers an [Item] event handler.
     *
     * <pre>
     * enum ItemEvents
     * {
     *     ITEM_EVENT_ON_DUMMY_EFFECT                      = 1,    // (event, caster, spellid, effindex, item)
     *     ITEM_EVENT_ON_USE                               = 2,    // (event, player, item, target) - Can return false to stop the spell casting
     *     ITEM_EVENT_ON_QUEST_ACCEPT                      = 3,    // (event, player, item, quest) - Can return true
     *     ITEM_EVENT_ON_EXPIRE                            = 4,    // (event, player, itemid) - Can return true
     *     ITEM_EVENT_ON_REMOVE                            = 5,    // (event, player, item) - Can return true
     *     ITEM_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : [Item] entry Id
     * @param uint32 event : [Item] event Id, refer to ItemEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterItemEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_ITEM);
    }

    /**
     * Registers an [Item] gossip event handler.
     *
     * <pre>
     * enum GossipEvents
     * {
     *     GOSSIP_EVENT_ON_HELLO                           = 1,    // (event, player, object) - Object is the Creature/GameObject/Item. Can return false to do default action. For item gossip can return false to stop spell casting.
     *     GOSSIP_EVENT_ON_SELECT                          = 2,    // (event, player, object, sender, intid, code, menu_id) - Object is the Creature/GameObject/Item/Player, menu_id is only for player gossip. Can return false to do default action.
     *     GOSSIP_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : [Item] entry Id
     * @param uint32 event : [Item] gossip event Id, refer to GossipEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterItemGossipEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_ITEM_GOSSIP);
    }

    /**
     * Registers a [Map] event handler for all instance of a [Map].
     *
     * <pre>
     * enum InstanceEvents
     * {
     *     INSTANCE_EVENT_ON_INITIALIZE                    = 1,    // (event, instance_data, map)
     *     INSTANCE_EVENT_ON_LOAD                          = 2,    // (event, instance_data, map)
     *     INSTANCE_EVENT_ON_UPDATE                        = 3,    // (event, instance_data, map, diff)
     *     INSTANCE_EVENT_ON_PLAYER_ENTER                  = 4,    // (event, instance_data, map, player)
     *     INSTANCE_EVENT_ON_CREATURE_CREATE               = 5,    // (event, instance_data, map, creature)
     *     INSTANCE_EVENT_ON_GAMEOBJECT_CREATE             = 6,    // (event, instance_data, map, go)
     *     INSTANCE_EVENT_ON_CHECK_ENCOUNTER_IN_PROGRESS   = 7,    // (event, instance_data, map)
     *     INSTANCE_EVENT_COUNT
     * };
     * </pre>
     *
     * @param uint32 map_id : ID of a [Map]
     * @param uint32 event : [Map] event ID, refer to MapEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     */
    int RegisterMapEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_MAP);
    }

    /**
     * Registers a [Map] event handler for one instance of a [Map].
     *
     * <pre>
     * enum InstanceEvents
     * {
     *     INSTANCE_EVENT_ON_INITIALIZE                    = 1,    // (event, instance_data, map)
     *     INSTANCE_EVENT_ON_LOAD                          = 2,    // (event, instance_data, map)
     *     INSTANCE_EVENT_ON_UPDATE                        = 3,    // (event, instance_data, map, diff)
     *     INSTANCE_EVENT_ON_PLAYER_ENTER                  = 4,    // (event, instance_data, map, player)
     *     INSTANCE_EVENT_ON_CREATURE_CREATE               = 5,    // (event, instance_data, map, creature)
     *     INSTANCE_EVENT_ON_GAMEOBJECT_CREATE             = 6,    // (event, instance_data, map, go)
     *     INSTANCE_EVENT_ON_CHECK_ENCOUNTER_IN_PROGRESS   = 7,    // (event, instance_data, map)
     *     INSTANCE_EVENT_COUNT
     * };
     * </pre>
     *
     * @param uint32 instance_id : ID of an instance of a [Map]
     * @param uint32 event : [Map] event ID, refer to MapEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     */
    int RegisterInstanceEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_INSTANCE);
    }

    /**
     * Registers a [Player] gossip event handler.
     *
     * Note that you can not use `GOSSIP_EVENT_ON_HELLO` with this hook. It does nothing since players dont have an "on hello".
     *
     * <pre>
     * enum GossipEvents
     * {
     *     GOSSIP_EVENT_ON_HELLO                           = 1,    // (event, player, object) - Object is the Creature/GameObject/Item. Can return false to do default action. For item gossip can return false to stop spell casting.
     *     GOSSIP_EVENT_ON_SELECT                          = 2,    // (event, player, object, sender, intid, code, menu_id) - Object is the Creature/GameObject/Item/Player, menu_id is only for player gossip. Can return false to do default action.
     *     GOSSIP_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (menu_id, event, function)
     * @proto cancel = (menu_id, event, function, shots)
     *
     * @param uint32 menu_id : [Player] gossip menu Id
     * @param uint32 event : [Player] gossip event Id, refer to GossipEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterPlayerGossipEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_PLAYER_GOSSIP);
    }

    /**
     * Registers a [Creature] event handler.
     *
     * <pre>
     * enum CreatureEvents
     * {
     *     CREATURE_EVENT_ON_ENTER_COMBAT                    = 1,  // (event, creature, target) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_LEAVE_COMBAT                    = 2,  // (event, creature) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_TARGET_DIED                     = 3,  // (event, creature, victim) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_DIED                            = 4,  // (event, creature, killer) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SPAWN                           = 5,  // (event, creature) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_REACH_WP                        = 6,  // (event, creature, type, id) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_AIUPDATE                        = 7,  // (event, creature, diff) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_RECEIVE_EMOTE                   = 8,  // (event, creature, player, emoteid) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_DAMAGE_TAKEN                    = 9,  // (event, creature, attacker, damage) - Can return true to stop normal action, can return new damage as second return value.
     *     CREATURE_EVENT_ON_PRE_COMBAT                      = 10, // (event, creature, target) - Can return true to stop normal action
     *     // UNUSED
     *     CREATURE_EVENT_ON_OWNER_ATTACKED                  = 12, // (event, creature, target) - Can return true to stop normal action            // Not on mangos
     *     CREATURE_EVENT_ON_OWNER_ATTACKED_AT               = 13, // (event, creature, attacker) - Can return true to stop normal action          // Not on mangos
     *     CREATURE_EVENT_ON_HIT_BY_SPELL                    = 14, // (event, creature, caster, spellid) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SPELL_HIT_TARGET                = 15, // (event, creature, target, spellid) - Can return true to stop normal action
     *     // UNUSED                                         = 16, // (event, creature)
     *     // UNUSED                                         = 17, // (event, creature)
     *     // UNUSED                                         = 18, // (event, creature)
     *     CREATURE_EVENT_ON_JUST_SUMMONED_CREATURE          = 19, // (event, creature, summon) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SUMMONED_CREATURE_DESPAWN       = 20, // (event, creature, summon) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SUMMONED_CREATURE_DIED          = 21, // (event, creature, summon, killer) - Can return true to stop normal action    // Not on mangos
     *     CREATURE_EVENT_ON_SUMMONED                        = 22, // (event, creature, summoner) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_RESET                           = 23, // (event, creature)
     *     CREATURE_EVENT_ON_REACH_HOME                      = 24, // (event, creature) - Can return true to stop normal action
     *     // UNUSED                                         = 25, // (event, creature)
     *     CREATURE_EVENT_ON_CORPSE_REMOVED                  = 26, // (event, creature, respawndelay) - Can return true to stop normal action, can return new respawndelay as second return value
     *     CREATURE_EVENT_ON_MOVE_IN_LOS                     = 27, // (event, creature, unit) - Can return true to stop normal action. Does not actually check LOS, just uses the sight range
     *     // UNUSED                                         = 28, // (event, creature)
     *     // UNUSED                                         = 29, // (event, creature)
     *     CREATURE_EVENT_ON_DUMMY_EFFECT                    = 30, // (event, caster, spellid, effindex, creature)
     *     CREATURE_EVENT_ON_QUEST_ACCEPT                    = 31, // (event, player, creature, quest) - Can return true
     *     // UNUSED                                         = 32, // (event, creature)
     *     // UNUSED                                         = 33, // (event, creature)
     *     CREATURE_EVENT_ON_QUEST_REWARD                    = 34, // (event, player, creature, quest, opt) - Can return true
     *     CREATURE_EVENT_ON_DIALOG_STATUS                   = 35, // (event, player, creature)
     *     CREATURE_EVENT_ON_ADD                             = 36, // (event, creature)
     *     CREATURE_EVENT_ON_REMOVE                          = 37, // (event, creature)
     *     CREATURE_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : the ID of one or more [Creature]s
     * @param uint32 event : refer to CreatureEvents above
     * @param function function : function that will be called when the event occurs
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterCreatureEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_CREATURE);
    }

    /**
     * Registers a [Creature] event handler for a *single* [Creature].
     *
     * <pre>
     * enum CreatureEvents
     * {
     *     CREATURE_EVENT_ON_ENTER_COMBAT                    = 1,  // (event, creature, target) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_LEAVE_COMBAT                    = 2,  // (event, creature) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_TARGET_DIED                     = 3,  // (event, creature, victim) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_DIED                            = 4,  // (event, creature, killer) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SPAWN                           = 5,  // (event, creature) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_REACH_WP                        = 6,  // (event, creature, type, id) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_AIUPDATE                        = 7,  // (event, creature, diff) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_RECEIVE_EMOTE                   = 8,  // (event, creature, player, emoteid) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_DAMAGE_TAKEN                    = 9,  // (event, creature, attacker, damage) - Can return true to stop normal action, can return new damage as second return value.
     *     CREATURE_EVENT_ON_PRE_COMBAT                      = 10, // (event, creature, target) - Can return true to stop normal action
     *     // UNUSED
     *     CREATURE_EVENT_ON_OWNER_ATTACKED                  = 12, // (event, creature, target) - Can return true to stop normal action            // Not on mangos
     *     CREATURE_EVENT_ON_OWNER_ATTACKED_AT               = 13, // (event, creature, attacker) - Can return true to stop normal action          // Not on mangos
     *     CREATURE_EVENT_ON_HIT_BY_SPELL                    = 14, // (event, creature, caster, spellid) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SPELL_HIT_TARGET                = 15, // (event, creature, target, spellid) - Can return true to stop normal action
     *     // UNUSED                                         = 16, // (event, creature)
     *     // UNUSED                                         = 17, // (event, creature)
     *     // UNUSED                                         = 18, // (event, creature)
     *     CREATURE_EVENT_ON_JUST_SUMMONED_CREATURE          = 19, // (event, creature, summon) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SUMMONED_CREATURE_DESPAWN       = 20, // (event, creature, summon) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_SUMMONED_CREATURE_DIED          = 21, // (event, creature, summon, killer) - Can return true to stop normal action    // Not on mangos
     *     CREATURE_EVENT_ON_SUMMONED                        = 22, // (event, creature, summoner) - Can return true to stop normal action
     *     CREATURE_EVENT_ON_RESET                           = 23, // (event, creature)
     *     CREATURE_EVENT_ON_REACH_HOME                      = 24, // (event, creature) - Can return true to stop normal action
     *     // UNUSED                                         = 25, // (event, creature)
     *     CREATURE_EVENT_ON_CORPSE_REMOVED                  = 26, // (event, creature, respawndelay) - Can return true to stop normal action, can return new respawndelay as second return value
     *     CREATURE_EVENT_ON_MOVE_IN_LOS                     = 27, // (event, creature, unit) - Can return true to stop normal action. Does not actually check LOS, just uses the sight range
     *     // UNUSED                                         = 28, // (event, creature)
     *     // UNUSED                                         = 29, // (event, creature)
     *     CREATURE_EVENT_ON_DUMMY_EFFECT                    = 30, // (event, caster, spellid, effindex, creature)
     *     CREATURE_EVENT_ON_QUEST_ACCEPT                    = 31, // (event, player, creature, quest) - Can return true
     *     // UNUSED                                         = 32, // (event, creature)
     *     // UNUSED                                         = 33, // (event, creature)
     *     CREATURE_EVENT_ON_QUEST_REWARD                    = 34, // (event, player, creature, quest, opt) - Can return true
     *     CREATURE_EVENT_ON_DIALOG_STATUS                   = 35, // (event, player, creature)
     *     CREATURE_EVENT_ON_ADD                             = 36, // (event, creature)
     *     CREATURE_EVENT_ON_REMOVE                          = 37, // (event, creature)
     *     CREATURE_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (guid, instance_id, event, function)
     * @proto cancel = (guid, instance_id, event, function, shots)
     *
     * @param ObjectGuid guid : the GUID of a single [Creature]
     * @param uint32 instance_id : the instance ID of a single [Creature]
     * @param uint32 event : refer to CreatureEvents above
     * @param function function : function that will be called when the event occurs
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterUniqueCreatureEvent(lua_State* L)
    {
        return RegisterUniqueHelper(L, Hooks::REGTYPE_CREATURE);
    }

    /**
     * Registers a [GameObject] event handler.
     *
     * <pre>
     * enum GameObjectEvents
     * {
     *     GAMEOBJECT_EVENT_ON_AIUPDATE                    = 1,    // (event, go, diff)
     *     GAMEOBJECT_EVENT_ON_SPAWN                       = 2,    // (event, go)
     *     GAMEOBJECT_EVENT_ON_DUMMY_EFFECT                = 3,    // (event, caster, spellid, effindex, go) - Can return true to stop normal action
     *     GAMEOBJECT_EVENT_ON_QUEST_ACCEPT                = 4,    // (event, player, go, quest) - Can return true to stop normal action
     *     GAMEOBJECT_EVENT_ON_QUEST_REWARD                = 5,    // (event, player, go, quest, opt) - Can return true to stop normal action
     *     GAMEOBJECT_EVENT_ON_DIALOG_STATUS               = 6,    // (event, player, go)
     *     GAMEOBJECT_EVENT_ON_DESTROYED                   = 7,    // (event, go, attacker)
     *     GAMEOBJECT_EVENT_ON_DAMAGED                     = 8,    // (event, go, attacker)
     *     GAMEOBJECT_EVENT_ON_LOOT_STATE_CHANGE           = 9,    // (event, go, state)
     *     GAMEOBJECT_EVENT_ON_GO_STATE_CHANGED            = 10,   // (event, go, state)
     *     // UNUSED                                       = 11,   // (event, gameobject)
     *     GAMEOBJECT_EVENT_ON_ADD                         = 12,   // (event, gameobject)
     *     GAMEOBJECT_EVENT_ON_REMOVE                      = 13,   // (event, gameobject)
     *     GAMEOBJECT_EVENT_ON_USE                         = 14,   // (event, go, player) - Can return true to stop normal action
     *     GAMEOBJECT_EVENT_COUNT
     * };
     * </pre>
     *
     * @proto cancel = (entry, event, function)
     * @proto cancel = (entry, event, function, shots)
     *
     * @param uint32 entry : [GameObject] entry Id
     * @param uint32 event : [GameObject] event Id, refer to GameObjectEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     *
     * @return function cancel : a function that cancels the binding when called
     */
    int RegisterGameObjectEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_GAMEOBJECT);
    }

    /**
     * Registers a [Ticket] event handler.
     *
     * <pre>
     * enum TicketEvents
     * {
     *     TICKET_EVENT_ON_CREATE                          = 1,    // (event, player, ticket)
     *     TICKET_EVENT_ON_UPDATE                          = 2,    // (event, player, ticket, message)
     *     TICKET_EVENT_ON_CLOSE                           = 3,    // (event, player, ticket)
     *     TICKET_EVENT_STATUS_UPDATE                      = 4,    // (event, player, ticket)
     *     TICKET_EVENT_ON_RESOLVE                         = 5,    // (event, player, ticket)
     *     TICKET_EVENT_COUNT
     * };
     * </pre>
     *
     * @param uint32 event : event ID, refer to UnitEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     */
    int RegisterTicketEvent(lua_State* L)
    {
        return RegisterEventHelper(L, Hooks::REGTYPE_TICKET);
    }

    /**
     * Registers a [Spell] event handler.
     *
     * <pre>
     * enum SpellEvents
     * {
     *     SPELL_EVENT_ON_PREPARE                          = 1, // (event, caster, spell)
     *     SPELL_EVENT_ON_CAST                             = 2, // (event, caster, spell, skipCheck)
     *     SPELL_EVENT_ON_CAST_CANCEL                      = 3, // (event, caster, spell, bySelf)
     *     SPELL_EVENT_COUNT
     * };
     * </pre>
     *
     * @param uint32 entry : [Spell] entry Id
     * @param uint32 event : event ID, refer to SpellEvents above
     * @param function function : function to register
     * @param uint32 shots = 0 : the number of times the function will be called, 0 means "always call this function"
     */
    int RegisterSpellEvent(lua_State* L)
    {
        return RegisterEntryHelper(L, Hooks::REGTYPE_SPELL);
    }

    /**
     * Reloads the Lua engine.
     */
    int ReloadEluna(lua_State* /*L*/)
    {
        Eluna::ReloadEluna();
        return 0;
    }

    /**
     * Runs a command.
     *
     * @param string command : the command to run
     */
    int RunCommand(lua_State* L)
    {
        const char* command = Eluna::CHECKVAL<const char*>(L, 1);

        eWorld->QueueCliCommand(new CliCommandHolder(nullptr, command, [](void*, std::string_view view)
        {
            std::string str = { view.begin(), view.end() };
            str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), str.end()); // Remove trailing spaces and line breaks
            ELUNA_LOG_INFO("{}", str);
        }, nullptr));

        return 0;
    }

    /**
     * Sends a message to all [Player]s online.
     *
     * @param string message : message to send
     */
    int SendWorldMessage(lua_State* L)
    {
        const char* message = Eluna::CHECKVAL<const char*>(L, 1);
        eWorldSessionMgr->SendServerMessage(SERVER_MSG_STRING, message);
        return 0;
    }

    template <typename T>
    static int DBQueryAsync(lua_State* L, DatabaseWorkerPool<T>& db)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        lua_pushvalue(L, 2);
        int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (funcRef == LUA_REFNIL || funcRef == LUA_NOREF)
        {
            luaL_argerror(L, 2, "unable to make a ref to function");
            return 0;
        }

        Eluna::GEluna->queryProcessor.AddCallback(db.AsyncQuery(query).WithCallback([L, funcRef](QueryResult result)
            {
                ElunaQuery* eq = result ? new ElunaQuery(result) : nullptr;

                LOCK_ELUNA;

                // Get function
                lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);

                // Push parameters
                Eluna::Push(L, eq);

                // Call function
                Eluna::GEluna->ExecuteCall(1, 0);

                luaL_unref(L, LUA_REGISTRYINDEX, funcRef);
            }));

        return 0;
    }

    /**
     * Executes a SQL query on the world database and returns an [ElunaQuery].
     *
     * The query is always executed synchronously
     *   (i.e. execution halts until the query has finished and then results are returned).
     * If you need to execute the query asynchronously, use [Global:WorldDBQueryAsync] instead.
     *
     *     local Q = WorldDBQuery("SELECT entry, name FROM creature_template LIMIT 10")
     *     if Q then
     *         repeat
     *             local entry, name = Q:GetUInt32(0), Q:GetString(1)
     *             print(entry, name)
     *         until not Q:NextRow()
     *     end
     *
     * @param string sql : query to execute
     * @return [ElunaQuery] results or nil if no rows found or nil if no rows found
     */
    int WorldDBQuery(lua_State* L)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);

        int numArgs = lua_gettop(L);
        if (numArgs > 1)
            query = Eluna::FormatQuery(L, query).c_str();

        ElunaQuery result = WorldDatabase.Query(query);
        if (result)
            Eluna::Push(L, new ElunaQuery(result));
        else
            Eluna::Push(L);
        return 1;
    }

    /**
     * Executes an asynchronous SQL query on the world database and passes an [ElunaQuery] to a callback function.
     *
     * The query is executed asynchronously
     *   (i.e. the server keeps running while the query is executed in parallel, and results are passed to a callback function).
     * If you need to execute the query synchronously, use [Global:WorldDBQuery] instead.
     *
     *     WorldDBQueryAsync("SELECT entry, name FROM creature_template LIMIT 10", function(Q)
     *         if Q then
     *             repeat
     *                 local entry, name = Q:GetUInt32(0), Q:GetString(1)
     *                 print(entry, name)
     *             until not Q:NextRow()
     *         end
     *     end)
     *
     * @param string sql : query to execute
     * @param function callback : function that will be called when the results are available
     */
    int WorldDBQueryAsync(lua_State* L)
    {
        return DBQueryAsync(L, WorldDatabase);
    }

    /**
     * Executes a SQL query on the world database.
     *
     * The query may be executed *asynchronously* (at a later, unpredictable time).
     * If you need to execute the query synchronously, use [Global:WorldDBQuery] instead.
     *
     * Any results produced are ignored.
     * If you need results from the query, use [Global:WorldDBQuery] or [Global:WorldDBQueryAsync] instead.
     *
     *     WorldDBExecute("DELETE FROM my_table")
     *
     * @param string sql : query to execute
     */
    int WorldDBExecute(lua_State* L)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);

        int numArgs = lua_gettop(L);
        if (numArgs > 1)
            query = Eluna::FormatQuery(L, query).c_str();

        WorldDatabase.Execute(query);
        return 0;
    }

    /**
     * Executes a SQL query on the character database and returns an [ElunaQuery].
     *
     * The query is always executed synchronously
     *   (i.e. execution halts until the query has finished and then results are returned).
     * If you need to execute the query asynchronously, use [Global:CharDBQueryAsync] instead.
     *
     * For an example see [Global:WorldDBQuery].
     *
     * @param string sql : query to execute
     * @return [ElunaQuery] results or nil if no rows found
     */
    int CharDBQuery(lua_State* L)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);

        int numArgs = lua_gettop(L);
        if (numArgs > 1)
            query = Eluna::FormatQuery(L, query).c_str();

        QueryResult result = CharacterDatabase.Query(query);
        if (result)
            Eluna::Push(L, new QueryResult(result));
        else
            Eluna::Push(L);
        return 1;
    }

    /**
     * Executes an asynchronous SQL query on the character database and passes an [ElunaQuery] to a callback function.
     *
     * The query is executed asynchronously
     *   (i.e. the server keeps running while the query is executed in parallel, and results are passed to a callback function).
     * If you need to execute the query synchronously, use [Global:CharDBQuery] instead.
     *
     * For an example see [Global:WorldDBQueryAsync].
     *
     * @param string sql : query to execute
     * @param function callback : function that will be called when the results are available
     */
    int CharDBQueryAsync(lua_State* L)
    {
        return DBQueryAsync(L, CharacterDatabase);
    }

    /**
     * Executes a SQL query on the character database.
     *
     * The query may be executed *asynchronously* (at a later, unpredictable time).
     * If you need to execute the query synchronously, use [Global:CharDBQuery] instead.
     *
     * Any results produced are ignored.
     * If you need results from the query, use [Global:CharDBQuery] or [Global:CharDBQueryAsync] instead.
     *
     *     CharDBExecute("DELETE FROM my_table")
     *
     * @param string sql : query to execute
     */
    int CharDBExecute(lua_State* L)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);

        int numArgs = lua_gettop(L);
        if (numArgs > 1)
            query = Eluna::FormatQuery(L, query).c_str();

        CharacterDatabase.Execute(query);
        return 0;
    }

    /**
     * Executes a SQL query on the login database and returns an [ElunaQuery].
     *
     * The query is always executed synchronously
     *   (i.e. execution halts until the query has finished and then results are returned).
     * If you need to execute the query asynchronously, use [Global:AuthDBQueryAsync] instead.
     *
     * For an example see [Global:WorldDBQuery].
     *
     * @param string sql : query to execute
     * @return [ElunaQuery] results or nil if no rows found
     */
    int AuthDBQuery(lua_State* L)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);

        int numArgs = lua_gettop(L);
        if (numArgs > 1)
            query = Eluna::FormatQuery(L, query).c_str();

        QueryResult result = LoginDatabase.Query(query);
        if (result)
            Eluna::Push(L, new QueryResult(result));
        else
            Eluna::Push(L);
        return 1;
    }

    /**
     * Executes an asynchronous SQL query on the character database and passes an [ElunaQuery] to a callback function.
     *
     * The query is executed asynchronously
     *   (i.e. the server keeps running while the query is executed in parallel, and results are passed to a callback function).
     * If you need to execute the query synchronously, use [Global:AuthDBQuery] instead.
     *
     * For an example see [Global:WorldDBQueryAsync].
     *
     * @param string sql : query to execute
     * @param function callback : function that will be called when the results are available
     */
    int AuthDBQueryAsync(lua_State* L)
    {
        return DBQueryAsync(L, LoginDatabase);
    }

    /**
     * Executes a SQL query on the login database.
     *
     * The query may be executed *asynchronously* (at a later, unpredictable time).
     * If you need to execute the query synchronously, use [Global:AuthDBQuery] instead.
     *
     * Any results produced are ignored.
     * If you need results from the query, use [Global:AuthDBQuery] or [Global:AuthDBQueryAsync] instead.
     *
     *     AuthDBExecute("DELETE FROM my_table")
     *
     * @param string sql : query to execute
     */
    int AuthDBExecute(lua_State* L)
    {
        const char* query = Eluna::CHECKVAL<const char*>(L, 1);

        int numArgs = lua_gettop(L);
        if (numArgs > 1)
            query = Eluna::FormatQuery(L, query).c_str();
            
        LoginDatabase.Execute(query);
        return 0;
    }

    /**
     * Registers a global timed event.
     *
     * When the passed function is called, the parameters `(eventId, delay, repeats)` are passed to it.
     *
     * Repeats will decrease on each call if the event does not repeat indefinitely
     *
     * @proto eventId = (function, delay)
     * @proto eventId = (function, delaytable)
     * @proto eventId = (function, delay, repeats)
     * @proto eventId = (function, delaytable, repeats)
     *
     * @param function function : function to trigger when the time has passed
     * @param uint32 delay : set time in milliseconds for the event to trigger
     * @param table delaytable : a table `{min, max}` containing the minimum and maximum delay time
     * @param uint32 repeats = 1 : how many times for the event to repeat, 0 is infinite
     * @return int eventId : unique ID for the timed event used to cancel it or nil
     */
    int CreateLuaEvent(lua_State* L)
    {
        luaL_checktype(L, 1, LUA_TFUNCTION);
        uint32 min, max;
        if (lua_istable(L, 2))
        {
            Eluna::Push(L, 1);
            lua_gettable(L, 2);
            min = Eluna::CHECKVAL<uint32>(L, -1);
            Eluna::Push(L, 2);
            lua_gettable(L, 2);
            max = Eluna::CHECKVAL<uint32>(L, -1);
            lua_pop(L, 2);
        }
        else
            min = max = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 repeats = Eluna::CHECKVAL<uint32>(L, 3, 1);

        if (min > max)
            return luaL_argerror(L, 2, "min is bigger than max delay");

        lua_pushvalue(L, 1);
        int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (functionRef != LUA_REFNIL && functionRef != LUA_NOREF)
        {
            Eluna::GetEluna(L)->eventMgr->globalProcessor->AddEvent(functionRef, min, max, repeats);
            Eluna::Push(L, functionRef);
        }
        return 1;
    }

    /**
     * Removes a global timed event specified by ID.
     *
     * @param int eventId : event Id to remove
     * @param bool all_Events = false : remove from all events, not just global
     */
    int RemoveEventById(lua_State* L)
    {
        int eventId = Eluna::CHECKVAL<int>(L, 1);
        bool all_Events = Eluna::CHECKVAL<bool>(L, 1, false);

        // not thread safe
        if (all_Events)
            Eluna::GetEluna(L)->eventMgr->SetState(eventId, LUAEVENT_STATE_ABORT);
        else
            Eluna::GetEluna(L)->eventMgr->globalProcessor->SetState(eventId, LUAEVENT_STATE_ABORT);
        return 0;
    }

    /**
     * Removes all global timed events.
     *
     * @param bool all_Events = false : remove all events, not just global
     */
    int RemoveEvents(lua_State* L)
    {
        bool all_Events = Eluna::CHECKVAL<bool>(L, 1, false);

        // not thread safe
        if (all_Events)
            Eluna::GetEluna(L)->eventMgr->SetStates(LUAEVENT_STATE_ABORT);
        else
            Eluna::GetEluna(L)->eventMgr->globalProcessor->SetStates(LUAEVENT_STATE_ABORT);
        return 0;
    }

    /**
     * Performs an in-game spawn and returns the [Creature] or [GameObject] spawned.
     *
     * @param int32 spawnType : type of object to spawn, 1 = [Creature], 2 = [GameObject]
     * @param uint32 entry : entry ID of the [Creature] or [GameObject]
     * @param uint32 mapId : map ID to spawn the [Creature] or [GameObject] in
     * @param uint32 instanceId : instance ID to put the [Creature] or [GameObject] in. Non instance is 0
     * @param float x : x coordinate of the [Creature] or [GameObject]
     * @param float y : y coordinate of the [Creature] or [GameObject]
     * @param float z : z coordinate of the [Creature] or [GameObject]
     * @param float o : o facing/orientation of the [Creature] or [GameObject]
     * @param bool save = false : optional to save the [Creature] or [GameObject] to the database
     * @param uint32 durorresptime = 0 : despawn time of the [Creature] if it's not saved or respawn time of [GameObject]
     * @param uint32 phase = 1 : phase to put the [Creature] or [GameObject] in
     * @return [WorldObject] worldObject : returns [Creature] or [GameObject]
     */
    int PerformIngameSpawn(lua_State* L)
    {
        int spawntype = Eluna::CHECKVAL<int>(L, 1);
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 mapID = Eluna::CHECKVAL<uint32>(L, 3);
        uint32 instanceID = Eluna::CHECKVAL<uint32>(L, 4);

        float x = Eluna::CHECKVAL<float>(L, 5);
        float y = Eluna::CHECKVAL<float>(L, 6);
        float z = Eluna::CHECKVAL<float>(L, 7);
        float o = Eluna::CHECKVAL<float>(L, 8);
        bool save = Eluna::CHECKVAL<bool>(L, 9, false);
        uint32 durorresptime = Eluna::CHECKVAL<uint32>(L, 10, 0);
        uint32 phase = Eluna::CHECKVAL<uint32>(L, 11, PHASEMASK_NORMAL);
        
        if (!phase)
        {
            Eluna::Push(L);
            return 1;
        }

        Map* map = eMapMgr->FindMap(mapID, instanceID);
        if (!map)
        {
            Eluna::Push(L);
            return 1;
        }

        Position pos = { x, y, z, o };

        if (spawntype == 1) // spawn creature
        {
            if (save)
            {
                Creature* creature = new Creature();
                if (!creature->Create(map->GenerateLowGuid<HighGuid::Unit>(), map, phase, entry, 0, x, y, z, o))
                {
                    delete creature;
                    Eluna::Push(L);
                    return 1;
                }

                creature->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), phase);

                uint32 db_guid = creature->GetSpawnId();

                // To call _LoadGoods(); _LoadQuests(); CreateTrainerSpells()
                // current "creature" variable is deleted and created fresh new, otherwise old values might trigger asserts or cause undefined behavior
                creature->CleanupsBeforeDelete();
                delete creature;
                creature = new Creature();

                if (!creature->LoadCreatureFromDB(db_guid, map, true, true))
                {
                    delete creature;
                    Eluna::Push(L);
                    return 1;
                }

                eObjectMgr->AddCreatureToGrid(db_guid, eObjectMgr->GetCreatureData(db_guid));
                Eluna::Push(L, creature);
            }
            else
            {
                TempSummon* creature = map->SummonCreature(entry, pos, NULL, durorresptime);
                if (!creature)
                {
                    Eluna::Push(L);
                    return 1;
                }

                if (durorresptime)
                    creature->SetTempSummonType(TEMPSUMMON_TIMED_OR_DEAD_DESPAWN);
                else
                    creature->SetTempSummonType(TEMPSUMMON_MANUAL_DESPAWN);

                Eluna::Push(L, creature);
            }

            return 1;
        }

        if (spawntype == 2) // Spawn object
        {
            const GameObjectTemplate* objectInfo = eObjectMgr->GetGameObjectTemplate(entry);
            if (!objectInfo)
            {
                Eluna::Push(L);
                return 1;
            }

            if (objectInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(objectInfo->displayId))
            {
                Eluna::Push(L);
                return 1;
            }

            GameObject* object = new GameObject;
            uint32 guidLow = map->GenerateLowGuid<HighGuid::GameObject>();

            if (!object->Create(guidLow, entry, map, phase, x, y, z, o, G3D::Quat(0.0f, 0.0f, 0.0f, 0.0f), 100, GO_STATE_READY))
            {
                delete object;
                Eluna::Push(L);
                return 1;
            }

            if (durorresptime)
                object->SetRespawnTime(durorresptime);

            if (save)
            {
                // fill the gameobject data and save to the db
                object->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()), phase);
                guidLow = object->GetSpawnId();

                // delete the old object and do a clean load from DB with a fresh new GameObject instance.
                // this is required to avoid weird behavior and memory leaks
                delete object;

                object = new GameObject();
                // this will generate a new lowguid if the object is in an instance
                if (!object->LoadGameObjectFromDB(guidLow, map, true))
                {
                    delete object;
                    Eluna::Push(L);
                    return 1;
                }

                eObjectMgr->AddGameobjectToGrid(guidLow, eObjectMgr->GetGameObjectData(guidLow));
            }
            else
                map->AddToMap(object);
            Eluna::Push(L, object);
            return 1;
        }
        Eluna::Push(L);
        return 1;
    }

    /**
     * Creates a [WorldPacket].
     *
     * @param [Opcodes] opcode : the opcode of the packet
     * @param uint32 size : the size of the packet
     * @return [WorldPacket] packet
     */
    int CreatePacket(lua_State* L)
    {
        uint32 opcode = Eluna::CHECKVAL<uint32>(L, 1);
        size_t size = Eluna::CHECKVAL<size_t>(L, 2);
        if (opcode >= NUM_MSG_TYPES)
            return luaL_argerror(L, 1, "valid opcode expected");

        Eluna::Push(L, new WorldPacket((OpcodesList)opcode, size));
        return 1;
    }

    /**
     * Adds an [Item] to a vendor and updates the world database.
     *
     * @param uint32 entry : [Creature] entry Id
     * @param uint32 item : [Item] entry Id
     * @param int32 maxcount : max [Item] stack count
     * @param uint32 incrtime : combined with maxcount, incrtime tells how often (in seconds) the vendor list is refreshed and the limited [Item] copies are restocked
     * @param uint32 extendedcost : unique cost of an [Item], such as conquest points for example
     */
    int AddVendorItem(lua_State* L)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 item = Eluna::CHECKVAL<uint32>(L, 2);
        int maxcount = Eluna::CHECKVAL<int>(L, 3);
        uint32 incrtime = Eluna::CHECKVAL<uint32>(L, 4);
        uint32 extendedcost = Eluna::CHECKVAL<uint32>(L, 5);

        if (!eObjectMgr->IsVendorItemValid(entry, item, maxcount, incrtime, extendedcost))
            return 0;
        eObjectMgr->AddVendorItem(entry, item, maxcount, incrtime, extendedcost);

        return 0;
    }

    /**
     * Removes an [Item] from a vendor and updates the database.
     *
     * @param uint32 entry : [Creature] entry Id
     * @param uint32 item : [Item] entry Id
     */
    int VendorRemoveItem(lua_State* L)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 item = Eluna::CHECKVAL<uint32>(L, 2);
        if (!eObjectMgr->GetCreatureTemplate(entry))
            return luaL_argerror(L, 1, "valid CreatureEntry expected");

        eObjectMgr->RemoveVendorItem(entry, item);
        return 0;
    }

    /**
     * Removes all [Item]s from a vendor and updates the database.
     *
     * @param uint32 entry : [Creature] entry Id
     */
    int VendorRemoveAllItems(lua_State* L)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

        VendorItemData const* items = eObjectMgr->GetNpcVendorItemList(entry);
        if (!items || items->Empty())
            return 0;

        auto const& itemlist = items->m_items;
        for (auto itr = itemlist.rbegin(); itr != itemlist.rend(); ++itr)
            eObjectMgr->RemoveVendorItem(entry, (*itr)->item);
        return 0;
    }

    /**
     * Kicks a [Player] from the server.
     *
     * @param [Player] player : [Player] to kick
     */
    int Kick(lua_State* L)
    {
        Player* player = Eluna::CHECKOBJ<Player>(L, 1);
        player->GetSession()->KickPlayer();
        return 0;
    }

    /**
     * Ban's a [Player]'s account, character or IP
     *
     *     enum BanMode
     *     {
     *         BAN_ACCOUNT = 0,
     *         BAN_CHARACTER = 1,
     *         BAN_IP = 2
     *     };
     *
     * @param [BanMode] banMode : method of ban, refer to BanMode above
     * @param string nameOrIP : If BanMode is 0 then accountname, if 1 then charactername if 2 then ip
     * @param uint32 duration : duration (in seconds) of the ban
     * @param string reason = "" : ban reason, this is optional
     * @param string whoBanned = "" : the [Player]'s name that banned the account, character or IP, this is optional
     * @return int result : status of the ban. 0 if success, 1 if syntax error, 2 if target not found, 3 if a longer ban already exists, nil if unknown result
     */
    int Ban(lua_State* L)
    {
        int banMode = Eluna::CHECKVAL<int>(L, 1);
        std::string nameOrIP = Eluna::CHECKVAL<std::string>(L, 2);
        uint32 duration = Eluna::CHECKVAL<uint32>(L, 3);
        const char* reason = Eluna::CHECKVAL<const char*>(L, 4, "");
        const char* whoBanned = Eluna::CHECKVAL<const char*>(L, 5, "");

        const int BAN_ACCOUNT = 0;
        const int BAN_CHARACTER = 1;
        const int BAN_IP = 2;

        switch (banMode)
        {
            case BAN_ACCOUNT:
                if (!Utf8ToUpperOnlyLatin(nameOrIP))
                    return luaL_argerror(L, 2, "invalid account name");
                break;
            case BAN_CHARACTER:
                if (!normalizePlayerName(nameOrIP))
                    return luaL_argerror(L, 2, "invalid character name");
                break;
            case BAN_IP:
                if (!IsIPAddress(nameOrIP.c_str()))
                    return luaL_argerror(L, 2, "invalid ip");
                break;
            default:
                return luaL_argerror(L, 1, "unknown banmode");
        }

        BanReturn result;
        switch (banMode)
        {
            case BAN_ACCOUNT:
                result = sBan->BanAccount(nameOrIP, std::to_string(duration) + "s", reason, whoBanned);
            break;
            case BAN_CHARACTER:
                result = sBan->BanCharacter(nameOrIP, std::to_string(duration) + "s", reason, whoBanned);
            break;
            case BAN_IP:
                result = sBan->BanIP(nameOrIP, std::to_string(duration) + "s", reason, whoBanned);
            break;
        }

        switch (result)
        {
        case BanReturn::BAN_SUCCESS:
            Eluna::Push(L, 0);
            break;
        case BanReturn::BAN_SYNTAX_ERROR:
            Eluna::Push(L, 1);
            break;
        case BanReturn::BAN_NOTFOUND:
            Eluna::Push(L, 2);
            break;
        case BanReturn::BAN_LONGER_EXISTS:
            Eluna::Push(L, 3);
            break;
        }
        return 1;
    }

    /**
     * Saves all [Player]s.
     */
    int SaveAllPlayers(lua_State* /*L*/)
    {
        eObjectAccessor()SaveAllPlayers();
        return 0;
    }

    /**
     * Sends mail to a [Player].
     *
     * There can be several item entry-amount pairs at the end of the function.
     * There can be maximum of 12 different items.
     *
     *     enum MailStationery
     *     {
     *         MAIL_STATIONERY_TEST = 1,
     *         MAIL_STATIONERY_DEFAULT = 41,
     *         MAIL_STATIONERY_GM = 61,
     *         MAIL_STATIONERY_AUCTION = 62,
     *         MAIL_STATIONERY_VAL = 64, // Valentine
     *         MAIL_STATIONERY_CHR = 65, // Christmas
     *         MAIL_STATIONERY_ORP = 67 // Orphan
     *     };
     *
     * @param string subject : title (subject) of the mail
     * @param string text : contents of the mail
     * @param uint32 receiverGUIDLow : low GUID of the receiver
     * @param uint32 senderGUIDLow = 0 : low GUID of the sender
     * @param [MailStationery] stationary = MAIL_STATIONERY_DEFAULT : type of mail that is being sent as, refer to MailStationery above
     * @param uint32 delay = 0 : mail send delay in milliseconds
     * @param uint32 money = 0 : money to send
     * @param uint32 cod = 0 : cod money amount
     * @param uint32 entry = 0 : entry of an [Item] to send with mail
     * @param uint32 amount = 0 : amount of the [Item] to send with mail
     * @return uint32 itemGUIDlow : low GUID of the item. Up to 12 values returned, returns nil if no further items are sent
     */
    int SendMail(lua_State* L)
    {
        int i = 0;
        std::string subject = Eluna::CHECKVAL<std::string>(L, ++i);
        std::string text = Eluna::CHECKVAL<std::string>(L, ++i);
        uint32 receiverGUIDLow = Eluna::CHECKVAL<uint32>(L, ++i);
        uint32 senderGUIDLow = Eluna::CHECKVAL<uint32>(L, ++i, 0);
        uint32 stationary = Eluna::CHECKVAL<uint32>(L, ++i, MAIL_STATIONERY_DEFAULT);
        uint32 delay = Eluna::CHECKVAL<uint32>(L, ++i, 0);
        uint32 money = Eluna::CHECKVAL<uint32>(L, ++i, 0);
        uint32 cod = Eluna::CHECKVAL<uint32>(L, ++i, 0);
        int argAmount = lua_gettop(L);

        MailSender sender(MAIL_NORMAL, senderGUIDLow, (MailStationery)stationary);
        MailDraft draft(subject, text);

        if (cod)
            draft.AddCOD(cod);
        if (money)
            draft.AddMoney(money);

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        uint8 addedItems = 0;
        while (addedItems <= MAX_MAIL_ITEMS && i + 2 <= argAmount)
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, ++i);
            uint32 amount = Eluna::CHECKVAL<uint32>(L, ++i);

            ItemTemplate const* item_proto = eObjectMgr->GetItemTemplate(entry);
            if (!item_proto)
            {
                luaL_error(L, "Item entry %d does not exist", entry);
                continue;
            }
            if (amount < 1 || (item_proto->MaxCount > 0 && amount > uint32(item_proto->MaxCount)))
            {
                luaL_error(L, "Item entry %d has invalid amount %d", entry, amount);
                continue;
            }
            if (Item* item = Item::CreateItem(entry, amount))
            {
                item->SaveToDB(trans);
                draft.AddItem(item);
                Eluna::Push(L, item->GetGUID().GetCounter());
                ++addedItems;
            }
        }

        Player* receiverPlayer = eObjectAccessor()FindPlayer(MAKE_NEW_GUID(receiverGUIDLow, 0, HIGHGUID_PLAYER));
        draft.SendMailTo(trans, MailReceiver(receiverPlayer, receiverGUIDLow), sender, MAIL_CHECK_MASK_NONE, delay);
        CharacterDatabase.CommitTransaction(trans);
        return addedItems;
    }

    /**
     * Performs a bitwise AND (a & b).
     *
     * @param uint32 a
     * @param uint32 b
     * @return uint32 result
     */
    int bit_and(lua_State* L)
    {
        uint32 a = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 b = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, a & b);
        return 1;
    }

    /**
     * Performs a bitwise OR (a | b).
     *
     * @param uint32 a
     * @param uint32 b
     * @return uint32 result
     */
    int bit_or(lua_State* L)
    {
        uint32 a = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 b = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, a | b);
        return 1;
    }

    /**
     * Performs a bitwise left-shift (a << b).
     *
     * @param uint32 a
     * @param uint32 b
     * @return uint32 result
     */
    int bit_lshift(lua_State* L)
    {
        uint32 a = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 b = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, a << b);
        return 1;
    }

    /**
     * Performs a bitwise right-shift (a >> b).
     *
     * @param uint32 a
     * @param uint32 b
     * @return uint32 result
     */
    int bit_rshift(lua_State* L)
    {
        uint32 a = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 b = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, a >> b);
        return 1;
    }

    /**
     * Performs a bitwise XOR (a ^ b).
     *
     * @param uint32 a
     * @param uint32 b
     * @return uint32 result
     */
    int bit_xor(lua_State* L)
    {
        uint32 a = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 b = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, a ^ b);
        return 1;
    }

    /**
     * Performs a bitwise NOT (~a).
     *
     * @param uint32 a
     * @return uint32 result
     */
    int bit_not(lua_State* L)
    {
        uint32 a = Eluna::CHECKVAL<uint32>(L, 1);
        Eluna::Push(L, ~a);
        return 1;
    }

    /**
     * Adds a taxi path to a specified map, returns the used pathId.
     *
     * Note that the first taxi point needs to be near the player when he starts the taxi path.
     * The function should also be used only **once** per path added so use it on server startup for example.
     *
     * Related function: [Player:StartTaxi]
     *
     *     -- Execute on startup
     *     local pathTable = {{mapid, x, y, z}, {mapid, x, y, z}}
     *     local path = AddTaxiPath(pathTable, 28135, 28135)
     *
     *     -- Execute when the player should fly
     *     player:StartTaxi(path)
     *
     * @param table waypoints : table containing waypoints: {map, x, y, z[, actionFlag, delay]}
     * @param uint32 mountA : alliance [Creature] entry
     * @param uint32 mountH : horde [Creature] entry
     * @param uint32 price = 0 : price of the taxi path
     * @param uint32 pathId = 0 : path Id of the taxi path
     * @return uint32 actualPathId
     */
    int AddTaxiPath(lua_State* L)
    {
        luaL_checktype(L, 1, LUA_TTABLE);
        uint32 mountA = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 mountH = Eluna::CHECKVAL<uint32>(L, 3);
        uint32 price = Eluna::CHECKVAL<uint32>(L, 4, 0);
        uint32 pathId = Eluna::CHECKVAL<uint32>(L, 5, 0);
        lua_pushvalue(L, 1);
        // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}

        std::list<TaxiPathNodeEntry> nodes;

        int start = lua_gettop(L);
        int end = start;

        Eluna::Push(L);
        // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, nil
        while (lua_next(L, -2) != 0)
        {
            // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, key, value
            luaL_checktype(L, -1, LUA_TTABLE);
            Eluna::Push(L);
            // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, key, value, nil
            while (lua_next(L, -2) != 0)
            {
                // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, key, value, key2, value2
                lua_insert(L, end++);
                // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, value2, key, value, key2
            }
            // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, value2, key, value
            if (start == end)
                continue;
            if (end - start < 4) // no mandatory args, dont add
                return luaL_argerror(L, 1, "all waypoints do not have mandatory arguments");

            while (end - start < 8) // fill optional args with 0
            {
                Eluna::Push(L, 0);
                lua_insert(L, end++);
                // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, node, key, value
            }
            TaxiPathNodeEntry entry;
            // mandatory
            entry.mapid = Eluna::CHECKVAL<uint32>(L, start);
            entry.x = Eluna::CHECKVAL<float>(L, start + 1);
            entry.y = Eluna::CHECKVAL<float>(L, start + 2);
            entry.z = Eluna::CHECKVAL<float>(L, start + 3);
            // optional
            entry.actionFlag = Eluna::CHECKVAL<uint32>(L, start + 4, 0);
            entry.delay = Eluna::CHECKVAL<uint32>(L, start + 5, 0);

            nodes.push_back(entry);

            while (end != start) // remove args
                if (!lua_isnone(L, --end))
                    lua_remove(L, end);
            // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, key, value

            lua_pop(L, 1);
            // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}, key
        }
        // Stack: {nodes}, mountA, mountH, price, pathid, {nodes}
        lua_pop(L, 1);
        // Stack: {nodes}, mountA, mountH, price, pathid

        if (nodes.size() < 2)
            return 1;
        if (!pathId)
            pathId = sTaxiPathNodesByPath.size();
        if (sTaxiPathNodesByPath.size() <= pathId)
            sTaxiPathNodesByPath.resize(pathId + 1);

        sTaxiPathNodesByPath[pathId].clear();
        sTaxiPathNodesByPath[pathId].resize(nodes.size());
        static uint32 nodeId = 500;
        uint32 startNode = nodeId;
        uint32 index = 0;

        for (std::list<TaxiPathNodeEntry>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            TaxiPathNodeEntry& entry = *it;
            TaxiNodesEntry* nodeEntry = new TaxiNodesEntry();
            entry.path = pathId;
            entry.index = nodeId;
            nodeEntry->ID = index;
            nodeEntry->map_id = entry.mapid;
            nodeEntry->x = entry.x;
            nodeEntry->y = entry.y;
            nodeEntry->z = entry.z;
            nodeEntry->MountCreatureID[0] = mountH;
            nodeEntry->MountCreatureID[1] = mountA;
            sTaxiNodesStore.SetEntry(nodeId++, nodeEntry);
            sTaxiPathNodesByPath[pathId][index++] = new TaxiPathNodeEntry(entry);
        }
        if (startNode >= nodeId)
            return 1;

        TaxiPathEntry* pathEntry = new TaxiPathEntry();
        pathEntry->from = startNode;
        pathEntry->to = nodeId - 1;
        pathEntry->price = price;
        pathEntry->ID = pathId;
        sTaxiPathStore.SetEntry(pathId, pathEntry);
        sTaxiPathSetBySource[startNode][nodeId - 1] = pathEntry;

        Eluna::Push(L, pathId);
        return 1;
    }

    /**
     * Returns `true` if Eluna is in compatibility mode, `false` if in multistate.
     *
     * @return bool isCompatibilityMode
     */
    int IsCompatibilityMode(lua_State* L)
    {
        // Until AC supports multistate, this will always return true
        Eluna::Push(L, true);
        return 1;
    }

    /**
     * Returns `true` if the bag and slot is a valid inventory position, otherwise `false`.
     *
     * Some commonly used combinations:
     *
     * *Bag 255 (common character inventory)*
     *
     * - Slots 0-18: equipment
     * - Slots 19-22: bag slots
     * - Slots 23-38: backpack
     * - Slots 39-66: bank main slots
     * - Slots 67-74: bank bag slots
     * - Slots 86-117: keyring
     *
     * *Bags 19-22 (equipped bags)*
     *
     * - Slots 0-35
     *
     * *Bags 67-74 (bank bags)*
     *
     * - Slots 0-35
     *
     * @param uint8 bag : the bag the [Item] is in, you can get this with [Item:GetBagSlot]
     * @param uint8 slot : the slot the [Item] is in within the bag, you can get this with [Item:GetSlot]
     * @return bool isInventoryPos
     */
    int IsInventoryPos(lua_State* L)
    {
        uint8 bag = Eluna::CHECKVAL<uint8>(L, 1);
        uint8 slot = Eluna::CHECKVAL<uint8>(L, 2);

        Eluna::Push(L, Player::IsInventoryPos(bag, slot));
        return 1;
    }

    /**
     * Returns `true` if the bag and slot is a valid equipment position, otherwise `false`.
     *
     * See [Global:IsInventoryPos] for bag/slot combination examples.
     *
     * @param uint8 bag : the bag the [Item] is in, you can get this with [Item:GetBagSlot]
     * @param uint8 slot : the slot the [Item] is in within the bag, you can get this with [Item:GetSlot]
     * @return bool isEquipmentPosition
     */
    int IsEquipmentPos(lua_State* L)
    {
        uint8 bag = Eluna::CHECKVAL<uint8>(L, 1);
        uint8 slot = Eluna::CHECKVAL<uint8>(L, 2);

        Eluna::Push(L, Player::IsEquipmentPos(bag, slot));
        return 1;
    }

    /**
     * Returns `true` if the bag and slot is a valid bank position, otherwise `false`.
     *
     * See [Global:IsInventoryPos] for bag/slot combination examples.
     *
     * @param uint8 bag : the bag the [Item] is in, you can get this with [Item:GetBagSlot]
     * @param uint8 slot : the slot the [Item] is in within the bag, you can get this with [Item:GetSlot]
     * @return bool isBankPosition
     */
    int IsBankPos(lua_State* L)
    {
        uint8 bag = Eluna::CHECKVAL<uint8>(L, 1);
        uint8 slot = Eluna::CHECKVAL<uint8>(L, 2);

        Eluna::Push(L, Player::IsBankPos(bag, slot));
        return 1;
    }

    /**
     * Returns `true` if the bag and slot is a valid bag position, otherwise `false`.
     *
     * See [Global:IsInventoryPos] for bag/slot combination examples.
     *
     * @param uint8 bag : the bag the [Item] is in, you can get this with [Item:GetBagSlot]
     * @param uint8 slot : the slot the [Item] is in within the bag, you can get this with [Item:GetSlot]
     * @return bool isBagPosition
     */
    int IsBagPos(lua_State* L)
    {
        uint8 bag = Eluna::CHECKVAL<uint8>(L, 1);
        uint8 slot = Eluna::CHECKVAL<uint8>(L, 2);

        Eluna::Push(L, Player::IsBagPos((bag << 8) + slot));
        return 1;
    }

    /**
     * Returns `true` if the event is currently active, otherwise `false`.
     *
     * @param uint16 eventId : the event id to check.
     * @return bool isActive
     */
    int IsGameEventActive(lua_State* L)
    {
        uint16 eventId = Eluna::CHECKVAL<uint16>(L, 1);

        Eluna::Push(L, eGameEventMgr->IsActiveEvent(eventId));
        return 1;
    }

    /**
     * Returns the server's current time.
     *
     * @return uint32 currTime : the current time, in milliseconds
     */
    int GetCurrTime(lua_State* L)
    {
        Eluna::Push(L, ElunaUtil::GetCurrTime());
        return 1;
    }

    /**
     * Returns the difference between an old timestamp and the current time.
     *
     * @param uint32 oldTime : an old timestamp, in milliseconds
     * @return uint32 timeDiff : the difference, in milliseconds
     */
    int GetTimeDiff(lua_State* L)
    {
        uint32 oldtimems = Eluna::CHECKVAL<uint32>(L, 1);

        Eluna::Push(L, ElunaUtil::GetTimeDiff(oldtimems));
        return 1;
    }

    static std::string GetStackAsString(lua_State* L)
    {
        std::ostringstream oss;
        int top = lua_gettop(L);
        for (int i = 1; i <= top; ++i)
        {
            oss << luaL_tolstring(L, i, NULL);
            lua_pop(L, 1);
        }
        return oss.str();
    }

    /**
     * Prints given parameters to the info log.
     *
     * @param ...
     */
    int PrintInfo(lua_State* L)
    {
        ELUNA_LOG_INFO("{}", GetStackAsString(L));
        return 0;
    }

    /**
     * Prints given parameters to the error log.
     *
     * @param ...
     */
    int PrintError(lua_State* L)
    {
        ELUNA_LOG_ERROR("{}", GetStackAsString(L));
        return 0;
    }

    /**
     * Prints given parameters to the debug log.
     *
     * @param ...
     */
    int PrintDebug(lua_State* L)
    {
        ELUNA_LOG_DEBUG("{}", GetStackAsString(L));
        return 0;
    }

    /**
    * Starts the event by eventId, if force is set, the event will force start regardless of previous event state.
    *
    * @param uint16 eventId : the event id to start.
    * @param bool force = false : set `true` to force start the event.
    */
    int StartGameEvent(lua_State* L)
    {
        uint16 eventId = Eluna::CHECKVAL<uint16>(L, 1);
        bool force = Eluna::CHECKVAL<bool>(L, 2, false);

        eGameEventMgr->StartEvent(eventId, force);
        return 0;
    }

    /**
    * Stops the event by eventId, if force is set, the event will force stop regardless of previous event state.
    *
    * @param uint16 eventId : the event id to stop.
    * @param bool force = false : set `true` to force stop the event.
    */
    int StopGameEvent(lua_State* L)
    {
        uint16 eventId = Eluna::CHECKVAL<uint16>(L, 1);
        bool force = Eluna::CHECKVAL<bool>(L, 2, false);

        eGameEventMgr->StopEvent(eventId, force);
        return 0;
    }

    /**
     * Performs a non-blocking HTTP request.
     *
     * When the passed callback function is called, the parameters `(status, body, headers)` are passed to it.
     *
     *     -- GET example (prints a random word)
     *     HttpRequest("GET", "https://random-word-api.herokuapp.com/word", function(status, body, headers)
     *         print("Random word: " .. string.sub(body, 3, body:len() - 2))
     *     end)
     *
     *     -- POST example with JSON request body
     *     HttpRequest("POST", "https://jsonplaceholder.typicode.com/posts", '{"userId": 1,"title": "Foo","body": "Bar!"}', "application/json", function(status, body, headers)
     *         print(body)
     *     end)
     *
     *     -- Example with request headers
     *     HttpRequest("GET", "https://postman-echo.com/headers", { Accept = "application/json", ["User-Agent"] = "Eluna Lua Engine" }, function(status, body, headers)
     *         print(body)
     *     end)
     *
     * @proto (httpMethod, url, function)
     * @proto (httpMethod, url, headers, function)
     * @proto (httpMethod, url, body, contentType, function)
     * @proto (httpMethod, url, body, contentType, headers, function)
     *
     * @param string httpMethod : the HTTP method to use (possible values are: `"GET"`, `"HEAD"`, `"POST"`, `"PUT"`, `"PATCH"`, `"DELETE"`, `"OPTIONS"`)
     * @param string url : the URL to query
     * @param table headers : a table with string key-value pairs containing the request headers
     * @param string body : the request's body (only used for POST, PUT and PATCH requests)
     * @param string contentType : the body's content-type
     * @param function function : function that will be called when the request is executed
     */
    int HttpRequest(lua_State* L)
    {
        std::string httpVerb = Eluna::CHECKVAL<std::string>(L, 1);
        std::string url = Eluna::CHECKVAL<std::string>(L, 2);
        std::string body;
        std::string bodyContentType;
        httplib::Headers headers;

        int headersIdx = 3;
        int callbackIdx = 3;

        if (!lua_istable(L, headersIdx) && lua_isstring(L, headersIdx) && lua_isstring(L, headersIdx + 1))
        {
            body = Eluna::CHECKVAL<std::string>(L, 3);
            bodyContentType = Eluna::CHECKVAL<std::string>(L, 4);
            headersIdx = 5;
            callbackIdx = 5;
        }

        if (lua_istable(L, headersIdx))
        {
            ++callbackIdx;

            lua_pushnil(L); // First key
            while (lua_next(L, headersIdx) != 0)
            {
                // Uses 'key' (at index -2) and 'value' (at index -1)
                if (lua_isstring(L, -2))
                {
                    std::string key(lua_tostring(L, -2));
                    std::string value(lua_tostring(L, -1));
                    headers.insert(std::pair<std::string, std::string>(key, value));
                }
                // Removes 'value'; keeps 'key' for next iteration
                lua_pop(L, 1);
            }
        }

        lua_pushvalue(L, callbackIdx);
        int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);
        if (funcRef >= 0)
        {
            Eluna::GEluna->httpManager.PushRequest(new HttpWorkItem(funcRef, httpVerb, url, body, bodyContentType, headers));
        }
        else
        {
            luaL_argerror(L, callbackIdx, "unable to make a ref to function");
        }

        return 0;
    }

    /**
     * Returns an object representing a `long long` (64-bit) value.
     *
     * The value by default is 0, but can be initialized to a value by passing a number or long long as a string.
     *
     * @proto value = ()
     * @proto value = (n)
     * @proto value = (n_ll)
     * @proto value = (n_str)
     * @param int32 n
     * @param int64 n_ll
     * @param string n_str
     * @return int64 value
     */
    int CreateLongLong(lua_State* L)
    {
        long long init = 0;
        if (lua_isstring(L, 1))
        {
            std::string str = Eluna::CHECKVAL<std::string>(L, 1);
            std::istringstream iss(str);
            iss >> init;
            if (iss.bad())
                return luaL_argerror(L, 1, "long long (as string) could not be converted");
        }
        else if (!lua_isnoneornil(L, 1))
            init = Eluna::CHECKVAL<long long>(L, 1);

        Eluna::Push(L, init);
        return 1;
    }

    /**
     * Returns an object representing an `unsigned long long` (64-bit) value.
     *
     * The value by default is 0, but can be initialized to a value by passing a number or unsigned long long as a string.
     *
     * @proto value = ()
     * @proto value = (n)
     * @proto value = (n_ull)
     * @proto value = (n_str)
     * @param uint32 n
     * @param uint64 n_ull
     * @param string n_str
     * @return uint64 value
     */
    int CreateULongLong(lua_State* L)
    {
        unsigned long long init = 0;
        if (lua_isstring(L, 1))
        {
            std::string str = Eluna::CHECKVAL<std::string>(L, 1);
            std::istringstream iss(str);
            iss >> init;
            if (iss.bad())
                return luaL_argerror(L, 1, "unsigned long long (as string) could not be converted");
        }
        else if (!lua_isnoneornil(L, 1))
            init = Eluna::CHECKVAL<unsigned long long>(L, 1);

        Eluna::Push(L, init);
        return 1;
    }

    /**
     * Unbinds event handlers for either all [BattleGround] events, or one type of event.
     *
     * If `event_type` is `nil`, all [BattleGround] event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto ()
     * @proto (event_type)
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterBGEvent]
     */
    int ClearBattleGroundEvents(lua_State* L)
    {
        typedef EventKey<Hooks::BGEvents> Key;

        if (lua_isnoneornil(L, 1))
        {
            Eluna::GetEluna(L)->BGEventBindings->Clear();
        }
        else
        {
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 1);
            Eluna::GetEluna(L)->BGEventBindings->Clear(Key((Hooks::BGEvents)event_type));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [Creature]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Creature]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect all instances of the [Creature], not just one.
     * To bind and unbind events to a single [Creature], see [Global:RegisterUniqueCreatureEvent] and [Global:ClearUniqueCreatureEvents].
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of one or more [Creature]s whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterCreatureEvent]
     */
    int ClearCreatureEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::CreatureEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::CREATURE_EVENT_COUNT; ++i)
                E->CreatureEventBindings->Clear(Key((Hooks::CreatureEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->CreatureEventBindings->Clear(Key((Hooks::CreatureEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [Creature]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Creature]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect only a single [Creature].
     * To bind and unbind events to all instances of a [Creature], see [Global:RegisterCreatureEvent] and [Global:ClearCreatureEvent].
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param ObjectGuid guid : the GUID of a single [Creature] whose handlers will be cleared
     * @param uint32 instance_id : the instance ID of a single [Creature] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterCreatureEvent]
     */
    int ClearUniqueCreatureEvents(lua_State* L)
    {
        typedef UniqueObjectKey<Hooks::CreatureEvents> Key;

        if (lua_isnoneornil(L, 3))
        {
            ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);
            uint32 instanceId = Eluna::CHECKVAL<uint32>(L, 2);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::CREATURE_EVENT_COUNT; ++i)
                E->CreatureUniqueBindings->Clear(Key((Hooks::CreatureEvents)i, guid, instanceId));
        }
        else
        {
            ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 1);
            uint32 instanceId = Eluna::CHECKVAL<uint32>(L, 2);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 3);
            Eluna::GetEluna(L)->CreatureUniqueBindings->Clear(Key((Hooks::CreatureEvents)event_type, guid, instanceId));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [Creature]'s gossip events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Creature]'s gossip event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect all instances of the [Creature], not just one.
     * To bind and unbind gossip events to a single [Creature], tell the Eluna developers to implement that.
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of a [Creature] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterCreatureGossipEvent]
     */
    int ClearCreatureGossipEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::GossipEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::GOSSIP_EVENT_COUNT; ++i)
                E->CreatureGossipBindings->Clear(Key((Hooks::GossipEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->CreatureGossipBindings->Clear(Key((Hooks::GossipEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [GameObject]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the [GameObject]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect all instances of the [GameObject], not just one.
     * To bind and unbind events to a single [GameObject], tell the Eluna developers to implement that.
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of a [GameObject] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterGameObjectEvent]
     */
    int ClearGameObjectEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::GameObjectEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::GAMEOBJECT_EVENT_COUNT; ++i)
                E->GameObjectEventBindings->Clear(Key((Hooks::GameObjectEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->GameObjectEventBindings->Clear(Key((Hooks::GameObjectEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [GameObject]'s gossip events, or one type of event.
     *
     * If `event_type` is `nil`, all the [GameObject]'s gossip event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect all instances of the [GameObject], not just one.
     * To bind and unbind gossip events to a single [GameObject], tell the Eluna developers to implement that.
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of a [GameObject] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterGameObjectGossipEvent]
     */
    int ClearGameObjectGossipEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::GossipEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::GOSSIP_EVENT_COUNT; ++i)
                E->GameObjectGossipBindings->Clear(Key((Hooks::GossipEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->GameObjectGossipBindings->Clear(Key((Hooks::GossipEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all [Group] events, or one type of [Group] event.
     *
     * If `event_type` is `nil`, all [Group] event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto ()
     * @proto (event_type)
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterGroupEvent]
     */
    int ClearGroupEvents(lua_State* L)
    {
        typedef EventKey<Hooks::GroupEvents> Key;

        if (lua_isnoneornil(L, 1))
        {
            Eluna::GetEluna(L)->GroupEventBindings->Clear();
        }
        else
        {
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 1);
            Eluna::GetEluna(L)->GroupEventBindings->Clear(Key((Hooks::GroupEvents)event_type));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all [Guild] events, or one type of [Guild] event.
     *
     * If `event_type` is `nil`, all [Guild] event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto ()
     * @proto (event_type)
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterGuildEvent]
     */
    int ClearGuildEvents(lua_State* L)
    {
        typedef EventKey<Hooks::GuildEvents> Key;

        if (lua_isnoneornil(L, 1))
        {
            Eluna::GetEluna(L)->GuildEventBindings->Clear();
        }
        else
        {
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 1);
            Eluna::GetEluna(L)->GuildEventBindings->Clear(Key((Hooks::GuildEvents)event_type));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of an [Item]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Item]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect all instances of the [Item], not just one.
     * To bind and unbind events to a single [Item], tell the Eluna developers to implement that.
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of an [Item] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterItemEvent]
     */
    int ClearItemEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::ItemEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::ITEM_EVENT_COUNT; ++i)
                E->ItemEventBindings->Clear(Key((Hooks::ItemEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->ItemEventBindings->Clear(Key((Hooks::ItemEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of an [Item]'s gossip events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Item]'s gossip event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * **NOTE:** this will affect all instances of the [Item], not just one.
     * To bind and unbind gossip events to a single [Item], tell the Eluna developers to implement that.
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of an [Item] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterItemGossipEvent]
     */
    int ClearItemGossipEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::GossipEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::GOSSIP_EVENT_COUNT; ++i)
                E->ItemGossipBindings->Clear(Key((Hooks::GossipEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->ItemGossipBindings->Clear(Key((Hooks::GossipEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [WorldPacket] opcode's events, or one type of event.
     *
     * If `event_type` is `nil`, all the [WorldPacket] opcode's event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto (opcode)
     * @proto (opcode, event_type)
     * @param uint32 opcode : the type of [WorldPacket] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterPacketEvent]
     */
    int ClearPacketEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::PacketEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::PACKET_EVENT_COUNT; ++i)
                E->PacketEventBindings->Clear(Key((Hooks::PacketEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->PacketEventBindings->Clear(Key((Hooks::PacketEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all [Player] events, or one type of [Player] event.
     *
     * If `event_type` is `nil`, all [Player] event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto ()
     * @proto (event_type)
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterPlayerEvent]
     */
    int ClearPlayerEvents(lua_State* L)
    {
        typedef EventKey<Hooks::PlayerEvents> Key;

        if (lua_isnoneornil(L, 1))
        {
            Eluna::GetEluna(L)->PlayerEventBindings->Clear();
        }
        else
        {
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 1);
            Eluna::GetEluna(L)->PlayerEventBindings->Clear(Key((Hooks::PlayerEvents)event_type));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [Player]'s gossip events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Player]'s gossip event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the low GUID of a [Player] whose handlers will be cleared
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterPlayerGossipEvent]
     */
    int ClearPlayerGossipEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::GossipEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::GOSSIP_EVENT_COUNT; ++i)
                E->PlayerGossipBindings->Clear(Key((Hooks::GossipEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->PlayerGossipBindings->Clear(Key((Hooks::GossipEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all server events, or one type of event.
     *
     * If `event_type` is `nil`, all server event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto ()
     * @proto (event_type)
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterServerEvent]
     */
    int ClearServerEvents(lua_State* L)
    {
        typedef EventKey<Hooks::ServerEvents> Key;

        if (lua_isnoneornil(L, 1))
        {
            Eluna::GetEluna(L)->ServerEventBindings->Clear();
        }
        else
        {
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 1);
            Eluna::GetEluna(L)->ServerEventBindings->Clear(Key((Hooks::ServerEvents)event_type));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a non-instanced [Map]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the non-instanced [Map]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto (map_id)
     * @proto (map_id, event_type)
     * @param uint32 map_id : the ID of a [Map]
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterPlayerGossipEvent]
     */
    int ClearMapEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::InstanceEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::INSTANCE_EVENT_COUNT; ++i)
                E->MapEventBindings->Clear(Key((Hooks::InstanceEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->MapEventBindings->Clear(Key((Hooks::InstanceEvents)event_type, entry));
        }

        return 0;
    }

    /**
     * Unbinds event handlers for either all of an instanced [Map]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the instanced [Map]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto (instance_id)
     * @proto (instance_id, event_type)
     * @param uint32 entry : the ID of an instance of a [Map]
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterInstanceEvent]
     */
    int ClearInstanceEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::InstanceEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::INSTANCE_EVENT_COUNT; ++i)
                E->InstanceEventBindings->Clear(Key((Hooks::InstanceEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->InstanceEventBindings->Clear(Key((Hooks::InstanceEvents)event_type, entry));
        }

        return 0;
    }

    /**
     * Unbinds event handlers for either all [Ticket] events, or one type of [Ticket] event.
     *
     * If `event_type` is `nil`, all [Ticket] event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     * @proto ()
     * @proto (event_type)
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterTicketEvent]
     */
    int ClearTicketEvents(lua_State* L)
    {
        typedef EventKey<Hooks::TicketEvents> Key;

        if (lua_isnoneornil(L, 1))
        {
            Eluna::GetEluna(L)->TicketEventBindings->Clear();
        }
        else
        {
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 1);
            Eluna::GetEluna(L)->TicketEventBindings->Clear(Key((Hooks::TicketEvents)event_type));
        }
        return 0;
    }

    /**
     * Unbinds event handlers for either all of a [Spell]'s events, or one type of event.
     *
     * If `event_type` is `nil`, all the [Spell]'s event handlers are cleared.
     *
     * Otherwise, only event handlers for `event_type` are cleared.
     *
     *
     * @proto (entry)
     * @proto (entry, event_type)
     * @param uint32 entry : the ID of a [Spell]s
     * @param uint32 event_type : the event whose handlers will be cleared, see [Global:RegisterSpellEvent]
     */
    int ClearSpellEvents(lua_State* L)
    {
        typedef EntryKey<Hooks::SpellEvents> Key;

        if (lua_isnoneornil(L, 2))
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);

            Eluna* E = Eluna::GetEluna(L);
            for (uint32 i = 1; i < Hooks::SPELL_EVENT_COUNT; ++i)
                E->SpellEventBindings->Clear(Key((Hooks::SpellEvents)i, entry));
        }
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 1);
            uint32 event_type = Eluna::CHECKVAL<uint32>(L, 2);
            Eluna::GetEluna(L)->SpellEventBindings->Clear(Key((Hooks::SpellEvents)event_type, entry));
        }
        return 0;
    }

    /**
     * Gets the faction which is the current owner of Halaa in Nagrand
     * 0 = Alliance
     * 1 = Horde
     *
     * 600 = slider max Alliance
     * -600 = slider max Horde
     *
     * @return int16 the ID of the team to own Halaa
     * @return float the slider position.
     */
    int GetOwnerHalaa(lua_State* L)
    {
        OutdoorPvPNA* nagrandPvp = (OutdoorPvPNA*)sOutdoorPvPMgr->GetOutdoorPvPToZoneId(3518);
        OPvPCapturePointNA* halaa = nagrandPvp->GetCapturePoint();
        Eluna::Push(L, halaa->GetControllingFaction());
        Eluna::Push(L, halaa->GetSlider());

        return 2;
    }

    /**
     * Sets the owner of Halaa in Nagrand to the respective faction
     * 0 = Alliance
     * 1 = Horde
     *
     * @param uint16 teamId : the ID of the team to own Halaa
     */
    int SetOwnerHalaa(lua_State* L)
    {
        uint16 teamId = Eluna::CHECKVAL<uint16>(L, 1);

        OutdoorPvPNA* nagrandPvp = (OutdoorPvPNA*)sOutdoorPvPMgr->GetOutdoorPvPToZoneId(3518);
        OPvPCapturePointNA* halaa = nagrandPvp->GetCapturePoint();

        if (teamId == 0)
        {
            halaa->SetSlider(599);
        }
        else if (teamId == 1)
        {
            halaa->SetSlider(-599);
        }
        else
        {
            return luaL_argerror(L, 1, "0 for Alliance or 1 for Horde expected");
        }

        return 0;
    }

    /**
     * Gets the localized OptionText and BoxText for a specific gossip menu option.
     * If the text for the specified locale is not found, it returns the default text.
     *
     * @param uint32 menuId : The ID of the gossip menu.
     * @param uint32 optionId : The ID of the gossip menu option.
     * @param uint8 locale : The locale to retrieve the text for. 0 represents the default locale.
     *
     * @return string, string : The localized OptionText and BoxText for the gossip menu option, or the default text if no localization is found.
     */
    int GetGossipMenuOptionLocale(lua_State* L)
    {
        uint32 menuId = Eluna::CHECKVAL<uint32>(L, 1);
        uint32 optionId = Eluna::CHECKVAL<uint32>(L, 2);
        uint8 locale = Eluna::CHECKVAL<uint8>(L, 3);

        std::string strOptionText;
        std::string strBoxText;

        if (locale != DEFAULT_LOCALE)
        {
            if (GossipMenuItemsLocale const* gossipMenuLocale = sObjectMgr->GetGossipMenuItemsLocale(MAKE_PAIR32(menuId, optionId)))
            {
                ObjectMgr::GetLocaleString(gossipMenuLocale->OptionText, LocaleConstant(locale), strOptionText);
                ObjectMgr::GetLocaleString(gossipMenuLocale->BoxText, LocaleConstant(locale), strBoxText);
            }
        }

        if (strOptionText.empty() || strBoxText.empty())
        {
            GossipMenuItemsMapBounds bounds = sObjectMgr->GetGossipMenuItemsMapBounds(menuId);
            for (auto itr = bounds.first; itr != bounds.second; ++itr)
            {
                if (itr->second.OptionID == optionId)
                {
                    if (strOptionText.empty())
                        strOptionText = itr->second.OptionText;
                    if (strBoxText.empty())
                        strBoxText = itr->second.BoxText;
                    break;
                }
            }
        }

        Eluna::Push(L, strOptionText);
        Eluna::Push(L, strBoxText);
        return 2;
    }

    /**
     * Return the entrance position (x, y, z, o) of the specified dungeon map id.
     *
     * @param uint32 mapId
     *
     * @return uint32 pos_x
     * @return uint32 pos_y
     * @return uint32 pos_z
     * @return uint32 pos_o
     */
    int GetMapEntrance(lua_State* L)
    {
        uint32 mapId = Eluna::CHECKVAL<uint32>(L, 1);
        AreaTriggerTeleport const* at = sObjectMgr->GetMapEntranceTrigger(mapId);

        if (!at)
        {
            lua_pushnil(L);
            return 1;
        }

        Eluna::Push(L, at->target_X);
        Eluna::Push(L, at->target_Y);
        Eluna::Push(L, at->target_Z);
        Eluna::Push(L, at->target_Orientation);

        return 5;
    }
      
    /**  
     * Get the [SpellInfo] for the specified [Spell] id
     *
     * @param uint32 spellId : the ID of the spell
     * @return [SpellInfo] spellInfo
     */
    int GetSpellInfo(lua_State* L)
    {
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 1);
        Eluna::Push(L, sSpellMgr->GetSpellInfo(spellId));
        return 1;

    }
  
    /**
     * Returns an entry from the specified DBC (DatabaseClient) store.
     *
     * This function looks up an entry in a DBC file by name and ID, and pushes it onto the Lua stack.
     *
     * @param string dbcName : The name of the DBC store (e.g., "ItemDisplayInfo")
     * @param uint32 id : The ID used to look up within the specified DBC store
     *
     * @return [DBCStore] store : The requested DBC store instance
     */
    int LookupEntry(lua_State* L)
    {
        const char* dbcName = Eluna::CHECKVAL<const char*>(L, 1);
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);

        for (const auto& dbc : dbcRegistry)
        {
            if (dbc.name == dbcName)
            {
                const void* entry = dbc.lookupFunction(id);
                if (!entry)
                    return 0;

                dbc.pushFunction(L, entry);
                return 1;
            }
        }

        return luaL_error(L, "Invalid DBC name: %s", dbcName);
    }
}
#endif
