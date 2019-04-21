/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"
#include "CBasePlayer.h"
#include "CServerGameInterface.h"

#if USE_ANGELSCRIPT && defined( SERVER_DLL )
#include "Angelscript/CHLASServerManager.h"
#endif

// Global Savedata for Delay
BEGIN_DATADESC_NOBASE(CBaseEntity)
DEFINE_FIELD(m_hGoalEnt, FIELD_EHANDLE),

DEFINE_FIELD(m_MoveWith, FIELD_STRING), //LRC
DEFINE_FIELD(m_pMoveWith, FIELD_CLASSPTR), //LRC
DEFINE_FIELD(m_pChildMoveWith, FIELD_CLASSPTR), //LRC
DEFINE_FIELD(m_pSiblingMoveWith, FIELD_CLASSPTR), //LRC

DEFINE_FIELD(m_iLFlags, FIELD_INTEGER), //LRC
DEFINE_FIELD(m_iStyle, FIELD_INTEGER), //LRC
DEFINE_FIELD(m_vecOffsetOrigin, FIELD_VECTOR), //LRC
DEFINE_FIELD(m_vecOffsetAngles, FIELD_VECTOR), //LRC
DEFINE_FIELD(m_vecPostAssistOrg, FIELD_VECTOR), //LRC
DEFINE_FIELD(m_vecPostAssistAng, FIELD_VECTOR), //LRC
DEFINE_FIELD(m_activated, FIELD_BOOLEAN), //LRC
DEFINE_FIELD(m_fNextThink, FIELD_TIME), //LRC
DEFINE_FIELD(m_fPevNextThink, FIELD_TIME), //LRC
DEFINE_FIELD(m_vecPostAssistVel, FIELD_VECTOR), //LRC
DEFINE_FIELD(m_vecPostAssistAVel, FIELD_VECTOR), //LRC

DEFINE_FIELD(m_pfnThink, FIELD_FUNCPTR),		// UNDONE: Build table of these!!!
DEFINE_FIELD(m_pfnTouch, FIELD_FUNCPTR),
DEFINE_FIELD(m_pfnUse, FIELD_FUNCPTR),
DEFINE_FIELD(m_pfnBlocked, FIELD_FUNCPTR),
DEFINE_THINKFUNC(SUB_Remove),
DEFINE_THINKFUNC(SUB_DoNothing),
DEFINE_THINKFUNC(SUB_StartFadeOut),
DEFINE_THINKFUNC(SUB_FadeOut),
DEFINE_THINKFUNC(SUB_CallUseToggle),
END_DATADESC()

// Global Savedata for Delay
BEGIN_DATADESC(CBaseDelay)
DEFINE_FIELD(m_flDelay, FIELD_FLOAT),
DEFINE_FIELD(m_iszKillTarget, FIELD_STRING),
DEFINE_THINKFUNC(DelayThink),
END_DATADESC()

BEGIN_DATADESC(CBaseAnimating)
DEFINE_FIELD(m_flFrameRate, FIELD_FLOAT),
DEFINE_FIELD(m_flGroundSpeed, FIELD_FLOAT),
DEFINE_FIELD(m_flLastEventCheck, FIELD_TIME),
DEFINE_FIELD(m_fSequenceFinished, FIELD_BOOLEAN),
DEFINE_FIELD(m_fSequenceLoops, FIELD_BOOLEAN),
END_DATADESC()

