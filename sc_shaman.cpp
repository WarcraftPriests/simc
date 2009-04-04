// ==========================================================================
// Dedmonwakeen's DPS-DPM Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================

#include "simcraft.h"

// ==========================================================================
// Shaman
// ==========================================================================

struct shaman_t : public player_t
{
  // Totems
  spell_t* fire_totem;
  spell_t* air_totem;
  spell_t* water_totem;
  spell_t* earth_totem;

  // Active
  action_t* active_flame_shock;
  action_t* active_lightning_charge;
  action_t* active_lightning_bolt_dot;

  // Buffs
  struct _buffs_t
  {
    int    elemental_devastation;
    int    elemental_focus;
    int    elemental_mastery;
    int    flurry;
    int    lightning_charges;
    int    lightning_shield;
    int    maelstrom_weapon;
    int    nature_vulnerability;
    int    nature_vulnerability_charges;
    int    natures_swiftness;
    int    shamanistic_rage;
    double totem_of_wrath_glyph;
    double water_shield;

    void reset() { memset( (void*) this, 0x00, sizeof( _buffs_t ) ); }
    _buffs_t() { reset(); }
  };
  _buffs_t _buffs;

  // Cooldowns
  struct _cooldowns_t
  {
    double windfury_weapon;
    double lava_burst;

    void reset() { memset( (void*) this, 0x00, sizeof( _cooldowns_t ) ); }
    _cooldowns_t() { reset(); }
  };
  _cooldowns_t _cooldowns;

  // Expirations
  struct _expirations_t
  {
    event_t* elemental_devastation;
    event_t* elemental_oath;
    event_t* maelstrom_weapon;
    event_t* nature_vulnerability;
    event_t* unleashed_rage;

    void reset() { memset( (void*) this, 0x00, sizeof( _expirations_t ) ); }
    _expirations_t() { reset(); }
  };
  _expirations_t _expirations;

  // Gains
  gain_t* gains_improved_stormstrike;
  gain_t* gains_shamanistic_rage;
  gain_t* gains_thunderstorm;
  gain_t* gains_water_shield;

  // Procs
  proc_t* procs_lightning_overload;
  
  // Up-Times
  uptime_t* uptimes_elemental_devastation;
  uptime_t* uptimes_elemental_focus;
  uptime_t* uptimes_elemental_oath;
  uptime_t* uptimes_flurry;
  uptime_t* uptimes_nature_vulnerability;
  uptime_t* uptimes_unleashed_rage;
  
  // Auto-Attack
  attack_t* main_hand_attack;
  attack_t*  off_hand_attack;

  // Weapon Enchants
  attack_t* windfury_weapon_attack;
  spell_t*  flametongue_weapon_spell;

  struct talents_t
  {
    int  ancestral_knowledge;
    int  blessing_of_the_eternals;
    int  booming_echoes;
    int  call_of_flame;
    int  call_of_thunder;
    int  concussion;
    int  convection;
    int  dual_wield;
    int  dual_wield_specialization;
    int  elemental_devastation;
    int  elemental_focus;
    int  elemental_fury;
    int  elemental_mastery;
    int  elemental_oath;
    int  elemental_precision;
    int  elemental_weapons;
    int  enhancing_totems;
    int  feral_spirit;
    int  flurry;
    int  frozen_power;
    int  improved_shields;
    int  improved_stormstrike;
    int  improved_windfury_totem;
    int  lava_flows;
    int  lava_lash;
    int  lightning_mastery;
    int  lightning_overload;
    int  maelstrom_weapon;
    int  mana_tide_totem;
    int  mental_dexterity;
    int  mental_quickness;
    int  natures_swiftness;
    int  restorative_totems;
    int  reverberation;
    int  shamanism;
    int  shamanistic_focus;
    int  shamanistic_rage;
    int  spirit_weapons;
    int  static_shock;
    int  stormstrike;
    int  storm_earth_and_fire;
    int  toughness;
    int  thundering_strikes;
    int  thunderstorm;
    int  tidal_mastery;
    int  totem_of_wrath;
    int  totemic_focus;
    int  unrelenting_storm;
    int  unleashed_rage;
    int  weapon_mastery;
    
    talents_t() { memset( (void*) this, 0x0, sizeof( talents_t ) ); }
  };
  talents_t talents;

  struct glyphs_t
  {
    int chain_lightning;
    int elemental_mastery;
    int feral_spirit;
    int flame_shock;
    int flametongue_weapon;
    int lava;
    int lava_lash;
    int lightning_bolt;
    int lightning_shield;
    int mana_tide;
    int shocking;
    int stormstrike;
    int totem_of_wrath;
    int thunderstorm;
    int windfury_weapon;
    glyphs_t() { memset( (void*) this, 0x0, sizeof( glyphs_t ) ); }
  };
  glyphs_t glyphs;

  struct totems_t
  {
    int dueling;
    int hex;
    int dancing_flame;
    int thunderfall;

    totems_t() { memset( (void*) this, 0x0, sizeof( totems_t ) ); }
  };
  totems_t totems;

  struct tiers_t
  {
    int t4_2pc_elemental;
    int t4_4pc_elemental;
    int t5_2pc_elemental;
    int t5_4pc_elemental;
    int t6_2pc_elemental;
    int t6_4pc_elemental;
    int t7_2pc_elemental;
    int t7_4pc_elemental;
    int t8_2pc_elemental;
    int t8_4pc_elemental;
    int t4_2pc_enhancement;
    int t4_4pc_enhancement;
    int t5_2pc_enhancement;
    int t5_4pc_enhancement;
    int t6_2pc_enhancement;
    int t6_4pc_enhancement;
    int t7_2pc_enhancement;
    int t7_4pc_enhancement;
    int t8_2pc_enhancement;
    int t8_4pc_enhancement;
    tiers_t() { memset( (void*) this, 0x0, sizeof( tiers_t ) ); }
  };
  tiers_t tiers;

  shaman_t( sim_t* sim, std::string& name ) : player_t( sim, SHAMAN, name )
  {
    // Totems
    fire_totem  = 0;
    air_totem   = 0;
    water_totem = 0;
    earth_totem = 0;;

    // Active
    active_flame_shock        = 0;
    active_lightning_charge   = 0;
    active_lightning_bolt_dot = 0;

    // Gains
    gains_improved_stormstrike = get_gain( "improved_stormstrike" );
    gains_shamanistic_rage     = get_gain( "shamanistic_rage" );
    gains_thunderstorm         = get_gain( "thunderstorm"     );
    gains_water_shield         = get_gain( "water_shield"     );

    // Procs
    procs_lightning_overload = get_proc( "lightning_overload" );

    // Up-Times
    uptimes_elemental_devastation = get_uptime( "elemental_devastation"    );
    uptimes_elemental_focus       = get_uptime( "elemental_focus"          );
    uptimes_elemental_oath        = get_uptime( "elemental_oath"           );
    uptimes_flurry                = get_uptime( "flurry"                   );
    uptimes_nature_vulnerability  = get_uptime( "nature_vulnerability"     );
    uptimes_unleashed_rage        = get_uptime( "unleashed_rage"           );
  
    // Auto-Attack
    main_hand_attack = 0;
    off_hand_attack  = 0;

    // Weapon Enchants
    windfury_weapon_attack   = 0;
    flametongue_weapon_spell = 0;
  }

  // Character Definition
  virtual void      init_rating();
  virtual void      init_base();
  virtual void      init_unique_gear();
  virtual void      reset();
  virtual double    composite_attack_power();
  virtual double    composite_spell_power( int school );
  virtual bool      get_talent_trees( std::vector<int*>& elemental, std::vector<int*>& enhancement, std::vector<int*>& restoration );
  virtual bool      parse_talents_mmo( const std::string& talent_string );
  virtual bool      parse_option( const std::string& name, const std::string& value );
  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual pet_t*    create_pet   ( const std::string& name );
  virtual int       primary_resource() { return RESOURCE_MANA; }

  // Event Tracking
  virtual void regen( double periodicity );
};

namespace { // ANONYMOUS NAMESPACE ==========================================

// ==========================================================================
// Shaman Attack
// ==========================================================================

struct shaman_attack_t : public attack_t
{
  shaman_attack_t( const char* n, player_t* player, int s=SCHOOL_PHYSICAL, int t=TREE_NONE, bool special=true ) : 
    attack_t( n, player, RESOURCE_MANA, s, t, special ) 
  {
    base_direct_dmg = 1;
    shaman_t* p = player -> cast_shaman();
    base_multiplier *= 1.0 + p -> talents.weapon_mastery * 0.1/3;
    base_crit += p -> talents.thundering_strikes * 0.01;
    if( p -> dual_wield() ) base_hit += p -> talents.dual_wield_specialization * 0.02;
  }

  virtual void execute();
  virtual void player_buff();
  virtual void assess_damage( double amount, int dmg_type );
};

// ==========================================================================
// Shaman Spell
// ==========================================================================

struct shaman_spell_t : public spell_t
{
  double base_cost_reduction;

  shaman_spell_t( const char* n, player_t* p, int s, int t ) : 
    spell_t( n, p, RESOURCE_MANA, s, t ), base_cost_reduction(0)
  {
    shaman_t* shaman = p -> cast_shaman();
    base_crit += shaman -> talents.thundering_strikes * 0.01;
    base_crit += shaman -> talents.blessing_of_the_eternals * 0.02;
  }

  virtual double cost();
  virtual double cost_reduction() { return base_cost_reduction; }
  virtual void   consume_resource();
  virtual double execute_time();
  virtual void   execute();
  virtual void   player_buff();
  virtual void   schedule_execute();
  virtual void   assess_damage( double amount, int dmg_type );
};

// ==========================================================================
// Pet Spirit Wolf
// ==========================================================================

struct spirit_wolf_pet_t : public pet_t
{
  struct melee_t : public attack_t
  {
    melee_t( player_t* player ) : 
      attack_t( "wolf_melee", player )
    {
      weapon = &( player -> main_hand_weapon );
      base_execute_time = weapon -> swing_time;
      base_direct_dmg = 1;
      background = true;
      repeating = true;

      // There are actually two wolves.....
      base_multiplier *= 2.0;
    }
    void player_buff()
    {
      attack_t::player_buff();
      shaman_t* o = player -> cast_pet() -> owner -> cast_shaman();
      if( o -> glyphs.feral_spirit )
      {
	player_power += 0.30 * o -> composite_attack_power();
      }
    }
  };

  melee_t* melee;

  spirit_wolf_pet_t( sim_t* sim, player_t* owner ) :
    pet_t( sim, owner, "spirit_wolf" ), melee(0)
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.damage     = 310;
    main_hand_weapon.swing_time = 1.6;
  }
  virtual void init_base()
  {
    attribute_base[ ATTR_STRENGTH  ] = 331;
    attribute_base[ ATTR_AGILITY   ] = 113;
    attribute_base[ ATTR_STAMINA   ] = 361;
    attribute_base[ ATTR_INTELLECT ] = 65;
    attribute_base[ ATTR_SPIRIT    ] = 109;

    base_attack_power = -20;
    initial_attack_power_per_strength = 2.0;
    initial_attack_crit_per_agility = rating_t::interpolate( level, 0.01/25.0, 0.01/40.0, 0.01/83.3 );

    melee = new melee_t( this );
  }
  virtual void summon()
  {
    pet_t::summon();
    melee -> execute(); // Kick-off repeating attack
  }
};

// trigger_flametongue_weapon ===============================================

static void trigger_flametongue_weapon( attack_t* a )
{
  struct flametongue_weapon_spell_t : public shaman_spell_t
  {
    flametongue_weapon_spell_t( player_t* player ) :
      shaman_spell_t( "flametongue", player, SCHOOL_FIRE, TREE_ENHANCEMENT )
    {
      shaman_t* p = player -> cast_shaman();

      background       = true;
      proc             = true;
      may_crit         = true;
      trigger_gcd      = 0;
      direct_power_mod = 0.10; // Fix-Me. In 3.1 this scales with weapon speed.
      base_hit        += p -> talents.elemental_precision * 0.01;

      reset();
    }
  };

  if( a -> weapon &&
      a -> weapon -> buff == FLAMETONGUE )
  {
    shaman_t* p = a -> player -> cast_shaman();

    if( ! p -> flametongue_weapon_spell )
    {
      p -> flametongue_weapon_spell = new flametongue_weapon_spell_t( p );
    }

    double fire_dmg = util_t::ability_rank( p -> level,  70.0,80,  70.0,77,  60.0,72,  35.0,65,  30.0,0 );

    p -> flametongue_weapon_spell -> base_direct_dmg = fire_dmg * a -> weapon -> swing_time;
    p -> flametongue_weapon_spell -> execute();
  }
}

// trigger_windfury_weapon ================================================

static void trigger_windfury_weapon( attack_t* a )
{
  struct windfury_weapon_attack_t : public shaman_attack_t
  {
    windfury_weapon_attack_t( player_t* player ) :
      shaman_attack_t( "windfury", player )
    {
      shaman_t* p = player -> cast_shaman();
      background  = true;
      trigger_gcd = 0;
      base_multiplier *= 1.0 + p -> talents.elemental_weapons * 0.133333;
      reset();
    }
    virtual void player_buff()
    {
      shaman_attack_t::player_buff();
      player_power += weapon -> buff_bonus;
    }
  };

  shaman_t* p = a -> player -> cast_shaman();

  if( a -> weapon == 0 ) return;
  if( a -> weapon -> buff != WINDFURY ) return;

  if( ! a -> sim -> cooldown_ready( p -> _cooldowns.windfury_weapon ) ) return;

  if( a -> sim -> roll( 0.20 + ( p -> glyphs.windfury_weapon ? 0.02 : 0 ) ) )
  {
    if( ! p -> windfury_weapon_attack )
    {
      p -> windfury_weapon_attack = new windfury_weapon_attack_t( p );
    }
    p -> _cooldowns.windfury_weapon = a -> sim -> current_time + 3.0;

    p -> procs.windfury -> occur();
    p -> windfury_weapon_attack -> weapon = a -> weapon;
    p -> windfury_weapon_attack -> execute();
    p -> windfury_weapon_attack -> execute();
  }
}

