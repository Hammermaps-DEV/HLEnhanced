#if USE_GM
/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CGunmanCycler.h"

BEGIN_DATADESC(CGunmanCycler)
	DEFINE_FIELD(m_animate, FIELD_INTEGER),
	DEFINE_FIELD(m_iBodygroup, FIELD_INTEGER),
END_DATADESC()

LINK_ENTITY_TO_CLASS(gunman_cycler, CGunmanCycler);

void CGunmanCycler::KeyValue(KeyValueData * pkvd)
{
	if (FStrEq(pkvd->szKeyName, "cyc_submodel1"))
	{
		m_iBodygroup[0] = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "cyc_submodel2"))
	{
		m_iBodygroup[1] = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "cyc_submodel3"))
	{
		m_iBodygroup[2] = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else
		CBaseMonster::KeyValue(pkvd);
}

void CGunmanCycler::Spawn(void)
{
	PRECACHE_MODEL(GetModelName());
	SetModel(GetModelName());

	SetBodygroup(1, m_iBodygroup[0]);
	SetBodygroup(2, m_iBodygroup[1]);
	SetBodygroup(3, m_iBodygroup[2]);

	InitBoneControllers();

	SetSolidType(SOLID_SLIDEBOX);
	SetMoveType(MOVETYPE_NONE);
	SetTakeDamageMode(DAMAGE_NO);
	GetEffects().ClearAll();
	SetHealth(80000);
	SetYawSpeed(5);
	SetIdealYaw(GetAbsAngles().y);

	m_flFrameRate = 75;
	m_flGroundSpeed = 0;

	SetNextThink(GetNextThink() + 1.0);

	ResetSequenceInfo();

	if (GetSequence() != 0 || GetFrame() != 0)
	{
		m_animate = 0;
		SetFrameRate(0);
	}
	else
	{
		m_animate = 1;
	}
}

void CGunmanCycler::Think(void)
{
	SetNextThink(gpGlobals->time + 0.1);

	if (m_animate > 0)
		StudioFrameAdvance();

	if (m_fSequenceFinished && !m_fSequenceLoops)
	{
		SetAnimTime(gpGlobals->time);
		SetFrameRate(1.0);

		m_fSequenceFinished = false;
		m_flLastEventCheck = gpGlobals->time;

		SetFrame(0);

		if (m_animate <= 0)
			SetFrameRate(0.0);	// FIX: don't reset framerate
	}
}

#endif