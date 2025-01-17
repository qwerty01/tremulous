/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2013 Darklegion Development
Copyright (C) 2015-2019 GrangerHub

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, see <https://www.gnu.org/licenses/>

===========================================================================
*/

// cg_weapons.c -- events and effects dealing with weapons

#include "cg_local.h"

/*
=================
CG_RegisterUpgrade

The server says this item is used on this level
=================
*/
void CG_RegisterUpgrade( int upgradeNum )
{
  upgradeInfo_t   *upgradeInfo;
  char            *icon;

  if( upgradeNum <= UP_NONE || upgradeNum >= UP_NUM_UPGRADES )
  {
    CG_Error( "CG_RegisterUpgrade: out of range: %d", upgradeNum );
    return;
  }

  upgradeInfo = &cg_upgrades[ upgradeNum ];

  if( upgradeInfo->registered )
  {
    CG_Printf( "CG_RegisterUpgrade: already registered: (%d) %s\n", upgradeNum,
      BG_Upgrade( upgradeNum )->name );
    return;
  }

  upgradeInfo->registered = qtrue;

  if( !BG_Upgrade( upgradeNum )->name[ 0 ] )
    CG_Error( "Couldn't find upgrade %i", upgradeNum );

  upgradeInfo->humanName = BG_Upgrade( upgradeNum )->humanName;

  //la la la la la, i'm not listening!
  if( upgradeNum == UP_GRENADE )
    upgradeInfo->upgradeIcon = cg_weapons[ WP_GRENADE ].weaponIcon;
  else if( ( icon = BG_Upgrade( upgradeNum )->icon ) )
    upgradeInfo->upgradeIcon = trap_R_RegisterShader( icon );
}

/*
===============
CG_InitUpgrades

Precaches upgrades
===============
*/
void CG_InitUpgrades( void )
{
  int   i;

  Com_Memset( cg_upgrades, 0, sizeof( cg_upgrades ) );

  for( i = UP_NONE + 1; i < UP_NUM_UPGRADES; i++ )
    CG_RegisterUpgrade( i );
}


/*
======================
CG_ParseWeaponAnimationFile

Read a configuration file containing animation counts and rates
models/weapons/rifle/animation.cfg, etc
======================
*/
static qboolean CG_ParseWeaponAnimationFile( const char *filename, weaponInfo_t *weapon )
{
  char          *text_p;
  int           len;
  int           i;
  char          *token;
  float         fps;
  char          text[ 20000 ];
  fileHandle_t  f;
  animation_t   *animations;

  animations = weapon->animations;

  // load the file
  len = trap_FS_FOpenFile( filename, &f, FS_READ );
  if( len < 0 )
    return qfalse;

  if( len == 0 || len >= sizeof( text ) - 1 )
  {
    trap_FS_FCloseFile( f );
    CG_Printf( "File %s is %s\n", filename, len == 0 ? "empty" : "too long" );
    return qfalse;
  }

  trap_FS_Read( text, len, f );
  text[ len ] = 0;
  trap_FS_FCloseFile( f );

  // parse the text
  text_p = text;

  // read information for each frame
  for( i = WANIM_NONE + 1; i < MAX_WEAPON_ANIMATIONS; i++ )
  {

    token = COM_Parse( &text_p );
    if( !*token )
      break;

    if( !Q_stricmp( token, "noDrift" ) )
    {
      weapon->noDrift = qtrue;
      continue;
    }

    animations[ i ].firstFrame = atoi( token );

    token = COM_Parse( &text_p );
    if( !*token )
      break;

    animations[ i ].numFrames = atoi( token );
    animations[ i ].reversed = qfalse;
    animations[ i ].flipflop = qfalse;

    // if numFrames is negative the animation is reversed
    if( animations[ i ].numFrames < 0 )
    {
      animations[ i ].numFrames = -animations[ i ].numFrames;
      animations[ i ].reversed = qtrue;
    }

    token = COM_Parse( &text_p );
    if ( !*token )
      break;

    animations[i].loopFrames = atoi( token );

    token = COM_Parse( &text_p );
    if( !*token )
      break;

    fps = atof( token );
    if( fps == 0 )
      fps = 1;

    animations[ i ].frameLerp = 1000 / fps;
    animations[ i ].initialLerp = 1000 / fps;
  }

  if( i != MAX_WEAPON_ANIMATIONS )
  {
    CG_Printf( "Error parsing animation file: %s\n", filename );
    return qfalse;
  }

  return qtrue;
}


