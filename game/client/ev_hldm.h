//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined ( EV_HLDMH )
#define EV_HLDMH

#include "event_args.h"

typedef struct pmtrace_s pmtrace_t;

void EV_HLDM_GunshotDecalTrace( pmtrace_t *pTrace, char *decalName );
void EV_HLDM_DecalGunshot( pmtrace_t *pTrace, int iBulletType );
int EV_HLDM_CheckTracer( int idx, const Vector& vecSrc, const Vector& end, const Vector& forward, const Vector& right, int iBulletType, int iTracerFreq, int& tracerCount );
void EV_HLDM_FireBullets( int idx, const Vector& forward, const Vector& right, const Vector& up, 
						  int cShots, const Vector& vecSrc, const Vector& vecDirShooting, 
						  float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, float flSpreadX, float flSpreadY );

extern "C"
{

// HLDM
void EV_FireGlock1( event_args_t *args );
void EV_FireGlock2( event_args_t *args );
void EV_FireShotGunSingle( event_args_t *args );
void EV_FireShotGunDouble( event_args_t *args );
void EV_FireMP5( event_args_t *args );
void EV_FireMP52( event_args_t *args );
void EV_FirePython( event_args_t *args );
void EV_FireGauss( event_args_t *args );
void EV_SpinGauss( event_args_t *args );
void EV_Crowbar( event_args_t *args );
void EV_FireCrossbow( event_args_t *args );
void EV_FireCrossbow2( event_args_t *args );
void EV_FireRpg( event_args_t *args );
void EV_EgonFire( event_args_t *args );
void EV_EgonStop( event_args_t *args );
void EV_HornetGunFire( event_args_t *args );
void EV_TripmineFire( event_args_t *args );
void EV_SnarkFire( event_args_t *args );


void EV_TrainPitchAdjust( event_args_t *args );

#if USE_OPFOR
void EV_SniperRifle( event_args_t* args );
#endif
}

#endif // EV_HLDMH