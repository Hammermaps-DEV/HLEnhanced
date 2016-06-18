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

#include "CMultiSource.h"

BEGIN_DATADESC( CMultiSource )
	//!!!BUGBUG FIX
	DEFINE_ARRAY( m_rgEntities, FIELD_EHANDLE, MS_MAX_TARGETS ),
	DEFINE_ARRAY( m_rgTriggered, FIELD_INTEGER, MS_MAX_TARGETS ),
	DEFINE_FIELD( m_iTotal, FIELD_INTEGER ),
	DEFINE_FIELD( m_globalstate, FIELD_STRING ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( multisource, CMultiSource );
//
// Cache user-entity-field values until spawn is called.
//

void CMultiSource::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "style" ) ||
		FStrEq( pkvd->szKeyName, "height" ) ||
		FStrEq( pkvd->szKeyName, "killtarget" ) ||
		FStrEq( pkvd->szKeyName, "value1" ) ||
		FStrEq( pkvd->szKeyName, "value2" ) ||
		FStrEq( pkvd->szKeyName, "value3" ) )
		pkvd->fHandled = true;
	else if( FStrEq( pkvd->szKeyName, "globalstate" ) )
	{
		m_globalstate = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CPointEntity::KeyValue( pkvd );
}

#define SF_MULTI_INIT		1

void CMultiSource::Spawn()
{
	// set up think for later registration

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	pev->nextthink = gpGlobals->time + 0.1;
	pev->spawnflags |= SF_MULTI_INIT;	// Until it's initialized
	SetThink( &CMultiSource::Register );
}

void CMultiSource::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	int i = 0;

	// Find the entity in our list
	while( i < m_iTotal )
		if( m_rgEntities[ i++ ] == pCaller )
			break;

	// if we didn't find it, report error and leave
	if( i > m_iTotal )
	{
		ALERT( at_console, "MultiSrc:Used by non member %s.\n", STRING( pCaller->pev->classname ) );
		return;
	}

	// CONSIDER: a Use input to the multisource always toggles.  Could check useType for ON/OFF/TOGGLE

	m_rgTriggered[ i - 1 ] ^= 1;

	// 
	if( IsTriggered( pActivator ) )
	{
		ALERT( at_aiconsole, "Multisource %s enabled (%d inputs)\n", STRING( pev->targetname ), m_iTotal );
		USE_TYPE useType = USE_TOGGLE;
		if( m_globalstate )
			useType = USE_ON;
		SUB_UseTargets( NULL, useType, 0 );
	}
}


bool CMultiSource::IsTriggered( const CBaseEntity* const ) const
{
	// Is everything triggered?
	int i = 0;

	// Still initializing?
	if( pev->spawnflags & SF_MULTI_INIT )
		return 0;

	while( i < m_iTotal )
	{
		if( m_rgTriggered[ i ] == 0 )
			break;
		i++;
	}

	if( i == m_iTotal )
	{
		if( !m_globalstate || gGlobalState.EntityGetState( m_globalstate ) == GLOBAL_ON )
			return true;
	}

	return false;
}

void CMultiSource::Register( void )
{
	edict_t *pentTarget = NULL;

	m_iTotal = 0;
	memset( m_rgEntities, 0, MS_MAX_TARGETS * sizeof( EHANDLE ) );

	SetThink( &CMultiSource::SUB_DoNothing );

	// search for all entities which target this multisource (pev->targetname)

	pentTarget = FIND_ENTITY_BY_STRING( NULL, "target", STRING( pev->targetname ) );

	while( !FNullEnt( pentTarget ) && ( m_iTotal < MS_MAX_TARGETS ) )
	{
		CBaseEntity *pTarget = CBaseEntity::Instance( pentTarget );
		if( pTarget )
			m_rgEntities[ m_iTotal++ ] = pTarget;

		pentTarget = FIND_ENTITY_BY_STRING( pentTarget, "target", STRING( pev->targetname ) );
	}

	pentTarget = FIND_ENTITY_BY_STRING( NULL, "classname", "multi_manager" );
	while( !FNullEnt( pentTarget ) && ( m_iTotal < MS_MAX_TARGETS ) )
	{
		CBaseEntity *pTarget = CBaseEntity::Instance( pentTarget );
		if( pTarget && pTarget->HasTarget( pev->targetname ) )
			m_rgEntities[ m_iTotal++ ] = pTarget;

		pentTarget = FIND_ENTITY_BY_STRING( pentTarget, "classname", "multi_manager" );
	}

	pev->spawnflags &= ~SF_MULTI_INIT;
}