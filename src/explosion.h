#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <map>
#include <string>

using itype_id = std::string;

struct tripoint;
class JsonObject;
class nc_color;

struct shrapnel_data {
    int casing_mass = 0;
    float fragment_mass = 0.005;
    // Percentage
    int recovery        = 0;
    itype_id drop       = "null";
};

struct explosion_data {
    float power             = -1.0f;
    float distance_factor   = 0.8f;
    bool fire               = false;
    shrapnel_data shrapnel;

    /** Returns the distance at which we have `ratio` of initial power. */
    float expected_range( float ratio ) const;
    /** Returns the expected power at a given distance from epicenter. */
    float power_at_range( float dist ) const;
    /** Returns the distance at which the power drops below 1. */
    int safe_range() const;
};

// handles explosion related functions
namespace explosion_handler
{
/** Create explosion at p of intensity (power) with (shrapnel) chunks of shrapnel.
    Explosion intensity formula is roughly power*factor^distance.
    If factor <= 0, no blast is produced */
void explosion(
    const tripoint &p, float power, float factor = 0.8f,
    bool fire = false, int casing_mass = 0, float fragment_mass = 0.05
);

void explosion( const tripoint &p, const explosion_data &ex );

void identifyExplosionNoise(const tripoint &p, const explosion_data &ex );
void calculateExplosionDistance(const tripoint &p, const explosion_data &ex );

void triggersFlashbangExplosion( const tripoint &p, bool player_immune = false );
void triggersResonance_cascade( const tripoint &p );
void triggersScrambler_blast( const tripoint &p );
void triggersEmp_blast( const tripoint &p );

/** Nuke the area at p - global overmap terrain coordinates! */
void nuke( const tripoint &p );
// shockwave applies knockback to all targets within radius of p
// parameters force, stun, and dam_mult are passed to knockback()
// ignore_player determines if player is affected, useful for bionic, etc.
void shockwave( const tripoint &p, int radius, int force, int stun, int dam_mult,
                bool ignore_player );

void draw_explosion( const tripoint &p, int radius, const nc_color &col );
void draw_custom_explosion( const tripoint &p, const std::map<tripoint, nc_color> &area );
} // namespace explosion_handler

shrapnel_data load_shrapnel_data( JsonObject &jo );
explosion_data load_explosion_data( JsonObject &jo );

#endif