// stack_maelstrom_weapon ==================================================

static void stack_maelstrom_weapon( attack_t* a )
{
  shaman_t* p = a -> player -> cast_shaman();

  if( p -> talents.maelstrom_weapon == 0 ) return;

  struct maelstrom_weapon_expiration_t : public event_t
  {
    maelstrom_weapon_expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
    {
      name = "Maelstrom Weapon Expiration";
      player -> aura_gain( "Maelstrom Weapon" );
      sim -> add_event( this, 30.0 );
    }
    virtual void execute()
    {
      shaman_t* p = player -> cast_shaman();
      p -> aura_loss( "Maelstrom Weapon" );
      p -> _buffs.maelstrom_weapon = 0;
      p -> _expirations.maelstrom_weapon = 0;
    }
  };
  double chance = a -> weapon -> proc_chance_on_swing( p -> talents.maelstrom_weapon * 2.0 );
  if( p -> tiers.t8_4pc_enhancement )
    chance *= 1.0 + 0.20;
    
  if( a -> sim -> roll( chance ) )
  {
    if( p -> _buffs.maelstrom_weapon < 5 ) 
    {
      p -> _buffs.maelstrom_weapon++;
      if( a -> sim -> log ) report_t::log( a -> sim, "%s gains Maelstrom Weapon %d", p -> name(), p -> _buffs.maelstrom_weapon );
    }

    event_t*& e = p -> _expirations.maelstrom_weapon;
    
    if( e )
    {
      e -> reschedule( 30.0 );
    }
    else
    {
      e = new ( a -> sim ) maelstrom_weapon_expiration_t( a -> sim, p );
    }
  }
}

// trigger_unleashed_rage =================================================

static void trigger_unleashed_rage( attack_t* a )
{
  struct unleashed_rage_expiration_t : public event_t
  {
    unleashed_rage_expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
    {
      name = "Unleashed Rage Expiration";
      for( player_t* p = sim -> player_list; p; p = p -> next )
      {
        if ( p -> sleeping ) continue;
        if( p -> buffs.unleashed_rage == 0 ) p -> aura_gain( "Unleashed Rage" );
        p -> buffs.unleashed_rage++;
      }
      sim -> add_event( this, 10.0 );
    }
    virtual void execute()
    {
      for( player_t* p = sim -> player_list; p; p = p -> next )
      {
        if ( p -> buffs.unleashed_rage > 0 )
        {
          p -> buffs.unleashed_rage--;
          if( p -> buffs.unleashed_rage == 0 ) p -> aura_loss( "Unleashed Rage" );
        }
      }
      player -> cast_shaman() -> _expirations.unleashed_rage = 0;
    }
  };

  if( a -> proc ) return;

  shaman_t* p = a -> player -> cast_shaman();
  
  if( p -> talents.unleashed_rage == 0 ) return;

  event_t*& e = p -> _expirations.unleashed_rage;

  if( e )
  {
    e -> reschedule( 10.0 );
  }
  else
  {
    e = new ( a -> sim ) unleashed_rage_expiration_t( a -> sim, p );
  }
}

// trigger_improved_stormstrike =============================================

static void trigger_improved_stormstrike( attack_t* a )
{
  shaman_t* p = a -> player -> cast_shaman();

  if( a -> sim -> P309 )
    return;

  if( ! a -> sim -> roll( p -> talents.improved_stormstrike / 2.0 ) )
    return;

  p -> resource_gain( RESOURCE_MANA, 0.20 * p -> resource_base[ RESOURCE_MANA ], p -> gains_improved_stormstrike );
}

// trigger_nature_vulnerability =============================================

static void trigger_nature_vulnerability( attack_t* a )
{
  struct nature_vulnerability_expiration_t : public event_t
  {
    nature_vulnerability_expiration_t( sim_t* sim, shaman_t* p, double duration ) : event_t( sim, p )
    {
      name = "Nature Vulnerability Expiration";
      p -> aura_gain( "Nature Vulnerability" );
      p -> _buffs.nature_vulnerability = p -> glyphs.stormstrike ? 28 : 20;
      sim -> add_event( this, duration );
    }
    virtual void execute()
    {
      shaman_t* p = player -> cast_shaman();
      p -> aura_loss( "Nature Vulnerability" );
      p -> _buffs.nature_vulnerability = 0;
      p -> _buffs.nature_vulnerability_charges = 0;
      p -> _expirations.nature_vulnerability = 0;
    }
  };

  shaman_t* p = a -> player -> cast_shaman();
  event_t*& e = p -> _expirations.nature_vulnerability;

  double duration = 12.0;

  if( a -> sim -> P309 )
  {
    p -> _buffs.nature_vulnerability_charges = 2 + p -> talents.improved_stormstrike;
  }
  else
  {
    p -> _buffs.nature_vulnerability_charges = 4;
  }
   
  if( e )
  {
    e -> reschedule( duration );
  }
  else
  {
    e = new ( a -> sim ) nature_vulnerability_expiration_t( a -> sim, p, duration );
  }
}

// trigger_tier5_4pc_elemental ===============================================

static void trigger_tier5_4pc_elemental( spell_t* s )
{
  if( s -> result != RESULT_CRIT ) return;

  shaman_t* p = s -> player -> cast_shaman();

  if( p -> tiers.t5_4pc_elemental && s -> sim -> roll( 0.25 ) )
  {
    p -> resource_gain( RESOURCE_MANA, 120.0, p -> gains.tier5_4pc );
  }
}

// trigger_tier8_4pc_elemental ===============================================

static void trigger_tier8_4pc_elemental( spell_t* s )
{
  shaman_t* p = s -> player -> cast_shaman();

  struct lightning_bolt_dot_t : public shaman_spell_t
  {
    lightning_bolt_dot_t( player_t* player ) : shaman_spell_t( "tier8_4pc_elemental", player, SCHOOL_NATURE, TREE_ELEMENTAL )
    {
      may_miss        = false;
      background      = true;
      proc            = true;
      trigger_gcd     = 0;
      base_cost       = 0;
      base_multiplier = 1.0;
      tick_power_mod  = 0;
      // FIX ME! I shamelessly copied from the hunters piercing shots and currently there is no information about those values.
      base_tick_time  = 1.0;
      num_ticks       = 8;
    }
    void player_buff() {}
    void target_debuff() {}
  };

  double dmg = s -> direct_dmg * 0.08;

  if( ! p -> active_lightning_bolt_dot ) p -> active_lightning_bolt_dot = new lightning_bolt_dot_t( p );

  if( p -> active_lightning_bolt_dot -> ticking )
  {
    int num_ticks = p -> active_lightning_bolt_dot -> num_ticks;
    int remaining_ticks = num_ticks - p -> active_lightning_bolt_dot -> current_tick;

    dmg += p -> active_lightning_bolt_dot -> base_tick_dmg * remaining_ticks;

    p -> active_lightning_bolt_dot -> cancel();
  }

  p -> active_lightning_bolt_dot -> base_tick_dmg = dmg / 8;
  p -> active_lightning_bolt_dot -> schedule_tick();
}

// trigger_ashtongue_talisman ===============================================

static void trigger_ashtongue_talisman( spell_t* s )
{
  shaman_t* p = s -> player -> cast_shaman();

  if( p -> gear.ashtongue_talisman && s -> sim -> roll( 0.15 ) )
  {
    p -> resource_gain( RESOURCE_MANA, 110.0, p -> gains.ashtongue_talisman );
  }
}

// trigger_lightning_overload ===============================================

static void trigger_lightning_overload( spell_t* s,
                                        stats_t* lightning_overload_stats,
					double   lightning_overload_chance )
{
  shaman_t* p = s -> player -> cast_shaman();

  if( lightning_overload_chance == 0 ) return;

  if( ! s -> proc && s -> sim -> roll( lightning_overload_chance ) )
  {
    p -> procs_lightning_overload -> occur();

    double   cost        = s -> base_cost;
    double   multiplier  = s -> base_multiplier;
    stats_t* stats       = s -> stats;

    s -> proc             = true;
    s -> base_cost        = 0;
    s -> base_multiplier /= 2.0;
    s -> stats            = lightning_overload_stats;

    s -> time_to_execute = 0;
    s -> execute();

    s -> proc            = false;
    s -> base_cost       = cost;
    s -> base_multiplier = multiplier;
    s -> stats           = stats;
  }
}

// trigger_elemental_devastation =================================================

static void trigger_elemental_devastation( spell_t* s )
{
  shaman_t* p = s -> player -> cast_shaman();

  if( p -> talents.elemental_devastation == 0 ) return;

  if( s -> proc ) return;

  struct elemental_devastation_expiration_t : public event_t
  {
    elemental_devastation_expiration_t( sim_t* sim, shaman_t* p ) : event_t( sim, p )
    {
      name = "Elemental Devastation Expiration";
      p -> _buffs.elemental_devastation = 1;
      sim -> add_event( this, 10.0 );
    }
    virtual void execute()
    {
      shaman_t* p = player -> cast_shaman();
      p -> _buffs.elemental_devastation = 0;
      p -> _expirations.elemental_devastation = 0;
    }
  };

  event_t*& e = p -> _expirations.elemental_devastation;
    
  if( e )
  {
    e -> reschedule( 10.0 );
  }
  else
  {
    e = new ( s -> sim ) elemental_devastation_expiration_t( s -> sim, p );
  }
}

// trigger_elemental_oath =====================================================

static void trigger_elemental_oath( spell_t* s )
{
  struct elemental_oath_expiration_t : public event_t
  {
    elemental_oath_expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
    {
      name = "Elemental Oath Expiration";
      for( player_t* p = sim -> player_list; p; p = p -> next )
      {
        if ( p -> sleeping ) continue;
        if( p -> buffs.elemental_oath == 0 ) p -> aura_gain( "Elemental Oath" );
        p -> buffs.elemental_oath++;
      }
      sim -> add_event( this, 15.0 );
    }
    virtual void execute()
    {
      for( player_t* p = sim -> player_list; p; p = p -> next )
      {
        if( p -> buffs.elemental_oath > 0 )
        {
          p -> buffs.elemental_oath--;
          if( p -> buffs.elemental_oath == 0 ) p -> aura_loss( "Elemental Oath" );
        }
      }
      player -> cast_shaman() -> _expirations.elemental_oath = 0;
    }
  };

  if( s -> proc ) return;

  shaman_t* p = s -> player -> cast_shaman();
  
  if( p -> talents.elemental_oath == 0 ) return;

  event_t*& e = p -> _expirations.elemental_oath;
    
  if( e )
  {
    e -> reschedule( 15.0 );
  }
  else
  {
    e = new ( s -> sim ) elemental_oath_expiration_t( s -> sim, p );
  }
}

// trigger_totem_of_dueling ================================================

static void trigger_totem_of_dueling( attack_t* a )
{
  shaman_t* p = a -> player -> cast_shaman();

  if( p -> totems.dueling )
  {
    struct totem_of_dueling_expiration_t : public event_t
    {
      totem_of_dueling_expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
      {
	name = "Totem of Dueling Expiration";
	player -> aura_gain( "Totem of Dueling" );
	player -> haste_rating += 60;
	player -> recalculate_haste();
	sim -> add_event( this, 6.0 );
      }
      virtual void execute()
      {
	player -> aura_loss( "Totem of Dueling" );
	player -> haste_rating -= 60;
	player -> recalculate_haste();
      }
    };

    assert( a -> cooldown > 6.0 );  // Make sure we don't have to perform refreshes
  
    new ( a -> sim ) totem_of_dueling_expiration_t( a -> sim, p );
  }
}

// =========================================================================
// Shaman Attack
// =========================================================================

// shaman_attack_t::execute ================================================

void shaman_attack_t::execute()
{
  shaman_t* p = player -> cast_shaman();

  attack_t::execute();

  if( result_is_hit() )
  {
    if( result == RESULT_CRIT )
    {
      trigger_unleashed_rage( this );
      if( p -> talents.flurry ) p -> _buffs.flurry = 3;
    }
    if( p -> _buffs.shamanistic_rage ) 
    {
      p -> resource_gain( RESOURCE_MANA, player_power * 0.30, p -> gains_shamanistic_rage );
    }
    trigger_flametongue_weapon( this );
    trigger_windfury_weapon( this );
    stack_maelstrom_weapon( this );
  }
}

// shaman_attack_t::player_buff ============================================

void shaman_attack_t::player_buff()
{
  attack_t::player_buff();
  shaman_t* p = player -> cast_shaman();
  if( p -> _buffs.elemental_devastation ) 
  {
    player_crit += p -> talents.elemental_devastation * 0.03;
  }
  p -> uptimes_elemental_devastation -> update( p -> _buffs.elemental_devastation != 0 );
  p -> uptimes_unleashed_rage        -> update( p -> buffs.unleashed_rage        != 0 );
}

// shaman_attack_t::assess_damage ==========================================