// Global Savedata for Toggle
BEGIN_DATADESC(CBaseToggle)
DEFINE_FIELD(m_toggle_state, FIELD_INTEGER),
DEFINE_FIELD(m_flActivateFinished, FIELD_TIME),
DEFINE_FIELD(m_flMoveDistance, FIELD_FLOAT),
DEFINE_FIELD(m_flWait, FIELD_FLOAT),
DEFINE_FIELD(m_flLip, FIELD_FLOAT),
DEFINE_FIELD(m_flTWidth, FIELD_FLOAT),
DEFINE_FIELD(m_flTLength, FIELD_FLOAT),
DEFINE_FIELD(m_vecPosition1, FIELD_POSITION_VECTOR),
DEFINE_FIELD(m_vecPosition2, FIELD_POSITION_VECTOR),
DEFINE_FIELD(m_vecAngle1, FIELD_VECTOR),		// UNDONE: Position could go through transition, but also angle?
DEFINE_FIELD(m_vecAngle2, FIELD_VECTOR),		// UNDONE: Position could go through transition, but also angle?
DEFINE_FIELD(m_cTriggersLeft, FIELD_INTEGER),
DEFINE_FIELD(m_flHeight, FIELD_FLOAT),
DEFINE_FIELD(m_hActivator, FIELD_EHANDLE),
DEFINE_FIELD(m_pfnCallWhenMoveDone, FIELD_FUNCPTR),
DEFINE_FIELD(m_vecFinalDest, FIELD_POSITION_VECTOR),
DEFINE_FIELD(m_vecFinalAngle, FIELD_VECTOR),
DEFINE_FIELD(m_sMaster, FIELD_STRING),
DEFINE_FIELD(m_bitsDamageInflict, FIELD_INTEGER),	// damage type inflicted
DEFINE_THINKFUNC(LinearMoveDone),
DEFINE_THINKFUNC(AngularMoveDone),
END_DATADESC()

// Global Savedata for monster
// UNDONE: Save schedule data?  Can this be done?  We may
// lose our enemy pointer or other data (goal ent, target, etc)
// that make the current schedule invalid, perhaps it's best
// to just pick a new one when we start up again.
BEGIN_DATADESC(CBaseMonster)
DEFINE_FIELD(m_hEnemy, FIELD_EHANDLE),
DEFINE_FIELD(m_hTargetEnt, FIELD_EHANDLE),
DEFINE_ARRAY(m_hOldEnemy, FIELD_EHANDLE, MAX_OLD_ENEMIES),
DEFINE_ARRAY(m_vecOldEnemy, FIELD_POSITION_VECTOR, MAX_OLD_ENEMIES),
DEFINE_FIELD(m_flFieldOfView, FIELD_FLOAT),
DEFINE_FIELD(m_flWaitFinished, FIELD_TIME),
DEFINE_FIELD(m_flMoveWaitFinished, FIELD_TIME),

DEFINE_FIELD(m_Activity, FIELD_INTEGER),
DEFINE_FIELD(m_IdealActivity, FIELD_INTEGER),
DEFINE_FIELD(m_LastHitGroup, FIELD_INTEGER),
DEFINE_FIELD(m_MonsterState, FIELD_INTEGER),
DEFINE_FIELD(m_IdealMonsterState, FIELD_INTEGER),
DEFINE_FIELD(m_iTaskStatus, FIELD_INTEGER),

//Schedule_t			*m_pSchedule;

DEFINE_FIELD(m_iScheduleIndex, FIELD_INTEGER),
DEFINE_FIELD(m_afConditions, FIELD_INTEGER),
//WayPoint_t			m_Route[ ROUTE_SIZE ];
//	DEFINE_FIELD( m_movementGoal, FIELD_INTEGER ),
//	DEFINE_FIELD( m_iRouteIndex, FIELD_INTEGER ),
//	DEFINE_FIELD( m_moveWaitTime, FIELD_FLOAT ),

DEFINE_FIELD(m_vecMoveGoal, FIELD_POSITION_VECTOR),
DEFINE_FIELD(m_movementActivity, FIELD_INTEGER),

//		int					m_iAudibleList; // first index of a linked list of sounds that the monster can hear.
//	DEFINE_FIELD( m_afSoundTypes, FIELD_INTEGER ),
DEFINE_FIELD(m_vecLastPosition, FIELD_POSITION_VECTOR),
DEFINE_FIELD(m_iHintNode, FIELD_INTEGER),
DEFINE_FIELD(m_afMemory, FIELD_INTEGER),
DEFINE_FIELD(m_iMaxHealth, FIELD_INTEGER),

