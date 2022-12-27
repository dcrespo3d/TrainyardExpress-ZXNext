SECTION TILES_ASM

PUBLIC tile_definitions

ORG $6000
tile_definitions:
    INCBIN "tiles.spr"
    INCBIN "font.spr"

