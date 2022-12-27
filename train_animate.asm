EXTERN setup_2x1_sprite
EXTERN disable_sprite

_train_animate_wn:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    LD (_param2b), A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    LD (_param3b), A

    CALL setup_2x1_sprite
    RET


_train_animate_es:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param2b)
    ADD A, 48
    SUB A, B
    LD (_param2b), A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param3b)
    ADD A, 48
    SUB A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_XY
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_ne:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param2b)
    ADD A, 48
    SUB A, B
    LD (_param2b), A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_R
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_sw:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    LD (_param2b), A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param3b)
    ADD A, 48
    SUB A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_XYR
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_ws:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    LD (_param2b), A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param3b)
    ADD A, 48
    SUB A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_Y
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_en:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param2b)
    ADD A, 48
    SUB A, B
    LD (_param2b), A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_X
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_se:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param2b)
    ADD A, 48
    SUB A, B
    LD (_param2b), A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param3b)
    ADD A, 48
    SUB A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_YR
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_nw:
    CALL _loadSpriteDefForFrame
    CALL _loadSpriteData

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    LD (_param2b), A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_XR
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_ns:
    CALL _loadSpriteDefForFrame0
    CALL _loadSpriteData

    LD A, (_train_frame)
    LD D, A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    LD (_param2b), A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    ADD A, D
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_XR
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_sn:
    CALL _loadSpriteDefForFrame0
    CALL _loadSpriteData

    LD A, (_train_frame)
    LD D, A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    LD (_param2b), A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param3b)
    ADD A, 16
    ADD A, B
    SUB A, D
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_XYR
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


_train_animate_we:
    CALL _loadSpriteDefForFrame0
    CALL _loadSpriteData

    LD A, (_train_frame)
    LD D, A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param2b)
    SUB A, 32
    ADD A, B
    ADD A, D
    LD (_param2b), A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    LD (_param3b), A

    CALL setup_2x1_sprite
    RET


_train_animate_ew:
    CALL _loadSpriteDefForFrame0
    CALL _loadSpriteData

    LD A, (_train_frame)
    LD D, A

    LD A, (_spr_dx)
    LD B, A
    LD A, (_param2b)
    ADD A, 16
    ADD A, B
    SUB A, D
    LD (_param2b), A

    LD A, (_spr_dy)
    LD B, A
    LD A, (_param3b)
    SUB A, 32
    ADD A, B
    LD (_param3b), A

    LD A, (_param4b)
    OR A, TSF_X
    LD (_param4b), A

    CALL setup_2x1_sprite
    RET