DEFINE_FIELD(m_vecEnemyLKP, FIELD_POSITION_VECTOR),
DEFINE_FIELD(m_cAmmoLoaded, FIELD_INTEGER),
DEFINE_FIELD(m_afCapability, FIELD_INTEGER),

DEFINE_FIELD(m_flNextAttack, FIELD_TIME),
DEFINE_FIELD(m_bloodColor, FIELD_INTEGER),
DEFINE_FIELD(m_failSchedule, FIELD_INTEGER),

DEFINE_FIELD(m_flHungryTime, FIELD_TIME),
DEFINE_FIELD(m_flDistTooFar, FIELD_FLOAT),
DEFINE_FIELD(m_flDistLook, FIELD_FLOAT),
DEFINE_FIELD(m_iTriggerCondition, FIELD_INTEGER),
DEFINE_FIELD(m_iszTriggerTarget, FIELD_STRING),

DEFINE_FIELD(m_HackedGunPos, FIELD_VECTOR),

DEFINE_FIELD(m_scriptState, FIELD_INTEGER),
DEFINE_FIELD(m_pCine, FIELD_CLASSPTR),
DEFINE_USEFUNC(MonsterUse),
DEFINE_THINKFUNC(CallMonsterThink),
DEFINE_THINKFUNC(CorpseFallThink),
DEFINE_THINKFUNC(MonsterInitThink),
END_DATADESC()

BEGIN_DATADESC(CBasePlayerWeapon)
#if defined( CLIENT_WEAPONS )
DEFINE_FIELD(m_pPlayer, FIELD_CLASSPTR),
DEFINE_FIELD(m_pNext, FIELD_CLASSPTR),
DEFINE_FIELD(m_iId, FIELD_INTEGER),
DEFINE_FIELD(m_flNextPrimaryAttack, FIELD_FLOAT),
DEFINE_FIELD(m_flNextSecondaryAttack, FIELD_FLOAT),
DEFINE_FIELD(m_flTimeWeaponIdle, FIELD_FLOAT),
#else	// CLIENT_WEAPONS
DEFINE_FIELD(m_flNextPrimaryAttack, FIELD_TIME),
DEFINE_FIELD(m_flNextSecondaryAttack, FIELD_TIME),
DEFINE_FIELD(m_flTimeWeaponIdle, FIELD_TIME),
#endif	// CLIENT_WEAPONS
DEFINE_FIELD(m_iClip, FIELD_INTEGER),
DEFINE_FIELD(m_iDefaultAmmo, FIELD_INTEGER),
//	DEFINE_FIELD( m_iClientClip, FIELD_INTEGER )	 , reset to zero on load so hud gets updated correctly
//  DEFINE_FIELD( m_iClientWeaponState, FIELD_INTEGER ), reset to zero on load so hud gets updated correctly
DEFINE_THINKFUNC(DestroyItem),
DEFINE_TOUCHFUNC(DefaultTouch),
DEFINE_THINKFUNC(FallThink),
DEFINE_THINKFUNC(Materialize),
DEFINE_THINKFUNC(AttemptToMaterialize),
END_DATADESC()

// Global Savedata for player
BEGIN_DATADESC(CBasePlayer)
DEFINE_FIELD(m_flFlashLightTime, FIELD_TIME),
DEFINE_FIELD(m_iFlashBattery, FIELD_INTEGER),

DEFINE_FIELD(m_afButtonLast, FIELD_INTEGER),
DEFINE_FIELD(m_afButtonPressed, FIELD_INTEGER),
DEFINE_FIELD(m_afButtonReleased, FIELD_INTEGER),

DEFINE_ARRAY(m_rgItems, FIELD_INTEGER, MAX_ITEMS),
DEFINE_FIELD(m_afPhysicsFlags, FIELD_INTEGER),

