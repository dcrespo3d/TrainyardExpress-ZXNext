#ifndef __gui_h__
#define __gui_h__

#include "types.h"

void gui_clear_screen();
void gui_clear_side_panel();

void gui_display_train_frame();
void gui_display_train_steps();

struct _GuiButton
{
    ubyte x;
    ubyte y;
    ubyte w;
    ubyte h;
};
typedef struct _GuiButton GuiButton;

#define GUIBUTTON_INIT(gb, xx, yy, ww, hh) { \
    gb.x=xx; gb.y=yy; gb.w=ww; gb.h=hh; }

void guibutton_draw(GuiButton* gb);
void guibutton_clear(GuiButton* gb);
ubyte guibutton_mouseinto(GuiButton* gb);
void guibutton_draw_fine(GuiButton* gb);

// abbreviation
#define GBMI guibutton_mouseinto


#endif
