PUBLIC setup_tilemap

START_OF_BANK_5:		EQU $4000
START_OF_TILEMAP:		EQU $4000	; Just after 
START_OF_TILES:			EQU $6000	; SEE tiles.asm

OFFSET_OF_MAP:			EQU (START_OF_TILEMAP - START_OF_BANK_5) >> 8
OFFSET_OF_TILES:		EQU (START_OF_TILES - START_OF_BANK_5) >> 8

setup_tilemap:
	; Enable tilemap mode
	NEXTREG $6B, %10000001		; 40x32, 16-bit entries
	NEXTREG $6C, %00000000		; palette offset, visuals

	; Tell hardware where to find tiles
	NEXTREG $6E, OFFSET_OF_MAP	; MSB of tilemap in bank 5
	NEXTREG $6F, OFFSET_OF_TILES	; MSB of tilemap definitions

	; Setup tilemap palette
	NEXTREG $43, %00110000		; Auto increment, select first tilemap palette
	NEXTREG $40, 0			; Start with first entry

	; Copy palette
	LD HL, palette			; Address of palette data in memory
	LD B,  0	; == 256	; Copy 16*16 colours
	CALL copyPalette		; Call routine for copying

	; Setup sprites palette
	NEXTREG $43, %00100000		; Auto increment, select first sprites palette
	NEXTREG $40, 0			; Start with first entry

	; Copy palette
	LD HL, palette			; Address of palette data in memory
	LD B,  0	; == 256	; Copy 16*16 colours
	CALL copyPalette		; Call routine for copying

	; NO NEED TO COPY TILEMAP, already loaded in place by TILES_ASM section
	; Copy tile definitions to expected memory
	; LD HL, tiles			; Address of tiles in memory
	; LD BC, tilesLength		; Number of bytes to copy
	; CALL copyTileDefinitions	; Copy all tiles data

	; Copy tilemap to expected memory
	; LD HL, tilemap			; Addreess of tilemap in memory
	; CALL copyTileMap40x32		; Copy 40x32 tilemaps

; 	; Give it some time
; 	CALL delay
; 	CALL delay
; 	CALL delay
; 	CALL delay

; 	; Then use offset registers to simulate shake.
; 	LD A, 1				; Offset by 1 pixel
; 	LD B, 40			; Number of repetitions
; .shakeLoop:
; 	NEXTREG $30, A			; Set current offset
; 	LD HL, 5000
; 	CALL customDelay
; 	XOR 1				; Change offset: if 0 to 1, then back to 0
; 	DJNZ .shakeLoop

    RET
	
;;--------------------------------------------------------------------
;; subroutines
;;--------------------------------------------------------------------

;---------------------------------------------------------------------
; HL = memory location of the palette
copyPalette256:
	LD B, 0			; This variant always starts with 0
;---------------------------------------------------------------------
; HL = memory location of the palette
; B = number of colours to copy
copyPalette:
	LD A, (HL)			; Load RRRGGGBB into A
	INC HL				; Increment to next entry
	NEXTREG $44, A		; Send entry to Next HW
	LD A, (HL)			; Load 0000000B into A
	INC HL				; Increment to next entry
	NEXTREG $44, A		; Send entry to Next HW
	DJNZ copyPalette	; Repeat until B=0
	RET

;---------------------------------------------------------------------
; HL = memory location of tile definitons
; BC = size of tile defitions in bytes.
copyTileDefinitions:
	LD DE, START_OF_TILES
	LDIR
	RET

;---------------------------------------------------------------------
; HL = memory location of tilemap
; copyTileMap40x32:
; 	LD BC, 40*32*2		; This variant always load 40x32
; 	JR copyTileMap
; copyTileMap80x32:
; 	LD BC, 80*32*2		; This variant always loads 80x32
;---------------------------------------------------------------------
; HL = memory location of tilemap
; BC = size of tilemap in bytes
copyTileMap:
	LD DE, START_OF_TILEMAP
	LDIR
	RET

;;--------------------------------------------------------------------
;; data
;;--------------------------------------------------------------------

; Note: all files created with https://zx.remysharp.com/sprites/#sprite-editor
; See individual notes besides each entry below:

; Tilemap settings: 8px, 40x32, disable "include header" when downloading, file is then usabe as is.
; tilemap:
; 	INCBIN "tiles.map"
; tilemapLength: EQU $ - tilemap

; NOT LOADED HERE
; Sprite Editor settings: 4bit, after downloading manually removed empty data (with HxD) to only leave first 192 bytes.
; tiles:
; 	INCBIN "tiles.spr"
; tilesLength: EQU $ - tiles

; After setting up palette, used Download button and then manually removed every second byte (with HxD) and only left 16 entries (so 16 bytes)
palette:
	INCBIN "trainyardR3B3G3.pal"
paletteLength: EQU $-palette



