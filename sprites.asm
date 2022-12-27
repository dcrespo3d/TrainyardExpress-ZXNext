SECTION PAGE_32_TRAIN_SPRITE_DATA

PUBLIC sprite_definitions
PUBLIC sprite_definitions_length

ORG $0000
sprite_definitions:
    INCBIN "train_sprites.spr"

sprite_definitions_length: EQU $ - sprite_definitions