void shaman_attack_t::assess_damage( double amount, 
                                     int    dmg_type )
{
  shaman_t* p = player -> cast_shaman();

  attack_t::assess_damage( amount, dmg_type );

  if( num_ticks == 0 && p -> _buffs.lightning_charges > 0 )
  {
    if( sim -> roll( p -> talents.static_shock * 0.02 ) )
    {
      p -> _buffs.lightning_charges--;
      p -> active_lightning_charge -> execute();

      if( p -> _buffs.lightning_charges == 0 )
      {
	p -> active_lightning_charge -> cancel();
      }
    }
  }
}

// =========================================================================
// Shaman Attacks
// =========================================================================

// Melee Attack ============================================================

struct melee_t : public shaman_attack_t
{
  melee_t( const char* name, player_t* player ) : 
    shaman_attack_t( name, player, SCHOOL_PHYSICAL, TREE_NONE, false )
  {
    shaman_t* p = player -> cast_shaman();

    background  = true;
    repeating   = true;
    trigger_gcd = 0;
    base_cost   = 0;

    if( p -> dual_wield() ) base_hit -= 0.19;
  }

  virtual double execute_time()
  {
    double t = shaman_attack_t::execute_time();
    shaman_t* p = player -> cast_shaman();
    if( p -> _buffs.flurry > 0 ) 
    {
      t *= 1.0 / ( 1.0 + 0.05 * ( ( sim -> P309 ? 1 : 0 ) + p -> talents.flurry + p -> tiers.t7_4pc_enhancement ) );
    }
    p -> uptimes_flurry -> update( p -> _buffs.flurry > 0 );
    return t;
  }

  void schedule_execute()
  {
    shaman_attack_t::schedule_execute();
    shaman_t* p = player -> cast_shaman();
    if( p -> _buffs.flurry > 0 ) p -> _buffs.flurry--;
  }
};

// Auto Attack =============================================================

struct auto_attack_t : public shaman_attack_t
{
  auto_attack_t( player_t* player, const std::string& options_str ) : 
    shaman_attack_t( "auto_attack", player )
  {
    shaman_t* p = player -> cast_shaman();

    assert( p -> main_hand_weapon.type != WEAPON_NONE );
    p -> main_hand_attack = new melee_t( "melee_main_hand", player );
    p -> main_hand_attack -> weapon = &( p -> main_hand_weapon );
    p -> main_hand_attack -> base_execute_time = p -> main_hand_weapon.swing_time;

    if( p -> off_hand_weapon.type != WEAPON_NONE )
    {
      assert( p -> talents.dual_wield );
      p -> off_hand_attack = new melee_t( "melee_off_hand", player );
      p -> off_hand_attack -> weapon = &( p -> off_hand_weapon );
      p -> off_hand_attack -> base_execute_time = p -> off_hand_weapon.swing_time;
    }

    trigger_gcd = 0;
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();
    p -> main_hand_attack -> schedule_execute();
    if( p -> off_hand_attack ) p -> off_hand_attack -> schedule_execute();
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();
    return( p -> main_hand_attack -> execute_event == 0 ); // not swinging
  }
};

// Lava Lash Attack =========================================================

struct lava_lash_t : public shaman_attack_t
{
  lava_lash_t( player_t* player, const std::string& options_str ) : 
    shaman_attack_t( "lava_lash", player, SCHOOL_FIRE, TREE_ENHANCEMENT )
  {
    shaman_t* p = player -> cast_shaman();

    weapon          = &( player -> off_hand_weapon );
    base_direct_dmg = 1;
    cooldown        = 6;
    base_cost       = p -> resource_base[ RESOURCE_MANA ] * 0.04;
    if( p -> tiers.t8_2pc_enhancement )
      base_multiplier *= 1.0 + 0.20;
  }

  virtual void player_buff()
  {
    shaman_attack_t::player_buff();
    if( weapon -> buff == FLAMETONGUE ) 
    {
      shaman_t* p = player -> cast_shaman();
      player_multiplier *= 1.25 + p -> glyphs.lava_lash * 0.10;
    }
  }
};

// Stormstrike Attack =======================================================

struct stormstrike_t : public shaman_attack_t
{
  stormstrike_t( player_t* player, const std::string& options_str ) : 
    shaman_attack_t( "stormstrike", player, SCHOOL_PHYSICAL, TREE_ENHANCEMENT )
  {
    shaman_t* p = player -> cast_shaman();

    base_cost = p -> resource_base[ RESOURCE_MANA ] * 0.08;
    cooldown  = ( sim -> P309 ? 10.0 : 8.0 );
    if( p -> tiers.t8_2pc_enhancement )
      base_multiplier *= 1.0 + 0.20;

    // Shaman T8 Enhancement Relic -- Increases weapon damage when you use Stormstrike by 155.
    if( p -> totems.dancing_flame )
    {
      base_dd_max += 155;
      base_dd_min += 155;
    }
    if( sim -> P309 ) cooldown -= p -> talents.improved_stormstrike;
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();

    attack_t::consume_resource();
    update_ready();

    weapon = &( p -> main_hand_weapon );
    shaman_attack_t::execute();

    if( result_is_hit() )
    {
      trigger_nature_vulnerability( this );

      if( p -> off_hand_weapon.type != WEAPON_NONE )
      {
        weapon = &( p -> off_hand_weapon );
        shaman_attack_t::execute();
      }
    }

    trigger_improved_stormstrike( this );
    trigger_totem_of_dueling( this );
    
    p -> action_finish( this );
  }
  virtual void consume_resource() { }
};

// =========================================================================
// Shaman Spell
// =========================================================================

// shaman_spell_t::cost ====================================================

double shaman_spell_t::cost()
{
  shaman_t* p = player -> cast_shaman();
  double c = spell_t::cost();
  if( c == 0 ) return 0;
  double cr = cost_reduction();
  if( p -> _buffs.elemental_focus   ) cr += 0.40;
  if ( sim -> P309 )
  {
    if( p -> _buffs.elemental_mastery ) cr += 0.20;
  }
  c *= 1.0 - cr;
  if( p -> buffs.tier4_4pc ) c -= 270;
  if( c < 0 ) c = 0;
  return c;
}

// shaman_spell_t::consume_resource ========================================

void shaman_spell_t::consume_resource()
{
  spell_t::consume_resource();
  shaman_t* p = player -> cast_shaman();
  if( p -> _buffs.elemental_focus > 0 ) 
  {
    p -> _buffs.elemental_focus--;
  }
  p -> buffs.tier4_4pc = 0;
}

// shaman_spell_t::execute_time ============================================

double shaman_spell_t::execute_time()
{
  shaman_t* p = player -> cast_shaman();
  if( p -> _buffs.natures_swiftness ) return 0;
  return spell_t::execute_time();
}

// shaman_spell_t::player_buff =============================================

void shaman_spell_t::player_buff()
{
  shaman_t* p = player -> cast_shaman();
  spell_t::player_buff();
  if( p -> _buffs.elemental_mastery )
  {
    player_crit += ( sim -> P309 ? 0.20 : 0.15 );
  }
  if( p -> _buffs.nature_vulnerability && school == SCHOOL_NATURE )
  {
    player_multiplier *= 1.0 + p -> _buffs.nature_vulnerability * 0.01;
  }
  p -> uptimes_nature_vulnerability -> update( p -> _buffs.nature_vulnerability != 0 );

  if( p -> glyphs.flametongue_weapon )
  {
    if( p -> main_hand_weapon.buff == FLAMETONGUE ||
        p ->  off_hand_weapon.buff == FLAMETONGUE ) 
    {
      player_crit += 0.02;
    }
  }

  if( p -> talents.elemental_oath ) 
  {
    if( p -> buffs.elemental_oath &&
        p -> _buffs.elemental_focus )
    {
      player_multiplier *= 1.0 + p -> talents.elemental_oath * 0.05;
    }

    p -> uptimes_elemental_focus -> update( p -> _buffs.elemental_focus != 0 );
    p -> uptimes_elemental_oath  -> update( p -> buffs.elemental_oath  != 0 );
  }
}

// shaman_spell_t::execute =================================================

void shaman_spell_t::execute()
{
  shaman_t* p = player -> cast_shaman();
  spell_t::execute();
  if( result_is_hit() )
  {
    if( result == RESULT_CRIT )
    {
      trigger_elemental_devastation( this );
      trigger_elemental_oath( this );

      p -> _buffs.elemental_focus = 2;

      if( p -> tiers.t4_4pc_elemental )
      {
        p -> buffs.tier4_4pc = sim -> roll( 0.11 ) ;
      }
    }
  }
}

// shaman_spell_t::schedule_execute ========================================

void shaman_spell_t::schedule_execute()
{
  spell_t::schedule_execute();

  if( time_to_execute > 0 )
  {
    shaman_t* p = player -> cast_shaman();
    if( p -> main_hand_attack ) p -> main_hand_attack -> cancel();
    if( p ->  off_hand_attack ) p ->  off_hand_attack -> cancel();
  }
}

// shaman_spell_t::assess_damage ============================================

void shaman_spell_t::assess_damage( double amount, 
                                    int    dmg_type )
{
  shaman_t* p = player -> cast_shaman();

  spell_t::assess_damage( amount, dmg_type );

  if( dmg_type == DMG_DIRECT    &&
      school   == SCHOOL_NATURE && 
      p -> _buffs.nature_vulnerability_charges > 0 )
  {
    p -> _buffs.nature_vulnerability_charges--;
    if( p -> _buffs.nature_vulnerability_charges == 0 )
    {
      event_t::early( p -> _expirations.nature_vulnerability );
    }
  }
}

// =========================================================================
// Shaman Spells
// =========================================================================

// Chain Lightning Spell ===================================================

struct chain_lightning_t : public shaman_spell_t
{
  double   max_lvb_cd;
  int      maelstrom;
  stats_t* lightning_overload_stats;
  double   lightning_overload_chance;

  chain_lightning_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "chain_lightning", player, SCHOOL_NATURE, TREE_ELEMENTAL ), 
    max_lvb_cd(0), maelstrom(0), lightning_overload_stats(0), lightning_overload_chance(0)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { "lvb_cd<",   OPT_FLT, &max_lvb_cd },
      { "maelstrom", OPT_INT, &maelstrom  },
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 8, 973, 1111, 0, 0.26 },
      { 74, 7, 806,  920, 0, 0.26 },
      { 70, 6, 734,  838, 0,  760 },
      { 63, 5, 603,  687, 0,  650 },
      { 56, 4, 493,  551, 0,  550 },
      { 0, 0 }
    };
    init_rank( ranks );

    base_execute_time    = 2.0; 
    may_crit             = true;
    cooldown             = 6.0;
    direct_power_mod     = ( base_execute_time / 3.5 );
    cooldown            -= util_t::talent_rank( p -> talents.storm_earth_and_fire, 3, 0.75, 1.5, 2.5 );
    base_execute_time   -= p -> talents.lightning_mastery * 0.1;
    base_cost_reduction += p -> talents.convection * 0.02;
    base_multiplier     *= 1.0 + p -> talents.concussion * 0.01;
    base_hit            += p -> talents.elemental_precision * 0.01;
    base_crit           += p -> talents.call_of_thunder * 0.05;
    base_crit           += p -> talents.tidal_mastery * 0.01;

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;

    if( p -> totems.hex ) base_power += 165;

    lightning_overload_stats = p -> get_stats( "lightning_overload" );
    lightning_overload_stats -> school = SCHOOL_NATURE;
    
    if( sim -> P309 )
    {
      lightning_overload_chance = p -> talents.lightning_overload * 0.04 / 3.0;
    }
    else
    {
      lightning_overload_chance = util_t::talent_rank( p -> talents.lightning_overload, 3, 0.07, 0.14, 0.20 ) / 3.0;
    }
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();
    shaman_spell_t::execute();
    event_t::early( player -> cast_shaman() -> _expirations.maelstrom_weapon );
    if ( ( ! sim -> P309 ) && ( p -> _buffs.elemental_mastery == 1 ) )
    {
      p -> _buffs.elemental_mastery = 2;
    }
    if( result_is_hit() )
    {
      trigger_lightning_overload( this, lightning_overload_stats, lightning_overload_chance );
    }
  }

  virtual double execute_time()
  {
    double t = shaman_spell_t::execute_time();
    shaman_t* p = player -> cast_shaman();
    if( p -> _buffs.maelstrom_weapon )
    {
      if( p -> _buffs.maelstrom_weapon == 5 )
      {
        t = 0;
      }
      else
      {
        t *= ( 1.0 - p -> _buffs.maelstrom_weapon * 0.20 );
      }
    }
    if ( ( ! sim -> P309 ) && ( p -> _buffs.elemental_mastery == 1 ) )
    {
      t = 0;
    }
    return t;
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();

    if( ! shaman_spell_t::ready() )
      return false;

    if( maelstrom > 0 ) 
      if( maelstrom > p -> _buffs.maelstrom_weapon )
      return false;

    if( max_lvb_cd > 0  )
      if( ( p -> _cooldowns.lava_burst - sim -> current_time ) > ( max_lvb_cd * haste() ) )
	return false;

    return true;
  }
};

// Lightning Bolt Spell =====================================================

