#ifndef __level_h__
#define __level_h__

#include "types.h"

#define MAX_LEV_SOURCES 8
#define MAX_LEV_SINKS 8
#define MAX_LEV_PAINTS 8
#define MAX_LEV_OBSTS 32
#define MAX_LEV_COLORS 64

#define MAX_LEV_NAME_LEN 20

#define LEV_COUNT 75
#define LEV_MAX 74

#define PROV_COUNT 13

// keep size to 8, otherwise some asm routines could be affected
typedef struct 
{
    ubyte dir;
    ubyte i;
    ubyte j;
    ubyte colidx;
    ubyte colcnt;
    ubyte spridx;
    ubyte chkidx;
    ubyte rsvd;     // padding for keep size to 8
}
SrcSnk;

typedef struct 
{
    ubyte i;
    ubyte j;
}
Obst;


extern Obst   lev_obsts  [MAX_LEV_OBSTS];
extern SrcSnk lev_sources[MAX_LEV_SOURCES];
extern SrcSnk lev_sinks  [MAX_LEV_SINKS];
extern SrcSnk lev_paints [MAX_LEV_PAINTS];
extern ubyte  lev_colors [MAX_LEV_COLORS];
extern ubyte  lev_num_sources;
extern ubyte  lev_num_sinks;
extern ubyte  lev_num_obsts;
extern ubyte  lev_num_colors;

// pitfall: if I use 
// extern ubyte** provarr;
// then provarr contains whatever is pointed at _provarr
// if I want the address of _provarr,
// i need to use
// extern ubyte* provarr[];

extern ubyte* levarr[LEV_COUNT];
extern ubyte* provarr[PROV_COUNT];

extern ubyte  lev_prov_idx;
extern ubyte  lev_stars;
extern ubyte  lev_name   [MAX_LEV_NAME_LEN];

extern ubyte  train_spr_idx;

// void level_pagein_levels();

void level_load(ubyte secnum, ubyte levnum);

void level_reset();

void level_start_trains();
void level_stop_trains();

void level_update();

ubyte level_sink_matches_color(SrcSnk* snk, ubyte color);
void level_sink_remove_color(SrcSnk* snk, ubyte color);
void level_sink_check_all_done(SrcSnk* snk);

#define LEV_STATE_IDLE 0x80

#define LEV_STATE_DESIGN 0
#define LEV_STATE_RUNNING 1
#define LEV_STATE_CRASHED 3
#define LEV_STATE_FINISHED 4

// accessor for setting state
void level_set_state(ubyte state);

// only read, use accessor for write
extern ubyte level_state;

//   -5  -4  -3  -2  -1   0   1   2   3   4   5
//  /32 /16  /8  /4  /2  x1  x2  x4  x8 x16 x32
// only read, use accessor for write
extern sbyte level_speed;

// accessor for setting speed
void level_set_speed(sbyte speed);

// private

void level_init();
void level_parse_levdef();
void level_parse_srcsnk(SrcSnk* srcsnk, ubyte dir);
void level_parse_colors(ubyte col0, ubyte ncols);
void level_parse_obst();
void level_populate_fixed();
void level_populate_fixed_sprites();

extern ubyte* level_parse_ptr;
void level_parse_rawdef();

#endif // __level_h__