DEFINE_FIELD(m_flTimeStepSound, FIELD_TIME),
DEFINE_FIELD(m_flTimeWeaponIdle, FIELD_TIME),
DEFINE_FIELD(m_flSwimTime, FIELD_TIME),
DEFINE_FIELD(m_flDuckTime, FIELD_TIME),
DEFINE_FIELD(m_flWallJumpTime, FIELD_TIME),

DEFINE_FIELD(m_flSuitUpdate, FIELD_TIME),
DEFINE_ARRAY(m_rgSuitPlayList, FIELD_INTEGER, CSUITPLAYLIST),
DEFINE_FIELD(m_iSuitPlayNext, FIELD_INTEGER),
DEFINE_ARRAY(m_rgiSuitNoRepeat, FIELD_INTEGER, CSUITNOREPEAT),
DEFINE_ARRAY(m_rgflSuitNoRepeatTime, FIELD_TIME, CSUITNOREPEAT),
//TODO also used by monsters. Move to CBaseCombatCharacter datadesc? - Solokiller
DEFINE_FIELD(m_lastDamageAmount, FIELD_INTEGER),

DEFINE_ARRAY(m_rgpPlayerItems, FIELD_CLASSPTR, MAX_WEAPON_SLOTS),
DEFINE_FIELD(m_pActiveItem, FIELD_CLASSPTR),
DEFINE_FIELD(m_pLastItem, FIELD_CLASSPTR),

DEFINE_ARRAY(m_rgAmmo, FIELD_INTEGER, CAmmoTypes::MAX_AMMO_TYPES),
DEFINE_FIELD(m_idrowndmg, FIELD_INTEGER),
DEFINE_FIELD(m_idrownrestored, FIELD_INTEGER),

DEFINE_FIELD(m_iTrain, FIELD_INTEGER),
DEFINE_FIELD(m_bitsHUDDamage, FIELD_INTEGER),
DEFINE_FIELD(m_flFallVelocity, FIELD_FLOAT),
DEFINE_FIELD(m_iTargetVolume, FIELD_INTEGER),
DEFINE_FIELD(m_iWeaponVolume, FIELD_INTEGER),
DEFINE_FIELD(m_iExtraSoundTypes, FIELD_INTEGER),
DEFINE_FIELD(m_iWeaponFlash, FIELD_INTEGER),
DEFINE_FIELD(m_fLongJump, FIELD_BOOLEAN),
DEFINE_FIELD(m_fInitHUD, FIELD_BOOLEAN),
DEFINE_FIELD(m_tbdPrev, FIELD_TIME),

DEFINE_FIELD(m_pTank, FIELD_EHANDLE),
DEFINE_FIELD(m_iHideHUD, FIELD_INTEGER),
DEFINE_FIELD(m_iFOV, FIELD_INTEGER),
#if USE_OPFOR
DEFINE_FIELD(m_pRope, FIELD_CLASSPTR),
DEFINE_FIELD(m_vecDisplacerReturn, FIELD_POSITION_VECTOR),
#endif
DEFINE_FIELD(m_flLastClimbTime, FIELD_TIME),
DEFINE_FIELD(m_bIsClimbing, FIELD_BOOLEAN),

DEFINE_FIELD(m_bUseCustomHudColors, FIELD_BOOLEAN),
DEFINE_FIELD(m_flLastHudColorChangeTime, FIELD_TIME),
DEFINE_FIELD(m_HudColors.m_PrimaryColor, FIELD_INTEGER),
DEFINE_FIELD(m_HudColors.m_EmptyItemColor, FIELD_INTEGER),
DEFINE_FIELD(m_HudColors.m_AmmoBarColor, FIELD_INTEGER),

DEFINE_THINKFUNC(PlayerDeathThink),

