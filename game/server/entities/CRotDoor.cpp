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
#include "entities/DoorConstants.h"
#include "CBaseDoor.h"

#include "CRotDoor.h"

LINK_ENTITY_TO_CLASS( func_door_rotating, CRotDoor );

void CRotDoor::Spawn( void )
{
	Precache();
	// set the axis of rotation
	CBaseToggle::AxisDir( this );

	// check for clockwise rotation
	if( FBitSet( pev->spawnflags, SF_DOOR_ROTATE_BACKWARDS ) )
		pev->movedir = pev->movedir * -1;

	//m_flWait			= 2; who the hell did this? (sjb)
	m_vecAngle1 = pev->angles;
	m_vecAngle2 = pev->angles + pev->movedir * m_flMoveDistance;

	ASSERTSZ( m_vecAngle1 != m_vecAngle2, "rotating door start/end positions are equal" );

	if( FBitSet( pev->spawnflags, SF_DOOR_PASSABLE ) )
		pev->solid = SOLID_NOT;
	else
		pev->solid = SOLID_BSP;

	pev->movetype = MOVETYPE_PUSH;
	SetAbsOrigin( GetAbsOrigin() );
	SetModel( STRING( pev->model ) );

	if( pev->speed == 0 )
		pev->speed = 100;

	// DOOR_START_OPEN is to allow an entity to be lighted in the closed position
	// but spawn in the open position
	if( FBitSet( pev->spawnflags, SF_DOOR_START_OPEN ) )
	{	// swap pos1 and pos2, put door at pos2, invert movement direction
		pev->angles = m_vecAngle2;
		Vector vecSav = m_vecAngle1;
		m_vecAngle2 = m_vecAngle1;
		m_vecAngle1 = vecSav;
		pev->movedir = pev->movedir * -1;
	}

	m_toggle_state = TS_AT_BOTTOM;

	if( FBitSet( pev->spawnflags, SF_DOOR_USE_ONLY ) )
	{
		SetTouch( NULL );
	}
	else // touchable button
		SetTouch( &CRotDoor::DoorTouch );
}


void CRotDoor::SetToggleState( int state )
{
	if( state == TS_AT_TOP )
		pev->angles = m_vecAngle2;
	else
		pev->angles = m_vecAngle1;

	SetAbsOrigin( GetAbsOrigin() );
}