struct lightning_bolt_t : public shaman_spell_t
{
  int      maelstrom;
  int      ss_wait;
  stats_t* lightning_overload_stats;
  double   lightning_overload_chance;
  bool     tier8_4pc_elemental;
  lightning_bolt_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "lightning_bolt", player, SCHOOL_NATURE, TREE_ELEMENTAL ), 
    maelstrom(0), ss_wait(0), lightning_overload_stats(0), lightning_overload_chance(0), tier8_4pc_elemental(false)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { "maelstrom", OPT_INT, &maelstrom  },
      { "ss_wait",   OPT_INT, &ss_wait    },
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 79, 14, 715, 815, 0, 0.10 },
      { 73, 13, 595, 679, 0, 0.10 },
      { 67, 12, 563, 643, 0, 330  },
      { 62, 11, 495, 565, 0, 300  },
      { 56, 10, 419, 467, 0, 265  },
      { 50,  9, 347, 389, 0, 230  },
      { 44,  8, 282, 316, 0, 195  },
      { 0, 0 }
    };
    init_rank( ranks );

    base_execute_time    = 2.5; 
    may_crit             = true;
    direct_power_mod     = ( base_execute_time / 3.5 );
    base_execute_time   -= p -> talents.lightning_mastery * 0.1;
    base_cost_reduction += p -> talents.convection * 0.02;
    base_multiplier     *= 1.0 + p -> talents.concussion * 0.01;
    base_hit            += p -> talents.elemental_precision * 0.01;
    base_crit           += p -> talents.call_of_thunder * 0.05;
    base_crit           += p -> talents.tidal_mastery * 0.01;
    direct_power_mod    += p -> talents.shamanism * 0.02;

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;

    if( p -> tiers.t6_4pc_elemental ) base_multiplier     *= 1.05;
    if( p -> tiers.t7_2pc_elemental ) base_cost_reduction += 0.05;
    if( p -> glyphs.lightning_bolt  ) base_multiplier     *= 1.04;
    if( p -> totems.hex             ) base_power          += 165;

    lightning_overload_stats = p -> get_stats( "lightning_overload" );
    lightning_overload_stats -> school = SCHOOL_NATURE;

    if( sim -> P309 )
    {
      lightning_overload_chance = p -> talents.lightning_overload * 0.04;
    }
    else
    {
      lightning_overload_chance = util_t::talent_rank( p -> talents.lightning_overload, 3, 0.07, 0.14, 0.20 );
    }
    tier8_4pc_elemental = (p -> tiers.t8_4pc_elemental == 1);
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();
    shaman_spell_t::execute();
    event_t::early( player -> cast_shaman() -> _expirations.maelstrom_weapon );
    if ( ( ! sim -> P309 ) && ( p -> _buffs.elemental_mastery == 1 ) )
    {
      p -> _buffs.elemental_mastery = 2;
    }
    if( result_is_hit() )
    {
      trigger_ashtongue_talisman( this );
      trigger_lightning_overload( this, lightning_overload_stats, lightning_overload_chance );
      trigger_tier5_4pc_elemental( this );
      if( result == RESULT_CRIT && tier8_4pc_elemental )
        trigger_tier8_4pc_elemental( this );

    }
  }

  virtual double execute_time()
  {
    double t = shaman_spell_t::execute_time();
    shaman_t* p = player -> cast_shaman();
    if( p -> _buffs.maelstrom_weapon )
    {
      if( p -> _buffs.maelstrom_weapon == 5 )
      {
        t = 0;
      }
      else
      {
        t *= ( 1.0 - p -> _buffs.maelstrom_weapon * 0.20 );
      }
    }
    if ( ( ! sim -> P309 ) && ( p -> _buffs.elemental_mastery == 1 ) )
    {
      t = 0;
    }
    return t;
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();

    if( ! shaman_spell_t::ready() )
      return false;

    if( maelstrom > p -> _buffs.maelstrom_weapon )
      return false;

    if( ss_wait && p -> _buffs.nature_vulnerability <= 0 )
      return false;

    return true;
  }
};

// Lava Burst Spell =========================================================

struct lava_burst_t : public shaman_spell_t
{
  int flame_shock;
  int max_ticks_consumed;

  lava_burst_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "lava_burst", player, SCHOOL_FIRE, TREE_ELEMENTAL ), flame_shock(0), max_ticks_consumed(0)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { "flame_shock",        OPT_INT, &flame_shock        },
      { "max_ticks_consumed", OPT_INT, &max_ticks_consumed },
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 2, 1192, 1518, 0, 0.10 },
      { 75, 1, 1012, 1290, 0, 0.10 },
      { 0, 0 }
    };
    init_rank( ranks );

    base_execute_time    = 2.0; 
    direct_power_mod     = base_execute_time / 3.5;
    direct_power_mod    += p -> glyphs.lava ? 0.10 : 0.00;
    may_crit             = true;
    cooldown             = 8.0;
    base_cost_reduction += p -> talents.convection * 0.02;
    base_execute_time   -= p -> talents.lightning_mastery * 0.1;
    base_multiplier     *= 1.0 + p -> talents.concussion * 0.01;
    base_multiplier     *= 1.0 + p -> talents.call_of_flame * 0.02;
    base_hit            += p -> talents.elemental_precision * 0.01;
    direct_power_mod    += p -> talents.shamanism * 0.04;

    base_crit_bonus_multiplier *= 1.0 + ( util_t::talent_rank( p -> talents.lava_flows,     3, 0.06, 0.12, 0.24 ) +
                                          util_t::talent_rank( p -> talents.elemental_fury, 5, 0.20 ) +
                                          ( p -> tiers.t7_4pc_elemental ? 0.10 : 0.00 ) );
    if(  p -> totems.thunderfall )
    {
      // Shaman T8 Elemental Relic -- Increases the base damage of your Lavaburst by 215. 
      base_dd_min       += 215;
      base_dd_max       += 215;
    }
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();
    shaman_spell_t::execute();
    if ( ( ! sim -> P309 ) && ( p -> _buffs.elemental_mastery == 1 ) )
    {
      p -> _buffs.elemental_mastery = 2;
    }
    if( result_is_hit() )
    {
      if( p -> active_flame_shock && ! p -> glyphs.flame_shock )
      {
        p -> active_flame_shock -> cancel();
      }
    }
    p -> _cooldowns.lava_burst = cooldown_ready;
  }

  virtual double execute_time()
  {
    double t = shaman_spell_t::execute_time();
    shaman_t* p = player -> cast_shaman();
    if ( ( ! sim -> P309 ) && ( p -> _buffs.elemental_mastery == 1 ) )
    {
      t = 0;
    }
    return t;
  }

  virtual void player_buff()
  {
    shaman_spell_t::player_buff();
    shaman_t* p = player -> cast_shaman();
    if( p -> active_flame_shock ) player_crit += 1.0;
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    if( flame_shock )
    {
      shaman_t* p = player -> cast_shaman();

      if( ! p -> active_flame_shock ) 
        return false;

      double lvb_finish = sim -> current_time + execute_time();
      double fs_finish  = p -> active_flame_shock -> duration_ready;

      if( lvb_finish > fs_finish )
        return false;

      if ( ! p -> glyphs.flame_shock )
      {
        if( max_ticks_consumed > 0 )
        {
          double earliest_finish = fs_finish - max_ticks_consumed * 3.0;

          if( lvb_finish < earliest_finish )
            return false;
        }
      }
      else
      {
        return true;
      }
    }

    return true;
  }
};

// Elemental Mastery Spell ==================================================

struct elemental_mastery_t : public shaman_spell_t
{
  elemental_mastery_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "elemental_mastery", player, SCHOOL_NATURE, TREE_ELEMENTAL )
  {
    shaman_t* p = player -> cast_shaman();
    assert( p -> talents.elemental_mastery );

    cooldown  = 180.0;
    cooldown -= p -> glyphs.elemental_mastery ? 30.0 : 0.0;

    trigger_gcd = 0;  
  }
   
  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
      {
        name = "Elemental Mastery Expiration";
        shaman_t* p = player -> cast_shaman();
        p -> aura_gain( "Elemental Mastery" );
        p -> _buffs.elemental_mastery = 1;
        // Fix-Me. Needs a shared cooldown with nature's swiftness in 3.1.0
        sim -> add_event( this, ( sim -> P309 ? 30.0 : 15.0 ) );
      }
      virtual void execute()
      {
        shaman_t* p = player -> cast_shaman();
        p -> aura_loss( "Elemental Mastery" );
        p -> _buffs.elemental_mastery = 0;
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs elemental_mastery", player -> name() );
    update_ready();
    new (sim) expiration_t( sim, player );
  }
};

// Natures Swiftness Spell ==================================================

struct shamans_swiftness_t : public shaman_spell_t
{
  shamans_swiftness_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "natures_swiftness", player, SCHOOL_NATURE, TREE_RESTORATION )
  {
    shaman_t* p = player -> cast_shaman();
    assert( p -> talents.natures_swiftness );
    trigger_gcd = 0;  
    cooldown = 180.0;
    if( ! options_str.empty() )
    {
      // This will prevent Natures Swiftness from being called before the desired "free spell" is ready to be cast.
      cooldown_group = options_str;
      duration_group = options_str;
    }
  }
   
  virtual void execute()
  {
    if( sim -> log ) report_t::log( sim, "%s performs natures_swiftness", player -> name() );
    update_ready();
    shaman_t* p = player -> cast_shaman();
    p -> aura_gain( "Natures Swiftness" );
    p -> _buffs.natures_swiftness = 1;
  }
};

// Earth Shock Spell =======================================================

struct earth_shock_t : public shaman_spell_t
{
  int ss_wait;

  earth_shock_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "earth_shock", player, SCHOOL_NATURE, TREE_ELEMENTAL ), ss_wait(0)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { "ss_wait", OPT_INT, &ss_wait     },
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 79, 10, 849, 895, 0, 0.18 },
      { 74,  9, 723, 761, 0, 0.18 },
      { 69,  8, 658, 692, 0, 535  },
      { 60,  7, 517, 545, 0, 450  },
      { 48,  6, 359, 381, 0, 345  },
      { 0, 0 }
    };
    init_rank( ranks );
    
    base_execute_time = 0; 
    direct_power_mod  = 0.41;
    may_crit          = true;
    cooldown          = 6.0;
    cooldown_group    = "shock";
    cooldown         -= ( p -> talents.reverberation * 0.2 );
    base_multiplier  *= 1.0 + p -> talents.concussion * 0.01;
    base_hit         += p -> talents.elemental_precision * 0.01;

    base_cost_reduction  += ( p -> talents.convection        * 0.02 +
			      p -> talents.mental_quickness  * 0.02 +
			      p -> talents.shamanistic_focus * 0.45 );

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;

    if( p -> glyphs.shocking ) 
    {
      trigger_gcd = 0.5;
      min_gcd     = 0.5;
    }
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();

    if( ! shaman_spell_t::ready() )
      return false;

    if( ss_wait && p -> _buffs.nature_vulnerability <= 0 )
      return false;

    return true;
  }
};

// Frost Shock Spell =======================================================

struct frost_shock_t : public shaman_spell_t
{
  frost_shock_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "frost_shock", player, SCHOOL_FROST, TREE_ELEMENTAL )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 78, 7, 802, 848, 0, 0.18 },
      { 73, 6, 681, 719, 0, 0.18 },
      { 68, 5, 640, 676, 0, 525  },
      { 58, 4, 486, 514, 0, 430  },
      { 46, 3, 333, 353, 0, 325  },
      { 0, 0 }
    };
    init_rank( ranks );
    
    base_execute_time = 0; 
    direct_power_mod  = 0.41;
    may_crit          = true;
    cooldown          = 6.0;
    cooldown_group    = "shock";
    cooldown         -= ( p -> talents.reverberation  * 0.2 +
			  p -> talents.booming_echoes * 1.0 );
    base_multiplier  *= 1.0 + ( p -> talents.concussion     * 0.01 +
				p -> talents.booming_echoes * 0.10 );
    base_hit         += p -> talents.elemental_precision * 0.01;

    base_cost_reduction  += ( p -> talents.convection        * 0.02 +
			      p -> talents.mental_quickness  * 0.02 +
			      p -> talents.shamanistic_focus * 0.45 );

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;

    if( p -> glyphs.shocking ) 
    {
      trigger_gcd = 0.5;
      min_gcd     = 0.5;
    }
  }
};

// Flame Shock Spell =======================================================

struct flame_shock_t : public shaman_spell_t
{
  flame_shock_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "flame_shock", player, SCHOOL_FIRE, TREE_ELEMENTAL )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 9, 500, 500, 139, 0.17 },
      { 75, 8, 425, 425, 119, 0.17 },
      { 70, 7, 377, 377, 105, 500  },
      { 60, 6, 309, 309,  86, 450  },
      { 0, 0 }
    };
    init_rank( ranks );
    
    base_execute_time = 0; 
    base_tick_time    = 3.0;
    num_ticks         = 4;
    direct_power_mod  = 0.215;
    tick_power_mod    = 0.100;
    may_crit          = true;
    cooldown          = 6.0;
    cooldown_group    = "shock";
    tick_may_crit     = ( p -> tiers.t8_2pc_elemental != 0 );

    if ( p -> glyphs.flame_shock ) num_ticks += 2;
      
    cooldown -= ( p -> talents.reverberation  * 0.2 +
		  p -> talents.booming_echoes * 1.0 );

    base_hit += p -> talents.elemental_precision * 0.01;

    base_dd_multiplier *= 1.0 + ( p -> talents.concussion     * 0.01 +
				  p -> talents.booming_echoes * 0.10 );

    base_td_multiplier *= 1.0 + ( p -> talents.concussion * 0.01 +
				  util_t::talent_rank( p -> talents.storm_earth_and_fire, 3, 0.20 ) );

    base_cost_reduction  += ( p -> talents.convection        * 0.02 +
			      p -> talents.mental_quickness  * 0.02 +
			      p -> talents.shamanistic_focus * 0.45 );

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;

    if( p -> glyphs.shocking ) 
    {
      trigger_gcd = 0.5;
      min_gcd     = 0.5;
    }

    observer = &( p -> active_flame_shock );
  }
};