//DEFINE_FIELD( m_fDeadTime, FIELD_FLOAT ), // only used in multiplayer games
//DEFINE_FIELD( m_fGameHUDInitialized, FIELD_INTEGER ), // only used in multiplayer games
//DEFINE_FIELD( m_flStopExtraSoundTime, FIELD_TIME ),
//DEFINE_FIELD( m_iPlayerSound, FIELD_INTEGER ),	// Don't restore, set in Precache()
//DEFINE_FIELD( m_SndLast, FIELD_EHANDLE ),	// Don't restore, client needs reset
//DEFINE_FIELD( m_flSndRoomtype, FIELD_FLOAT ),	// Don't restore, client needs reset
//DEFINE_FIELD( m_flSndRange, FIELD_FLOAT ),	// Don't restore, client needs reset
//DEFINE_FIELD( m_flgeigerRange, FIELD_FLOAT ),	// Don't restore, reset in Precache()
//DEFINE_FIELD( m_flgeigerDelay, FIELD_FLOAT ),	// Don't restore, reset in Precache()
//DEFINE_FIELD( m_igeigerRangePrev, FIELD_FLOAT ),	// Don't restore, reset in Precache()
//DEFINE_FIELD( m_iStepLeft, FIELD_INTEGER ), // Don't need to restore
//DEFINE_ARRAY( m_szTextureName, FIELD_CHARACTER, CBTEXTURENAMEMAX ), // Don't need to restore
//DEFINE_FIELD( m_chTextureType, FIELD_CHARACTER ), // Don't need to restore
//DEFINE_FIELD( m_fNoPlayerSound, FIELD_BOOLEAN ), // Don't need to restore, debug
//DEFINE_FIELD( m_iUpdateTime, FIELD_INTEGER ), // Don't need to restore
//DEFINE_FIELD( m_iClientHealth, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( m_iClientBattery, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( m_iClientHideHUD, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( m_fWeapon, FIELD_BOOLEAN ),  // Don't restore, client needs reset
//DEFINE_FIELD( m_nCustomSprayFrames, FIELD_INTEGER ), // Don't restore, depends on server message after spawning and only matters in multiplayer
//DEFINE_FIELD( m_vecAutoAim, FIELD_VECTOR ), // Don't save/restore - this is recomputed
//DEFINE_ARRAY( m_rgAmmoLast, FIELD_INTEGER, CAmmoTypes::MAX_AMMO_TYPES ), // Don't need to restore
//DEFINE_FIELD( m_fOnTarget, FIELD_BOOLEAN ), // Don't need to restore
//DEFINE_FIELD( m_nCustomSprayFrames, FIELD_INTEGER ), // Don't need to restore
END_DATADESC()

void CBaseEntity::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq("classificationOverride", pkvd->szKeyName))
	{
		SetClassificationOverride(EntityClassifications().GetClassificationId(pkvd->szValue));
		pkvd->fHandled = true;
	}
	else if (FStrEq("movewith", pkvd->szKeyName)) // LRC
	{
		m_MoveWith = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq("skill", pkvd->szKeyName)) // LRC
	{
		m_iLFlags = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq("style", pkvd->szKeyName)) // LRC
	{
		m_iStyle = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else
		pkvd->fHandled = false;
}

void CBaseEntity::TakeDamage(const CTakeDamageInfo& info)
{
	//This method exists so we can intercept damage events in the base class unconditionally.
	OnTakeDamage(info);
}

void CBaseEntity::TakeDamage(CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int bitsDamageType)
{
	TakeDamage(CTakeDamageInfo(pInflictor, pAttacker, flDamage, bitsDamageType));
}

// Convenient way to explicitly do nothing (passed to functions that require a method)
void CBaseEntity::SUB_DoNothing(void) { }

//LRC to support movewith
void CBaseEntity::Activate(void)
{
	//LRC - rebuild the new assistlist as the game starts
	if (m_iLFlags & LF_ASSISTLIST)
	{
		//	UTIL_AddToAssistList(this);
	}

	//LRC - and the aliaslist too
	if (m_iLFlags & LF_ALIASLIST)
	{
		//	UTIL_AddToAliasList((CBaseAlias*)this);
	}

	if (m_activated) return;
	m_activated = true;

	InitMoveWith();
	PostSpawn();
}

