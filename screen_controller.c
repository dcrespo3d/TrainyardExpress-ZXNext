#include "screen_controller.h"
#include "screen_levels.h"
#include "player_data.h"
#include "print_tile.h"
#include "sound.h"

FunPtr sc_curr_entry  = NULL;
FunPtr sc_curr_update = NULL;
FunPtr sc_curr_exit   = NULL;

FunPtr sc_next_entry = NULL;
FunPtr sc_next_update = NULL;
FunPtr sc_next_exit = NULL;

ubyte sc_delay_frames = 0;

void sc_switch_delay(ubyte nframes)
{
    sc_delay_frames = nframes;
}

void sc_switch_screen(FunPtr entry, FunPtr update, FunPtr exit)
{
    sc_next_entry  = entry;
    sc_next_update = update;
    sc_next_exit   = exit;
}

void sc_update()
{
    // perform update for current screen
    if (sc_curr_update) sc_curr_update();

    if (sc_delay_frames) {
        sc_delay_frames--;
        return;
    }

    if (sc_next_update) {
        // exit from previous screen, if any
        if (sc_curr_exit) sc_curr_exit();

        // assign function pointers
        sc_curr_entry  = sc_next_entry;
        sc_curr_update = sc_next_update;
        sc_curr_exit   = sc_next_exit;
        
        sc_next_entry  = NULL;
        sc_next_update = NULL;
        sc_next_exit   = NULL;

        // stop all sound before entering any screen
        snd_panic();

        // enter next (current) screen, if any
        if (sc_curr_entry) sc_curr_entry();
    }
}

#include "level.h"
#include "screen_main.h"
#include "screen_game.h"

void sc_switch_level(ubyte index)
{
    ubyte* levrec = levarr[index];
    ubyte levstars = *levrec;

    if (levstars)
        sc_switch_screen(sg_enter, sg_update, NULL);
    else
        sc_switch_screen(stut_enter, stut_update, NULL);

}

ubyte sc_is_level_unlocked(ubyte level_idx)
{
    ubyte* levrec = levarr[level_idx];
    ubyte providx = levrec[1];

    ubyte* provrec = provarr[providx];
    uword ustars = *((uword*)provrec);

    // print_set_pos(1, 20);
    // print_str("lev  ");
    // print_dec_byte(level_idx);
    // print_char('\n');
    // print_str("prov ");
    // print_dec_byte(providx);
    // print_char('\n');
    // print_str("provrec0 ");
    // print_dec_byte(provrec[0]);
    // print_char('\n');
    // print_str("provrec1 ");
    // print_dec_byte(provrec[1]);
    // print_char('\n');
    // print_str("ustr ");
    // print_dec_word(ustars);
    // print_char('\n');
    // print_str("ustr ");
    // print_hex_word(ustars);
    // print_char('\n');
    // print_str("scor ");
    // print_dec_word(pd_score);
    // print_char('\n');

    if (ustars <= pd_score)
        return 1;

    return 0;
}

// ubyte sc_find_previous_unsolved_level()
// {
//     for (ubyte i = sg_level_idx; i != 0; i--) {
//         if (!pd_levels_solved[i])
//             return i;
//     }
//     return 0;
// }

void sc_switch_next_level()
{
    // check next level valid
    if (sc_is_level_unlocked(sg_level_idx + 1)) {
        sg_level_idx++;
        sc_switch_level(sg_level_idx);
        return;
    }

    sc_switch_screen(sl_enter, sl_update, NULL);

    // ubyte next_level = sc_find_previous_unsolved_level();
    // if (next_level) {
    //     sg_level_idx = next_level;
    //     sc_switch_level(sg_level_idx);
    //     return;
    // }

    // sc_switch_screen(sm_enter, sm_update, NULL);
}