// Searing Totem Spell =======================================================

struct searing_totem_t : public shaman_spell_t
{
  searing_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "searing_totem", player, SCHOOL_FIRE, TREE_ELEMENTAL )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 10, 90, 120, 0, 0.07 },
      { 75,  9, 77, 103, 0, 0.07 },
      { 71,  8, 68,  92, 0, 0.07 },
      { 69,  7, 56,  74, 0, 0.07 },
      { 60,  6, 40,  54, 0, 0.09 },
      { 0, 0 }
    };
    init_rank( ranks );

    base_execute_time = 0; 
    base_tick_time    = 2.5;
    direct_power_mod  = base_tick_time / 15.0;
    num_ticks         = 24;
    may_crit          = true;
    duration_group    = "fire_totem";
    trigger_gcd       = 1.0;
    base_multiplier  *= 1.0 + p -> talents.call_of_flame * 0.05;
    base_hit         += p -> talents.elemental_precision * 0.01;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;
  }

  // Odd things to handle:
  // (1) Execute is guaranteed.
  // (2) Each "tick" is like an "execute".
  // (3) No hit/miss events are triggered.

  virtual void execute() 
  {
    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    player_buff();
    schedule_tick();
    update_ready();
    direct_dmg = 0;
    update_stats( DMG_DIRECT );
    player -> action_finish( this );
  }

  virtual void tick() 
  {
    if( sim -> debug ) report_t::log( sim, "%s ticks (%d of %d)", name(), current_tick, num_ticks );
    may_resist = false;
    target_debuff( DMG_DIRECT );
    calculate_result();
    may_resist = true;
    if( result_is_hit() )
    {
      calculate_direct_damage();
      if( direct_dmg > 0 )
      {
        tick_dmg = direct_dmg;
        assess_damage( tick_dmg, DMG_OVER_TIME );
      }
    }
    else
    {
      if( sim -> log ) report_t::log( sim, "%s avoids %s (%s)", sim -> target -> name(), name(), util_t::result_type_string( result ) );
    }
    update_stats( DMG_OVER_TIME );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Magma Totem Spell =======================================================

struct magma_totem_t : public shaman_spell_t
{
  magma_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "magma_totem", player, SCHOOL_FIRE, TREE_ELEMENTAL )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 78,  7, 371, 371, 0, 0.27 },
      { 73,  6, 314, 314, 0, 0.27 },
      { 65,  5, 180, 180, 0, 0.27 },
      { 56,  4, 131, 131, 0, 0.27 },
      { 0, 0 }
    };
    init_rank( ranks );

    base_execute_time = 0; 
    base_tick_time    = 2.0;
    direct_power_mod  = 0.75 * base_tick_time / 15.0;
    num_ticks         = 10;
    may_crit          = true;
    duration_group    = "fire_totem";
    trigger_gcd       = 1.0;
    base_multiplier  *= 1.0 + p -> talents.call_of_flame * 0.05;
    base_hit         += p -> talents.elemental_precision * 0.01;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;
  }

  // Odd things to handle:
  // (1) Execute is guaranteed.
  // (2) Each "tick" is like an "execute".
  // (3) No hit/miss events are triggered.

  virtual void execute() 
  {
    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    player_buff();
    schedule_tick();
    update_ready();
    direct_dmg = 0;
    update_stats( DMG_DIRECT );
    player -> action_finish( this );
  }

  virtual void tick() 
  {
    if( sim -> debug ) report_t::log( sim, "%s ticks (%d of %d)", name(), current_tick, num_ticks );
    may_resist = false;
    target_debuff( DMG_DIRECT );
    calculate_result();
    may_resist = true;
    if( result_is_hit() )
    {
      calculate_direct_damage();
      if( direct_dmg > 0 )
      {
        tick_dmg = direct_dmg;
        assess_damage( tick_dmg, DMG_OVER_TIME );
      }
    }
    else
    {
      if( sim -> log ) report_t::log( sim, "%s avoids %s (%s)", sim -> target -> name(), name(), util_t::result_type_string( result ) );
    }
    update_stats( DMG_OVER_TIME );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Totem of Wrath Spell =====================================================

struct totem_of_wrath_t : public shaman_spell_t
{
  double bonus_spell_power;

  totem_of_wrath_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "totem_of_wrath", player, SCHOOL_NATURE, TREE_ELEMENTAL ), bonus_spell_power(0)
  {
    shaman_t* p = player -> cast_shaman();
    assert( p -> talents.totem_of_wrath );

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );
      
    base_cost      = 400;
    duration_group = "fire_totem";
    trigger_gcd    = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    bonus_spell_power = util_t::ability_rank( p -> level,  280.0,80,  140.0,70,  120.0,0 );
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();

    if( sim -> log ) report_t::log( sim, "%s performs %s", p -> name(), name() );

    consume_resource();
    update_ready();

    if( p -> buffs.totem_of_wrath == 0 )
    {
      struct totem_expiration_t : public event_t
      {
	totem_expiration_t( sim_t* sim, player_t* player, double bonus_spell_power ) : event_t( sim, player )
	{
	  name = "Totem of Wrath Expiration";
	  sim -> target -> debuffs.totem_of_wrath++;
	  for( player_t* p = sim -> player_list; p; p = p -> next )
	  {
	    p -> aura_gain( "Totem of Wrath" );
	    p -> buffs.totem_of_wrath = bonus_spell_power;
	  }
	  sim -> add_event( this, 300.0 );
	}
	virtual void execute()
	{
	  sim -> target -> debuffs.totem_of_wrath--;
	  for( player_t* p = sim -> player_list; p; p = p -> next )
	  {
	    p -> aura_loss( "Totem of Wrath" );
	    p -> buffs.totem_of_wrath = 0;
	  }
	}
      };

      new ( sim ) totem_expiration_t( sim, p, bonus_spell_power );
    }

    if( p -> glyphs.totem_of_wrath )
    {
      struct glyph_expiration_t : public event_t
      {
	glyph_expiration_t( sim_t* sim, shaman_t* p, double bonus_spell_power ) : event_t( sim, p )
	{
	  name = "Totem of Wrath Glyph Expiration";
	  p -> aura_gain( "Totem of Wrath Glyph" );
	  p -> _buffs.totem_of_wrath_glyph = bonus_spell_power;
	  sim -> add_event( this, 300.0 );
	}
	virtual void execute()
	{
	  shaman_t* p = player -> cast_shaman();
	  p -> aura_loss( "Totem of Wrath Glyph" );
	  p -> _buffs.totem_of_wrath_glyph = 0;
	}
      };

      new ( sim ) glyph_expiration_t( sim, p, bonus_spell_power * 0.30 );
    }

    p -> action_finish( this );
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();

    if( ! shaman_spell_t::ready() )
      return false;

    if( p -> buffs.totem_of_wrath == 0 )
      return true;

    if( p -> glyphs.totem_of_wrath && p -> _buffs.totem_of_wrath_glyph == 0 )
      return true;

    return false;
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Flametongue Totem Spell ====================================================

struct flametongue_totem_t : public shaman_spell_t
{
  double bonus;

  flametongue_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "flametongue_totem", player, SCHOOL_FIRE, TREE_ENHANCEMENT ), bonus(0)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 8,  0,  0, 144, 0.11 },
      { 76, 7,  0,  0, 122, 0.11 },
      { 72, 6,  0,  0, 106, 0.11 },
      { 67, 5, 12, 12,  73, 325  },
      { 58, 4, 15, 15,  62, 275  },
      { 0, 0 }
    };
    init_rank( ranks );

    base_cost      = p -> resource_base[ RESOURCE_MANA ] * 0.11;
    duration_group = "fire_totem";
    trigger_gcd    = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    bonus = util_t::ability_rank( p -> level,  144,80,  122,76,  106,72,  73,67,  62,0 );
    bonus *= 1 + p -> talents.enhancing_totems * 0.05;
  }

  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      flametongue_totem_t* totem;

      expiration_t( sim_t* sim, player_t* player, flametongue_totem_t* t ) : event_t( sim, player ), totem(t)
      {
        name = "Flametongue Totem Expiration";
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          p -> aura_gain( "Flametongue Totem" );
          p -> buffs.flametongue_totem = t -> bonus;
        }
        sim -> add_event( this, 300.0 );
      }

      virtual void execute()
      {
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          p -> aura_loss( "Flametongue Totem" );
          p -> buffs.flametongue_totem = 0;
        }
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    update_ready();
    player -> action_finish( this );
    new ( sim ) expiration_t( sim, player, this );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return( player -> buffs.flametongue_totem == 0 );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Windfury Totem Spell =====================================================

struct windfury_totem_t : public shaman_spell_t
{
  double bonus;

  windfury_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "windfury_totem", player, SCHOOL_NATURE, TREE_ENHANCEMENT )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );
      
    base_cost      = 275;
    duration_group = "air_totem";
    trigger_gcd    = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    bonus = 0.16 + p -> talents.improved_windfury_totem * 0.02;
  }

  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      windfury_totem_t* totem;

      expiration_t( sim_t* sim, player_t* player, windfury_totem_t* t ) : event_t( sim, player ), totem(t)
      {
        name = "Windfury Totem Expiration";
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          p -> aura_gain( "Windfury Totem" );
          p -> buffs.windfury_totem = t -> bonus;
        }
        sim -> add_event( this, 300.0 );
      }
      virtual void execute()
      {
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          // Make sure it hasn't already been overriden by a more powerful totem.
          if( totem -> bonus < p -> buffs.windfury_totem )
            continue;

          p -> aura_loss( "Windfury Totem" );
          p -> buffs.windfury_totem = 0;
        }
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    update_ready();
    player -> action_finish( this );
    new ( sim ) expiration_t( sim, player, this );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return( player -> buffs.windfury_totem < bonus );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Flametongue Weapon Spell ===================================================

struct flametongue_weapon_t : public shaman_spell_t
{
  double bonus_power;
  int    main, off;

  flametongue_weapon_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "flametongue_weapon", player, SCHOOL_FIRE, TREE_ENHANCEMENT ), bonus_power(0), main(0), off(0)
  {
    shaman_t* p = player -> cast_shaman();
    
    std::string weapon_str;

    option_t options[] =
    {
      { "weapon", OPT_STRING, &weapon_str },
      { NULL }
    };
    parse_options( options, options_str );

    if( weapon_str.empty() ) 
    {
      main = off = 1;
    }
    else if( weapon_str == "main" )
    {
      main = 1;
    }
    else if( weapon_str == "off" )
    {
      off = 1;
    }
    else if( weapon_str == "both" )
    {
      main = 1;
      off = 1;
    }
    else
    {
      fprintf( sim -> output_file, "flametongue_weapon: weapon option must be one of main/off/both\n" );
      assert(0);
    }
    trigger_gcd = 0;

    bonus_power = util_t::ability_rank( p -> level,  211.0,80,  186.0,77,  157.0,72,  96.0,65,  45.0,0  );

    bonus_power *= 1.0 + p -> talents.elemental_weapons * 0.10;
  }

  virtual void execute()
  {
    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );

    if( main ) 
    {
      player -> main_hand_weapon.buff = FLAMETONGUE;
      player -> main_hand_weapon.buff_bonus = bonus_power;
    }
    if( off )
    {
      player -> off_hand_weapon.buff = FLAMETONGUE;
      player -> off_hand_weapon.buff_bonus = bonus_power;
    }
  };

  virtual bool ready()
  {
    if( main && ( player -> main_hand_weapon.buff != FLAMETONGUE ) )
      return true;

    if( off && ( player -> off_hand_weapon.buff != FLAMETONGUE ) )
      return true;

    return false;
  }
};

// Windfury Weapon Spell ====================================================

struct windfury_weapon_t : public shaman_spell_t
{
  double bonus_power;
  int    main, off;

  windfury_weapon_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "windfury_weapon", player, SCHOOL_NATURE, TREE_ENHANCEMENT ), bonus_power(0), main(0), off(0)
  {
    shaman_t* p = player -> cast_shaman();

    std::string weapon_str;
    option_t options[] =
    {
      { "weapon", OPT_STRING, &weapon_str },
      { NULL }
    };
    parse_options( options, options_str );

    if( weapon_str.empty() ) 
    {
      main = off = 1;
    }
    else if( weapon_str == "main" )
    {
      main = 1;
    }
    else if( weapon_str == "off" )
    {
      off = 1;
    }
    else if( weapon_str == "both" )
    {
      main = 1;
      off = 1;
    }
    else
    {
      fprintf( sim -> output_file, "windfury_weapon: weapon option must be one of main/off/both\n" );
      assert(0);
    }
    trigger_gcd = 0;

    bonus_power = util_t::ability_rank( p -> level,  1250.0,80,  1090.0,76,  835.0,71,  445.0,0  );

    if( p -> glyphs.windfury_weapon ) bonus_power *= 1.40;
  }

  virtual void execute()
  {
    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );

    if( main ) 
    {
      player -> main_hand_weapon.buff = WINDFURY;
      player -> main_hand_weapon.buff_bonus = bonus_power;
    }
    if( off ) 
    {
      player -> off_hand_weapon.buff = WINDFURY;
      player -> off_hand_weapon.buff_bonus = bonus_power;
    }
  };

  virtual bool ready()
  {
    if( main && ( player -> main_hand_weapon.buff != WINDFURY ) )
      return true;

    if( off && ( player -> off_hand_weapon.buff != WINDFURY ) )
      return true;

    return false;
  }
};