// LRC - called by activate() to support movewith
void CBaseEntity::InitMoveWith(void)
{
	SetParent(m_MoveWith);
}

//g-cont. upgrade system to xashParentSystem 0.3 beta
void CBaseEntity::SetParent(int m_iNewParent, int m_iAttachment)
{
#if !defined( CLIENT_DLL )
	if (!m_iNewParent) //unlink entity from chain
	{
		//ResetParent(); //g-cont. temporary disabled
		return;//disable
	}

	CBaseEntity* pParent;

	if (!m_iAttachment) //try to extract aiment from name
	{
		char *name = (char*)STRING(m_iNewParent);
		for (char *c = name; *c; c++)
		{
			if (*c == '.')
			{
				m_iAttachment = atoi(c + 1);
				name[strlen(name) - 2] = 0;
				pParent = UTIL_FindEntityByTargetname(NULL, name);
				SetParent(pParent, m_iAttachment);
				return;
			}
		}
	}
	pParent = UTIL_FindEntityByTargetname(NULL, STRING(m_iNewParent));
	SetParent(pParent, m_iAttachment);//check pointer to valid later
#endif
}

void CBaseEntity::SetParent(CBaseEntity *pParent, int m_iAttachment)
{
	m_pMoveWith = pParent;

	if (!m_pMoveWith)
	{
		ALERT(at_console, "Missing movewith entity %s\n", STRING(m_MoveWith));
		return;
	}

	//	if (pev->targetname)
	//		ALERT(at_console,"Init: %s %s moves with %s\n", STRING(pev->classname), STRING(pev->targetname), STRING(m_MoveWith));
	//	else
	//		ALERT(at_console,"Init: %s moves with %s\n", STRING(pev->classname), STRING(m_MoveWith));

		//check for himself parent
	if (m_pMoveWith == this)
	{
		ALERT(at_console, "%s has himself parent!\n", GetClassname());
		return;
	}

	CBaseEntity *pSibling = m_pMoveWith->m_pChildMoveWith;
	while (pSibling) // check that this entity isn't already in the list of children
	{
		if (pSibling == this) break;
		pSibling = pSibling->m_pSiblingMoveWith;
	}

	if (!pSibling) // if movewith is being set up for the first time...
	{
		// add this entity to the list of children
		m_pSiblingMoveWith = m_pMoveWith->m_pChildMoveWith; // may be null: that's fine by me.
		m_pMoveWith->m_pChildMoveWith = this;

		if (m_iAttachment)//parent has attcahment
		{
			if (m_iLFlags & LF_POINTENTITY || GetFlags().Any(FL_MONSTER))
			{
				SetSkin(ENTINDEX(m_pMoveWith->edict()));
				SetBody(m_iAttachment);
				SetMoveType(MOVETYPE_FOLLOW);
				pev->aiment = m_pMoveWith->edict();
			}
			else //error
			{
				ALERT(at_console, "%s not following with aiment %d!(yet)\n", GetClassname(), m_iAttachment);
			}
			return;
		}

		//appllayed to origin
		if (GetMoveType() == MOVETYPE_NONE)
		{
			if (GetSolidType() == SOLID_BSP)
				SetMoveType(MOVETYPE_PUSH);
			else
				SetMoveType(MOVETYPE_NOCLIP); // or _FLY, perhaps?

			SetBits(m_iLFlags, LF_MOVENONE); //member movetype
		}

		if (m_pMoveWith->GetMoveType() == MOVETYPE_WALK)//parent is walking monster?
		{
			SetBits(m_iLFlags, LF_POSTORG);//copy pos from parent every frame
			SetSolidType(SOLID_NOT); //set non solid
		}

		m_vecParentOrigin = m_pMoveWith->GetAbsOrigin();
		m_vecParentAngles = m_pMoveWith->GetAbsAngles();

		// was the parent shifted at spawn-time?
		if (m_pMoveWith->m_vecSpawnOffset != g_vecZero)
		{
			//ALERT(at_console,"Corrected using SpawnOffset\n");

			//UTIL_AssignOrigin(this, GetAbsOrigin() + m_pMoveWith->m_vecSpawnOffset);
			m_vecSpawnOffset = m_vecSpawnOffset + m_pMoveWith->m_vecSpawnOffset;
		}

		m_vecOffsetOrigin = GetAbsOrigin() - m_vecParentOrigin;
		m_vecOffsetAngles = GetAbsAngles() - m_vecParentAngles;

		if ((m_pMoveWith->m_iLFlags & LF_ANGULAR && m_vecOffsetOrigin != g_vecZero) || m_pMoveWith->m_iLFlags & LF_POINTENTITY)
			SetBits(m_iLFlags, LF_POSTORG);//magic stuff

#if !defined( CLIENT_DLL )
		if (g_Server.IsActive())//maybe parent is moving ?
		{
			SetAbsVelocity(GetAbsVelocity() + m_pMoveWith->GetAbsVelocity());
			SetAngularVelocity(GetAngularVelocity() + m_pMoveWith->GetAngularVelocity());
		}
#endif
	}
}