/*
===============
CG_ParseWeaponModeSection

Parse a weapon mode section
===============
*/
static qboolean CG_ParseWeaponModeSection( weaponInfoMode_t *wim, char **text_p )
{
  char  *token;
  int   i;

  // read optional parameters
  while( 1 )
  {
    token = COM_Parse( text_p );

    if( !token )
      break;

    if( !Q_stricmp( token, "" ) )
      return qfalse;

    if( !Q_stricmp( token, "missileModel" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileModel = trap_R_RegisterModel( token );

      if( !wim->missileModel )
        CG_Printf( S_COLOR_RED "ERROR: missile model not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "missileSprite" ) )
    {
      int size = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      size = atoi( token );

      if( size < 0 )
        size = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileSprite = trap_R_RegisterShader( token );
      wim->missileSpriteSize = size;
      wim->usesSpriteMissle = qtrue;

      if( !wim->missileSprite )
        CG_Printf( S_COLOR_RED "ERROR: missile sprite not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "missileSpriteCharge" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileSpriteCharge = atof( token );

      continue;
    }
    else if( !Q_stricmp( token, "missileRotates" ) )
    {
      wim->missileRotates = qtrue;

      continue;
    }
    else if( !Q_stricmp( token, "missileAnimates" ) )
    {
      wim->missileAnimates = qtrue;

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileAnimStartFrame = atoi( token );

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileAnimNumFrames = atoi( token );

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileAnimFrameRate = atoi( token );

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileAnimLooping = atoi( token );

      continue;
    }
    else if( !Q_stricmp( token, "missileParticleSystem" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileParticleSystem = CG_RegisterParticleSystem( token );

      if( !wim->missileParticleSystem )
        CG_Printf( S_COLOR_RED "ERROR: missile particle system not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "missileTrailSystem" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileTrailSystem = CG_RegisterTrailSystem( token );

      if( !wim->missileTrailSystem )
        CG_Printf( S_COLOR_RED "ERROR: missile trail system not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "muzzleParticleSystem" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->muzzleParticleSystem = CG_RegisterParticleSystem( token );

      if( !wim->muzzleParticleSystem )
        CG_Printf( S_COLOR_RED "ERROR: muzzle particle system not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "impactParticleSystem" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->impactParticleSystem = CG_RegisterParticleSystem( token );

      if( !wim->impactParticleSystem )
        CG_Printf( S_COLOR_RED "ERROR: impact particle system not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "impactMark" ) )
    {
      int size = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      size = atoi( token );

      if( size < 0 )
        size = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->impactMark = trap_R_RegisterShader( token );
      wim->impactMarkSize = size;

      if( !wim->impactMark )
        CG_Printf( S_COLOR_RED "ERROR: impact mark shader not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "impactSound" ) )
    {
      int index = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      index = atoi( token );

      if( index < 0 )
        index = 0;
      else if( index > 3 )
        index = 3;

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->impactSound[ index ] = trap_S_RegisterSound( token, qfalse );

      continue;
    }
    else if( !Q_stricmp( token, "impactFleshSound" ) )
    {
      int index = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      index = atoi( token );

      if( index < 0 )
        index = 0;
      else if( index > 3 )
        index = 3;

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->impactFleshSound[ index ] = trap_S_RegisterSound( token, qfalse );

      continue;
    }
    else if( !Q_stricmp( token, "alwaysImpact" ) )
    {
      wim->alwaysImpact = qtrue;

      continue;
    }
    else if( !Q_stricmp( token, "flashDLightColor" ) )
    {
      for( i = 0 ; i < 3 ; i++ )
      {
        token = COM_Parse( text_p );
        if( !token )
          break;

        wim->flashDlightColor[ i ] = atof( token );
      }

      continue;
    }
    else if( !Q_stricmp( token, "continuousFlash" ) )
    {
      wim->continuousFlash = qtrue;

      continue;
    }
    else if( !Q_stricmp( token, "missileDlightColor" ) )
    {
      for( i = 0 ; i < 3 ; i++ )
      {
        token = COM_Parse( text_p );
        if( !token )
          break;

        wim->missileDlightColor[ i ] = atof( token );
      }

      continue;
    }
    else if( !Q_stricmp( token, "missileDlight" ) )
    {
      int size = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      size = atoi( token );

      if( size < 0 )
        size = 0;

      wim->missileDlight = size;

      continue;
    }
    else if( !Q_stricmp( token, "firingSound" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->firingSound = trap_S_RegisterSound( token, qfalse );

      continue;
    }
    else if( !Q_stricmp( token, "missileSound" ) )
    {
      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->missileSound = trap_S_RegisterSound( token, qfalse );

      continue;
    }
    else if( !Q_stricmp( token, "flashSound" ) )
    {
      int index = 0;

      token = COM_Parse( text_p );
      if( !token )
        break;

      index = atoi( token );

      if( index < 0 )
        index = 0;
      else if( index > 3 )
        index = 3;

      token = COM_Parse( text_p );
      if( !token )
        break;

      wim->flashSound[ index ] = trap_S_RegisterSound( token, qfalse );

      continue;
    }
    else if( !Q_stricmp( token, "}" ) )
      return qtrue; //reached the end of this weapon section
    else
    {
      CG_Printf( S_COLOR_RED "ERROR: unknown token '%s' in weapon section\n", token );
      return qfalse;
    }
  }

  return qfalse;
}


/*
======================
CG_ParseWeaponFile

Parses a configuration file describing a weapon
======================
*/
static qboolean CG_ParseWeaponFile( const char *filename, weaponInfo_t *wi )
{
  char          *text_p;
  int           len;
  char          *token;
  char          text[ 20000 ];
  fileHandle_t  f;
  weaponMode_t  weaponMode = WPM_NONE;

  // load the file
  len = trap_FS_FOpenFile( filename, &f, FS_READ );
  if( len < 0 )
    return qfalse;

  if( len == 0 || len >= sizeof( text ) - 1 )
  {
    trap_FS_FCloseFile( f );
    CG_Printf( "File %s is %s\n", filename, len == 0 ? "empty" : "too long" );
    return qfalse;
  }

  trap_FS_Read( text, len, f );
  text[ len ] = 0;
  trap_FS_FCloseFile( f );

  // parse the text
  text_p = text;

  // read optional parameters
  while( 1 )
  {
    token = COM_Parse( &text_p );

    if( !token )
      break;

    if( !Q_stricmp( token, "" ) )
      break;

    if( !Q_stricmp( token, "{" ) )
    {
      if( weaponMode == WPM_NONE )
      {
        CG_Printf( S_COLOR_RED "ERROR: weapon mode section started without a declaration\n" );
        return qfalse;
      }
      else if( !CG_ParseWeaponModeSection( &wi->wim[ weaponMode ], &text_p ) )
      {
        CG_Printf( S_COLOR_RED "ERROR: failed to parse weapon mode section\n" );
        return qfalse;
      }

      //start parsing ejectors again
      weaponMode = WPM_NONE;

      continue;
    }
    else if( !Q_stricmp( token, "primary" ) )
    {
      weaponMode = WPM_PRIMARY;
      continue;
    }
    else if( !Q_stricmp( token, "secondary" ) )
    {
      weaponMode = WPM_SECONDARY;
      continue;
    }
    else if( !Q_stricmp( token, "tertiary" ) )
    {
      weaponMode = WPM_TERTIARY;
      continue;
    }
    else if( !Q_stricmp( token, "weaponModel" ) )
    {
      char path[ MAX_QPATH ];

      token = COM_Parse( &text_p );
      if( !token )
        break;

      wi->weaponModel = trap_R_RegisterModel( token );

      if( !wi->weaponModel )
        CG_Printf( S_COLOR_RED "ERROR: weapon model not found %s\n", token );

      strcpy( path, token );
      COM_StripExtension( path, path, MAX_QPATH );
      strcat( path, "_flash.md3" );
      wi->flashModel = trap_R_RegisterModel( path );

      strcpy( path, token );
      COM_StripExtension( path, path, MAX_QPATH );
      strcat( path, "_barrel.md3" );
      wi->barrelModel = trap_R_RegisterModel( path );

      strcpy( path, token );
      COM_StripExtension( path, path, MAX_QPATH );
      strcat( path, "_hand.md3" );
      wi->handsModel = trap_R_RegisterModel( path );

      continue;
    }
    else if( !Q_stricmp( token, "weaponModel3rdPerson" ) )
    {
      char path[ MAX_QPATH ];

      token = COM_Parse( &text_p );
      if( !token )
        break;

      wi->weaponModel3rdPerson = trap_R_RegisterModel( token );

      if( !wi->weaponModel3rdPerson )
      {
        CG_Printf( S_COLOR_RED "ERROR: 3rd person weapon "
            "model not found %s\n", token );
      }

      strcpy( path, token );
      COM_StripExtension( path, path, MAX_QPATH );
      strcat( path, "_flash.md3" );
      wi->flashModel3rdPerson = trap_R_RegisterModel( path );

      strcpy( path, token );
      COM_StripExtension( path, path, MAX_QPATH );
      strcat( path, "_barrel.md3" );
      wi->barrelModel3rdPerson = trap_R_RegisterModel( path );

      continue;
    }
    else if( !Q_stricmp( token, "idleSound" ) )
    {
      token = COM_Parse( &text_p );
      if( !token )
        break;

      wi->readySound = trap_S_RegisterSound( token, qfalse );

      continue;
    }
    else if( !Q_stricmp( token, "icon" ) )
    {
      token = COM_Parse( &text_p );
      if( !token )
        break;

      wi->weaponIcon = wi->ammoIcon = trap_R_RegisterShader( token );

      if( !wi->weaponIcon )
        CG_Printf( S_COLOR_RED "ERROR: weapon icon not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "crosshair" ) )
    {
      int size = 0;

      token = COM_Parse( &text_p );
      if( !token )
        break;

      size = atoi( token );

      if( size < 0 )
        size = 0;

      token = COM_Parse( &text_p );
      if( !token )
        break;

      wi->crossHair = trap_R_RegisterShader( token );
      wi->crossHairSize = size;

      if( !wi->crossHair )
        CG_Printf( S_COLOR_RED "ERROR: weapon crosshair not found %s\n", token );

      continue;
    }
    else if( !Q_stricmp( token, "disableIn3rdPerson" ) )
    {
      wi->disableIn3rdPerson = qtrue;

      continue;
    }

    Com_Printf( S_COLOR_RED "ERROR: unknown token '%s'\n", token );
    return qfalse;
  }

  return qtrue;
}

/*
=================
CG_RegisterWeapon
=================
*/
void CG_RegisterWeapon( int weaponNum )
{
  weaponInfo_t  *weaponInfo;
  char          path[ MAX_QPATH ];
  vec3_t        mins, maxs;
  int           i;

  if( weaponNum <= WP_NONE || weaponNum >= WP_NUM_WEAPONS )
  {
    CG_Error( "CG_RegisterWeapon: out of range: %d", weaponNum );
    return;
  }

  weaponInfo = &cg_weapons[ weaponNum ];

  if( weaponInfo->registered )
  {
    CG_Printf( "CG_RegisterWeapon: already registered: (%d) %s\n", weaponNum,
      BG_Weapon( weaponNum )->name );
    return;
  }

  weaponInfo->registered = qtrue;

  if( !BG_Weapon( weaponNum )->name[ 0 ] )
    CG_Error( "Couldn't find weapon %i", weaponNum );

  Com_sprintf( path, MAX_QPATH, "models/weapons/%s/weapon.cfg", BG_Weapon( weaponNum )->name );

  weaponInfo->humanName = BG_Weapon( weaponNum )->humanName;

  if( !CG_ParseWeaponFile( path, weaponInfo ) )
    Com_Printf( S_COLOR_RED "ERROR: failed to parse %s\n", path );

  Com_sprintf( path, MAX_QPATH, "models/weapons/%s/animation.cfg", BG_Weapon( weaponNum )->name );

  if( !CG_ParseWeaponAnimationFile( path, weaponInfo ) )
    Com_Printf( S_COLOR_RED "ERROR: failed to parse %s\n", path );

  // calc midpoint for rotation
  trap_R_ModelBounds( weaponInfo->weaponModel, mins, maxs );
  for( i = 0 ; i < 3 ; i++ )
    weaponInfo->weaponMidpoint[ i ] = mins[ i ] + 0.5 * ( maxs[ i ] - mins[ i ] );
}

/*
===============
CG_InitWeapons

Precaches weapons
===============
*/
void CG_InitWeapons( void )
{
  int   i;

  Com_Memset( cg_weapons, 0, sizeof( cg_weapons ) );

  for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
    CG_RegisterWeapon( i );

  cgs.media.level2ZapTS = CG_RegisterTrailSystem( "models/weapons/lev2zap/lightning" );
}


/*
========================================================================================

VIEW WEAPON

========================================================================================
*/

/*
===============
CG_SetWeaponLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
static void CG_SetWeaponLerpFrameAnimation( weapon_t weapon, lerpFrame_t *lf, int newAnimation )
{
  animation_t *anim;

  lf->animationNumber = newAnimation;
  newAnimation &= ~ANIM_TOGGLEBIT;

  if( newAnimation < 0 || newAnimation >= MAX_WEAPON_ANIMATIONS )
    CG_Error( "Bad animation number: %i", newAnimation );

  anim = &cg_weapons[ weapon ].animations[ newAnimation ];

  lf->animation = anim;
  lf->animationTime = lf->frameTime + anim->initialLerp;

  if( cg_debugAnim.integer )
    CG_Printf( "Anim: %i\n", newAnimation );
}

/*
===============
CG_WeaponAnimation
===============
*/
static void CG_WeaponAnimation( centity_t *cent, int *old, int *now, float *backLerp )
{
  lerpFrame_t   *lf = &cent->pe.weapon;
  entityState_t *es = &cent->currentState;

  // see if the animation sequence is switching
  if( es->weaponAnim != lf->animationNumber || !lf->animation )
    CG_SetWeaponLerpFrameAnimation( es->weapon, lf, es->weaponAnim );

  CG_RunLerpFrame( lf, 1.0f );

  *old      = lf->oldFrame;
  *now      = lf->frame;
  *backLerp = lf->backlerp;
}

/*
=================
CG_MapTorsoToWeaponFrame

=================
*/
static int CG_MapTorsoToWeaponFrame( clientInfo_t *ci, int frame )
{

  // change weapon
  if( frame >= ci->animations[ TORSO_DROP ].firstFrame &&
      frame < ci->animations[ TORSO_DROP ].firstFrame + 9 )
    return frame - ci->animations[ TORSO_DROP ].firstFrame + 6;

  // stand attack
  if( frame >= ci->animations[ TORSO_ATTACK ].firstFrame &&
      frame < ci->animations[ TORSO_ATTACK ].firstFrame + 6 )
    return 1 + frame - ci->animations[ TORSO_ATTACK ].firstFrame;

  // stand attack 2
  if( frame >= ci->animations[ TORSO_ATTACK2 ].firstFrame &&
      frame < ci->animations[ TORSO_ATTACK2 ].firstFrame + 6 )
    return 1 + frame - ci->animations[ TORSO_ATTACK2 ].firstFrame;

  return 0;
}


/*
==============
CG_CalculateWeaponPosition
==============
*/
static void CG_CalculateWeaponPosition( vec3_t origin, vec3_t angles )
{
  float         scale;
  int           delta;
  float         fracsin;
  float         bob;
  weaponInfo_t  *weapon;

  weapon = &cg_weapons[ cg.predictedPlayerState.weapon ];

  VectorCopy( cg.refdef.vieworg, origin );
  VectorCopy( cg.refdefViewAngles, angles );

  // on odd legs, invert some angles
  if( cg.bobcycle & 1 )
    scale = -cg.xyspeed;
  else
    scale = cg.xyspeed;

  // gun angles from bobbing
  // bob amount is class dependant
  bob = BG_Class( cg.predictedPlayerState.stats[ STAT_CLASS ] )->bob;

  if( bob != 0 )
  {
    angles[ ROLL ] += scale * cg.bobfracsin * 0.005;
    angles[ YAW ] += scale * cg.bobfracsin * 0.01;
    angles[ PITCH ] += cg.xyspeed * cg.bobfracsin * 0.005;
  }

  // drop the weapon when landing
  if( !weapon->noDrift )
  {
    delta = cg.time - cg.landTime;
    if( delta < LAND_DEFLECT_TIME )
      origin[ 2 ] += cg.landChange*0.25 * delta / LAND_DEFLECT_TIME;
    else if( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME )
      origin[ 2 ] += cg.landChange*0.25 *
        ( LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta ) / LAND_RETURN_TIME;

    // idle drift
    scale = cg.xyspeed + 40;
    fracsin = sin( cg.time * 0.001 );
    angles[ ROLL ] += scale * fracsin * 0.01;
    angles[ YAW ] += scale * fracsin * 0.01;
    angles[ PITCH ] += scale * fracsin * 0.01;
  }
}


/*
======================
CG_MachinegunSpinAngle
======================
*/
#define   SPIN_SPEED  0.9
#define   COAST_TIME  1000
static float CG_MachinegunSpinAngle( centity_t *cent, qboolean firing )
{
  int   delta;
  float angle;
  float speed;

  delta = cg.time - cent->pe.barrelTime;
  if( cent->pe.barrelSpinning )
    angle = cent->pe.barrelAngle + delta * SPIN_SPEED;
  else
  {
    if( delta > COAST_TIME )
      delta = COAST_TIME;

    speed = 0.5 * ( SPIN_SPEED + (float)( COAST_TIME - delta ) / COAST_TIME );
    angle = cent->pe.barrelAngle + delta * speed;
  }

  if( cent->pe.barrelSpinning == !firing )
  {
    cent->pe.barrelTime = cg.time;
    cent->pe.barrelAngle = AngleMod( angle );
    cent->pe.barrelSpinning = firing;
  }

  return angle;
}


/*
=============
CG_AddPlayerWeapon

Used for both the view weapon (ps is valid) and the world modelother character models (ps is NULL)
The main player will have this called for BOTH cases, so effects like light and
sound should only be done on the world model case.
=============
*/
void CG_AddPlayerWeapon( refEntity_t *parent, playerState_t *ps, centity_t *cent )
{
  refEntity_t   gun;
  refEntity_t   barrel;
  refEntity_t   flash;
  vec3_t        angles;
  weapon_t      weaponNum;
  weaponMode_t  weaponMode;
  weaponInfo_t  *weapon;
  qboolean      noGunModel;
  qboolean      firing;

  weaponNum = cent->currentState.weapon;
  weaponMode = cent->currentState.generic1;

  if( weaponNum <= WP_NONE || weaponNum >= WP_NUM_WEAPONS )
  {
    Com_Printf( S_COLOR_YELLOW "WARNING: CG_AddPlayerWeapon: weapon "
                "number %i is out of bounds", weaponNum );
    return;
  }

  if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    weaponMode = WPM_PRIMARY;

  if( ( ( cent->currentState.eFlags & EF_FIRING ) && weaponMode == WPM_PRIMARY ) ||
      ( ( cent->currentState.eFlags & EF_FIRING2 ) && weaponMode == WPM_SECONDARY ) ||
      ( ( cent->currentState.eFlags & EF_FIRING3 ) && weaponMode == WPM_TERTIARY ) )
    firing = qtrue;
  else
    firing = qfalse;

  weapon = &cg_weapons[ weaponNum ];
  if( !weapon->registered )
  {
    Com_Printf( S_COLOR_YELLOW "WARNING: CG_AddPlayerWeapon: weapon %d (%s) "
        "is not registered\n", weaponNum, BG_Weapon( weaponNum )->name );
    return;
  }

  // add the weapon
  Com_Memset( &gun, 0, sizeof( gun ) );
  Com_Memset( &barrel, 0, sizeof( barrel ) );
  Com_Memset( &flash, 0, sizeof( flash ) );

  VectorCopy( parent->lightingOrigin, gun.lightingOrigin );
  gun.shadowPlane = parent->shadowPlane;
  gun.renderfx = parent->renderfx;

  if( ps )
  {
    gun.shaderRGBA[ 0 ] = 255;
    gun.shaderRGBA[ 1 ] = 255;
    gun.shaderRGBA[ 2 ] = 255;
    gun.shaderRGBA[ 3 ] = 255;

    //set weapon[1/2]Time when respective buttons change state
    if( cg.weapon1Firing != ( cg.predictedPlayerState.eFlags & EF_FIRING ) )
    {
      cg.weapon1Time = cg.time;
      cg.weapon1Firing = ( cg.predictedPlayerState.eFlags & EF_FIRING );
    }

    if( cg.weapon2Firing != ( cg.predictedPlayerState.eFlags & EF_FIRING2 ) )
    {
      cg.weapon2Time = cg.time;
      cg.weapon2Firing = ( cg.predictedPlayerState.eFlags & EF_FIRING2 );
    }

    if( cg.weapon3Firing != ( cg.predictedPlayerState.eFlags & EF_FIRING3 ) )
    {
      cg.weapon3Time = cg.time;
      cg.weapon3Firing = ( cg.predictedPlayerState.eFlags & EF_FIRING3 );
    }
  }

  if( !ps )
  {
    gun.hModel = weapon->weaponModel3rdPerson;

    if( !gun.hModel )
      gun.hModel = weapon->weaponModel;
  }
  else
    gun.hModel = weapon->weaponModel;

  noGunModel = ( ( !ps || cg.renderingThirdPerson ) && weapon->disableIn3rdPerson ) || !gun.hModel;

  if( !ps )
  {
    // add weapon ready sound
    if( firing && weapon->wim[ weaponMode ].firingSound )
    {
      trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin,
                              weapon->wim[ weaponMode ].firingSound );
    }
    else if( weapon->readySound )
      trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->readySound );
  }

  // Lucifer cannon charge warning beep
  if( weaponNum == WP_LUCIFER_CANNON &&
      ( cent->currentState.eFlags & EF_WARN_CHARGE ) &&
      cg.snap->ps.stats[ STAT_TEAM ] != TEAM_ALIENS )
  {
    trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin,
                            vec3_origin, ps ? cgs.media.lCannonWarningSound :
                                              cgs.media.lCannonWarningSound2 );
  }

  if( !noGunModel )
  {
    CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_weapon" );
    CG_WeaponAnimation( cent, &gun.oldframe, &gun.frame, &gun.backlerp );

    trap_R_AddRefEntityToScene( &gun );

    if( !ps )
    {
      barrel.hModel = weapon->barrelModel3rdPerson;

      if( !barrel.hModel )
        barrel.hModel = weapon->barrelModel;
    }
    else
      barrel.hModel = weapon->barrelModel;

    // add the spinning barrel
    if( barrel.hModel )
    {
      VectorCopy( parent->lightingOrigin, barrel.lightingOrigin );
      barrel.shadowPlane = parent->shadowPlane;
      barrel.renderfx = parent->renderfx;

      angles[ YAW ] = 0;
      angles[ PITCH ] = 0;
      angles[ ROLL ] = CG_MachinegunSpinAngle( cent, firing );
      AnglesToAxis( angles, barrel.axis );

      CG_PositionRotatedEntityOnTag( &barrel, &gun, gun.hModel, "tag_barrel" );

      trap_R_AddRefEntityToScene( &barrel );
    }
  }

  if( CG_IsParticleSystemValid( &cent->muzzlePS ) )
  {
    if( ps || cg.renderingThirdPerson ||
        cent->currentState.number != cg.predictedPlayerState.clientNum )
    {
      if( noGunModel )
        CG_SetAttachmentTag( &cent->muzzlePS->attachment, *parent, parent->hModel, "tag_weapon" );
      else
        CG_SetAttachmentTag( &cent->muzzlePS->attachment, gun, gun.hModel, "tag_flash" );
    }

    //if the PS is infinite disable it when not firing
    if( !firing && CG_IsParticleSystemInfinite( cent->muzzlePS ) )
      CG_DestroyParticleSystem( &cent->muzzlePS );
  }

  // add the flash
  if( !weapon->wim[ weaponMode ].continuousFlash || !firing )
  {
    // impulse flash
    if( cg.time - cent->muzzleFlashTime > MUZZLE_FLASH_TIME )
      return;
  }

  VectorCopy( parent->lightingOrigin, flash.lightingOrigin );
  flash.shadowPlane = parent->shadowPlane;
  flash.renderfx = parent->renderfx;

  if( !ps )
  {
    flash.hModel = weapon->flashModel3rdPerson;

    if( !flash.hModel )
      flash.hModel = weapon->flashModel;
  }
  else
    flash.hModel = weapon->flashModel;

  if( flash.hModel )
  {
    angles[ YAW ] = 0;
    angles[ PITCH ] = 0;
    angles[ ROLL ] = crandom( ) * 10;
    AnglesToAxis( angles, flash.axis );

    if( noGunModel )
      CG_PositionRotatedEntityOnTag( &flash, parent, parent->hModel, "tag_weapon" );
    else
      CG_PositionRotatedEntityOnTag( &flash, &gun, gun.hModel, "tag_flash" );

    trap_R_AddRefEntityToScene( &flash );
  }

  if( ps || cg.renderingThirdPerson ||
      cent->currentState.number != cg.predictedPlayerState.clientNum )
  {
    if( weapon->wim[ weaponMode ].muzzleParticleSystem && cent->muzzlePsTrigger )
    {
      cent->muzzlePS = CG_SpawnNewParticleSystem( weapon->wim[ weaponMode ].muzzleParticleSystem );

      if( CG_IsParticleSystemValid( &cent->muzzlePS ) )
      {
        if( noGunModel )
          CG_SetAttachmentTag( &cent->muzzlePS->attachment, *parent, parent->hModel, "tag_weapon" );
        else
          CG_SetAttachmentTag( &cent->muzzlePS->attachment, gun, gun.hModel, "tag_flash" );

        CG_SetAttachmentCent( &cent->muzzlePS->attachment, cent );
        CG_AttachToTag( &cent->muzzlePS->attachment );
      }

      cent->muzzlePsTrigger = qfalse;
    }

    // make a dlight for the flash
    if( weapon->wim[ weaponMode ].flashDlightColor[ 0 ] ||
        weapon->wim[ weaponMode ].flashDlightColor[ 1 ] ||
        weapon->wim[ weaponMode ].flashDlightColor[ 2 ] )
    {
      trap_R_AddLightToScene( flash.origin, 300 + ( rand( ) & 31 ),
          weapon->wim[ weaponMode ].flashDlightColor[ 0 ],
          weapon->wim[ weaponMode ].flashDlightColor[ 1 ],
          weapon->wim[ weaponMode ].flashDlightColor[ 2 ] );
    }
  }
}

/*
==============
CG_AddViewWeapon

Add the weapon, and flash for the player's view
==============
*/

#define WEAPON_CLICK_REPEAT 500

void CG_AddViewWeapon( playerState_t *ps )
{
  refEntity_t   hand;
  centity_t     *cent;
  clientInfo_t  *ci;
  float         fovOffset;
  vec3_t        angles;
  weaponInfo_t  *wi;
  weapon_t      weapon = ps->weapon;
  weaponMode_t  weaponMode = ps->generic1;

  // no weapon carried - can't draw it
  if( weapon == WP_NONE )
    return;

  if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    weaponMode = WPM_PRIMARY;

  wi = &cg_weapons[ weapon ];
  if( !wi->registered )
  {
    Com_Printf( S_COLOR_YELLOW "WARNING: CG_AddViewWeapon: weapon %d (%s) "
        "is not registered\n", weapon, BG_Weapon( weapon )->name );
    return;
  }
  cent = &cg.predictedPlayerEntity; // &cg_entities[cg.snap->ps.clientNum];

  if( ps->persistant[PERS_SPECSTATE] != SPECTATOR_NOT )
    return;

  if( ps->pm_type == PM_INTERMISSION )
    return;

  // draw a prospective buildable infront of the player
  if( ( ps->stats[ STAT_BUILDABLE ] & SB_BUILDABLE_MASK ) > BA_NONE )
    CG_GhostBuildable( ps->stats[ STAT_BUILDABLE ] & SB_BUILDABLE_MASK );

  // no gun if in third person view
  if( cg.renderingThirdPerson )
    return;

  // allow the gun to be completely removed
  if( !cg_drawGun.integer )
  {
    vec3_t origin;

    VectorCopy( cg.refdef.vieworg, origin );
    VectorMA( origin, -8, cg.refdef.viewaxis[ 2 ], origin );

    if( cent->muzzlePS )
      CG_SetAttachmentPoint( &cent->muzzlePS->attachment, origin );

    //check for particle systems
    if( wi->wim[ weaponMode ].muzzleParticleSystem && cent->muzzlePsTrigger )
    {
      cent->muzzlePS = CG_SpawnNewParticleSystem( wi->wim[ weaponMode ].muzzleParticleSystem );

      if( CG_IsParticleSystemValid( &cent->muzzlePS ) )
      {
        CG_SetAttachmentPoint( &cent->muzzlePS->attachment, origin );
        CG_SetAttachmentCent( &cent->muzzlePS->attachment, cent );
        CG_AttachToPoint( &cent->muzzlePS->attachment );
      }
      cent->muzzlePsTrigger = qfalse;
    }

    return;
  }

  // don't draw if testing a gun model
  if( cg.testGun )
    return;

  // drop gun lower at higher fov
  if( cg.refdef.fov_y > 90 )
    fovOffset = -0.4 * ( cg.refdef.fov_y - 90 );
  else
    fovOffset = 0;

  Com_Memset( &hand, 0, sizeof( hand ) );

  // set up gun position
  CG_CalculateWeaponPosition( hand.origin, angles );

  VectorMA( hand.origin, cg_gun_x.value, cg.refdef.viewaxis[ 0 ], hand.origin );
  VectorMA( hand.origin, cg_gun_y.value, cg.refdef.viewaxis[ 1 ], hand.origin );
  VectorMA( hand.origin, ( cg_gun_z.value + fovOffset ), cg.refdef.viewaxis[ 2 ], hand.origin );

  // Lucifer Cannon vibration effect
  if( weapon == WP_LUCIFER_CANNON && ps->stats[ STAT_MISC ] > 0 )
  {
    float fraction;

    fraction = (float)ps->stats[ STAT_MISC ] / LCANNON_CHARGE_TIME_MAX;
    VectorMA( hand.origin, random( ) * fraction, cg.refdef.viewaxis[ 0 ],
              hand.origin );
    VectorMA( hand.origin, random( ) * fraction, cg.refdef.viewaxis[ 1 ],
              hand.origin );
  }

  AnglesToAxis( angles, hand.axis );

  // map torso animations to weapon animations
  if( cg_gun_frame.integer )
  {
    // development tool
    hand.frame = hand.oldframe = cg_gun_frame.integer;
    hand.backlerp = 0;
  }
  else
  {
    // get clientinfo for animation map
    ci = &cgs.clientinfo[ cent->currentState.clientNum ];
    hand.frame = CG_MapTorsoToWeaponFrame( ci, cent->pe.torso.frame );
    hand.oldframe = CG_MapTorsoToWeaponFrame( ci, cent->pe.torso.oldFrame );
    hand.backlerp = cent->pe.torso.backlerp;
  }

  hand.hModel = wi->handsModel;
  hand.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON | RF_MINLIGHT;

  // add everything onto the hand
  CG_AddPlayerWeapon( &hand, ps, &cg.predictedPlayerEntity );
}

/*
==============================================================================

WEAPON SELECTION

==============================================================================
*/

/*
===============
CG_WeaponSelectable
===============
*/
static qboolean CG_WeaponSelectable( weapon_t weapon )
{
  return BG_InventoryContainsWeapon( weapon, cg.snap->ps.stats );
}


/*
===============
CG_UpgradeSelectable
===============
*/
static qboolean CG_UpgradeSelectable( upgrade_t upgrade )
{
  if( !BG_InventoryContainsUpgrade( upgrade, cg.snap->ps.stats ) )
    return qfalse;

  return BG_Upgrade( upgrade )->usable;
}


#define ICON_BORDER 4

/*
===================
CG_DrawItemSelect
===================
*/
void CG_DrawItemSelect( rectDef_t *rect, vec4_t color )
{
  int           i;
  float         x = rect->x;
  float         y = rect->y;
  float         width = rect->w;
  float         height = rect->h;
  float         iconWidth;
  float         iconHeight;
  int           items[ 64 ];
  int           colinfo[ 64 ];
  int           numItems = 0, selectedItem = 0;
  int           length;
  qboolean      vertical;
  playerState_t *ps;

  ps = &cg.snap->ps;

  // don't display if dead
  if( cg.predictedPlayerState.stats[ STAT_HEALTH ] <= 0 )
    return;

  CG_AutoSelectWeapon_f( );

  // showing weapon select clears pickup item display, but not the blend blob
  cg.itemPickupTime = 0;

  // put all weapons in the items list
  for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
  {
    if( !BG_InventoryContainsWeapon( i, cg.snap->ps.stats ) )
      continue;

    if( !ps->ammo && !ps->clips && !BG_Weapon( i )->infiniteAmmo )
      colinfo[ numItems ] = 1;
    else
      colinfo[ numItems ] = 0;

    if( i == cg.weaponSelect )
      selectedItem = numItems;

    if( !cg_weapons[ i ].registered )
    {
      Com_Printf( S_COLOR_YELLOW "WARNING: CG_DrawItemSelect: weapon %d (%s) "
  	"is not registered\n", i, BG_Weapon( i )->name );
      continue;
    }
    items[ numItems ] = i;
    numItems++;
  }

  // put all upgrades in the weapons list
  for( i = UP_NONE + 1; i < UP_NUM_UPGRADES; i++ )
  {
    if( !BG_InventoryContainsUpgrade( i, cg.snap->ps.stats ) )
      continue;
    colinfo[ numItems ] = 0;
    if( !BG_Upgrade( i )->usable )
      colinfo[ numItems ] = 2;

    if( i == cg.weaponSelect - 32 )
      selectedItem = numItems;

    if( !cg_upgrades[ i ].registered )
    {
      Com_Printf( S_COLOR_YELLOW "WARNING: CG_DrawItemSelect: upgrade %d (%s) "
  	"is not registered\n", i, BG_Upgrade( i )->name );
      continue;
    }
    items[ numItems ] = i + 32;
    numItems++;
  }

  // compute the length of the display window and determine orientation
  vertical = height > width;
  if( vertical )
  {
    iconWidth = width * cgDC.aspectScale;
    iconHeight = width;
    length = height / ( width * cgDC.aspectScale );
  }
  else
  {
    iconWidth = height * cgDC.aspectScale;
    iconHeight = height;
    length = width / ( height * cgDC.aspectScale );
  }

  // render icon ring
  for( i = 0; i < length; i++ )
  {
    int item = i - length / 2 + selectedItem;

    if( item < 0 )
      item += length;
    else if( item >= length )
      item -= length;
    if( item >= 0 && item < numItems )
    {
      switch( colinfo[ item ] )
      {
       case 0:
         color = colorCyan;
         break;
       case 1:
         color = colorRed;
         break;
       case 2:
         color = colorGray;
         break;
      }
      color[3] = 0.5;
      trap_R_SetColor( color );

      if( items[ item ] < 32 )
        CG_DrawPic( x, y, iconWidth, iconHeight,
                    cg_weapons[ items[ item ] ].weaponIcon );
      else
        CG_DrawPic( x, y, iconWidth, iconHeight,
                    cg_upgrades[ items[ item ] - 32 ].upgradeIcon );
    }
    if( vertical )
      y += iconHeight;
    else
      x += iconWidth;
  }
  trap_R_SetColor( NULL );
}


/*
===================
CG_DrawItemSelectText
===================
*/
void CG_DrawItemSelectText( rectDef_t *rect, float scale, int textStyle )
{
  int x, w;
  char  *name;
  float *color;

  color = CG_FadeColor( cg.weaponSelectTime, WEAPON_SELECT_TIME );
  if( !color )
    return;

  trap_R_SetColor( color );

  // draw the selected name
  if( cg.weaponSelect < 32 )
  {
    if( cg_weapons[ cg.weaponSelect ].registered &&
        BG_InventoryContainsWeapon( cg.weaponSelect, cg.snap->ps.stats ) )
    {
      if( ( name = cg_weapons[ cg.weaponSelect ].humanName ) )
      {
        w = UI_Text_Width( name, scale );
        x = rect->x + rect->w / 2;
        UI_Text_Paint( x - w / 2, rect->y + rect->h, scale, color, name, 0, 0, textStyle );
      }
    }
  }
  else
  {
    if( cg_upgrades[ cg.weaponSelect - 32 ].registered &&
        BG_InventoryContainsUpgrade( cg.weaponSelect - 32, cg.snap->ps.stats ) )
    {
      if( ( name = cg_upgrades[ cg.weaponSelect - 32 ].humanName ) )
      {
        w = UI_Text_Width( name, scale );
        x = rect->x + rect->w / 2;
        UI_Text_Paint( x - w / 2, rect->y + rect->h, scale, color, name, 0, 0, textStyle );
      }
    }
  }

  trap_R_SetColor( NULL );
}


/*
===============
CG_AutoSelectWeapon_f
===============
*/
void CG_AutoSelectWeapon_f( void )
{
  static weapon_t oldWeapon;
  weapon_t        weapon;
  playerState_t   *ps;

  if( ( cg.snap->ps.pm_flags & PMF_FOLLOW ) )
    return;

  ps = &cg.snap->ps;

  weapon = BG_GetPlayerWeapon( ps );

  // Allways reset cursor on current weapon
  if ( weapon != oldWeapon )
  {
    oldWeapon = weapon;
    if( cg.weaponSelect != weapon && CG_WeaponSelectable ( weapon ) )
      cg.weaponSelect = weapon;
  }

  // make sure that whatever it selected is actually selectable
  if( cg.weaponSelect < 32 )
  {
    if( !CG_WeaponSelectable( cg.weaponSelect ) )
      CG_NextWeapon_f( );
  }
  else
  {
    if( !CG_UpgradeSelectable( cg.weaponSelect - 32 ) )
      CG_NextWeapon_f( );
  }
}


/*
===============
CG_NextWeapon_f
===============
*/
void CG_NextWeapon_f( void )
{
  int   i;
  int   original;

  if( !cg.snap )
    return;

  if( cg.snap->ps.pm_flags & PMF_FOLLOW )
  {
    trap_SendClientCommand( "followprev\n" );
    return;
  }

  cg.weaponSelectTime = cg.time;
  original = cg.weaponSelect;

  for( i = 0; i < 64; i++ )
  {
    cg.weaponSelect++;
    if( cg.weaponSelect == 64 )
      cg.weaponSelect = 0;

    if( cg.weaponSelect < 32 )
    {
      if( CG_WeaponSelectable( cg.weaponSelect ) )
        break;
    }
    else
    {
      if( CG_UpgradeSelectable( cg.weaponSelect - 32 ) )
        break;
    }
  }

  if( i == 64 )
    cg.weaponSelect = original;
}

/*
===============
CG_PrevWeapon_f
===============
*/
void CG_PrevWeapon_f( void )
{
  int   i;
  int   original;

  if( !cg.snap )
    return;

  if( cg.snap->ps.pm_flags & PMF_FOLLOW )
  {
    trap_SendClientCommand( "follownext\n" );
    return;
  }

  cg.weaponSelectTime = cg.time;
  original = cg.weaponSelect;

  for( i = 0; i < 64; i++ )
  {
    cg.weaponSelect--;
    if( cg.weaponSelect == -1 )
      cg.weaponSelect = 63;

    if( cg.weaponSelect < 32 )
    {
      if( CG_WeaponSelectable( cg.weaponSelect ) )
        break;
    }
    else
    {
      if( CG_UpgradeSelectable( cg.weaponSelect - 32 ) )
        break;
    }
  }

  if( i == 64 )
    cg.weaponSelect = original;
}

/*
===============
CG_Weapon_f
===============
*/
void CG_Weapon_f( void )
{
  int   num;

  if( !cg.snap )
    return;

  if( cg.snap->ps.pm_flags & PMF_FOLLOW )
    return;

  num = atoi( CG_Argv( 1 ) );

  if( num < 1 || num > 31 )
    return;

  cg.weaponSelectTime = cg.time;

  if( !BG_InventoryContainsWeapon( num, cg.snap->ps.stats ) )
    return;   // don't have the weapon

  cg.weaponSelect = num;
}


/*
===================================================================================================

WEAPON EVENTS

===================================================================================================
*/

/*
================
CG_FireWeapon

Caused by an EV_FIRE_WEAPON event
================
*/
void CG_FireWeapon( centity_t *cent, weaponMode_t weaponMode )
{
  entityState_t     *es;
  int               c;
  weaponInfo_t      *wi;
  weapon_t          weaponNum;

  es = &cent->currentState;

  weaponNum = es->weapon;

  if( weaponNum == WP_NONE )
    return;

  if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    weaponMode = WPM_PRIMARY;

  if( weaponNum >= WP_NUM_WEAPONS )
  {
    CG_Error( "CG_FireWeapon: ent->weapon >= WP_NUM_WEAPONS" );
    return;
  }

  wi = &cg_weapons[ weaponNum ];

  // mark the entity as muzzle flashing, so when it is added it will
  // append the flash to the weapon model
  cent->muzzleFlashTime = cg.time;

  if( wi->wim[ weaponMode ].muzzleParticleSystem )
  {
    if( !CG_IsParticleSystemValid( &cent->muzzlePS ) ||
        !CG_IsParticleSystemInfinite( cent->muzzlePS ) )
      cent->muzzlePsTrigger = qtrue;
  }

  // play a sound
  for( c = 0; c < 4; c++ )
  {
    if( !wi->wim[ weaponMode ].flashSound[ c ] )
      break;
  }

  if( c > 0 )
  {
    c = rand( ) % c;
    if( wi->wim[ weaponMode ].flashSound[ c ] )
      trap_S_StartSound( NULL, es->number, CHAN_WEAPON, wi->wim[ weaponMode ].flashSound[ c ] );
  }
}


/*
=================
CG_MissileHitWall

Caused by an EV_MISSILE_MISS event, or directly by local bullet tracing
=================
*/
void CG_MissileHitWall( weapon_t weaponNum, weaponMode_t weaponMode, int clientNum,
                        vec3_t origin, vec3_t dir, impactSound_t soundType, int charge )
{
  qhandle_t           mark = 0;
  qhandle_t           ps = 0;
  int                 c;
  float               radius = 1.0f;
  weaponInfo_t        *weapon = &cg_weapons[ weaponNum ];

  if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    weaponMode = WPM_PRIMARY;

  mark = weapon->wim[ weaponMode ].impactMark;
  radius = weapon->wim[ weaponMode ].impactMarkSize;
  ps = weapon->wim[ weaponMode ].impactParticleSystem;

  if( soundType == IMPACTSOUND_FLESH )
  {
    //flesh sound
    for( c = 0; c < 4; c++ )
    {
      if( !weapon->wim[ weaponMode ].impactFleshSound[ c ] )
        break;
    }

    if( c > 0 )
    {
      c = rand( ) % c;
      if( weapon->wim[ weaponMode ].impactFleshSound[ c ] )
        trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, weapon->wim[ weaponMode ].impactFleshSound[ c ] );
    }
  }
  else
  {
    //normal sound
    for( c = 0; c < 4; c++ )
    {
      if( !weapon->wim[ weaponMode ].impactSound[ c ] )
        break;
    }

    if( c > 0 )
    {
      c = rand( ) % c;
      if( weapon->wim[ weaponMode ].impactSound[ c ] )
        trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, weapon->wim[ weaponMode ].impactSound[ c ] );
    }
  }

  //create impact particle system
  if( ps )
  {
    particleSystem_t *partSystem = CG_SpawnNewParticleSystem( ps );

    if( CG_IsParticleSystemValid( &partSystem ) )
    {
      CG_SetAttachmentPoint( &partSystem->attachment, origin );
      CG_SetParticleSystemNormal( partSystem, dir );
      CG_AttachToPoint( &partSystem->attachment );
      partSystem->charge = charge;
    }
  }

  //
  // impact mark
  //
  if( radius > 0.0f )
    CG_ImpactMark( mark, origin, dir, random( ) * 360, 1, 1, 1, 1, qfalse, radius, qfalse );
}


/*
=================
CG_MissileHitEntity
=================
*/
void CG_MissileHitEntity( weapon_t weaponNum, weaponMode_t weaponMode,
    vec3_t origin, vec3_t dir, int entityNum, int charge )
{
  vec3_t        normal;
  weaponInfo_t  *weapon = &cg_weapons[ weaponNum ];

  VectorCopy( dir, normal );
  VectorInverse( normal );

  CG_Bleed( origin, normal, entityNum );

  if( weaponMode <= WPM_NONE || weaponMode >= WPM_NUM_WEAPONMODES )
    weaponMode = WPM_PRIMARY;

  if( weapon->wim[ weaponMode ].alwaysImpact )
  {
    int sound;

    if( cg_entities[ entityNum ].currentState.eType == ET_PLAYER )
    {
      // Players
      sound = IMPACTSOUND_FLESH;
    }
    else if( cg_entities[ entityNum ].currentState.eType == ET_BUILDABLE &&
             BG_Buildable( cg_entities[ entityNum ].currentState.modelindex )->team == TEAM_ALIENS )
    {
      // Alien buildables
      sound = IMPACTSOUND_FLESH;
    }
    else
      sound = IMPACTSOUND_DEFAULT;

    CG_MissileHitWall( weaponNum, weaponMode, 0, origin, dir, sound, charge );
  }
}


/*
============================================================================

BULLETS

============================================================================
*/


/*
===============
CG_Tracer
===============
*/
void CG_Tracer( vec3_t source, vec3_t dest )
{
  vec3_t      forward, right;
  polyVert_t  verts[ 4 ];
  vec3_t      line;
  float       len, begin, end;
  vec3_t      start, finish;
  vec3_t      midpoint;

  // tracer
  VectorSubtract( dest, source, forward );
  len = VectorNormalize( forward );

  // start at least a little ways from the muzzle
  if( len < 100 )
    return;

  begin = 50 + random( ) * ( len - 60 );
  end = begin + cg_tracerLength.value;
  if( end > len )
    end = len;

  VectorMA( source, begin, forward, start );
  VectorMA( source, end, forward, finish );

  line[ 0 ] = DotProduct( forward, cg.refdef.viewaxis[ 1 ] );
  line[ 1 ] = DotProduct( forward, cg.refdef.viewaxis[ 2 ] );

  VectorScale( cg.refdef.viewaxis[ 1 ], line[ 1 ], right );
  VectorMA( right, -line[ 0 ], cg.refdef.viewaxis[ 2 ], right );
  VectorNormalize( right );

  VectorMA( finish, cg_tracerWidth.value, right, verts[ 0 ].xyz );
  verts[ 0 ].st[ 0 ] = 0;
  verts[ 0 ].st[ 1 ] = 1;
  verts[ 0 ].modulate[ 0 ] = 255;
  verts[ 0 ].modulate[ 1 ] = 255;
  verts[ 0 ].modulate[ 2 ] = 255;
  verts[ 0 ].modulate[ 3 ] = 255;

  VectorMA( finish, -cg_tracerWidth.value, right, verts[ 1 ].xyz );
  verts[ 1 ].st[ 0 ] = 1;
  verts[ 1 ].st[ 1 ] = 0;
  verts[ 1 ].modulate[ 0 ] = 255;
  verts[ 1 ].modulate[ 1 ] = 255;
  verts[ 1 ].modulate[ 2 ] = 255;
  verts[ 1 ].modulate[ 3 ] = 255;

  VectorMA( start, -cg_tracerWidth.value, right, verts[ 2 ].xyz );
  verts[ 2 ].st[ 0 ] = 1;
  verts[ 2 ].st[ 1 ] = 1;
  verts[ 2 ].modulate[ 0 ] = 255;
  verts[ 2 ].modulate[ 1 ] = 255;
  verts[ 2 ].modulate[ 2 ] = 255;
  verts[ 2 ].modulate[ 3 ] = 255;

  VectorMA( start, cg_tracerWidth.value, right, verts[ 3 ].xyz );
  verts[ 3 ].st[ 0 ] = 0;
  verts[ 3 ].st[ 1 ] = 0;
  verts[ 3 ].modulate[ 0 ] = 255;
  verts[ 3 ].modulate[ 1 ] = 255;
  verts[ 3 ].modulate[ 2 ] = 255;
  verts[ 3 ].modulate[ 3 ] = 255;

  trap_R_AddPolyToScene( cgs.media.tracerShader, 4, verts );

  midpoint[ 0 ] = ( start[ 0 ] + finish[ 0 ] ) * 0.5;
  midpoint[ 1 ] = ( start[ 1 ] + finish[ 1 ] ) * 0.5;
  midpoint[ 2 ] = ( start[ 2 ] + finish[ 2 ] ) * 0.5;

  // add the tracer sound
  trap_S_StartSound( midpoint, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.tracerSound );
}


/*
======================
CG_CalcMuzzlePoint
======================
*/
static qboolean CG_CalcMuzzlePoint( int entityNum, vec3_t muzzle )
{
  vec3_t    forward;
  centity_t *cent;
  int       anim;

  if( entityNum == cg.snap->ps.clientNum )
  {
    VectorCopy( cg.snap->ps.origin, muzzle );
    muzzle[ 2 ] += cg.snap->ps.viewheight;
    AngleVectors( cg.snap->ps.viewangles, forward, NULL, NULL );
    VectorMA( muzzle, 14, forward, muzzle );
    return qtrue;
  }

  cent = &cg_entities[entityNum];

  if( !cent->currentValid )
    return qfalse;

  VectorCopy( cent->currentState.pos.trBase, muzzle );

  AngleVectors( cent->currentState.apos.trBase, forward, NULL, NULL );
  anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;

  if( anim == LEGS_WALKCR || anim == LEGS_IDLECR )
    muzzle[ 2 ] += CROUCH_VIEWHEIGHT;
  else
    muzzle[ 2 ] += DEFAULT_VIEWHEIGHT;

  VectorMA( muzzle, 14, forward, muzzle );

  return qtrue;

}


/*
======================
CG_Bullet

Renders bullet effects.
======================
*/
void CG_Bullet( vec3_t end, int sourceEntityNum, vec3_t normal, qboolean flesh, int fleshEntityNum )
{
  vec3_t  start;

  // if the shooter is currently valid, calc a source point and possibly
  // do trail effects
  if( sourceEntityNum >= 0 && cg_tracerChance.value > 0 )
  {
    if( CG_CalcMuzzlePoint( sourceEntityNum, start ) )
    {
      // draw a tracer
      if( random( ) < cg_tracerChance.value )
        CG_Tracer( start, end );
    }
  }

  // impact splash and mark
  if( flesh )
    CG_Bleed( end, normal, fleshEntityNum );
  else
    CG_MissileHitWall( WP_MACHINEGUN, WPM_PRIMARY, 0, end, normal, IMPACTSOUND_DEFAULT, 0 );
}

/*
============================================================================

SHOTGUN TRACING

============================================================================
*/

/*
================
CG_ShotgunPattern

Perform the same traces the server did to locate the
hit splashes
================
*/
static void CG_ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, int otherEntNum )
{
  int       i;
  float     r, u;
  vec3_t    end;
  vec3_t    forward, right, up;
  trace_t   tr;

  // derive the right and up vectors from the forward vector, because
  // the client won't have any other information
  VectorNormalize2( origin2, forward );
  PerpendicularVector( right, forward );
  CrossProduct( forward, right, up );

  // generate the "random" spread pattern
  for( i = 0; i < SHOTGUN_PELLETS; i++ )
  {
    r = Q_crandom( &seed ) * SHOTGUN_SPREAD * 16;
    u = Q_crandom( &seed ) * SHOTGUN_SPREAD * 16;
    VectorMA( origin, 8192 * 16, forward, end );
    VectorMA( end, r, right, end );
    VectorMA( end, u, up, end );

    CG_Trace( &tr, origin, NULL, NULL, end, otherEntNum, MASK_SHOT );

    if( !( tr.surfaceFlags & SURF_NOIMPACT ) )
    {
      if( cg_entities[ tr.entityNum ].currentState.eType == ET_PLAYER ||
          cg_entities[ tr.entityNum ].currentState.eType == ET_BUILDABLE )
        CG_MissileHitEntity( WP_SHOTGUN, WPM_PRIMARY, tr.endpos, tr.plane.normal, tr.entityNum, 0 );
      else if( tr.surfaceFlags & SURF_METALSTEPS )
        CG_MissileHitWall( WP_SHOTGUN, WPM_PRIMARY, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_METAL, 0 );
      else
        CG_MissileHitWall( WP_SHOTGUN, WPM_PRIMARY, 0, tr.endpos, tr.plane.normal, IMPACTSOUND_DEFAULT, 0 );
    }
  }
}

/*
==============
CG_ShotgunFire
==============
*/
void CG_ShotgunFire( entityState_t *es )
{
  vec3_t  v;

  VectorSubtract( es->origin2, es->pos.trBase, v );
  VectorNormalize( v );
  VectorScale( v, 32, v );
  VectorAdd( es->pos.trBase, v, v );

  CG_ShotgunPattern( es->pos.trBase, es->origin2, es->eventParm, es->otherEntityNum );
}

/*
=================
CG_Bleed

This is the spurt of blood when a character gets hit
=================
*/
void CG_Bleed( vec3_t origin, vec3_t normal, int entityNum )
{
  team_t            team;
  qhandle_t         bleedPS;
  particleSystem_t  *ps;

  if( !cg_blood.integer )
    return;

  if( cg_entities[ entityNum ].currentState.eType == ET_PLAYER )
  {
    team = cgs.clientinfo[ entityNum ].team;
    if( team == TEAM_ALIENS )
      bleedPS = cgs.media.alienBleedPS;
    else if( team == TEAM_HUMANS )
      bleedPS = cgs.media.humanBleedPS;
    else
      return;
  }
  else if( cg_entities[ entityNum ].currentState.eType == ET_BUILDABLE )
  {
    //ew
    team = BG_Buildable( cg_entities[ entityNum ].currentState.modelindex )->team;
    if( team == TEAM_ALIENS )
      bleedPS = cgs.media.alienBuildableBleedPS;
    else if( team == TEAM_HUMANS )
      bleedPS = cgs.media.humanBuildableBleedPS;
    else
      return;
  }
  else
    return;

  ps = CG_SpawnNewParticleSystem( bleedPS );

  if( CG_IsParticleSystemValid( &ps ) )
  {
    CG_SetAttachmentPoint( &ps->attachment, origin );
    CG_SetAttachmentCent( &ps->attachment, &cg_entities[ entityNum ] );
    CG_AttachToPoint( &ps->attachment );

    CG_SetParticleSystemNormal( ps, normal );
  }
}