// Strength of Earth Totem Spell ==============================================

struct strength_of_earth_totem_t : public shaman_spell_t
{
  double bonus;

  strength_of_earth_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "strength_of_earth_totem", player, SCHOOL_NATURE, TREE_ENHANCEMENT ), bonus(0)
  {
    shaman_t* p = player -> cast_shaman();

    base_cost      = util_t::ability_rank( p -> level,  300,65,  275,0 );
    duration_group = "earth_totem";
    trigger_gcd    = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    bonus  = util_t::ability_rank( p -> level,  86,65,  77,0 );
    bonus *= 1.0 + p -> talents.enhancing_totems * 0.05;
  }

  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      double bonus;

      expiration_t( sim_t* sim, player_t* player, double b ) : event_t( sim, player ), bonus(b)
      {
        name = "Strength of Earth Totem Expiration";
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          p -> aura_gain( "Strength of Earth Totem" );
          p -> buffs.strength_of_earth = bonus;
        }
        sim -> add_event( this, 300.0 );
      }
      virtual void execute()
      {
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          // Make sure it hasn't already been overriden by a more powerful totem.
          if( bonus < p -> buffs.strength_of_earth )
            continue;

          p -> aura_loss( "Strength of Earth Totem" );
          p -> buffs.strength_of_earth = 0;
        }
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    update_ready();
    player -> action_finish( this );
    new ( sim ) expiration_t( sim, player, bonus );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return( player -> buffs.strength_of_earth < bonus );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Wrath of Air Totem Spell =================================================

struct wrath_of_air_totem_t : public shaman_spell_t
{
  wrath_of_air_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "wrath_of_air_totem", player, SCHOOL_NATURE, TREE_ENHANCEMENT )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );
      
    base_cost      = 320;
    duration_group = "air_totem";
    trigger_gcd    = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );
  }

  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
      {
        name = "Wrath of Air Totem Expiration";
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          p -> aura_gain( "Wrath of Air Totem" );
          p -> buffs.wrath_of_air = 1;
        }
        sim -> add_event( this, 300.0 );
      }
      virtual void execute()
      {
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          p -> aura_loss( "Wrath of Air Totem" );
          p -> buffs.wrath_of_air = 0;
        }
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    update_ready();
    player -> action_finish( this );
    new ( sim ) expiration_t( sim, player );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return( player -> buffs.wrath_of_air == 0 );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Mana Tide Totem Spell ==================================================

struct mana_tide_totem_t : public shaman_spell_t
{
  mana_tide_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "mana_tide_totem", player, SCHOOL_NATURE, TREE_RESTORATION )
  {
    shaman_t* p = player -> cast_shaman();
    assert( p -> talents.mana_tide_totem );

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    harmful        = false;
    base_tick_time = 3.0; 
    num_ticks      = 4;
    cooldown       = 300.0;
    duration_group = "water_totem";
    base_cost      = 320;
    trigger_gcd     = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );
  }

  virtual void execute() 
  {
    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    schedule_tick();
    update_ready();
    player -> action_finish( this );
  }

  virtual void tick() 
  {
    if( sim -> debug ) report_t::log( sim, "%s ticks (%d of %d)", name(), current_tick, num_ticks );

    double pct = 0.06;
    if( player -> cast_shaman() -> glyphs.mana_tide ) pct += 0.01;

    for( player_t* p = sim -> player_list; p; p = p -> next )
    {
      if( p -> party == player -> party )
      {
        p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * pct, p -> gains.mana_tide );
      }
    }
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return( player -> resource_current[ RESOURCE_MANA ] < ( 0.75 * player -> resource_max[ RESOURCE_MANA ] ) );
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Mana Spring Totem Spell ================================================

struct mana_spring_totem_t : public shaman_spell_t
{
  double regen;

  mana_spring_totem_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "mana_spring_totem", player, SCHOOL_NATURE, TREE_RESTORATION ), regen(0)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    harmful         = false;
    duration_group  = "water_totem";
    base_cost       = 120;
    trigger_gcd     = 1.0;

    base_cost_reduction += ( p -> talents.totemic_focus    * 0.05 +
			     p -> talents.mental_quickness * 0.02 );

    if ( sim -> P309 )
    {
      regen = util_t::ability_rank( p -> level,  34.0,80,  30.0,76,  25.0,71,  20.0,65,  17.0,0 );

      regen *= 1.0 + p -> talents.restorative_totems * 0.05;
    }
    else
    {
      regen = util_t::ability_rank( p -> level,  91.0,80,  82.0,76,  73.0,71,  41.0,65,  31.0,0 );

      regen *= 1.0 + util_t::talent_rank( p -> talents.restorative_totems, 3, 0.07, 0.12, 0.20 );
    }
  }

  virtual void execute() 
  {
    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    for( player_t* p = sim -> player_list; p; p = p -> next )
    {
      p -> buffs.mana_spring = regen;
    }
    update_ready();
    player -> action_finish( this );
  }

  virtual bool ready()
  {
    if( player -> buffs.mana_spring >= regen )
      return false;

    return shaman_spell_t::ready();
  }

  virtual double gcd() { return player -> in_combat ? shaman_spell_t::gcd() : 0; }
};

// Bloodlust Spell ===========================================================

struct bloodlust_t : public shaman_spell_t
{
  int target_pct;

  bloodlust_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "bloodlust", player, SCHOOL_NATURE, TREE_ENHANCEMENT ), target_pct(0)
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { "target_pct", OPT_DEPRECATED, (void*) "health_percentage<" },
      { NULL }
    };
    parse_options( options, options_str );

    harmful = false;
    base_cost = ( 0.26 * player -> resource_base[ RESOURCE_MANA ] );
    base_cost_reduction += p -> talents.mental_quickness * 0.02;
    cooldown = ( sim -> P309 ? 600 : 300 );
  }

  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
      {
        name = "Bloodlust Expiration";
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          if ( p -> sleeping ) continue;
          if( sim -> cooldown_ready( p -> cooldowns.bloodlust ) )
          {
            p -> aura_gain( "Bloodlust" );
            p -> buffs.bloodlust = 1;
            p -> cooldowns.bloodlust = sim -> current_time + ( sim -> P309 ? 300 : 600 );
          }
        }
        sim -> add_event( this, 40.0 );
      }
      virtual void execute()
      {
        for( player_t* p = sim -> player_list; p; p = p -> next )
        {
          if (p -> buffs.bloodlust > 0 )
          {
            p -> aura_loss( "Bloodlust" );
            p -> buffs.bloodlust = 0;
          }
        }
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    consume_resource();
    update_ready();
    player -> action_finish( this );
    new ( sim ) expiration_t( sim, player );
  }

  virtual bool ready()
  {
    if( player -> buffs.bloodlust )
      return false;

    if( ! sim -> cooldown_ready( player -> cooldowns.bloodlust ) )
      return false;

    return shaman_spell_t::ready();
  }
};

// Shamanisitc Rage Spell ===========================================================

struct shamanistic_rage_t : public shaman_spell_t
{
  shamanistic_rage_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "shamanistic_rage", player, SCHOOL_NATURE, TREE_ENHANCEMENT )
  {
    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );
     
    cooldown = 120;
  }

  virtual void execute()
  {
    struct expiration_t : public event_t
    {
      expiration_t( sim_t* sim, player_t* player ) : event_t( sim, player )
      {
        name = "Shamanistic Rage Expiration";
        shaman_t* p = player -> cast_shaman();
        p -> aura_gain( "Shamanistic Rage" );
        p -> _buffs.shamanistic_rage = 1;
        sim -> add_event( this, 15.0 );
      }
      virtual void execute()
      {
        shaman_t* p = player -> cast_shaman();
        p -> aura_loss( "Shamanistic Rage" );
        p -> _buffs.shamanistic_rage = 0;
      }
    };

    if( sim -> log ) report_t::log( sim, "%s performs %s", player -> name(), name() );
    update_ready();
    player -> action_finish( this );
    new ( sim ) expiration_t( sim, player );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return( player -> resource_current[ RESOURCE_MANA ] < ( 0.10 * player -> resource_max[ RESOURCE_MANA ] ) );
  }
};

// Lightning Shield Spell =====================================================

struct lightning_shield_t : public shaman_spell_t
{
  struct lightning_charge_t : public shaman_spell_t
  {
    lightning_charge_t( player_t* player, double base_dmg ) : 
      shaman_spell_t( "lightning_shield", player, SCHOOL_NATURE, TREE_ENHANCEMENT )
    {
      // Use the same name "lightning_shield" to make sure the cost of refreshing the shield is included with the procs.
    
      shaman_t* p = player -> cast_shaman();

      trigger_gcd      = 0;
      background       = true;
      direct_power_mod = 0.33;

      base_direct_dmg  = base_dmg;
      base_hit        += p -> talents.elemental_precision * 0.01;
      base_multiplier *= 1.0 + p -> talents.improved_shields * 0.05 + ( p -> tiers.t7_2pc_enhancement ? 0.10 : 0.00 );

      base_crit_bonus_multiplier *= 1.0 + p -> talents.elemental_fury * 0.20;

      if( p -> glyphs.lightning_shield ) base_multiplier *= 1.20;
    }
    virtual void cancel()
    {
      shaman_t* p = player -> cast_shaman();
      p -> aura_loss( "Lightning Shield" );
      p -> _buffs.lightning_charges = 0;
      p -> _buffs.lightning_shield = 0;    
    }
  };

  lightning_shield_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "lightning_shield", player, SCHOOL_NATURE, TREE_ENHANCEMENT )
  {
    shaman_t* p = player -> cast_shaman();

    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 80, 7, 380, 380, 0, 0 },
      { 75, 6, 325, 325, 0, 0 },
      { 70, 5, 287, 287, 0, 0 },
      { 63, 4, 232, 232, 0, 0 },
      { 56, 3, 198, 198, 0, 0 },
      { 0, 0 }
    };
    init_rank( ranks );
    
    if( ! p -> active_lightning_charge )
    {
      p -> active_lightning_charge = new lightning_charge_t( p, ( base_dd_min + base_dd_max ) / 2.0 ) ;
    }
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();
    if( sim -> log ) report_t::log( sim, "%s performs %s", p -> name(), name() );
    if( p -> _buffs.water_shield )
    {
      p -> aura_loss( "Water Shield" );
      p -> _buffs.water_shield = 0;
    }
    p -> _buffs.lightning_charges = 3 + 2 * p -> talents.static_shock;
    p -> _buffs.lightning_shield = 1;    
    p -> aura_gain( "Lightning Shield" );
    consume_resource();
    update_ready();
    direct_dmg = 0;
    p -> action_finish( this );
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();

    if( p -> _buffs.lightning_shield )
      return false;

    return shaman_spell_t::ready();
  }
};

// Water Shield Spell =========================================================

struct water_shield_t : public shaman_spell_t
{
  water_shield_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "water_shield", player, SCHOOL_NATURE, TREE_ENHANCEMENT )
  {
    option_t options[] =
    {
      { NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 76, 9, 0, 0, 0, 100 },
      { 69, 8, 0, 0, 0,  50 },
      { 62, 7, 0, 0, 0,  43 },
      { 55, 6, 0, 0, 0,  38 },
      { 0, 0 }
    };
    init_rank( ranks );

    trigger_gcd = 0;
  }

  virtual void execute()
  {
    shaman_t* p = player -> cast_shaman();
    if( sim -> log ) report_t::log( sim, "%s performs %s", p -> name(), name() );
    if( p -> _buffs.lightning_shield )
    {
      p -> active_lightning_charge -> cancel();
    }
    p -> aura_gain( "Water Shield" );
    p -> _buffs.water_shield = base_cost;
  }

  virtual bool ready()
  {
    shaman_t* p = player -> cast_shaman();

    if( p -> _buffs.water_shield )
      return false;

    return shaman_spell_t::ready();
  }
};

// Thunderstorm Spell ==========================================================

struct thunderstorm_t : public shaman_spell_t
{
  thunderstorm_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "thunderstorm", player, SCHOOL_NATURE, TREE_ELEMENTAL )
  {
    shaman_t* p = player -> cast_shaman();
    assert( p -> talents.thunderstorm );
    cooldown = 45.0;
    if( p -> glyphs.thunderstorm ) cooldown -= 7.0;
  }

  virtual void execute() 
  {
    shaman_t* p = player -> cast_shaman();
    update_ready();
    p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * 0.08, p -> gains_thunderstorm );
    p -> action_finish( this );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    return player -> resource_current[ RESOURCE_MANA ] < ( 0.92 * player -> resource_max[ RESOURCE_MANA ] );
  }
};

// Spirit Wolf Spell ==========================================================

struct spirit_wolf_spell_t : public shaman_spell_t
{
  struct spirit_wolf_expiration_t : public event_t
  {
    spirit_wolf_expiration_t( sim_t* sim, player_t* p ) : event_t( sim, p )
    {
      sim -> add_event( this, 45.0 );
    }
    virtual void execute()
    {
      player -> dismiss_pet( "spirit_wolf" );
    }
  };

  int target_pct;