void CBaseEntity::ResetParent(void)
{
	CBaseEntity* pTemp;

	if (m_iLFlags & LF_MOVENONE)//this entity was static e.g. func_wall
	{
		ClearBits(m_iLFlags, LF_MOVENONE);
		pev->movetype = MOVETYPE_NONE;
	}

	if (!CWorld::GetInstance())
	{
		ALERT(at_console, "ResetParent has no AssistList!\n");
		return;
	}

	//LRC - remove this from the AssistList.
	for (pTemp = CWorld::GetInstance(); pTemp->m_pAssistLink != NULL; pTemp = pTemp->m_pAssistLink)
	{
		if (this == pTemp->m_pAssistLink)
		{
			//			ALERT(at_console,"REMOVE: %s removed from the Assist List.\n", STRING(pev->classname));
			pTemp->m_pAssistLink = this->m_pAssistLink;
			this->m_pAssistLink = NULL;
			break;
		}
	}

	//LRC
	if (m_pMoveWith)
	{
		// if I'm moving with another entity, take me out of the list. (otherwise things crash!)
		pTemp = m_pMoveWith->m_pChildMoveWith;
		if (pTemp == this)
		{
			m_pMoveWith->m_pChildMoveWith = this->m_pSiblingMoveWith;
		}
		else
		{
			while (pTemp->m_pSiblingMoveWith)
			{
				if (pTemp->m_pSiblingMoveWith == this)
				{
					pTemp->m_pSiblingMoveWith = this->m_pSiblingMoveWith;
					break;
				}
				pTemp = pTemp->m_pSiblingMoveWith;
			}

		}
		//		ALERT(at_console,"REMOVE: %s removed from the %s ChildMoveWith list.\n", STRING(pev->classname), STRING(m_pMoveWith->pev->targetname));
	}

	//LRC - do the same thing if another entity is moving with _me_.
	if (m_pChildMoveWith)
	{
		CBaseEntity* pCur = m_pChildMoveWith;
		CBaseEntity* pNext;
		while (pCur != NULL)
		{
			pNext = pCur->m_pSiblingMoveWith;
			// bring children to a stop
			//UTIL_SetMoveWithVelocity(pCur, g_vecZero, 100);
			//UTIL_SetMoveWithAvelocity(pCur, g_vecZero, 100);
			pCur->m_pMoveWith = NULL;
			pCur->m_pSiblingMoveWith = NULL;
			pCur = pNext;
		}
	}
}

// LRC
// Directly clear all movewith pointer before changelevel
void CBaseEntity::ClearPointers(void)
{
	m_pChildMoveWith = NULL;
	m_pSiblingMoveWith = NULL;
	m_pAssistLink = NULL;
}

