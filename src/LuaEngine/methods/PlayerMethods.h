/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef PLAYERMETHODS_H
#define PLAYERMETHODS_H

#include "Chat.h"
#include "GameTime.h"
#include "GossipDef.h"

/***
 * Inherits all methods from: [Object], [WorldObject], [Unit]
 */
namespace LuaPlayer
{
    /**
     * Returns `true` if the [Player] can Titan Grip, `false` otherwise.
     *
     * @return bool canTitanGrip
     */
    int CanTitanGrip(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->CanTitanGrip());
        return 1;
    }

    /**
     * Returns `true` if the [Player] has a talent by ID in specified spec, `false` otherwise.
     *
     * @param uint32 spellId : talent spellId to check
     * @param uint8 spec : specified spec. 0 for primary, 1 for secondary.
     * @return bool hasTalent
     */
    int HasTalent(lua_State* L, Player* player)
    {
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);
        uint8 maxSpecs = MAX_TALENT_SPECS;
        uint8 spec = Eluna::CHECKVAL<uint8>(L, 3);
        if (spec >= maxSpecs)
            return 1;
        Eluna::Push(L, player->HasTalent(spellId, spec));
        return 1;
    }

    /**
     * Returns `true` if the [Player] has completed the specified achievement, `false` otherwise.
     *
     * @param uint32 achievementId
     * @return bool hasAchieved
     */
    int HasAchieved(lua_State* L, Player* player)
    {
        uint32 achievementId = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L, player->HasAchieved(achievementId));
        return 1;
    }

    /**
     * Returns the progress of the [Player] for the specified achievement criteria.
     *
     * @param uint32 criteriaId
     * @return uint32 progress : progress value or nil
     */
    int GetAchievementCriteriaProgress(lua_State* L, Player* player)
    {
        uint32 criteriaId = Eluna::CHECKVAL<uint32>(L, 2);
        const AchievementCriteriaEntry* criteria = sAchievementCriteriaStore.LookupEntry(criteriaId);
        CriteriaProgress* progress = player->GetAchievementMgr()->GetCriteriaProgress(criteria);
        if (progress)
        {
            Eluna::Push(L, progress->counter);
        }
        else
        {
            Eluna::Push(L, (void*)nullptr);
        }
        return 1;
    }

    /**
     * Returns `true` if the [Player] has an active [Quest] by specific ID, `false` otherwise.
     *
     * @param uint32 questId
     * @return bool hasQuest
     */
    int HasQuest(lua_State* L, Player* player)
    {
        uint32 quest = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->IsActiveQuest(quest));
        return 1;
    }

    /**
     * Returns `true` if the [Player] has a skill by specific ID, `false` otherwise.
     *
     * @param uint32 skill
     * @return bool hasSkill
     */
    int HasSkill(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->HasSkill(skill));
        return 1;
    }

    /**
     * Returns `true` if the [Player] has a [Spell] by specific ID, `false` otherwise.
     *
     * @param uint32 spellId
     * @return bool hasSpell
     */
    int HasSpell(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->HasSpell(id));
        return 1;
    }

    /**
     * Returns true if [Player] has specified login flag
     *
     * @param uint32 flag
     * @return bool hasLoginFlag
     */
    int HasAtLoginFlag(lua_State* L, Player* player)
    {
        uint32 flag = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->HasAtLoginFlag((AtLoginFlags)flag));
        return 1;
    }

    /**
     * Returns true if [Player] has [Quest] for [GameObject]
     *
     * @param int32 entry : entry of a [GameObject]
     * @return bool hasQuest
     */
    int HasQuestForGO(lua_State* L, Player* player)
    {
        int32 entry = Eluna::CHECKVAL<int32>(L, 2);

        Eluna::Push(L, player->HasQuestForGO(entry));
        return 1;
    }

    /**
     * Returns `true` if the [Player] has a title by specific ID, `false` otherwise.
     *
     * @param uint32 titleId
     * @return bool hasTitle
     */
    int HasTitle(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (titleInfo)
            Eluna::Push(L, player->HasTitle(titleInfo));
        return 1;
    }
    
    /**
     * Returns `true` if the [Player] has the given amount of item entry specified, `false` otherwise.
     *
     * @param uint32 itemId : entry of the item
     * @param uint32 count = 1 : amount of items the player needs should have
     * @param bool check_bank = false : determines if the item can be in player bank
     * @return bool hasItem
     */
    int HasItem(lua_State* L, Player* player)
    {
        uint32 itemId = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 count = Eluna::CHECKVAL<uint32>(L, 3, 1);
        bool check_bank = Eluna::CHECKVAL<bool>(L, 4, false);
        Eluna::Push(L, player->HasItemCount(itemId, count, check_bank));
        return 1;
    }
    
    /**
     * Returns `true` if the [Player] has a quest for the item entry specified, `false` otherwise.
     *
     * @param uint32 entry : entry of the item
     * @return bool hasQuest
     */
    int HasQuestForItem(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->HasQuestForItem(entry));
        return 1;
    }
    
    /**
     * Returns `true` if the [Player] can use the item or item entry specified, `false` otherwise.
     *
     * @proto canUse = (item)
     * @proto canUse = (entry)
     * @param [Item] item : an instance of an item
     * @param uint32 entry : entry of the item
     * @return bool canUse
     */
    int CanUseItem(lua_State* L, Player* player)
    {
        Item* item = Eluna::CHECKOBJ<Item>(L, 2, false);
        if (item)
            Eluna::Push(L, player->CanUseItem(item) == EQUIP_ERR_OK);
        else
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
            const ItemTemplate* temp = eObjectMgr->GetItemTemplate(entry);
            if (temp)
                Eluna::Push(L, player->CanUseItem(temp) == EQUIP_ERR_OK);
            else
                Eluna::Push(L, false);
        }
        return 1;
    }

    /**
     * Returns `true` if the [Spell] specified by ID is currently on cooldown for the [Player], `false` otherwise.
     *
     * @param uint32 spellId
     * @return bool hasSpellCooldown
     */
    int HasSpellCooldown(lua_State* L, Player* player)
    {
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->HasSpellCooldown(spellId));
        return 1;
    }

    /**
     * Returns `true` if the [Player] can share [Quest] specified by ID, `false` otherwise.
     *
     * @param uint32 entryId
     * @return bool hasSpellCooldown
     */
    int CanShareQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->CanShareQuest(entry));
        return 1;
    }

    /**
     * Returns `true` if the [Player] can currently communicate through chat, `false` otherwise.
     *
     * @return bool canSpeak
     */
    int CanSpeak(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->CanSpeak());
        return 1;
    }

    /**
     * Returns `true` if the [Player] has permission to uninvite others from the current group, `false` otherwise.
     *
     * @return bool canUninviteFromGroup
     */
    int CanUninviteFromGroup(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->CanUninviteFromGroup() == ERR_PARTY_RESULT_OK);
        return 1;
    }

    /**
     * Returns `true` if the [Player] can fly, `false` otherwise.
     *
     * @return bool canFly
     */
    int CanFly(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->CanFly());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently in water, `false` otherwise.
     *
     * @return bool isInWater
     */
    int IsInWater(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->IsInWater());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently moving, `false` otherwise.
     *
     * @return bool isMoving
     */
    int IsMoving(lua_State* L, Player* player) // enable for unit when mangos support it
    {
        Eluna::Push(L, player->isMoving());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently flying, `false` otherwise.
     *
     * @return bool isFlying
     */
    int IsFlying(lua_State* L, Player* player) // enable for unit when mangos support it
    {
        Eluna::Push(L, player->IsFlying());
        return 1;
    }

    /**
     * Returns `true` if the [Player] has a Tank Specialization, `false` otherwise.
     *
     * @return bool HasTankSpec
     */
    int HasTankSpec(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->HasTankSpec());
        return 1;
    }
    
    /**
     * Returns `true` if the [Player] has a Melee Specialization, `false` otherwise.
     *
     * @return bool HasMeleeSpec
     */
    int HasMeleeSpec(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->HasMeleeSpec());
        return 1;
    }
    
    /**
     * Returns `true` if the [Player] has a Caster Specialization, `false` otherwise.
     *
     * @return bool HasCasterSpec
     */
    int HasCasterSpec(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->HasCasterSpec());
        return 1;
    }
    
    /**
     * Returns `true` if the [Player] has a Heal Specialization, `false` otherwise.
     *
     * @return bool HasHealSpec
     */
    int HasHealSpec(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->HasHealSpec());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is in a [Group], `false` otherwise.
     *
     * @return bool isInGroup
     */
    int IsInGroup(lua_State* L, Player* player)
    {
        Eluna::Push(L, (player->GetGroup() != NULL));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is in a [Guild], `false` otherwise.
     *
     * @return bool isInGuild
     */
    int IsInGuild(lua_State* L, Player* player)
    {
        Eluna::Push(L, (player->GetGuildId() != 0));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is a Game Master, `false` otherwise.
     *
     * Note: This is only true when GM tag is activated! For alternative see [Player:GetGMRank]
     *
     * @return bool isGM
     */
    int IsGM(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->IsGameMaster());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is in an arena team specified by type, `false` otherwise.
     *
     * @param uint32 type
     * @return bool isInArenaTeam
     */
    int IsInArenaTeam(lua_State* L, Player* player)
    {
        uint32 type = Eluna::CHECKVAL<uint32>(L, 2);
        if (type < MAX_ARENA_SLOT && player->GetArenaTeamId(type))
            Eluna::Push(L, true);
        else
            Eluna::Push(L, false);
        return 1;
    }

    /**
     * Returns `true` if the [Player] is immune to everything.
     *
     * @return bool isImmune
     */
    int IsImmuneToDamage(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isTotalImmune());
        return 1;
    }

    /**
     * Returns `true` if the [Player] satisfies all requirements to complete the quest entry.
     *
     * @param uint32 questId
     * @return bool canCompleteRepeatableQuest
     */
    int CanCompleteRepeatableQuest(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);
        const Quest* quest = sObjectMgr->GetQuestTemplate(questId); // Retrieve the Quest object
        if (!quest)
        {
            Eluna::Push(L, false);
            return 1;
        }

        Eluna::Push(L, player->CanCompleteRepeatableQuest(quest));
        return 1;
    }

    /**
     * Returns `true` if the [Player] satisfies all requirements to reward the quest entry.
     *
     * @param uint32 questId
     * @return bool canRewardQuest
     */
    int CanRewardQuest(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);
        const Quest* quest = sObjectMgr->GetQuestTemplate(questId); // Retrieve the Quest object
        if (!quest)
        {
            Eluna::Push(L, false);
            return 1;
        }

        Eluna::Push(L, player->CanRewardQuest(quest, true)); // Modify the second argument as needed
        return 1;
    }

    /**
     * Returns `true` if the [Player] satisfies all requirements to complete the quest entry.
     *
     * @param uint32 entry
     * @return bool canComplete
     */
    int CanCompleteQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->CanCompleteQuest(entry));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is a part of the Horde faction, `false` otherwise.
     *
     * @return bool isHorde
     */
    int IsHorde(lua_State* L, Player* player)
    {
        Eluna::Push(L, (player->GetTeamId() == TEAM_HORDE));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is a part of the Alliance faction, `false` otherwise.
     *
     * @return bool isAlliance
     */
    int IsAlliance(lua_State* L, Player* player)
    {
        Eluna::Push(L, (player->GetTeamId() == TEAM_ALLIANCE));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is 'Do Not Disturb' flagged, `false` otherwise.
     *
     * @return bool isDND
     */
    int IsDND(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isDND());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is 'Away From Keyboard' flagged, `false` otherwise.
     *
     * @return bool isAFK
     */
    int IsAFK(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isAFK());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently falling, `false` otherwise.
     *
     * @return bool isFalling
     */
    int IsFalling(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->IsFalling());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is in the same group and visible to the specified [Player], `false` otherwise.
     *
     * @param [Player] player : the source player
     * @param [Player] target : the player to check visibility from
     * @return bool isGroupVisible
     */
    int IsGroupVisibleFor(lua_State* L, Player* player)
    {
        Player* target = Eluna::CHECKOBJ<Player>(L, 2);
        Eluna::Push(L, player->IsGroupVisibleFor(target));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently in the same raid as another [Player] by object, `false` otherwise.
     *
     * @param [Player] player
     * @return bool isInSameRaidWith
     */
    int IsInSameRaidWith(lua_State* L, Player* player)
    {
        Player* target = Eluna::CHECKOBJ<Player>(L, 2);
        Eluna::Push(L, player->IsInSameRaidWith(target));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently in the same [Group] as another [Player] by object, `false` otherwise.
     *
     * @param [Player] player
     * @return bool isInSameGroupWith
     */
    int IsInSameGroupWith(lua_State* L, Player* player)
    {
        Player* target = Eluna::CHECKOBJ<Player>(L, 2);
        Eluna::Push(L, player->IsInSameGroupWith(target));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is eligible for Honor or XP gain by [Unit] specified, `false` otherwise.
     *
     * @param [Unit] unit
     * @return bool isHonorOrXPTarget
     */
    int IsHonorOrXPTarget(lua_State* L, Player* player)
    {
        Unit* victim = Eluna::CHECKOBJ<Unit>(L, 2);

        Eluna::Push(L, player->isHonorOrXPTarget(victim));
        return 1;
    }

    /**
     * Returns `true` if the [Player] can see anoter [Player] specified by object, `false` otherwise.
     *
     * @param [Player] player
     * @return bool isVisibleForPlayer
     */
    int IsVisibleForPlayer(lua_State* L, Player* player)
    {
        Player* target = Eluna::CHECKOBJ<Player>(L, 2);

        Eluna::Push(L, player->IsVisibleGloballyFor(target));
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently visible to other players, `false` if hidden via GM invisibility.
     *
     * @param [Player] player
     * @return bool isVisible
     */
    int IsGMVisible(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isGMVisible());
        return 1;
    }

    /**
     * Returns `true` if the [Player] has taxi cheat activated, `false` otherwise.
     *
     * @return bool isTaxiCheater
     */
    int IsTaxiCheater(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isTaxiCheater());
        return 1;
    }

    /**
     * Returns `true` if the [Player] has GM chat enabled, `false` otherwise.
     *
     * @param [Player] player
     * @return bool isGMChat
     */
    int IsGMChat(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isGMChat());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is accepting whispers, `false` otherwise.
     *
     * @return bool isAcceptingWhispers
     */
    int IsAcceptingWhispers(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isAcceptWhispers());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently rested, `false` otherwise.
     *
     * @return bool isRested
     */
    int IsRested(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetRestBonus() > 0.0f);
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently in a [BattleGround] queue, `false` otherwise.
     *
     * @return bool inBattlegroundQueue
     */
    int InBattlegroundQueue(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->InBattlegroundQueue());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently in an arena, `false` otherwise.
     *
     * @return bool inArena
     */
    int InArena(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->InArena());
        return 1;
    }

    /**
     * Returns `true` if the [Player] is currently in a [BattleGround], `false` otherwise.
     *
     * @return bool inBattleGround
     */
    int InBattleground(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->InBattleground());
        return 1;
    }

    /**
     * Returns `true` if the [Player] can block incomming attacks, `false` otherwise.
     *
     * @return bool canBlock
     */
    int CanBlock(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->CanBlock());
        return 1;
    }

    /**
     * Returns `true` if the [Player] can parry incomming attacks, `false` otherwise.
     *
     * @return bool canParry
     */
    int CanParry(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->CanParry());
        return 1;
    }

    /*int HasReceivedQuestReward(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->IsQuestRewarded(entry));
        return 1;
    }*/

    /*int IsOutdoorPvPActive(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->IsOutdoorPvPActive());
        return 1;
    }*/

    /*int IsImmuneToEnvironmentalDamage(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->IsImmuneToEnvironmentalDamage());
        return 1;
    }*/

    /*int InRandomLfgDungeon(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->inRandomLfgDungeon());
        return 1;
    }*/

    /*int IsUsingLfg(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->isUsingLfg());
        return 1;
    }*/

    /*int IsNeverVisible(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->IsNeverVisible());
        return 1;
    }*/

    /*int CanFlyInZone(lua_State* L, Player* player)
    {
        uint32 mapid = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 zone = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->IsKnowHowFlyIn(mapid, zone));
        return 1;
    }*/

    /*int HasPendingBind(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->PendingHasPendingBind());
        return 1;
    }*/

    /*int IsARecruiter(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->IsARecruiter() || (player->GetSession()->GetRecruiterId() != 0));
        return 1;
    }*/

    /**
     * Returns the amount of available specs the [Player] currently has
     *
     * @return uint8 specCount
     */
    int GetSpecsCount(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSpecsCount());
        return 1;
    }

    /**
     * Returns the [Player]s active spec ID
     *
     * @return uint32 specId
     */
    int GetActiveSpec(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetActiveSpec());
        return 1;
    }

    /**
     * Returns the normal phase of the player instead of the actual phase possibly containing GM phase
     *
     * @return uint32 phasemask
     */
    int GetPhaseMaskForSpawn(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetPhaseMaskForSpawn());
        return 1;
    }

    /**
     * Returns the [Player]s current amount of Achievement Points
     *
     * @return uint32 achievementPoints
     */
    int GetAchievementPoints(lua_State* L, Player* player)
    {
        uint32 count = 0;
        const CompletedAchievementMap& completedAchievements = player->GetAchievementMgr()->GetCompletedAchievements();
        for (auto& pair : completedAchievements)
        {
            AchievementEntry const* achievement = sAchievementStore.LookupEntry(pair.first);
            if (achievement)
            {
                count += achievement->points;
            }
        }

        Eluna::Push(L, count);
        return 1;
    }

    /**
     * Returns the [Player]s current amount of Achievements Completed
     *
     * @return uint32 achievementsCount
     */
    int GetCompletedAchievementsCount(lua_State* L, Player* player)
    {
        uint32 count = 0;
        bool countFeatsOfStrength = Eluna::CHECKVAL<bool>(L, 2, false);
        const CompletedAchievementMap& completedAchievements = player->GetAchievementMgr()->GetCompletedAchievements();
        for (auto& pair : completedAchievements)
        {
            AchievementEntry const* achievement = sAchievementStore.LookupEntry(pair.first);
            if (achievement && (achievement->categoryId != 81 || countFeatsOfStrength))
            {               
                    count++;             
            }
        }

        Eluna::Push(L, count);
        return 1;
    }

    /**
     * Returns the [Player]s current amount of Arena Points
     *
     * @return uint32 arenaPoints
     */
    int GetArenaPoints(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetArenaPoints());
        return 1;
    }

    /**
     * Returns the [Player]s current amount of Honor Points
     *
     * @return uint32 honorPoints
     */
    int GetHonorPoints(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetHonorPoints());
        return 1;
    }

    /**
     * Returns the [Player]s current shield block value
     *
     * @return uint32 blockValue
     */
    int GetShieldBlockValue(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetShieldBlockValue());
        return 1;
    }

    /**
     * Returns the [Player]s cooldown delay by specified [Spell] ID
     *
     * @param uint32 spellId
     * @return uint32 spellCooldownDelay
     */
    int GetSpellCooldownDelay(lua_State* L, Player* player)
    {
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, uint32(player->GetSpellCooldownDelay(spellId)));
        return 1;
    }

    /**
     * Returns the [Player]s current latency in MS
     *
     * @return uint32 latency
     */
    int GetLatency(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetLatency());
        return 1;
    }

    /**
     * Returns the faction ID the [Player] is currently flagged as champion for
     *
     * @return uint32 championingFaction
     */
    int GetChampioningFaction(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetChampioningFaction());
        return 1;
    }

    /**
     * Returns [Player]s original sub group
     *
     * @return uint8 subGroup
     */
    int GetOriginalSubGroup(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetOriginalSubGroup());
        return 1;
    }

    /**
     * Returns [Player]s original [Group] object
     *
     * @return [Group] group
     */
    int GetOriginalGroup(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetOriginalGroup());
        return 1;
    }

    /**
     * Returns a random Raid Member [Player] object within radius specified of [Player]
     *
     * @param float radius
     * @return [Player] player
     */
    int GetNextRandomRaidMember(lua_State* L, Player* player)
    {
        float radius = Eluna::CHECKVAL<float>(L, 2);

        Eluna::Push(L, player->GetNextRandomRaidMember(radius));
        return 1;
    }

    /**
     * Returns [Player]s current sub group
     *
     * @return uint8 subGroup
     */
    int GetSubGroup(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSubGroup());
        return 1;
    }

    /**
     * Returns [Group] invitation
     *
     * @return [Group] group
     */
    int GetGroupInvite(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetGroupInvite());
        return 1;
    }

    /**
     * Returns the [Player]'s experience points
     *
     * @return uint32 xp
     */
    int GetXP(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetUInt32Value(PLAYER_XP));
        return 1;
    }

    /**
     * Returns rested experience bonus
     *
     * @param uint32 xp
     * @return uint32 xpBonus
     */
    int GetXPRestBonus(lua_State* L, Player* player)
    {
        uint32 xp = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetXPRestBonus(xp));
        return 1;
    }

    /**
     * Returns the [Player]s current [BattleGround] type ID
     *
     * @return [BattleGroundTypeId] typeId
     */
    int GetBattlegroundTypeId(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetBattlegroundTypeId());
        return 1;
    }

    /**
     * Returns the [Player]s current [BattleGround] ID
     *
     * @return uint32 battleGroundId
     */
    int GetBattlegroundId(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetBattlegroundId());
        return 1;
    }

    /**
     * Returns the [Player]s reputation rank of faction specified
     *
     * @param uint32 faction
     * @return [ReputationRank] rank
     */
    int GetReputationRank(lua_State* L, Player* player)
    {
        uint32 faction = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetReputationRank(faction));
        return 1;
    }

    /**
     * Returns the [Player]s current level of intoxication
     *
     * @return uint16 drunkValue
     */
    int GetDrunkValue(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetDrunkValue());
        return 1;
    }

    /**
     * Returns skill temporary bonus value
     *
     * @param uint32 skill
     * @param int16 bonusVal
     */
    int GetSkillTempBonusValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetSkillTempBonusValue(skill));
        return 1;
    }

    /**
     * Returns skill permanent bonus value
     *
     * @param uint32 skill
     * @param int16 bonusVal
     */
    int GetSkillPermBonusValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetSkillPermBonusValue(skill));
        return 1;
    }

    /**
     * Returns skill value without bonus'
     *
     * @param uint32 skill
     * @return uint16 pureVal
     */
    int GetPureSkillValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetPureSkillValue(skill));
        return 1;
    }

    /**
     * Returns base skill value
     *
     * @param uint32 skill
     * @return uint16 baseVal
     */
    int GetBaseSkillValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetBaseSkillValue(skill));
        return 1;
    }

    /**
     * Returns skill value
     *
     * @param uint32 skill
     * @return uint16 val
     */
    int GetSkillValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetSkillValue(skill));
        return 1;
    }

    /**
     * Returns max value of specified skill without bonus'
     *
     * @param uint32 skill
     * @return uint16 pureVal
     */
    int GetPureMaxSkillValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetPureMaxSkillValue(skill));
        return 1;
    }

    /**
     * Returns max value of specified skill
     *
     * @param uint32 skill
     * @return uint16 val
     */
    int GetMaxSkillValue(lua_State* L, Player* player)
    {
        uint32 skill = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetMaxSkillValue(skill));
        return 1;
    }

    /**
     * Returns mana bonus from amount of intellect
     *
     * @return float bonus
     */
    int GetManaBonusFromIntellect(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetManaBonusFromIntellect());
        return 1;
    }

    /**
     * Returns health bonus from amount of stamina
     *
     * @return float bonus
     */
    int GetHealthBonusFromStamina(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetHealthBonusFromStamina());
        return 1;
    }

    /**
     * Returns raid or dungeon difficulty
     *
     * @param bool isRaid = true : argument is TrinityCore only
     * @return int32 difficulty
     */
    int GetDifficulty(lua_State* L, Player* player)
    {
        bool isRaid = Eluna::CHECKVAL<bool>(L, 2, true);
        Eluna::Push(L, player->GetDifficulty(isRaid));
        return 1;
    }

    /**
     * Returns the [Player]s current guild rank
     *
     * @return uint32 guildRank
     */
    int GetGuildRank(lua_State* L, Player* player) // TODO: Move to Guild Methods
    {
        Eluna::Push(L, player->GetRank());
        return 1;
    }

    /**
     * Returns the [Player]s free talent point amount
     *
     * @return uint32 freeTalentPointAmt
     */
    int GetFreeTalentPoints(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetFreeTalentPoints());
        return 1;
    }

    /**
     * Returns the name of the [Player]s current [Guild]
     *
     * @return string guildName
     */
    int GetGuildName(lua_State* L, Player* player)
    {
        if (!player->GetGuildId())
            return 1;
        Eluna::Push(L, eGuildMgr->GetGuildNameById(player->GetGuildId()));
        return 1;
    }

    /**
     * Returns the amount of reputation the [Player] has with the faction specified
     *
     * @param uint32 faction
     * @return int32 reputationAmt
     */
    int GetReputation(lua_State* L, Player* player)
    {
        uint32 faction = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetReputationMgr().GetReputation(faction));
        return 1;
    }

    /**
     * Returns [Unit] target combo points are on
     *
     * @return [Unit] target
     */
    int GetComboTarget(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetComboTarget());
        return 1;
    }

    /**
     * Returns [Player]'s combo points
     *
     * @return uint8 comboPoints
     */
    int GetComboPoints(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetComboPoints());
        return 1;
    }

    /**
     * Returns the amount of time the [Player] has spent ingame
     *
     * @return uint32 inGameTime
     */
    int GetInGameTime(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetInGameTime());
        return 1;
    }

    /**
     * Returns the status of the [Player]s [Quest] specified by entry ID
     *
     * @param uint32 questId
     * @return [QuestStatus] questStatus
     */
    int GetQuestStatus(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetQuestStatus(entry));
        return 1;
    }

    /**
     * Returns `true` if the [Player]s [Quest] specified by entry ID has been rewarded, `false` otherwise.
     *
     * @param uint32 questId
     * @return bool questRewardStatus
     */
    int GetQuestRewardStatus(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetQuestRewardStatus(questId));
        return 1;
    }

    /**
     * Returns [Quest] required [Creature] or [GameObject] count
     *
     * @param uint32 quest : entry of a quest
     * @param int32 entry : entry of required [Creature]
     * @return uint16 count
     */
    int GetReqKillOrCastCurrentCount(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);
        int32 entry = Eluna::CHECKVAL<int32>(L, 3);

        Eluna::Push(L, player->GetReqKillOrCastCurrentCount(questId, entry));
        return 1;
    }

    /**
     * Returns the quest level of the [Player]s [Quest] specified by object
     *
     * @param uint32 questId
     * @return [QuestStatus] questRewardStatus
     */
    int GetQuestLevel(lua_State* L, Player* player)
    {
        Quest* quest = Eluna::CHECKOBJ<Quest>(L, 2);

        Eluna::Push(L, player->GetQuestLevel(quest));
        return 1;
    }

    /**
     * Returns a [Player]s [Item] object by gear slot specified
     *
     * @param uint8 slot
     * @return [Item] item
     */
    int GetEquippedItemBySlot(lua_State* L, Player* player)
    {
        uint8 slot = Eluna::CHECKVAL<uint8>(L, 2);
        if (slot >= EQUIPMENT_SLOT_END)
            return 1;

        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        Eluna::Push(L, item);
        return 1;
    }

    /**
     * Returns the [Player]s current resting bonus
     *
     * @return float restBonus
     */
    int GetRestBonus(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetRestBonus());
        return 1;
    }

    /**
     * Returns active GM chat tag
     *
     * @return uint8 tag
     */
    int GetChatTag(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetChatTag());
        return 1;
    }

    /**
     * Returns an item in given bag on given slot.
     *
     * <pre>
     * Possible and most commonly used combinations:
     *
     * bag = 255
     * slots 0-18 equipment
     * slots 19-22 equipped bag slots
     * slots 23-38 backpack
     * slots 39-66 bank main slots
     * slots 67-74 bank bag slots
     * slots 86-117 keyring
     *
     * bag = 19-22
     * slots 0-35 for equipped bags
     *
     * bag = 67-74
     * slots 0-35 for bank bags
     * </pre>
     *
     * @param uint8 bag : the bag the [Item] is in, you can get this with [Item:GetBagSlot]
     * @param uint8 slot : the slot the [Item] is in within the bag, you can get this with [Item:GetSlot]
     * @return [Item] item : [Item] or nil
     */
    int GetItemByPos(lua_State* L, Player* player)
    {
        uint8 bag = Eluna::CHECKVAL<uint8>(L, 2);
        uint8 slot = Eluna::CHECKVAL<uint8>(L, 3);

        Eluna::Push(L, player->GetItemByPos(bag, slot));
        return 1;
    }

    /**
     * Returns an [Item] from the player by guid.
     *
     * The item can be equipped, in bags or in bank.
     *
     * @param ObjectGuid guid : an item guid
     * @return [Item] item
     */
    int GetItemByGUID(lua_State* L, Player* player)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 2);

        Eluna::Push(L, player->GetItemByGuid(guid));
        return 1;
    }

    /**
     * Returns the amount of mails in the player's mailbox.
     *
     * @return uint32 mailCount
     */
    int GetMailCount(lua_State* L, Player* player)
    {
        const CharacterCacheEntry* cache = sCharacterCache->GetCharacterCacheByGuid(player->GetGUID());
        if (cache)
        {
            Eluna::Push(L, static_cast<uint32>(cache->MailCount));
        }
        else
        {
            Eluna::Push(L, player->GetMailSize());
        }

        return 1;
    }

    /**
     * Returns a mailed [Item] by guid.
     *
     * @param ObjectGuid guid : an item guid
     * @return [Item] item
     */
    int GetMailItem(lua_State* L, Player* player)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 2);

        Eluna::Push(L, player->GetMItem(guid.GetCounter()));
        return 1;
    }

    /**
     * Returns an [Item] from the player by entry.
     *
     * The item can be equipped, in bags or in bank.
     *
     * @param uint32 entryId
     * @return [Item] item
     */
    int GetItemByEntry(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, player->GetItemByEntry(entry));
        return 1;
    }
    
    /**
     * Returns the database textID of the [WorldObject]'s gossip header text for the [Player]
     *
     * @param [WorldObject] object
     * @return uint32 textId : key to npc_text database table
     */
    int GetGossipTextId(lua_State* L, Player* player)
    {
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 2);
        Eluna::Push(L, player->GetGossipTextId(obj));
        return 1;
    }

    /**
     * Returns the [Player]s currently selected [Unit] object
     *
     * @return [Unit] unit
     */
    int GetSelection(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSelectedUnit());
        return 1;
    }

    /**
     * Returns the [Player]s GM Rank
     *
     * @return [AccountTypes] gmRank
     */
    int GetGMRank(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetSecurity());
        return 1;
    }

    /**
     * Returns the [Player]s amount of money in copper
     *
     * @return uint32 coinage
     */
    int GetCoinage(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetMoney());
        return 1;
    }

    /**
     * Returns the [Player]s current [Guild] ID
     *
     * @return uint32 guildId
     */
    int GetGuildId(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetGuildId());
        return 1;
    }

    /**
     * Returns the [Player]s [TeamId]
     *
     * @return [TeamId] teamId
     */
    int GetTeam(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetTeamId());
        return 1;
    }
    
    /**
     * Returns amount of the specified [Item] the [Player] has.
     *
     * @param uint32 entry : entry of the item
     * @param bool checkinBank = false : also counts the items in player's bank if true
     * @return uint32 itemamount
     */
    int GetItemCount(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        bool checkinBank = Eluna::CHECKVAL<bool>(L, 3, false);
        Eluna::Push(L, player->GetItemCount(entry, checkinBank));
        return 1;
    }

    /**
     * Returns the [Player]s lifetime Honorable Kills
     *
     * @return uint32 lifeTimeKils
     */
    int GetLifetimeKills(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
        return 1;
    }

    /**
     * Returns the [Player]s IP address
     *
     * @return string ip
     */
    int GetPlayerIP(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetRemoteAddress());
        return 1;
    }

    /**
     * Returns the [Player]s time played at current level
     *
     * @return uint32 currLevelPlayTime
     */
    int GetLevelPlayedTime(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetLevelPlayedTime());
        return 1;
    }

    /**
     * Returns the [Player]s total time played
     *
     * @return uint32 totalPlayTime
     */
    int GetTotalPlayedTime(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetTotalPlayedTime());
        return 1;
    }

    /**
     * Returns the [Player]s [Guild] object
     *
     * @return [Guild] guild
     */
    int GetGuild(lua_State* L, Player* player)
    {
        Eluna::Push(L, eGuildMgr->GetGuildById(player->GetGuildId()));
        return 1;
    }

    /**
     * Returns the [Player]s [Group] object
     *
     * @return [Group] group
     */
    int GetGroup(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetGroup());
        return 1;
    }

    /**
     * Returns the [Player]s account ID
     *
     * @return uint32 accountId
     */
    int GetAccountId(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetAccountId());
        return 1;
    }

    /**
     * Returns the [Player]s account name
     *
     * @return string accountName
     */
    int GetAccountName(lua_State* L, Player* player)
    {
        std::string accName;
        if (AccountMgr::GetName(player->GetSession()->GetAccountId(), accName))
            Eluna::Push(L, accName);
        return 1;
    }

    /**
     * Returns the [Player]s completed quest count
     *
     * @return int32 questcount
     */
    int GetCompletedQuestsCount(lua_State* L, Player* player)
    {
        uint32 count = player->GetRewardedQuestCount();

        Eluna::Push(L, count);
        return 1;
    }

    /**
     * Returns the [Player]s [Corpse] object
     *
     * @return [Corpse] corpse
     */
    int GetCorpse(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetCorpse());
        return 1;
    }

    /**
     * Returns the [Player]s database locale index
     *
     * @return int localeIndex
     */
    int GetDbLocaleIndex(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetSessionDbLocaleIndex());
        return 1;
    }

    /**
     * Returns the [Player]s game client locale
     *
     * @return [LocaleConstant] locale
     */
    int GetDbcLocale(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetSessionDbcLocale());
        return 1;
    }

    /**
    * Returns known taxi nodes (flight paths) that the player has unlocked.
    *
    * @return table nodes : A table containing the IDs of the known taxi nodes
    */
    int GetKnownTaxiNodes(lua_State* L, Player* player)
    {
        if (!player)
            return 0;

        lua_newtable(L);

        ByteBuffer data;
        player->m_taxi.AppendTaximaskTo(data, false);

        for (uint8 i = 0; i < TaxiMaskSize; i++)
        {
            uint32 mask;
            data >> mask;

            for (uint8 bit = 0; bit < 32; bit++)
            {
                if (mask & (1 << bit))
                {
                    uint8 nodeId = (i * 32) + bit + 1;
                    lua_pushinteger(L, nodeId);
                    lua_rawseti(L, -2, lua_rawlen(L, -2) + 1);
                }
            }
        }

        return 1;
    }

    /*int GetRecruiterId(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSession()->GetRecruiterId());
        return 1;
    }*/

    /*int GetSelectedPlayer(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSelectedPlayer());
        return 1;
    }*/

    /*int GetSelectedUnit(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetSelectedUnit());
        return 1;
    }*/

    /*int GetNearbyGameObject(lua_State* L, Player* player)
    {
        Eluna::Push(L, ChatHandler(player->GetSession()).GetNearbyGameObject());
        return 1;
    }*/
    
    /**
     * Locks the player controls and disallows all movement and casting.
     *
     * @param bool apply = true : lock if true and unlock if false
     */
    int SetPlayerLock(lua_State* L, Player* player)
    {
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);

        if (apply)
        {
            player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
            player->SetClientControl(player, 0);
        }
        else
        {
            player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_SILENCED);
            player->SetClientControl(player, 1);
        }
        return 0;
    }

    /**
     * Sets the [Player]s login flag to the flag specified
     *
     * @param uint32 flag
     */
    int SetAtLoginFlag(lua_State* L, Player* player)
    {
        uint32 flag = Eluna::CHECKVAL<uint32>(L, 2);

        player->SetAtLoginFlag((AtLoginFlags)flag);
        return 0;
    }

    /**
     * Sets the [Player]s sheathe state to the state specified
     *
     * @param uint32 sheatheState
     */
    int SetSheath(lua_State* L, Player* player)
    {
        uint32 sheathed = Eluna::CHECKVAL<uint32>(L, 2);
        if (sheathed >= MAX_SHEATH_STATE)
            return 0;

        player->SetSheath((SheathState)sheathed);
        return 0;
    }

    /**
     * Sets the [Player]s intoxication level to the level specified
     *
     * @param uint8 drunkValue
     */
    int SetDrunkValue(lua_State* L, Player* player)
    {
        uint8 newDrunkValue = Eluna::CHECKVAL<uint8>(L, 2);

        player->SetDrunkValue(newDrunkValue);
        return 0;
    }

    /**
     * Sets the [Player]s faction standing to that of the race specified
     *
     * @param uint8 raceId
     */
    int SetFactionForRace(lua_State* L, Player* player)
    {
        uint8 race = Eluna::CHECKVAL<uint8>(L, 2);

        player->SetFactionForRace(race);
        return 0;
    }

    /**
     * Sets (increases) skill of the [Player]
     *
     * @param uint16 id
     * @param uint16 step
     * @param uint16 currVal
     * @param uint16 maxVal
     */
    int SetSkill(lua_State* L, Player* player)
    {
        uint16 id = Eluna::CHECKVAL<uint16>(L, 2);
        uint16 step = Eluna::CHECKVAL<uint16>(L, 3);
        uint16 currVal = Eluna::CHECKVAL<uint16>(L, 4);
        uint16 maxVal = Eluna::CHECKVAL<uint16>(L, 5);

        player->SetSkill(id, currVal, maxVal, step);
        return 0;
    }

    /**
     * Sets the [Player]s guild rank to the rank specified
     *
     * @param uint8 rank
     */
    int SetGuildRank(lua_State* L, Player* player) // TODO: Move to Guild Methods
    {
        uint8 rank = Eluna::CHECKVAL<uint8>(L, 2);

        if (!player->GetGuildId())
            return 0;

        player->SetRank(rank);
        return 0;
    }

    /**
     * Sets the [Player]s free talent points to the amount specified for the current spec
     *
     * @param uint32 talentPointAmt
     */
    int SetFreeTalentPoints(lua_State* L, Player* player)
    {
        uint32 points = Eluna::CHECKVAL<uint32>(L, 2);

        player->SetFreeTalentPoints(points);
        player->SendTalentsInfoData(false);
        return 0;
    }

    /**
     * Sets the [Player]s reputation amount for the faction specified
     *
     * @param uint32 factionId
     * @param int32 reputationValue
     */
    int SetReputation(lua_State* L, Player* player)
    {
        uint32 faction = Eluna::CHECKVAL<uint32>(L, 2);
        int32 value = Eluna::CHECKVAL<int32>(L, 3);

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction);
        player->GetReputationMgr().SetReputation(factionEntry, value);
        return 0;
    }

    /**
     * Sets [Quest] state
     *
     * @param uint32 entry : entry of a quest
     * @param uint32 status
     */
    int SetQuestStatus(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 status = Eluna::CHECKVAL<uint32>(L, 3);
        if (status >= MAX_QUEST_STATUS)
            return 0;

        player->SetQuestStatus(entry, (QuestStatus)status);
        return 0;
    }

    /**
     * Sets the [Player]s rest bonus to the amount specified
     *
     * @param float restBonus
     */
    int SetRestBonus(lua_State* L, Player* player)
    {
        float bonus = Eluna::CHECKVAL<float>(L, 2);

        player->SetRestBonus(bonus);
        return 0;
    }

    /**
     * Toggles whether the [Player] accepts whispers or not
     *
     * @param bool acceptWhispers = true
     */
    int SetAcceptWhispers(lua_State* L, Player* player)
    {
        bool on = Eluna::CHECKVAL<bool>(L, 2, true);

        player->SetAcceptWhispers(on);
        return 0;
    }

    /**
     * Toggles PvP Death
     *
     * @param bool on = true
     */
    int SetPvPDeath(lua_State* L, Player* player)
    {
        bool on = Eluna::CHECKVAL<bool>(L, 2, true);

        player->SetPvPDeath(on);
        return 0;
    }

    /**
     * Toggles whether the [Player] has GM visibility on or off
     *
     * @param bool gmVisible = true
     */
    int SetGMVisible(lua_State* L, Player* player)
    {
        bool on = Eluna::CHECKVAL<bool>(L, 2, true);

        player->SetGMVisible(on);
        return 0;
    }

    /**
    * Sets the player's known taxi nodes (flight paths).
    *
    * @param table nodes : A table containing the taxi node IDs to set as known
    */
    int SetKnownTaxiNodes(lua_State* L, Player* player)
    {
        if (!player)
            return 0;
    
        if (!lua_istable(L, 2))
        {
            return 0;
        }
    
        lua_pushnil(L);
    
        while (lua_next(L, 2) != 0)
        {
            uint32 nodeId = luaL_checkinteger(L, -1);
    
            if (nodeId > 0) 
            {
                player->m_taxi.SetTaximaskNode(nodeId);
            }
    
            lua_pop(L, 1);
        }
    
        return 0;
    }

    /**
     * Toggles whether the [Player] has taxi cheat enabled or not
     *
     * @param bool taxiCheat = true
     */
    int SetTaxiCheat(lua_State* L, Player* player)
    {
        bool on = Eluna::CHECKVAL<bool>(L, 2, true);

        player->SetTaxiCheater(on);
        return 0;
    }

    /**
     * Toggle Blizz (GM) tag
     *
     * @param bool on = true
     */
    int SetGMChat(lua_State* L, Player* player)
    {
        bool on = Eluna::CHECKVAL<bool>(L, 2, true);

        player->SetGMChat(on);
        return 0;
    }

    /**
     * Toggles the [Player]s GM mode on or off
     *
     * @param bool setGmMode = true
     */
    int SetGameMaster(lua_State* L, Player* player)
    {
        bool on = Eluna::CHECKVAL<bool>(L, 2, true);

        player->SetGameMaster(on);
        return 0;
    }

    /**
     * Sets the [Player]s gender to gender specified
     *
     * - GENDER_MALE    = 0
     * - GENDER_FEMALE  = 1
     *
     * @param [Gender] gender
     */
    int SetGender(lua_State* L, Player* player)
    {
        uint32 _gender = Eluna::CHECKVAL<uint32>(L, 2);

        Gender gender;
        switch (_gender)
        {
            case 0:
                gender = GENDER_MALE;
                break;
            case 1:
                gender = GENDER_FEMALE;
                break;
            default:
                return luaL_argerror(L, 2, "valid Gender expected");
        }

        player->SetByteValue(UNIT_FIELD_BYTES_0, 2, gender);
        player->SetByteValue(PLAYER_BYTES_3, 0, gender);
        player->InitDisplayIds();
        return 0;
    }

    /**
     * Sets the [Player]s Arena Points to the amount specified
     *
     * @param uint32 arenaPoints
     */
    int SetArenaPoints(lua_State* L, Player* player)
    {
        uint32 arenaP = Eluna::CHECKVAL<uint32>(L, 2);
        player->SetArenaPoints(arenaP);
        return 0;
    }

    /**
     * Sets the [Player]s Honor Points to the amount specified
     *
     * @param uint32 honorPoints
     */
    int SetHonorPoints(lua_State* L, Player* player)
    {
        uint32 honorP = Eluna::CHECKVAL<uint32>(L, 2);
        player->SetHonorPoints(honorP);
        return 0;
    }

    /**
     * Sets the [Player]s amount of Lifetime Honorable Kills to the value specified
     *
     * @param uint32 honorableKills
     */
    int SetLifetimeKills(lua_State* L, Player* player)
    {
        uint32 val = Eluna::CHECKVAL<uint32>(L, 2);
        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, val);
        return 0;
    }

    /**
     * Sets the [Player]s amount of money to copper specified
     *
     * @param uint32 copperAmt
     */
    int SetCoinage(lua_State* L, Player* player)
    {
        uint32 amt = Eluna::CHECKVAL<uint32>(L, 2);
        player->SetMoney(amt);
        return 0;
    }

    /**
     * Sets the [Player]s home location to the location specified
     *
     * @param float x : X Coordinate
     * @param float y : Y Coordinate
     * @param float z : Z Coordinate
     * @param uint32 mapId : Map ID
     * @param uint32 areaId : Area ID
     */
    int SetBindPoint(lua_State* L, Player* player)
    {
        float x = Eluna::CHECKVAL<float>(L, 2);
        float y = Eluna::CHECKVAL<float>(L, 3);
        float z = Eluna::CHECKVAL<float>(L, 4);
        uint32 mapId = Eluna::CHECKVAL<uint32>(L, 5);
        uint32 areaId = Eluna::CHECKVAL<uint32>(L, 6);

        WorldLocation loc(mapId, x, y, z);
        player->SetHomebind(loc, areaId);
        return 0;
    }

    /**
     * Adds the specified title to the [Player]s list of known titles
     *
     * @param uint32 titleId
     */
    int SetKnownTitle(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        CharTitlesEntry const* t = sCharTitlesStore.LookupEntry(id);
        if (t)
            player->SetTitle(t, false);
        return 0;
    }

    /**
     * Adds the specified achievement to the [Player]s
     *
     * @param uint32 achievementid
     */
    int SetAchievement(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        AchievementEntry const* t = sAchievementStore.LookupEntry(id);
        if (t)
            player->CompletedAchievement(t);
        return 0;
    }

    /*int SetMovement(lua_State* L, Player* player)
    {
        int32 pType = Eluna::CHECKVAL<int32>(L, 2);

        player->SetMovement((PlayerMovementType)pType);
        return 0;
    }*/

    /**
     * Resets the [Player]s pets talent points
     */
    int ResetPetTalents(lua_State* /*L*/, Player* player)
    {
        Pet* pet = player->GetPet();
        Pet::resetTalentsForAllPetsOf(player, pet);
        if (pet)
            player->SendTalentsInfoData(true);
        return 0;
    }

    /**
     * Reset the [Player]s completed achievements
     */
    int ResetAchievements(lua_State* /*L*/, Player* player)
    {
        player->ResetAchievements();
        return 0;
    }
    
    /**
     * Shows the mailbox window to the player from specified guid.
     *
     * @param ObjectGuid guid = playerguid : guid of the mailbox window sender
     */
    int SendShowMailBox(lua_State* L, Player* player)
    {
        ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 2, player->GET_GUID());

        player->GetSession()->SendShowMailBox(guid);
        return 0;
    }

    /**
     * Adds or detracts from the [Player]s current Arena Points
     *
     * @param int32 amount
     */
    int ModifyArenaPoints(lua_State* L, Player* player)
    {
        int32 amount = Eluna::CHECKVAL<int32>(L, 2);

        player->ModifyArenaPoints(amount);
        return 0;
    }

    /**
     * Adds or detracts from the [Player]s current Honor Points
     *
     * @param int32 amount
     */
    int ModifyHonorPoints(lua_State* L, Player* player)
    {
        int32 amount = Eluna::CHECKVAL<int32>(L, 2);

        player->ModifyHonorPoints(amount);
        return 0;
    }

    /**
     * Saves the [Player] to the database
     */
    int SaveToDB(lua_State* /*L*/, Player* player)
    {
        player->SaveToDB(false, false);
        return 0;
    }

    /**
     * Sends a summon request to the player from the given summoner
     *
     * @param [Unit] summoner
     */
    int SummonPlayer(lua_State* L, Player* player)
    {
        Unit* summoner = Eluna::CHECKOBJ<Unit>(L, 2);

        float x, y, z;
        summoner->GetPosition(x,y,z);
        player->SetSummonPoint(summoner->GetMapId(), x, y, z);

        WorldPacket data(SMSG_SUMMON_REQUEST, 8 + 4 + 4);
        data << summoner->GET_GUID();
        data << uint32(summoner->GetZoneId());
        data << uint32(MAX_PLAYER_SUMMON_DELAY * IN_MILLISECONDS);
        player->GetSession()->SendPacket(&data);
        return 0;
    }

    /**
     * Mutes the [Player] for the amount of seconds specified
     *
     * @param uint32 muteTime
     */
    int Mute(lua_State* L, Player* player)
    {
        uint32 muteseconds = Eluna::CHECKVAL<uint32>(L, 2);
        /*const char* reason = luaL_checkstring(E, 2);*/ // Mangos does not have a reason field in database.

        time_t muteTime = GameTime::GetGameTime().count() + muteseconds;
        player->GetSession()->m_muteTime = muteTime;
        LoginDatabase.Execute("UPDATE account SET mutetime = {} WHERE id = {}", muteTime, player->GetSession()->GetAccountId());
        return 0;
    }

    /**
     * Rewards the given quest entry for the [Player] if he has completed it.
     *
     * @param uint32 entry : quest entry
     */
    int RewardQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if (!quest || player->GetQuestStatus(entry) != QUEST_STATUS_COMPLETE)
            return 0;

        player->RewardQuest(quest, 0, player);
        return 0;
    }

    /**
     * Sends an auction house window to the [Player] from the [Unit] specified
     *
     * @param [Unit] sender
     */
    int SendAuctionMenu(lua_State* L, Player* player)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 2);

        AuctionHouseEntry const* ahEntry = AuctionHouseMgr::GetAuctionHouseEntryFromFactionTemplate(unit->GetFaction());
        if (!ahEntry)
            return 0;

        WorldPacket data(MSG_AUCTION_HELLO, 12);
        data << unit->GET_GUID();
        data << uint32(ahEntry->houseId);
        data << uint8(1);
        player->GetSession()->SendPacket(&data);
        return 0;
    }

    /**
     * Sends a flightmaster window to the [Player] from the [Creature] specified
     *
     * @param [Creature] sender
     */
    int SendTaxiMenu(lua_State* L, Player* player)
    {
        Creature* creature = Eluna::CHECKOBJ<Creature>(L, 2);

        player->GetSession()->SendTaxiMenu(creature);
        return 0;
    }

    /**
     * Sends a spirit resurrection request to the [Player]
     */
    int SendSpiritResurrect(lua_State* /*L*/, Player* player)
    {
        player->GetSession()->SendSpiritResurrect();
        return 0;
    }

    /**
     * Sends a tabard vendor window to the [Player] from the [WorldObject] specified
     *
     * @param [WorldObject] sender
     */
    int SendTabardVendorActivate(lua_State* L, Player* player)
    {
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 2);

        player->GetSession()->SendTabardVendorActivate(obj->GET_GUID());
        return 0;
    }

    /**
     * Sends a bank window to the [Player] from the [WorldObject] specified.
     *
     * @param [WorldObject] sender
     */
    int SendShowBank(lua_State* L, Player* player)
    {
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 2);

        player->GetSession()->SendShowBank(obj->GET_GUID());
        return 0;
    }
    
    /**
     * Sends a vendor window to the [Player] from the [WorldObject] specified.
     *
     * @param [WorldObject] sender
     */
    int SendListInventory(lua_State* L, Player* player)
    {
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 2);
        uint32 vendorId = Eluna::CHECKVAL<uint32>(L, 3, 0);

        player->GetSession()->SendListInventory(obj->GET_GUID(), vendorId);
        return 0;
    }

    /**
     * Sends a trainer window to the [Player] from the [Creature] specified
     *
     * @param [Creature] sender
     */
    int SendTrainerList(lua_State* L, Player* player)
    {
        Creature* obj = Eluna::CHECKOBJ<Creature>(L, 2);

        player->GetSession()->SendTrainerList(obj->GET_GUID());
        return 0;
    }

    /**
     * Sends a guild invitation from the [Player]s [Guild] to the [Player] object specified
     *
     * @param [Player] invitee
     */
    int SendGuildInvite(lua_State* L, Player* player)
    {
        Player* plr = Eluna::CHECKOBJ<Player>(L, 2);

        if (Guild* guild = player->GetGuild())
            guild->HandleInviteMember(player->GetSession(), plr->GetName());
        return 0;
    }

    /**
     * Sends an update for the world state to the [Player]
     *
     * @param uint32 field
     * @param uint32 value
     */
    int SendUpdateWorldState(lua_State* L, Player* player)
    {
        uint32 field = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 value = Eluna::CHECKVAL<uint32>(L, 3);

        player->SendUpdateWorldState(field, value);
        return 0;
    }

    /**
     * Forces the [Player] to log out
     *
     * @param bool saveToDb = true
     */
    int LogoutPlayer(lua_State* L, Player* player)
    {
        bool save = Eluna::CHECKVAL<bool>(L, 2, true);

        player->GetSession()->LogoutPlayer(save);
        return 0;
    }

    /**
     * Forcefully removes the [Player] from a [BattleGround] raid group
     */
    int RemoveFromBattlegroundRaid(lua_State* /*L*/, Player* player)
    {
        player->RemoveFromBattlegroundOrBattlefieldRaid();
        return 0;
    }

    /**
     * Unbinds the [Player] from his instances except the one he currently is in.
     *
     * Difficulty is not used on classic.
     *
     * @param uint32 map = true
     * @param uint32 difficulty = 0
     */
    int UnbindInstance(lua_State* L, Player* player)
    {
        uint32 map = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 difficulty = Eluna::CHECKVAL<uint32>(L, 3, 0);

        if (difficulty < MAX_DIFFICULTY)
            sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUID(), map, Difficulty(difficulty), true, player);
        return 0;
    }

    /**
     * Unbinds the [Player] from his instances except the one he currently is in.
     */
    int UnbindAllInstances(lua_State* /*L*/, Player* player)
    {
        for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        {
            const BoundInstancesMap& binds = sInstanceSaveMgr->PlayerGetBoundInstances(player->GetGUID(), Difficulty(i));
            for (BoundInstancesMap::const_iterator itr = binds.begin(); itr != binds.end();)
            {
                if (itr->first != player->GetMapId())
                {
                    sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUID(), itr->first, Difficulty(i), true, player);
                    itr = binds.begin();
                }
                else
                {
                    ++itr;
                }
            }
        }

        return 0;
    }

    /**
     * Forces the [Player] to leave a [BattleGround]
     *
     * @param bool teleToEntry = true
     */
    int LeaveBattleground(lua_State* L, Player* player)
    {
        (void)L; // ensure that the variable is referenced in order to pass compiler checks
        player->LeaveBattleground();
        return 0;
    }

    /**
     * Repairs [Item] at specified position.
     *
     * @param uint16 position
     * @param bool cost = true
     * @param float discountMod = 1.0
     */
    int DurabilityRepair(lua_State* L, Player* player)
    {
        uint16 position = Eluna::CHECKVAL<uint16>(L, 2);
        bool takeCost = Eluna::CHECKVAL<bool>(L, 3, true);
        float discountMod = Eluna::CHECKVAL<float>(L, 4, 1.0f);

        player->DurabilityRepair(position, takeCost, discountMod, false);
        return 0;
    }

    /**
     * Repairs all [Item]s.
     *
     * @param bool takeCost = true
     * @param float discountMod = 1.0
     * @param bool guidBank = false
     */
    int DurabilityRepairAll(lua_State* L, Player* player)
    {
        bool takeCost = Eluna::CHECKVAL<bool>(L, 2, true);
        float discountMod = Eluna::CHECKVAL<float>(L, 3, 1.0f);
        bool guildBank = Eluna::CHECKVAL<bool>(L, 4, false);

        player->DurabilityRepairAll(takeCost, discountMod, guildBank);
        return 0;
    }

    /**
     * Sets durability loss for an [Item] in the specified slot
     *
     * @param int32 slot
     */
    int DurabilityPointLossForEquipSlot(lua_State* L, Player* player)
    {
        int32 slot = Eluna::CHECKVAL<int32>(L, 2);

        if (slot >= EQUIPMENT_SLOT_START && slot < EQUIPMENT_SLOT_END)
            player->DurabilityPointLossForEquipSlot((EquipmentSlots)slot);
        return 0;
    }

    /**
     * Sets durability loss on all [Item]s equipped
     *
     * If inventory is true, sets durability loss for [Item]s in bags
     *
     * @param int32 points
     * @param bool inventory = true
     */
    int DurabilityPointsLossAll(lua_State* L, Player* player)
    {
        int32 points = Eluna::CHECKVAL<int32>(L, 2);
        bool inventory = Eluna::CHECKVAL<bool>(L, 3, true);

        player->DurabilityPointsLossAll(points, inventory);
        return 0;
    }

    /**
     * Sets durability loss for the specified [Item]
     *
     * @param [Item] item
     * @param int32 points
     */
    int DurabilityPointsLoss(lua_State* L, Player* player)
    {
        Item* item = Eluna::CHECKOBJ<Item>(L, 2);
        int32 points = Eluna::CHECKVAL<int32>(L, 3);

        player->DurabilityPointsLoss(item, points);
        return 0;
    }

    /**
     * Damages specified [Item]
     *
     * @param [Item] item
     * @param double percent
     */
    int DurabilityLoss(lua_State* L, Player* player)
    {
        Item* item = Eluna::CHECKOBJ<Item>(L, 2);
        double percent = Eluna::CHECKVAL<double>(L, 3);

        player->DurabilityLoss(item, percent);
        return 0;
    }

    /**
     * Damages all [Item]s equipped. If inventory is true, damages [Item]s in bags
     *
     * @param double percent
     * @param bool inventory = true
     */
    int DurabilityLossAll(lua_State* L, Player* player)
    {
        double percent = Eluna::CHECKVAL<double>(L, 2);
        bool inventory = Eluna::CHECKVAL<bool>(L, 3, true);

        player->DurabilityLossAll(percent, inventory);
        return 0;
    }

    /**
     * Kills the [Player]
     */
    int KillPlayer(lua_State* /*L*/, Player* player)
    {
        player->KillPlayer();
        return 0;
    }

    /**
     * Forces the [Player] to leave a [Group]
     */
    int RemoveFromGroup(lua_State* /*L*/, Player* player)
    {
        if (!player->GetGroup())
            return 0;

        player->RemoveFromGroup();
        return 0;
    }

    /**
     * Returns the [Player]s accumulated talent reset cost
     *
     * @return uint32 resetCost
     */
    int ResetTalentsCost(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->resetTalentsCost());
        return 1;
    }

    /**
     * Resets the [Player]s talents
     *
     * @param bool noCost = true
     */
    int ResetTalents(lua_State* L, Player* player)
    {
        bool no_cost = Eluna::CHECKVAL<bool>(L, 2, true);

        player->resetTalents(no_cost);
        player->SendTalentsInfoData(false);
        return 0;
    }

    /**
     * Removes the [Spell] from the [Player]
     *
     * @param uint32 entry : entry of a [Spell]
     */
    int RemoveSpell(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        player->removeSpell(entry, SPEC_MASK_ALL, false);
        return 0;
    }

    /**
     * Clears the [Player]s combo points
     */
    int ClearComboPoints(lua_State* /*L*/, Player* player)
    {
        player->ClearComboPoints();
        return 0;
    }

    /**
     * Adds combo points to the [Player]
     *
     * @param [Unit] target
     * @param int8 count
     */
    int AddComboPoints(lua_State* L, Player* player)
    {
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        int8 count = Eluna::CHECKVAL<int8>(L, 3);

        player->AddComboPoints(target, count);
        return 0;
    }

    /**
     * Gives [Quest] monster talked to credit
     *
     * @param uint32 entry : entry of a [Creature]
     * @param [Creature] creature
     */
    int TalkedToCreature(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
        Creature* creature = Eluna::CHECKOBJ<Creature>(L, 3);

        player->TalkedToCreature(entry, creature->GET_GUID());
        return 0;
    }

    /**
     * Gives [Quest] monster killed credit
     *
     * @param uint32 entry : entry of a [Creature]
     */
    int KilledMonsterCredit(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        player->KilledMonsterCredit(entry, player->GET_GUID());
        return 0;
    }

    /**
     * Completes a [Quest] if in a [Group]
     *
     * @param uint32 quest : entry of a quest
     * @param [WorldObject] obj
     */
    int GroupEventHappens(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 3);

        player->GroupEventHappens(questId, obj);
        return 0;
    }

    /**
     * Completes the [Quest] if a [Quest] area is explored, or completes the [Quest]
     *
     * @param uint32 quest : entry of a [Quest]
     */
    int AreaExploredOrEventHappens(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);

        player->AreaExploredOrEventHappens(questId);
        return 0;
    }

    /**
     * Sets the given [Quest] entry failed for the [Player].
     *
     * @param uint32 entry : entry of a [Quest]
     */
    int FailQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        player->FailQuest(entry);
        return 0;
    }

    /**
     * Sets the given quest entry incomplete for the [Player].
     *
     * @param uint32 entry : quest entry
     */
    int IncompleteQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        player->IncompleteQuest(entry);
        return 0;
    }

    /**
     * Completes the given quest entry for the [Player] and tries to satisfy all quest requirements.
     *
     * The player should have the quest to complete it.
     *
     * @param uint32 entry : quest entry
     */
    int CompleteQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        // If player doesn't have the quest
        if (!quest || player->GetQuestStatus(entry) == QUEST_STATUS_NONE)
            return 0;

        // Add quest items for quests that require items
        for (uint8 x = 0; x < QUEST_ITEM_OBJECTIVES_COUNT; ++x)
        {
            uint32 id = quest->RequiredItemId[x];
            uint32 count = quest->RequiredItemCount[x];

            if (!id || !count)
                continue;

            uint32 curItemCount = player->GetItemCount(id, true);

            ItemPosCountVec dest;
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, id, count - curItemCount);
            if (msg == EQUIP_ERR_OK)
            {
                Item* item = player->StoreNewItem(dest, id, true);
                player->SendNewItem(item, count - curItemCount, true, false);
            }
        }

        // All creature/GO slain/cast (not required, but otherwise it will display "Creature slain 0/10")
        for (uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        {
            int32 creature = quest->RequiredNpcOrGo[i];
            uint32 creatureCount = quest->RequiredNpcOrGoCount[i];

            if (creature > 0)
            {
                if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creature))
                    for (uint16 z = 0; z < creatureCount; ++z)
                        player->KilledMonster(creatureInfo, ObjectGuid::Empty);
            }
            else if (creature < 0)
                for (uint16 z = 0; z < creatureCount; ++z)
                    player->KillCreditGO(creature);
        }


        // If the quest requires reputation to complete
        if (uint32 repFaction = quest->GetRepObjectiveFaction())
        {
            uint32 repValue = quest->GetRepObjectiveValue();
            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
            if (curRep < repValue)
                if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                    player->GetReputationMgr().SetReputation(factionEntry, repValue);
        }

        // If the quest requires a SECOND reputation to complete
        if (uint32 repFaction = quest->GetRepObjectiveFaction2())
        {
            uint32 repValue2 = quest->GetRepObjectiveValue2();
            uint32 curRep = player->GetReputationMgr().GetReputation(repFaction);
            if (curRep < repValue2)
                if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(repFaction))
                    player->GetReputationMgr().SetReputation(factionEntry, repValue2);
        }

        // If the quest requires money
        int32 ReqOrRewMoney = quest->GetRewOrReqMoney();
        if (ReqOrRewMoney < 0)
            player->ModifyMoney(-ReqOrRewMoney);

        player->CompleteQuest(entry);
        return 0;
    }

    /**
     * Tries to add the given quest entry for the [Player].
     *
     * @param uint32 entry : quest entry
     */
    int AddQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);
        if (!quest)
            return 0;

        // check item starting quest (it can work incorrectly if added without item in inventory)
        ItemTemplateContainer const* itc = sObjectMgr->GetItemTemplateStore();
        ItemTemplateContainer::const_iterator result = find_if(itc->begin(), itc->end(), Finder<uint32, ItemTemplate>(entry, &ItemTemplate::StartQuest));

        if (result != itc->end())
            return 0;

        // ok, normal (creature/GO starting) quest
        if (player->CanAddQuest(quest, true))
            player->AddQuestAndCheckCompletion(quest, NULL);

        return 0;
    }

    /**
     * Removes the given quest entry from the [Player].
     *
     * @param uint32 entry : quest entry
     */
    int RemoveQuest(lua_State* L, Player* player)
    {
        uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

        Quest const* quest = eObjectMgr->GetQuestTemplate(entry);

        if (!quest)
            return 0;

        // remove all quest entries for 'entry' from quest log
        for (uint8 slot = 0; slot < MAX_QUEST_LOG_SIZE; ++slot)
        {
            uint32 logQuest = player->GetQuestSlotQuestId(slot);
            if (logQuest == entry)
            {
                player->SetQuestSlot(slot, 0);

                // we ignore unequippable quest items in this case, its' still be equipped
                player->TakeQuestSourceItem(logQuest, false);

                if (quest->HasFlag(QUEST_FLAGS_FLAGS_PVP))
                {
                    player->pvpInfo.IsHostile = player->pvpInfo.IsInHostileArea || player->HasPvPForcingQuest();
                    player->UpdatePvPState();
                }
            }
        }

        player->RemoveActiveQuest(entry, false);
        player->RemoveRewardedQuest(entry);
        return 0;
    }

    /**
     * Sends whisper text from the [Player]
     *
     * @param string text
     * @param uint32 lang : language the [Player] will speak
     * @param [Player] receiver : is the [Player] that will receive the whisper, if TrinityCore
     * @param ObjectGuid guid : is the GUID of a [Player] that will receive the whisper, not TrinityCore
     */
    int Whisper(lua_State* L, Player* player)
    {
        std::string text = Eluna::CHECKVAL<std::string>(L, 2);
        uint32 lang = Eluna::CHECKVAL<uint32>(L, 3);
        Player* receiver = Eluna::CHECKOBJ<Player>(L, 4);
        player->Whisper(text, (Language)lang, receiver);
        return 0;
    }

    /**
     * Sends a text emote from the [Player]
     *
     * @param string emoteText
     */
    int TextEmote(lua_State* L, Player* player)
    {
        std::string text = Eluna::CHECKVAL<std::string>(L, 2);

        player->TextEmote(text);
        return 0;
    }

    /**
     * Sends yell text from the [Player]
     *
     * @param string text : text for the [Player] to yells
     * @param uint32 lang : language the [Player] will speak
     */
    int Yell(lua_State* L, Player* player)
    {
        std::string text = Eluna::CHECKVAL<std::string>(L, 2);
        uint32 lang = Eluna::CHECKVAL<uint32>(L, 3);
        player->Yell(text, (Language)lang);
        return 0;
    }

    /**
     * Sends say text from the [Player]
     *
     * @param string text : text for the [Player] to say
     * @param uint32 lang : language the [Player] will speak
     */
    int Say(lua_State* L, Player* player)
    {
        std::string text = Eluna::CHECKVAL<std::string>(L, 2);
        uint32 lang = Eluna::CHECKVAL<uint32>(L, 3);
        player->Say(text, (Language)lang);
        return 0;
    }

    /**
     * Gives the [Player] experience
     *
     * @param uint32 xp : experience to give
     * @param [Unit] victim = nil
     */
    int GiveXP(lua_State* L, Player* player)
    {
        uint32 xp = Eluna::CHECKVAL<uint32>(L, 2);
        Unit* victim = Eluna::CHECKOBJ<Unit>(L, 3, false);

        player->GiveXP(xp, victim);
        return 0;
    }

    /**
     * Toggle the [Player]s 'Do Not Disturb' flag
     */
    int ToggleDND(lua_State* /*L*/, Player* player)
    {
        player->ToggleDND();
        return 0;
    }

    /**
     * Toggle the [Player]s 'Away From Keyboard' flag
     */
    int ToggleAFK(lua_State* /*L*/, Player* player)
    {
        player->ToggleAFK();
        return 0;
    }

    /**
     * Equips the given item or item entry to the given slot. Returns the equipped item or nil.
     *
     *     enum EquipmentSlots // 19 slots
     *     {
     *         EQUIPMENT_SLOT_START        = 0,
     *         EQUIPMENT_SLOT_HEAD         = 0,
     *         EQUIPMENT_SLOT_NECK         = 1,
     *         EQUIPMENT_SLOT_SHOULDERS    = 2,
     *         EQUIPMENT_SLOT_BODY         = 3,
     *         EQUIPMENT_SLOT_CHEST        = 4,
     *         EQUIPMENT_SLOT_WAIST        = 5,
     *         EQUIPMENT_SLOT_LEGS         = 6,
     *         EQUIPMENT_SLOT_FEET         = 7,
     *         EQUIPMENT_SLOT_WRISTS       = 8,
     *         EQUIPMENT_SLOT_HANDS        = 9,
     *         EQUIPMENT_SLOT_FINGER1      = 10,
     *         EQUIPMENT_SLOT_FINGER2      = 11,
     *         EQUIPMENT_SLOT_TRINKET1     = 12,
     *         EQUIPMENT_SLOT_TRINKET2     = 13,
     *         EQUIPMENT_SLOT_BACK         = 14,
     *         EQUIPMENT_SLOT_MAINHAND     = 15,
     *         EQUIPMENT_SLOT_OFFHAND      = 16,
     *         EQUIPMENT_SLOT_RANGED       = 17,
     *         EQUIPMENT_SLOT_TABARD       = 18,
     *         EQUIPMENT_SLOT_END          = 19
     *     };
     *
     *     enum InventorySlots // 4 slots
     *     {
     *         INVENTORY_SLOT_BAG_START    = 19,
     *         INVENTORY_SLOT_BAG_END      = 23
     *     };
     *
     * @proto equippedItem = (item, slot)
     * @proto equippedItem = (entry, slot)
     * @param [Item] item : item to equip
     * @param uint32 entry : entry of the item to equip
     * @param uint32 slot : equipment slot to equip the item to The slot can be [EquipmentSlots] or [InventorySlots]
     * @return [Item] equippedItem : item or nil if equipping failed
     */
    int EquipItem(lua_State* L, Player* player)
    {
        uint16 dest = 0;
        Item* item = Eluna::CHECKOBJ<Item>(L, 2, false);
        uint32 slot = Eluna::CHECKVAL<uint32>(L, 3);

        if (slot >= INVENTORY_SLOT_BAG_END)
            return 1;

        if (!item)
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
            item = Item::CreateItem(entry, 1, player);
            if (!item)
                return 1;

            InventoryResult result = player->CanEquipItem(slot, dest, item, false);
            if (result != EQUIP_ERR_OK)
            {
                delete item;
                return 1;
            }
            player->ItemAddedQuestCheck(entry, 1);
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM, entry, 1);
        }
        else
        {
            InventoryResult result = player->CanEquipItem(slot, dest, item, false);
            if (result != EQUIP_ERR_OK)
                return 1;
            player->RemoveItem(item->GetBagSlot(), item->GetSlot(), true);
        }

        Eluna::Push(L, player->EquipItem(dest, item, true));
        player->AutoUnequipOffhandIfNeed();
        return 1;
    }
    
    /**
     * Returns true if the player can equip the given [Item] or item entry to the given slot, false otherwise.
     *
     * @proto canEquip = (item, slot)
     * @proto canEquip = (entry, slot)
     * @param [Item] item : item to equip
     * @param uint32 entry : entry of the item to equip
     * @param uint32 slot : equipment slot to test
     * @return bool canEquip
     */
    int CanEquipItem(lua_State* L, Player* player)
    {
        Item* item = Eluna::CHECKOBJ<Item>(L, 2, false);
        uint32 slot = Eluna::CHECKVAL<uint32>(L, 3);
        if (slot >= EQUIPMENT_SLOT_END)
        {
            Eluna::Push(L, false);
            return 1;
        }

        if (!item)
        {
            uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
            uint16 dest;
            InventoryResult msg = player->CanEquipNewItem(slot, dest, entry, false);
            if (msg != EQUIP_ERR_OK)
            {
                Eluna::Push(L, false);
                return 1;
            }
        }
        else
        {
            uint16 dest;
            InventoryResult msg = player->CanEquipItem(slot, dest, item, false);
            if (msg != EQUIP_ERR_OK)
            {
                Eluna::Push(L, false);
                return 1;
            }
        }
        Eluna::Push(L, true);
        return 1;
    }

    /**
     * Removes a title by ID from the [Player]s list of known titles
     *
     * @param uint32 titleId
     */
    int UnsetKnownTitle(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        CharTitlesEntry const* t = sCharTitlesStore.LookupEntry(id);
        if (t)
            player->SetTitle(t, true);
        return 0;
    }

    /**
     * Advances all of the [Player]s weapon skills to the maximum amount available
     */
    int AdvanceSkillsToMax(lua_State* /*L*/, Player* player)
    {
        player->UpdateSkillsToMaxSkillsForLevel();
        return 0;
    }

    /**
     * Advances all of the [Player]s skills to the amount specified
     *
     * @param uint32 skillStep
     */
    int AdvanceAllSkills(lua_State* L, Player* player)
    {
        uint32 step = Eluna::CHECKVAL<uint32>(L, 2);

        if (!step)
            return 0;

        for (uint32 i = 0; i < sSkillLineStore.GetNumRows(); ++i)
        {
            if (SkillLineEntry const* entry = sSkillLineStore.LookupEntry(i))
            {
                if (entry->categoryId == SKILL_CATEGORY_LANGUAGES || entry->categoryId == SKILL_CATEGORY_GENERIC)
                    continue;

                if (player->HasSkill(entry->id))
                    player->UpdateSkill(entry->id, step);
            }
        }

        return 0;
    }

    /**
     * Advances a [Player]s specific skill to the amount specified
     *
     * @param uint32 skillId
     * @param uint32 skillStep
     */
    int AdvanceSkill(lua_State* L, Player* player)
    {
        uint32 _skillId = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 _step = Eluna::CHECKVAL<uint32>(L, 3);
        if (_skillId && _step)
        {
            if (player->HasSkill(_skillId))
                player->UpdateSkill(_skillId, _step);
        }
        return 0;
    }

    /**
     * Teleports a [Player] to the location specified
     *
     * @param uint32 mappId
     * @param float xCoord
     * @param float yCoord
     * @param float zCoord
     * @param float orientation
     */
    int Teleport(lua_State* L, Player* player)
    {
        uint32 mapId = Eluna::CHECKVAL<uint32>(L, 2);
        float x = Eluna::CHECKVAL<float>(L, 3);
        float y = Eluna::CHECKVAL<float>(L, 4);
        float z = Eluna::CHECKVAL<float>(L, 5);
        float o = Eluna::CHECKVAL<float>(L, 6);

        if (player->IsInFlight())
        {
            player->GetMotionMaster()->MovementExpired();
            player->m_taxi.ClearTaxiDestinations();
        }

        Eluna::Push(L, player->TeleportTo(mapId, x, y, z, o));
        return 1;
    }

    /**
     * Adds a specified number of lifetime honorable kills to the [Player].
     *
     * @param [Player] player
     * @param uint32 kills
     */
    int AddLifetimeKills(lua_State* L, Player* player)
    {
        uint32 val = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 currentKills = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, currentKills + val);
        return 0;
    }
    
    /**
     * Adds the given amount of the specified item entry to the player.
     *
     * @param uint32 entry : entry of the item to add
     * @param uint32 itemCount = 1 : amount of the item to add
     * @return [Item] item : the item that was added or nil
     */
    int AddItem(lua_State* L, Player* player)
    {
        uint32 itemId = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 itemCount = Eluna::CHECKVAL<uint32>(L, 3, 1);

        uint32 noSpaceForCount = 0;
        ItemPosCountVec dest;
        InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, itemCount, &noSpaceForCount);
        if (msg != EQUIP_ERR_OK)
            itemCount -= noSpaceForCount;

        if (itemCount == 0 || dest.empty())
            return 1;

        Item* item = player->StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId));
        if (item)
            player->SendNewItem(item, itemCount, true, false);
        Eluna::Push(L, item);

        return 1;
    }
    
    /**
     * Removes the given amount of the specified [Item] from the player.
     *
     * @proto (item, itemCount)
     * @proto (entry, itemCount)
     * @param [Item] item : item to remove
     * @param uint32 entry : entry of the item to remove
     * @param uint32 itemCount = 1 : amount of the item to remove
     */
    int RemoveItem(lua_State* L, Player* player)
    {
        Item* item = Eluna::CHECKOBJ<Item>(L, 2, false);
        uint32 itemCount = Eluna::CHECKVAL<uint32>(L, 3);
        if (!item)
        {
            uint32 itemId = Eluna::CHECKVAL<uint32>(L, 2);
            player->DestroyItemCount(itemId, itemCount, true);
        }
        else
        {
            bool all = itemCount >= item->GetCount();
            player->DestroyItemCount(item, itemCount, true);
            if (all)
                Eluna::CHECKOBJ<ElunaObject>(L, 2)->Invalidate();
        }
        return 0;
    }

    /**
     * Removes specified amount of lifetime kills
     *
     * @param uint32 val : kills to remove
     */
    int RemoveLifetimeKills(lua_State* L, Player* player)
    {
        uint32 val = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 currentKills = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
        if (val > currentKills)
            val = currentKills;
        player->SetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS, currentKills - val);
        return 0;
    }

    /**
     * Resets cooldown of the specified spell
     *
     * @param uint32 spellId
     * @param bool update = true
     */
    int ResetSpellCooldown(lua_State* L, Player* player)
    {
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);
        bool update = Eluna::CHECKVAL<bool>(L, 3, true);
        player->RemoveSpellCooldown(spellId, update);
        return 0;
    }

    /**
     * Resets cooldown of the specified category
     *
     * @param uint32 category
     * @param bool update = true
     */
    int ResetTypeCooldowns(lua_State* L, Player* player)
    {
        uint32 category = Eluna::CHECKVAL<uint32>(L, 2);
        bool update = Eluna::CHECKVAL<bool>(L, 3, true);
        (void)update; // ensure that the variable is referenced in order to pass compiler checks

        player->RemoveCategoryCooldown(category);
        return 0;
    }

    /**
     * Resets all of the [Player]'s cooldowns
     */
    int ResetAllCooldowns(lua_State* /*L*/, Player* player)
    {
        player->RemoveAllSpellCooldown();
        return 0;
    }

    /**
     * Sends a Broadcast Message to the [Player]
     *
     * @param string message
     */
    int SendBroadcastMessage(lua_State* L, Player* player)
    {
        const char* message = Eluna::CHECKVAL<const char*>(L, 2);
        if (std::string(message).length() > 0)
            ChatHandler(player->GetSession()).SendSysMessage(message);
        return 0;
    }

    /**
     * Sends an Area Trigger Message to the [Player]
     *
     * @param string message
     */
    int SendAreaTriggerMessage(lua_State* L, Player* player)
    {
        std::string msg = Eluna::CHECKVAL<std::string>(L, 2);
        if (msg.length() > 0)
            player->GetSession()->SendAreaTriggerMessage("{}", msg.c_str());
        return 0;
    }

    /**
     * Sends a Notification to the [Player]
     *
     * @param string message
     */
    int SendNotification(lua_State* L, Player* player)
    {
        std::string msg = Eluna::CHECKVAL<std::string>(L, 2);
        if (msg.length() > 0)
            ChatHandler(player->GetSession()).SendNotification("{}", msg);
        return 0;
    }

    /**
     * Sends a [WorldPacket] to the [Player]
     *
     * @param [WorldPacket] packet
     * @param bool selfOnly = true
     */
    int SendPacket(lua_State* L, Player* player)
    {
        WorldPacket* data = Eluna::CHECKOBJ<WorldPacket>(L, 2);
        bool selfOnly = Eluna::CHECKVAL<bool>(L, 3, true);
        if (selfOnly)
            player->GetSession()->SendPacket(data);
        else
            player->SendMessageToSet(data, true);
        return 0;
    }

    /**
     * Sends addon message to the [Player] receiver
     *
     * @param string prefix
     * @param string message
     * @param [ChatMsg] channel
     * @param [Player] receiver
     *
     */
    int SendAddonMessage(lua_State* L, Player* player)
    {
        std::string prefix = Eluna::CHECKVAL<std::string>(L, 2);
        std::string message = Eluna::CHECKVAL<std::string>(L, 3);
        uint8 channel = Eluna::CHECKVAL<uint8>(L, 4);
        Player* receiver = Eluna::CHECKOBJ<Player>(L, 5);

        std::string fullmsg = prefix + "\t" + message;

        WorldPacket data(SMSG_MESSAGECHAT, 100);
        data << uint8(channel);
        data << int32(LANG_ADDON);
        data << player->GET_GUID();
        data << uint32(0);
        data << receiver->GET_GUID();
        data << uint32(fullmsg.length() + 1);
        data << fullmsg;
        data << uint8(0);
        receiver->GetSession()->SendPacket(&data);
        return 0;
    }

    /**
     * Kicks the [Player] from the server
     */
    int KickPlayer(lua_State* /*L*/, Player* player)
    {
        player->GetSession()->KickPlayer();
        return 0;
    }

    /**
     * Adds or subtracts from the [Player]s money in copper
     *
     * @param int32 copperAmt : negative to remove, positive to add
     */
    int ModifyMoney(lua_State* L, Player* player)
    {
        int32 amt = Eluna::CHECKVAL<int32>(L, 2);

        player->ModifyMoney(amt);
        return 1;
    }

    /**
     * Teaches the [Player] the [Spell] specified by entry ID
     *
     * @param uint32 spellId
     */
    int LearnSpell(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        player->learnSpell(id);
        return 0;
    }

    /**
     * Learn the [Player] the talent specified by talent_id and talentRank
     *
     * @param uint32 talent_id
     * @param uint32 talentRank
     */
    int LearnTalent(lua_State* L, Player* player)
    {
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 rank = Eluna::CHECKVAL<uint32>(L, 3);

        player->LearnTalent(id, rank);
        player->SendTalentsInfoData(false);

        return 0;
    }
    /**
    * Run a chat command as if the player typed it into the chat
    *
    * @param string command: text to display in chat or console
    */
    int RunCommand(lua_State* L, Player* player)
    {
        auto command = Eluna::CHECKVAL<std::string>(L, 2);

        // In _ParseCommands which is used below no leading . or ! is allowed for the command string.
        if (command[0] == '.' || command[0] == '!') {
            command = command.substr(1);
        }

        auto handler = ChatHandler(player->GetSession());
        handler._ParseCommands(command);

        return 0;
    }

    /**
    * Adds a glyph specified by `glyphId` to the [Player]'s current talent specialization into the slot with the index `slotIndex`
    *
    * @param uint32 glyphId
    * @param uint32 slotIndex
    */
    int SetGlyph(lua_State* L, Player* player)
    {
        uint32 glyphId = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 slotIndex = Eluna::CHECKVAL<uint32>(L, 3);

        player->SetGlyph(slotIndex, glyphId, true);
        player->SendTalentsInfoData(false); // Also handles GlyphData

        return 0;
    }

    /**
    * Returns the glyph ID in the specified glyph slot of the [Player]'s current talent specialization.
    *
    * @param [uint32] slotIndex
    * @return [uint32] glyphId
    */
    int GetGlyph(lua_State* L, Player* player)
    {
        auto slotIndex = Eluna::CHECKVAL<uint32>(L, 2);
        Eluna::Push(L,player->GetGlyph(slotIndex));
        return 1;
    }

    /**
     * Remove cooldowns on spells that have less than 10 minutes of cooldown from the [Player], similarly to when you enter an arena.
     */
    int RemoveArenaSpellCooldowns(lua_State* /*L*/, Player* player)
    {
        player->RemoveArenaSpellCooldowns();
        return 0;
    }

    /**
     * Resurrects the [Player].
     *
     * @param float healthPercent = 100.0f
     * @param bool ressSickness = false
     */
    int ResurrectPlayer(lua_State* L, Player* player)
    {
        float percent = Eluna::CHECKVAL<float>(L, 2, 100.0f);
        bool sickness = Eluna::CHECKVAL<bool>(L, 3, false);
        player->ResurrectPlayer(percent, sickness);
        player->SpawnCorpseBones();
        return 0;
    }

    /**
     * Adds a new item to the gossip menu shown to the [Player] on next call to [Player:GossipSendMenu].
     *
     * sender and intid are numbers which are passed directly to the gossip selection handler. Internally they are partly used for the database gossip handling.  
     * code specifies whether to show a box to insert text to. The player inserted text is passed to the gossip selection handler.  
     * money specifies an amount of money the player needs to have to click the option. An error message is shown if the player doesn't have enough money.  
     * Note that the money amount is only checked client side and is not removed from the player either. You will need to check again in your code before taking action.
     *
     * See also: [Player:GossipSendMenu], [Player:GossipAddQuests], [Player:GossipComplete], [Player:GossipClearMenu]
     *
     * @param uint32 icon : number that specifies used icon
     * @param string msg : label on the gossip item
     * @param uint32 sender : number passed to gossip handlers
     * @param uint32 intid : number passed to gossip handlers
     * @param bool code = false : show text input on click if true
     * @param string popup = nil : if non empty string, a popup with given text shown on click
     * @param uint32 money = 0 : required money in copper
     */
    int GossipMenuAddItem(lua_State* L, Player* player)
    {
        uint32 _icon = Eluna::CHECKVAL<uint32>(L, 2);
        const char* msg = Eluna::CHECKVAL<const char*>(L, 3);
        uint32 _sender = Eluna::CHECKVAL<uint32>(L, 4);
        uint32 _intid = Eluna::CHECKVAL<uint32>(L, 5);
        bool _code = Eluna::CHECKVAL<bool>(L, 6, false);
        const char* _promptMsg = Eluna::CHECKVAL<const char*>(L, 7, "");
        uint32 _money = Eluna::CHECKVAL<uint32>(L, 8, 0);
        if (player->PlayerTalkClass->GetGossipMenu().GetMenuItemCount() < GOSSIP_MAX_MENU_ITEMS)
        {
            player->PlayerTalkClass->GetGossipMenu().AddMenuItem(-1, _icon, msg, _sender, _intid, _promptMsg, _money,
                                                                 _code);
        }
        else
        {
            return luaL_error(L, "GossipMenuItem not added. Reached Max amount of possible GossipMenuItems in this GossipMenu");
        }
        return 0;
    }

    /**
     * Closes the [Player]s currently open Gossip Menu.
     *
     * See also: [Player:GossipMenuAddItem], [Player:GossipAddQuests], [Player:GossipSendMenu], [Player:GossipClearMenu]
     */
    int GossipComplete(lua_State* /*L*/, Player* player)
    {
        player->PlayerTalkClass->SendCloseGossip();
        return 0;
    }

    /**
     * Sends the current gossip items of the player to him as a gossip menu with header text from the given textId.
     *
     * If sender is a [Player] then menu_id is mandatory, otherwise it is not used for anything.
     * menu_id is the ID used to trigger the OnGossipSelect registered for players. See [Global:RegisterPlayerGossipEvent]
     *
     * See also: [Player:GossipMenuAddItem], [Player:GossipAddQuests], [Player:GossipComplete], [Player:GossipClearMenu]
     *
     * @proto (npc_text, sender)
     * @proto (npc_text, sender, menu_id)
     * @param uint32 npc_text : entry ID of a header text in npc_text database table, common default is 100
     * @param [Object] sender : object acting as the source of the sent gossip menu
     * @param uint32 menu_id : if sender is a [Player] then menu_id is mandatory
     */
    int GossipSendMenu(lua_State* L, Player* player)
    {
        uint32 npc_text = Eluna::CHECKVAL<uint32>(L, 2);
        Object* sender = Eluna::CHECKOBJ<Object>(L, 3);
        if (sender->GetTypeId() == TYPEID_PLAYER)
        {
            uint32 menu_id = Eluna::CHECKVAL<uint32>(L, 4);
            player->PlayerTalkClass->GetGossipMenu().SetMenuId(menu_id);
        }
        player->PlayerTalkClass->SendGossipMenu(npc_text, sender->GET_GUID());
        return 0;
    }

    /**
     * Clears the [Player]s current gossip item list.
     *
     * See also: [Player:GossipMenuAddItem], [Player:GossipSendMenu], [Player:GossipAddQuests], [Player:GossipComplete]
     * 
     *     Note: This is needed when you show a gossip menu without using gossip hello or select hooks which do this automatically.
     *     Usually this is needed when using [Player] is the sender of a Gossip Menu.
     */
    int GossipClearMenu(lua_State* /*L*/, Player* player)
    {
        player->PlayerTalkClass->ClearMenus();
        return 0;
    }

    /**
     * Attempts to start the taxi/flying to the given pathID
     *
     * @param uint32 pathId : pathId from DBC or [Global:AddTaxiPath]
     */
    int StartTaxi(lua_State* L, Player* player)
    {
        uint32 pathId = Eluna::CHECKVAL<uint32>(L, 2);

        player->ActivateTaxiPathTo(pathId);
        return 0;
    }

    /**
     * Sends POI to the location on your map
     *
     * @param float x
     * @param float y
     * @param uint32 icon : map icon to show
     * @param uint32 flags
     * @param uint32 data
     * @param string iconText
     */
    int GossipSendPOI(lua_State* L, Player* player)
    {
        float x = Eluna::CHECKVAL<float>(L, 2);
        float y = Eluna::CHECKVAL<float>(L, 3);
        uint32 icon = Eluna::CHECKVAL<uint32>(L, 4);
        uint32 flags = Eluna::CHECKVAL<uint32>(L, 5);
        uint32 data = Eluna::CHECKVAL<uint32>(L, 6);
        std::string iconText = Eluna::CHECKVAL<std::string>(L, 7);

        WorldPacket packet(SMSG_GOSSIP_POI, 4 + 4 + 4 + 4 + 4 + 10);
        packet << flags;
        packet << x;
        packet << y;
        packet << icon;
        packet << data;
        packet << iconText;
        player->GetSession()->SendPacket(&packet);
        return 0;
    }

    /**
     * Adds the gossip items to the [Player]'s gossip for the quests the given [WorldObject] can offer to the player.
     *
     * @param [WorldObject] source : a questgiver with quests
     */
    int GossipAddQuests(lua_State* L, Player* player)
    {
        WorldObject* source = Eluna::CHECKOBJ<WorldObject>(L, 2);

        if (source->GetTypeId() == TYPEID_UNIT)
        {
            if (source->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_QUESTGIVER)
                player->PrepareQuestMenu(source->GET_GUID());
        }
        else if (source->GetTypeId() == TYPEID_GAMEOBJECT)
        {
            if (source->ToGameObject()->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER)
                player->PrepareQuestMenu(source->GET_GUID());
        }
        return 0;
    }

    /**
     * Shows a quest accepting window to the [Player] for the given quest.
     *
     * @param uint32 questId : entry of a quest
     * @param bool activateAccept = true : auto finish the quest
     */
    int SendQuestTemplate(lua_State* L, Player* player)
    {
        uint32 questId = Eluna::CHECKVAL<uint32>(L, 2);
        bool activateAccept = Eluna::CHECKVAL<bool>(L, 3, true);

        Quest const* quest = eObjectMgr->GetQuestTemplate(questId);
        if (!quest)
            return 0;

        player->PlayerTalkClass->SendQuestGiverQuestDetails(quest, player->GET_GUID(), activateAccept);
        return 0;
    }

    /**
     * Converts [Player]'s corpse to bones
     */
    int SpawnBones(lua_State* /*L*/, Player* player)
    {
        player->SpawnCorpseBones();
        return 0;
    }

    /**
     * Loots [Player]'s bones for insignia
     *
     * @param [Player] looter
     */
    int RemovedInsignia(lua_State* L, Player* player)
    {
        Player* looter = Eluna::CHECKOBJ<Player>(L, 2);
        player->RemovedInsignia(looter);
        return 0;
    }

    /**
     * Makes the [Player] invite another player to a group.
     *
     * @param [Player] invited : player to invite to group
     * @return bool success : true if the player was invited to a group
     */
    int GroupInvite(lua_State* L, Player* player)
    {
        Player* invited = Eluna::CHECKOBJ<Player>(L, 2);

        if (invited->GetGroup() || invited->GetGroupInvite())
        {
            Eluna::Push(L, false);
            return 1;
        }

        // Get correct existing group if any
        Group* group = player->GetGroup();
        if (group && group->isBGGroup())
            group = player->GetOriginalGroup();

        bool success = false;

        // Try invite if group found
        if (group)
            success = !group->IsFull() && group->AddInvite(invited);
        else
        {
            // Create new group if one not found
            group = new Group;
            success = group->AddLeaderInvite(player) && group->AddInvite(invited);
            if (!success)
                delete group;
        }

        if (success)
        {
            WorldPacket data(SMSG_GROUP_INVITE, 10);                // guess size
            data << uint8(1);                                       // invited/already in group flag
            data << player->GetName();                              // max len 48
            data << uint32(0);                                      // unk
            data << uint8(0);                                       // count
            data << uint32(0);                                      // unk
            invited->GetSession()->SendPacket(&data);
        }

        Eluna::Push(L, success);
        return 1;
    }

    /**
     * Creates a new [Group] with the creator [Player] as leader.
     *
     * @param [Player] invited : player to add to group
     * @return [Group] createdGroup : the created group or nil
     */
    int GroupCreate(lua_State* L, Player* player)
    {
        Player* invited = Eluna::CHECKOBJ<Player>(L, 2);

        if (player->GetGroup() || invited->GetGroup())
            return 0;

        if (player->GetGroupInvite())
            player->UninviteFromGroup();
        if (invited->GetGroupInvite())
            invited->UninviteFromGroup();

        // Try create new group
        Group* group = new Group;
        if (!group->AddLeaderInvite(player))
        {
            delete group;
            return 0;
        }

        // Forming a new group, create it
        if (!group->IsCreated())
        {
            group->RemoveInvite(player);
            group->Create(player);
            sGroupMgr->AddGroup(group);
        }

        if (!group->AddMember(invited))
            return 0;
        group->BroadcastGroupUpdate();
        Eluna::Push(L, group);
        return 1;
    }

    /**
     * Starts a cinematic for the [Player]
     *
     * @param uint32 CinematicSequenceId : entry of a cinematic
     */
    int SendCinematicStart(lua_State* L, Player* player)
    {
        uint32 CinematicSequenceId = Eluna::CHECKVAL<uint32>(L, 2);

        player->SendCinematicStart(CinematicSequenceId);
        return 0;
    }

    /**
     * Starts a movie for the [Player]
     *
     * @param uint32 MovieId : entry of a movie
     */
    int SendMovieStart(lua_State* L, Player* player)
    {
        uint32 MovieId = Eluna::CHECKVAL<uint32>(L, 2);

        player->SendMovieStart(MovieId);
        return 0;
    }

    /**
     * Sets a setting value for the [Player]
     *
     * @param string source
     * @param uint32 index
     * @param uint32 value
     */
    int UpdatePlayerSetting(lua_State* L, Player* player)
    {
        std::string source = Eluna::CHECKVAL<std::string>(L, 2);
        uint32 index = Eluna::CHECKVAL<uint32>(L, 3);
        uint32 value = Eluna::CHECKVAL<uint32>(L, 4);
        player->UpdatePlayerSetting(source, index, value);
        return 0;
    }

    /**
     * Gets a setting value for the [Player]
     *
     * @param string source
     * @param uint32 index
     */
    int GetPlayerSettingValue(lua_State* L, Player* player)
    {
        std::string source = Eluna::CHECKVAL<std::string>(L, 2);
        uint32 index = Eluna::CHECKVAL<uint32>(L, 3);
        uint32 value = player->GetPlayerSetting(source, index).value;
        Eluna::Push(L, value);
        return 1;
    }

    /**
     * Returns the [Player] that is currently trading with this [Player]
     *
     * @return [Player] trader : the player trading, or nil
     */
    int GetTrader(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetTrader());
        return 1;
    }

    /**
     * The [Player] sets the spell power
     *
     * @param int value : The spell power value to set
     * @param bool apply = false : Whether the spell power should be applied or removed
     */
    int SetSpellPower(lua_State* L, Player* player)
    {
        int value  = Eluna::CHECKVAL<int>(L, 2);
        bool apply = Eluna::CHECKVAL<bool>(L, 3, false);

        player->ApplySpellPowerBonus(value, apply);
        return 0;
    }

    /*int BindToInstance(lua_State* L, Player* player)
    {
    player->BindToInstance();
    return 0;
    }*/

    /*int AddTalent(lua_State* L, Player* player)
    {
    uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);
    uint8 spec = Eluna::CHECKVAL<uint8>(L, 3);
    bool learning = Eluna::CHECKVAL<bool>(L, 4, true);
    if (spec >= MAX_TALENT_SPECS)
    Eluna::Push(L, false);
    else
    Eluna::Push(L, player->AddTalent(spellId, spec, learning));
    return 1;
    }*/

    /*int GainSpellComboPoints(lua_State* L, Player* player)
    {
    int8 count = Eluna::CHECKVAL<int8>(L, 2);

    player->GainSpellComboPoints(count);
    return 0;
    }*/

    /*int KillGOCredit(lua_State* L, Player* player)
    {
    uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
    ObjectGuid guid = Eluna::CHECKVAL<ObjectGuid>(L, 3);
    player->KillCreditGO(entry, guid);
    return 0;
    }*/

    /*int KilledPlayerCredit(lua_State* L, Player* player)
    {
    player->KilledPlayerCredit();
    return 0;
    }*/

    /*int RemoveRewardedQuest(lua_State* L, Player* player)
    {
    uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

    player->RemoveRewardedQuest(entry);
    return 0;
    }*/

    /*int RemoveActiveQuest(lua_State* L, Player* player)
    {
    uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);

    player->RemoveActiveQuest(entry);
    return 0;
    }*/

    /*int SummonPet(lua_State* L, Player* player)
    {
    uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
    float x = Eluna::CHECKVAL<float>(L, 3);
    float y = Eluna::CHECKVAL<float>(L, 4);
    float z = Eluna::CHECKVAL<float>(L, 5);
    float o = Eluna::CHECKVAL<float>(L, 6);
    uint32 petType = Eluna::CHECKVAL<uint32>(L, 7);
    uint32 despwtime = Eluna::CHECKVAL<uint32>(L, 8);

    if (petType >= MAX_PET_TYPE)
    return 0;

    player->SummonPet(entry, x, y, z, o, (PetType)petType, despwtime);
    return 0;
    }*/

    /*int RemovePet(lua_State* L, Player* player)
    {
    int mode = Eluna::CHECKVAL<int>(L, 2, PET_SAVE_AS_DELETED);
    bool returnreagent = Eluna::CHECKVAL<bool>(L, 2, false);

    if (!player->GetPet())
    return 0;

    player->RemovePet(player->GetPet(), (PetSaveMode)mode, returnreagent);
    return 0;
    }*/

    /**
     * Set bonus talent count to a specific count for the [Player]
     *
     * @param uint32 value : bonus talent points
     */
    int SetBonusTalentCount(lua_State* L, Player* player)
    {
        uint32 value = Eluna::CHECKVAL<uint32>(L, 2);

        player->SetBonusTalentCount(value);
        return 0;
    }

    /**
     * Get bonus talents count from the [Player]
     *
     * @return uint32 bonusTalent
     */
    int GetBonusTalentCount(lua_State* L, Player* player)
    {
        Eluna::Push(L, player->GetBonusTalentCount());
        return 1;
    }
  
    /**
     *  Returns the [Player] spells list
     *
     * @return table playerSpells
     */
    int GetSpells(lua_State* L, Player* player)
    {
        std::list<uint32> list;
        lua_createtable(L, list.size(), 0);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        PlayerSpellMap spellMap = player->GetSpellMap();
        for (PlayerSpellMap::const_iterator itr = spellMap.begin(); itr != spellMap.end(); ++itr)
        {
            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(itr->first);
            Eluna::Push(L, spellInfo->Id);
            lua_rawseti(L, tbl, ++i);
        }

        lua_settop(L, tbl);
        return 1;
    }

    /**
     * Add bonus talents count to the [Player]
     *
     * @param uint32 count = count of bonus talent
     */
    int AddBonusTalent(lua_State* L, Player* player)
    {
        uint32 count = Eluna::CHECKVAL<uint32>(L, 2);

        player->AddBonusTalent(count);
        return 0;
    }

    /**
     * Remove bonus talents count to the [Player]
     *
     * @param uint32 count = count of bonus talent
     */
    int RemoveBonusTalent(lua_State* L, Player* player)
    {
        uint32 count = Eluna::CHECKVAL<uint32>(L, 2);

        player->RemoveBonusTalent(count);
        return 0;
    }
  
    /**
     *  Returns the [Player] homebind location.
     *
     *  @return table homebind : a table containing the player's homebind information:
     *      - uint32 mapId: The ID of the map where the player is bound.
     *      - float x: The X coordinate of the homebind location.
     *      - float y: The Y coordinate of the homebind location.
     *      - float z: The Z coordinate of the homebind location.
     */
    int GetHomebind(lua_State* L, Player* player)
    {
        lua_newtable(L);
        lua_pushinteger(L, player->m_homebindMapId);
        lua_setfield(L, -2, "mapId");

        lua_pushnumber(L, player->m_homebindX);
        lua_setfield(L, -2, "x");

        lua_pushnumber(L, player->m_homebindY);
        lua_setfield(L, -2, "y");

        lua_pushnumber(L, player->m_homebindZ);
        lua_setfield(L, -2, "z");

        return 1;
    }

    /**
     *  Teleports [Player] to a predefined location based on the teleport name.
     *
     *  @param string tele : The name of the predefined teleport location.
     */
    int TeleportTo(lua_State* L, Player* player)
    {
        std::string tele = Eluna::CHECKVAL<std::string>(L, 2);
        const GameTele* game_tele = sObjectMgr->GetGameTele(tele);

        if (player->IsInFlight())
        {
            player->GetMotionMaster()->MovementExpired();
            player->m_taxi.ClearTaxiDestinations();
        }

        player->TeleportTo(game_tele->mapId, game_tele->position_x, game_tele->position_y, game_tele->position_z, game_tele->orientation);
        return 0;
    }
};
#endif
