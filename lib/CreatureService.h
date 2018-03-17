/*
 * CreatureService.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

#pragma once

class CreatureID;
class Creature;

class DLL_LINKAGE CreatureService
{
public:
	virtual ~CreatureService() = default;
	virtual const Creature * getCreature(const CreatureID & creatureID) const = 0;
};