// LRC
// PUSH entities won't have their velocity applied unless they're thinking.
// make them do so for the foreseeable future.
void CBaseEntity::SetEternalThink(void)
{
	if (pev->movetype == MOVETYPE_PUSH)
	{
		// record m_fPevNextThink as well, because we want to be able to
		// tell when the bloody engine CHANGES IT!
		pev->nextthink = pev->ltime + 1E6;
		m_fPevNextThink = pev->nextthink;
	}

	CBaseEntity *pChild;
	for (pChild = m_pChildMoveWith; pChild != nullptr; pChild = pChild->m_pSiblingMoveWith)
		pChild->SetEternalThink();
}

// LRC - for getting round the engine's preconceptions.
// MoveWith entities have to be able to think independently of moving.
// This is how we do so.
void CBaseEntity::SetNextThink(float delay, bool correctSpeed)
{
	// now monsters use this method, too.
	if (m_pMoveWith || m_pChildMoveWith || pev->flags & FL_MONSTER)
	{
		// use the Assist system, so that thinking doesn't mess up movement.
		if (pev->movetype == MOVETYPE_PUSH)
			m_fNextThink = pev->ltime + delay;
		else
			m_fNextThink = gpGlobals->time + delay;

		SetEternalThink();
		//UTIL_MarkForAssist(this, correctSpeed);

		ALERT(at_console, "SetAssistedThink for %s: %f\n", STRING(pev->targetname), m_fNextThink);
	}
	else
	{
		// set nextthink as normal.
		if (pev->movetype == MOVETYPE_PUSH)
		{
			pev->nextthink = pev->ltime + delay;
		}
		else
		{
			pev->nextthink = gpGlobals->time + delay;
		}

		m_fPevNextThink = m_fNextThink = pev->nextthink;

		if (pev->classname)
			ALERT(at_console, "SetNormThink for %s: %f\n", STRING(pev->targetname), m_fNextThink);
	}
}

void CBaseEntity::AbsoluteNextThink(float time, bool correctSpeed) //LRC
{
	if (m_pMoveWith || m_pChildMoveWith)
	{
		// use the Assist system, so that thinking doesn't mess up movement.
		m_fNextThink = time;

		SetEternalThink();
		//UTIL_MarkForAssist(this, correctSpeed);
	}
	else
	{
		// set nextthink as normal.
		pev->nextthink = time;
		m_fPevNextThink = m_fNextThink = pev->nextthink;
	}
}

void CBaseEntity::DontThink(void) //LRC
{
	m_fNextThink = 0;
	if (m_pMoveWith == nullptr && m_pChildMoveWith == nullptr)
	{
		pev->nextthink = 0;
		m_fPevNextThink = 0;
	}

	ALERT(at_console, "DontThink for %s\n", STRING(pev->targetname));
}

// LRC - check in case the engine has changed our nextthink. (which it does
// on a depressingly frequent basis.)
// for some reason, this doesn't always produce perfect movement - but it's close
// enough for government work. (the player doesn't get stuck, at least.)
void CBaseEntity::ThinkCorrection(void)
{
	if (pev->nextthink != m_fPevNextThink)
	{
		// The engine has changed our nextthink, in its typical endearing way.
		// Now we have to apply that change in the _right_ places.
		ALERT(at_console, "StoredThink corrected for %s \"%s\": %f -> %f\n", STRING(pev->classname), STRING(pev->targetname), m_fNextThink, m_fNextThink + pev->nextthink - m_fPevNextThink);
		m_fNextThink += pev->nextthink - m_fPevNextThink;
		m_fPevNextThink = pev->nextthink;
	}
}

#if USE_ANGELSCRIPT && defined( SERVER_DLL )
CScriptDictionary* CBaseEntity::GetUserData() const
{
	if (!m_UserData)
	{
		m_UserData = CScriptDictionary::Create(g_ASManager.GetASManager().GetEngine());

		//Create added a reference for us, as did operator=. Release the one added by Create. - Solokiller
		m_UserData->Release();
	}

	auto pUserData = m_UserData;

	pUserData->AddRef();

	return pUserData;
}
#endif