  spirit_wolf_spell_t( player_t* player, const std::string& options_str ) : 
    shaman_spell_t( "spirit_wolf", player, SCHOOL_NATURE, TREE_ENHANCEMENT ), target_pct(0)
  {
    shaman_t* p = player -> cast_shaman();
    assert( p -> talents.feral_spirit );

    option_t options[] =
    {
      { "trigger", OPT_INT, &target_pct },
      { NULL }
    };
    parse_options( options, options_str );

    cooldown   = 180.0;
    base_cost  = p -> resource_max[ RESOURCE_MANA ] * 0.12;
  }

  virtual void execute() 
  {
    consume_resource();
    update_ready();
    player -> summon_pet( "spirit_wolf" );
    player -> action_finish( this );
    new ( sim ) spirit_wolf_expiration_t( sim, player );
  }

  virtual bool ready()
  {
    if( ! shaman_spell_t::ready() )
      return false;

    target_t* t = sim -> target;

    if( t -> time_to_die() > 60 )
    {
      if( target_pct > 0 )
      {
	if( t -> health_percentage() > target_pct )
	  return false;
      }
    }

    return true;
  }
};

} // ANONYMOUS NAMESPACE ===================================================

// ==========================================================================
// Shaman Character Definition
// ==========================================================================

// shaman_t::create_action  =================================================

action_t* shaman_t::create_action( const std::string& name,
                                   const std::string& options_str )
{
  if( name == "auto_attack"             ) return new              auto_attack_t( this, options_str );
  if( name == "bloodlust"               ) return new                bloodlust_t( this, options_str );
  if( name == "chain_lightning"         ) return new          chain_lightning_t( this, options_str );
  if( name == "earth_shock"             ) return new              earth_shock_t( this, options_str );
  if( name == "elemental_mastery"       ) return new        elemental_mastery_t( this, options_str );
  if( name == "flame_shock"             ) return new              flame_shock_t( this, options_str );
  if( name == "flametongue_totem"       ) return new        flametongue_totem_t( this, options_str );
  if( name == "flametongue_weapon"      ) return new       flametongue_weapon_t( this, options_str );
  if( name == "frost_shock"             ) return new              frost_shock_t( this, options_str );
  if( name == "lava_burst"              ) return new               lava_burst_t( this, options_str );
  if( name == "lava_lash"               ) return new                lava_lash_t( this, options_str );
  if( name == "lightning_bolt"          ) return new           lightning_bolt_t( this, options_str );
  if( name == "lightning_shield"        ) return new         lightning_shield_t( this, options_str );
  if( name == "mana_spring_totem"       ) return new        mana_spring_totem_t( this, options_str );
  if( name == "mana_tide_totem"         ) return new          mana_tide_totem_t( this, options_str );
  if( name == "natures_swiftness"       ) return new        shamans_swiftness_t( this, options_str );
  if( name == "searing_totem"           ) return new            searing_totem_t( this, options_str );
  if( name == "magma_totem"             ) return new              magma_totem_t( this, options_str );
  if( name == "shamanistic_rage"        ) return new         shamanistic_rage_t( this, options_str );
  if( name == "spirit_wolf"             ) return new        spirit_wolf_spell_t( this, options_str );
  if( name == "stormstrike"             ) return new              stormstrike_t( this, options_str );
  if( name == "strength_of_earth_totem" ) return new  strength_of_earth_totem_t( this, options_str );
  if( name == "thunderstorm"            ) return new             thunderstorm_t( this, options_str );
  if( name == "totem_of_wrath"          ) return new           totem_of_wrath_t( this, options_str );
  if( name == "water_shield"            ) return new             water_shield_t( this, options_str );
  if( name == "windfury_totem"          ) return new           windfury_totem_t( this, options_str );
  if( name == "windfury_weapon"         ) return new          windfury_weapon_t( this, options_str );
  if( name == "wrath_of_air_totem"      ) return new       wrath_of_air_totem_t( this, options_str );

  return player_t::create_action( name, options_str );
}

// shaman_t::create_pet ======================================================

pet_t* shaman_t::create_pet( const std::string& pet_name )
{
  if( pet_name == "spirit_wolf" ) return new spirit_wolf_pet_t( sim, this );

  return 0;
}

// shaman_t::init_rating ======================================================

void shaman_t::init_rating()
{
  player_t::init_rating();

  if ( !sim -> P309 )
    rating.attack_haste *= 1.0 / 1.30;
}

// shaman_t::init_base ========================================================

void shaman_t::init_base()
{
  attribute_base[ ATTR_STRENGTH  ] = 125;
  attribute_base[ ATTR_AGILITY   ] =  69;
  attribute_base[ ATTR_STAMINA   ] = 135;
  attribute_base[ ATTR_INTELLECT ] = 123;
  attribute_base[ ATTR_SPIRIT    ] = 140;

  if( sim -> P309 )
  {
    attribute_multiplier_initial[ ATTR_INTELLECT ] *= 1.0 + talents.ancestral_knowledge * 0.02;
  }
  else
  {
    attribute_multiplier_initial[ ATTR_INTELLECT ] *= 1.0 + talents.ancestral_knowledge * 0.02;
    attribute_multiplier_initial[ ATTR_STAMINA   ] *= 1.0 + talents.toughness           * 0.02;
    base_attack_expertise = 0.25 * talents.unleashed_rage * 0.03;
  }

  base_spell_crit = 0.0220045;
  initial_spell_crit_per_intellect = rating_t::interpolate( level, 0.01/60.0, 0.01/80.0, 0.01/166.79732 );

  base_attack_power = ( level * 2 ) - 20;
  base_attack_crit  = 0.0292234;
  initial_attack_power_per_strength = 1.0;
  initial_attack_power_per_agility  = 1.0;
  initial_attack_crit_per_agility = rating_t::interpolate( level, 0.01/25.0, 0.01/40.0, 0.01/83.388 );


  resource_base[ RESOURCE_HEALTH ] = 3185;
  resource_base[ RESOURCE_MANA   ] = rating_t::interpolate( level, 1415, 2680, 4396 );

  health_per_stamina = 10;
  mana_per_intellect = 15;

  mp5_per_intellect = util_t::talent_rank( talents.unrelenting_storm, 3, 0.04 );

  if( tiers.t6_2pc_elemental )
  {
    // Simply assume the totems are out all the time.

    gear.spell_power_enchant[ SCHOOL_MAX ] += 45;
    gear.crit_rating_enchant               += 35;
    gear.mp5_enchant                       += 15;
  }
}

// shaman_t::init_unique_gear ===============================================

void shaman_t::init_unique_gear()
{
  player_t::init_unique_gear();

  if( talents.dual_wield )
  {
    if( gear.tier4_2pc ) tiers.t4_2pc_enhancement = 1;
    if( gear.tier4_4pc ) tiers.t4_4pc_enhancement = 1;
    if( gear.tier5_2pc ) tiers.t5_2pc_enhancement = 1;
    if( gear.tier5_4pc ) tiers.t5_4pc_enhancement = 1;
    if( gear.tier6_2pc ) tiers.t6_2pc_enhancement = 1;
    if( gear.tier6_4pc ) tiers.t6_4pc_enhancement = 1;
    if( gear.tier7_2pc ) tiers.t7_2pc_enhancement = 1;
    if( gear.tier7_4pc ) tiers.t7_4pc_enhancement = 1;
    if( gear.tier8_2pc ) tiers.t8_2pc_enhancement = 1;
    if( gear.tier8_4pc ) tiers.t8_4pc_enhancement = 1;
  }
  else
  {
    if( gear.tier4_2pc ) tiers.t4_2pc_elemental = 1;
    if( gear.tier4_4pc ) tiers.t4_4pc_elemental = 1;
    if( gear.tier5_2pc ) tiers.t5_2pc_elemental = 1;
    if( gear.tier5_4pc ) tiers.t5_4pc_elemental = 1;
    if( gear.tier6_2pc ) tiers.t6_2pc_elemental = 1;
    if( gear.tier6_4pc ) tiers.t6_4pc_elemental = 1;
    if( gear.tier7_2pc ) tiers.t7_2pc_elemental = 1;
    if( gear.tier7_4pc ) tiers.t7_4pc_elemental = 1;
    if( gear.tier8_2pc ) tiers.t8_2pc_elemental = 1;
    if( gear.tier8_4pc ) tiers.t8_4pc_elemental = 1;
  }
}

// shaman_t::reset ===========================================================

void shaman_t::reset()
{
  player_t::reset();

  // Totems
  fire_totem  = 0;
  air_totem   = 0;
  water_totem = 0;
  earth_totem = 0;;

  _buffs.reset();
  _cooldowns.reset();
  _expirations.reset();
}

// shaman_t::composite_attack_power ==========================================

double shaman_t::composite_attack_power()
{
  double ap = player_t::composite_attack_power();

  if( talents.mental_dexterity )
  {
    ap += composite_attack_power_multiplier() * intellect() * talents.mental_dexterity / 3.0;
  }

  return ap;
}

// shaman_t::composite_spell_power ==========================================

double shaman_t::composite_spell_power( int school )
{
  double sp = player_t::composite_spell_power( school );

  if( talents.mental_quickness )
  {
    sp += composite_attack_power() * 0.30 * talents.mental_quickness / 3.0;
  }

  if( main_hand_weapon.buff == FLAMETONGUE )
  {
    sp += main_hand_weapon.buff_bonus;
  }
  if( off_hand_weapon.buff == FLAMETONGUE )
  {
    sp += off_hand_weapon.buff_bonus;
  }

  return sp;
}

// shaman_t::regen  =======================================================

void shaman_t::regen( double periodicity )
{
  player_t::regen( periodicity );

  if( _buffs.water_shield )
  {
    double water_shield_regen = periodicity * _buffs.water_shield / 5.0;

    resource_gain( RESOURCE_MANA, water_shield_regen, gains_water_shield );
  }
}

// shaman_t::get_talent_trees ==============================================

bool shaman_t::get_talent_trees( std::vector<int*>& elemental,
                                 std::vector<int*>& enhancement,
                                 std::vector<int*>& restoration )
{
  if( sim -> patch.after( 3, 1, 0 ) )
  {
    talent_translation_t translation[][3] =
    {
      { {  1, &( talents.convection            ) }, {  1, &( talents.enhancing_totems          ) }, {  1, NULL                                  } },
      { {  2, &( talents.concussion            ) }, {  2, NULL                                   }, {  2, &( talents.totemic_focus            ) } },
      { {  3, &( talents.call_of_flame         ) }, {  3, &( talents.ancestral_knowledge       ) }, {  3, NULL                                  } },
      { {  4, NULL                               }, {  4, NULL                                   }, {  4, NULL                                  } },
      { {  5, &( talents.elemental_devastation ) }, {  5, &( talents.thundering_strikes        ) }, {  5, NULL                                  } },
      { {  6, &( talents.reverberation         ) }, {  6, NULL                                   }, {  6, NULL                                  } },
      { {  7, &( talents.elemental_focus       ) }, {  7, &( talents.improved_shields          ) }, {  7, NULL                                  } },
      { {  8, &( talents.elemental_fury        ) }, {  8, &( talents.elemental_weapons         ) }, {  8, NULL                                  } },
      { {  9, NULL                               }, {  9, &( talents.shamanistic_focus         ) }, {  9, NULL                                  } },
      { { 10, NULL                               }, { 10, NULL                                   }, { 10, &( talents.restorative_totems       ) } },
      { { 11, NULL                               }, { 11, &( talents.flurry                    ) }, { 11, &( talents.tidal_mastery            ) } },
      { { 12, &( talents.call_of_thunder       ) }, { 12, NULL                                   }, { 12, NULL                                  } },
      { { 13, &( talents.unrelenting_storm     ) }, { 13, &( talents.improved_windfury_totem   ) }, { 13, &( talents.natures_swiftness        ) } },
      { { 14, &( talents.elemental_precision   ) }, { 14, &( talents.spirit_weapons            ) }, { 14, NULL                                  } },
      { { 15, &( talents.lightning_mastery     ) }, { 15, &( talents.mental_dexterity          ) }, { 15, NULL                                  } },
      { { 16, &( talents.elemental_mastery     ) }, { 16, &( talents.unleashed_rage            ) }, { 16, NULL                                  } },
      { { 17, &( talents.storm_earth_and_fire  ) }, { 17, &( talents.weapon_mastery            ) }, { 17, &( talents.mana_tide_totem          ) } },
      { { 18, &( talents.booming_echoes        ) }, { 18, &( talents.frozen_power              ) }, { 18, NULL                                  } },
      { { 19, &( talents.elemental_oath        ) }, { 19, &( talents.dual_wield_specialization ) }, { 19, &( talents.blessing_of_the_eternals ) } },
      { { 20, &( talents.lightning_overload    ) }, { 20, &( talents.dual_wield                ) }, { 20, NULL                                  } },
      { { 21, NULL                               }, { 21, &( talents.stormstrike               ) }, { 21, NULL                                  } },
      { { 22, &( talents.totem_of_wrath        ) }, { 22, &( talents.static_shock              ) }, { 22, NULL                                  } },
      { { 23, &( talents.lava_flows            ) }, { 23, &( talents.lava_lash                 ) }, { 23, NULL                                  } },
      { { 24, &( talents.shamanism             ) }, { 24, &( talents.improved_stormstrike      ) }, { 24, NULL                                  } },
      { { 25, &( talents.thunderstorm          ) }, { 25, &( talents.mental_quickness          ) }, { 25, NULL                                  } },
      { {  0, NULL                               }, { 26, &( talents.shamanistic_rage          ) }, { 26, NULL                                  } },
      { {  0, NULL                               }, { 27, NULL                                   }, {  0, NULL                                  } },
      { {  0, NULL                               }, { 28, &( talents.maelstrom_weapon          ) }, {  0, NULL                                  } },
      { {  0, NULL                               }, { 29, &( talents.feral_spirit              ) }, {  0, NULL                                  } },
      { {  0, NULL                               }, {  0, NULL                                   }, {  0, NULL                                  } }
    };
  
    return player_t::get_talent_trees( elemental, enhancement, restoration, translation );
  }
  else
  {
    talent_translation_t translation[][3] =
    {
      { {  1, &( talents.convection            ) }, {  1, &( talents.enhancing_totems          ) }, {  1, NULL                                  } },
      { {  2, &( talents.concussion            ) }, {  2, NULL                                   }, {  2, &( talents.totemic_focus            ) } },
      { {  3, &( talents.call_of_flame         ) }, {  3, &( talents.ancestral_knowledge       ) }, {  3, NULL                                  } },
      { {  4, NULL                               }, {  4, NULL                                   }, {  4, NULL                                  } },
      { {  5, &( talents.elemental_devastation ) }, {  5, &( talents.thundering_strikes        ) }, {  5, NULL                                  } },
      { {  6, &( talents.reverberation         ) }, {  6, NULL                                   }, {  6, NULL                                  } },
      { {  7, &( talents.elemental_focus       ) }, {  7, &( talents.improved_shields          ) }, {  7, NULL                                  } },
      { {  8, &( talents.elemental_fury        ) }, {  8, &( talents.elemental_weapons         ) }, {  8, NULL                                  } },
      { {  9, NULL                               }, {  9, &( talents.shamanistic_focus         ) }, {  9, NULL                                  } },
      { { 10, NULL                               }, { 10, NULL                                   }, { 10, &( talents.restorative_totems       ) } },
      { { 11, NULL                               }, { 11, &( talents.flurry                    ) }, { 11, &( talents.tidal_mastery            ) } },
      { { 12, &( talents.call_of_thunder       ) }, { 12, NULL                                   }, { 12, NULL                                  } },
      { { 13, &( talents.unrelenting_storm     ) }, { 13, &( talents.improved_windfury_totem   ) }, { 13, &( talents.natures_swiftness        ) } },
      { { 14, &( talents.elemental_precision   ) }, { 14, &( talents.spirit_weapons            ) }, { 14, NULL                                  } },
      { { 15, &( talents.lightning_mastery     ) }, { 15, &( talents.mental_dexterity          ) }, { 15, NULL                                  } },
      { { 16, &( talents.elemental_mastery     ) }, { 16, &( talents.unleashed_rage            ) }, { 16, NULL                                  } },
      { { 17, &( talents.storm_earth_and_fire  ) }, { 17, &( talents.weapon_mastery            ) }, { 17, &( talents.mana_tide_totem          ) } },
      { { 18, &( talents.elemental_oath        ) }, { 18, &( talents.dual_wield_specialization ) }, { 18, NULL                                  } },
      { { 19, &( talents.lightning_overload    ) }, { 19, &( talents.dual_wield                ) }, { 19, &( talents.blessing_of_the_eternals ) } },
      { { 20, NULL                               }, { 20, &( talents.stormstrike               ) }, { 20, NULL                                  } },
      { { 21, &( talents.totem_of_wrath        ) }, { 21, &( talents.static_shock              ) }, { 21, NULL                                  } },
      { { 22, &( talents.lava_flows            ) }, { 22, &( talents.lava_lash                 ) }, { 22, NULL                                  } },
      { { 23, &( talents.shamanism             ) }, { 23, &( talents.improved_stormstrike      ) }, { 23, NULL                                  } },
      { { 24, &( talents.thunderstorm          ) }, { 24, &( talents.mental_quickness          ) }, { 24, NULL                                  } },
      { {  0, NULL                               }, { 25, &( talents.shamanistic_rage          ) }, { 25, NULL                                  } },
      { {  0, NULL                               }, { 26, NULL                                   }, { 26, NULL                                  } },
      { {  0, NULL                               }, { 27, &( talents.maelstrom_weapon          ) }, {  0, NULL                                  } },
      { {  0, NULL                               }, { 28, &( talents.feral_spirit              ) }, {  0, NULL                                  } },
      { {  0, NULL                               }, {  0, NULL                                   }, {  0, NULL                                  } }
    };
  
    return player_t::get_talent_trees( elemental, enhancement, restoration, translation );
  }
}

// shaman_t::parse_talents_mmo =============================================

bool shaman_t::parse_talents_mmo( const std::string& talent_string )
{
  // shaman mmo encoding: Elemental-Restoration-Enhancement

  int size1 = 25;
  int size2 = 26;

  std::string   elemental_string( talent_string,     0,  size1 );
  std::string restoration_string( talent_string, size1,  size2 );
  std::string enhancement_string( talent_string, size1 + size2  );

  return parse_talents( elemental_string + enhancement_string + restoration_string );
}

// shaman_t::parse_option  ==============================================

bool shaman_t::parse_option( const std::string& name,
                             const std::string& value )
{
  option_t options[] =
  {
    { "ancestral_knowledge",       OPT_INT,  &( talents.ancestral_knowledge       ) },
    { "blessing_of_the_eternals",  OPT_INT,  &( talents.blessing_of_the_eternals  ) },
    { "booming_echoes",            OPT_INT,  &( talents.booming_echoes            ) },
    { "call_of_flame",             OPT_INT,  &( talents.call_of_flame             ) },
    { "call_of_thunder",           OPT_INT,  &( talents.call_of_thunder           ) },
    { "concussion",                OPT_INT,  &( talents.concussion                ) },
    { "convection",                OPT_INT,  &( talents.convection                ) },
    { "dual_wield",                OPT_INT,  &( talents.dual_wield                ) },
    { "dual_wield_specialization", OPT_INT,  &( talents.dual_wield_specialization ) },
    { "elemental_devastation",     OPT_INT,  &( talents.elemental_devastation     ) },
    { "elemental_focus",           OPT_INT,  &( talents.elemental_focus           ) },
    { "elemental_fury",            OPT_INT,  &( talents.elemental_fury            ) },
    { "elemental_mastery",         OPT_INT,  &( talents.elemental_mastery         ) },
    { "elemental_oath",            OPT_INT,  &( talents.elemental_oath            ) },
    { "elemental_precision",       OPT_INT,  &( talents.elemental_precision       ) },
    { "elemental_weapons",         OPT_INT,  &( talents.elemental_weapons         ) },
    { "enhancing_totems",          OPT_INT,  &( talents.enhancing_totems          ) },
    { "feral_spirit",              OPT_INT,  &( talents.feral_spirit              ) },
    { "flurry",                    OPT_INT,  &( talents.flurry                    ) },
    { "frozen_power",              OPT_INT,  &( talents.frozen_power              ) },
    { "improved_shields",          OPT_INT,  &( talents.improved_shields          ) },
    { "improved_stormstrike",      OPT_INT,  &( talents.improved_stormstrike      ) },
    { "improved_windfury_totem",   OPT_INT,  &( talents.improved_windfury_totem   ) },
    { "lava_flows",                OPT_INT,  &( talents.lava_flows                ) },
    { "lava_lash",                 OPT_INT,  &( talents.lava_lash                 ) },
    { "lightning_mastery",         OPT_INT,  &( talents.lightning_mastery         ) },
    { "lightning_overload",        OPT_INT,  &( talents.lightning_overload        ) },
    { "maelstrom_weapon",          OPT_INT,  &( talents.maelstrom_weapon          ) },
    { "mana_tide_totem",           OPT_INT,  &( talents.mana_tide_totem           ) },
    { "mental_dexterity",          OPT_INT,  &( talents.mental_dexterity          ) },
    { "mental_quickness",          OPT_INT,  &( talents.mental_quickness          ) },
    { "natures_swiftness",         OPT_INT,  &( talents.natures_swiftness         ) },
    { "restorative_totems",        OPT_INT,  &( talents.restorative_totems        ) },
    { "reverberation",             OPT_INT,  &( talents.reverberation             ) },
    { "shamanism",                 OPT_INT,  &( talents.shamanism                 ) },
    { "shamanistic_focus",         OPT_INT,  &( talents.shamanistic_focus         ) },
    { "shamanistic_rage",          OPT_INT,  &( talents.shamanistic_rage          ) },
    { "spirit_weapons",            OPT_INT,  &( talents.spirit_weapons            ) },
    { "static_shock",              OPT_INT,  &( talents.static_shock              ) },
    { "stormstrike",               OPT_INT,  &( talents.stormstrike               ) },
    { "storm_earth_and_fire",      OPT_INT,  &( talents.storm_earth_and_fire      ) },
    { "toughness",                 OPT_INT,  &( talents.toughness                 ) },
    { "thundering_strikes",        OPT_INT,  &( talents.thundering_strikes        ) },
    { "thunderstorm",              OPT_INT,  &( talents.thunderstorm              ) },
    { "tidal_mastery",             OPT_INT,  &( talents.tidal_mastery             ) },
    { "totem_of_wrath",            OPT_INT,  &( talents.totem_of_wrath            ) },
    { "totemic_focus",             OPT_INT,  &( talents.totemic_focus             ) },
    { "unrelenting_storm",         OPT_INT,  &( talents.unrelenting_storm         ) },
    { "unleashed_rage",            OPT_INT,  &( talents.unleashed_rage            ) },
    { "weapon_mastery",            OPT_INT,  &( talents.weapon_mastery            ) },
    // Glyphs
    { "glyph_elemental_mastery",   OPT_INT,  &( glyphs.elemental_mastery          ) },
    { "glyph_feral_spirit",        OPT_INT,  &( glyphs.feral_spirit               ) },
    { "glyph_flame_shock",         OPT_INT,  &( glyphs.flame_shock                ) },
    { "glyph_flametongue_weapon",  OPT_INT,  &( glyphs.flametongue_weapon         ) },
    { "glyph_lava",                OPT_INT,  &( glyphs.lava                       ) },
    { "glyph_lava_lash",           OPT_INT,  &( glyphs.lava_lash                  ) },
    { "glyph_lightning_bolt",      OPT_INT,  &( glyphs.lightning_bolt             ) },
    { "glyph_lightning_shield",    OPT_INT,  &( glyphs.lightning_shield           ) },
    { "glyph_mana_tide",           OPT_INT,  &( glyphs.mana_tide                  ) },
    { "glyph_shocking",            OPT_INT,  &( glyphs.shocking                   ) },
    { "glyph_stormstrike",         OPT_INT,  &( glyphs.stormstrike                ) },
    { "glyph_thunderstorm",        OPT_INT,  &( glyphs.thunderstorm               ) },
    { "glyph_totem_of_wrath",      OPT_INT,  &( glyphs.totem_of_wrath             ) },
    { "glyph_windfury_weapon",     OPT_INT,  &( glyphs.windfury_weapon            ) },
    // Totems
    { "totem_of_dueling",          OPT_INT,  &( totems.dueling                    ) },
    { "totem_of_hex",              OPT_INT,  &( totems.hex                        ) },
    { "totem_of_the_dancing_flame",OPT_INT,  &( totems.dancing_flame              ) },
    { "thunderfall_totem",         OPT_INT,  &( totems.thunderfall                ) },
    // Tier Bonuses
    { "tier4_2pc_elemental",       OPT_INT,  &( tiers.t4_2pc_elemental            ) },
    { "tier4_4pc_elemental",       OPT_INT,  &( tiers.t4_4pc_elemental            ) },
    { "tier5_2pc_elemental",       OPT_INT,  &( tiers.t5_2pc_elemental            ) },
    { "tier5_4pc_elemental",       OPT_INT,  &( tiers.t5_4pc_elemental            ) },
    { "tier6_2pc_elemental",       OPT_INT,  &( tiers.t6_2pc_elemental            ) },
    { "tier6_4pc_elemental",       OPT_INT,  &( tiers.t6_4pc_elemental            ) },
    { "tier7_2pc_elemental",       OPT_INT,  &( tiers.t7_2pc_elemental            ) },
    { "tier7_4pc_elemental",       OPT_INT,  &( tiers.t7_4pc_elemental            ) },
    { "tier4_2pc_enhancement",     OPT_INT,  &( tiers.t4_2pc_enhancement          ) },
    { "tier4_4pc_enhancement",     OPT_INT,  &( tiers.t4_4pc_enhancement          ) },
    { "tier5_2pc_enhancement",     OPT_INT,  &( tiers.t5_2pc_enhancement          ) },
    { "tier5_4pc_enhancement",     OPT_INT,  &( tiers.t5_4pc_enhancement          ) },
    { "tier6_2pc_enhancement",     OPT_INT,  &( tiers.t6_2pc_enhancement          ) },
    { "tier6_4pc_enhancement",     OPT_INT,  &( tiers.t6_4pc_enhancement          ) },
    { "tier7_2pc_enhancement",     OPT_INT,  &( tiers.t7_2pc_enhancement          ) },
    { "tier7_4pc_enhancement",     OPT_INT,  &( tiers.t7_4pc_enhancement          ) },
    { NULL, OPT_UNKNOWN }
  };

  if( name.empty() )
  {
    player_t::parse_option( std::string(), std::string() );
    option_t::print( sim, options );
    return false;
  }

  if( option_t::parse( sim, options, name, value ) ) return true;

  return player_t::parse_option( name, value );
}

// player_t::create_shaman  =================================================

player_t* player_t::create_shaman( sim_t*       sim, 
                                   std::string& name ) 
{
  shaman_t* p = new shaman_t( sim, name );

  new spirit_wolf_pet_t( sim, p );

 return p;
}
