PUBLIC load_static_sprites
PUBLIC setup_sprites
PUBLIC clear_sprites
PUBLIC init_cursor_sprite
PUBLIC init_2x2_quad_sprite
PUBLIC update_2x2_quad_sprite_palettes
PUBLIC update_2x2_quad_sprite_hide
PUBLIC update_2x2_quad_sprite_show
PUBLIC update_2x2_quad_sprite_pos
PUBLIC update_cursor_sprite
PUBLIC update_cursor_sprite_ex
PUBLIC setup_scale2x_sprite
PUBLIC setup_2x2_sprite
PUBLIC setup_2x1_sprite
PUBLIC _setup_2x1_sprite_ex
PUBLIC update_2x2_sprite_palette
PUBLIC disable_sprite
PUBLIC init_src9snk_sprite
PUBLIC update_src9snk_sprite_color


EXTERN sprite_definitions
EXTERN sprite_definitions_length

EXTERN _param0w

EXTERN _param0b
EXTERN _param1b
EXTERN _param2b
EXTERN _param3b
EXTERN _param4b
EXTERN _param5b
EXTERN _param6b
EXTERN _param7b

load_static_sprites:
	; load bank/page for train sprite data
 	NEXTREG $50, 32
	NEXTREG $51, 33

	; Load sprite data using DMA
	LD HL, sprite_definitions			; Sprites data source
	                        			; SEE sprites.asm!
	LD BC, sprite_definitions_length	; Copy 5 sprites, each 16x16 pixels
	LD A, 0								; Start with first sprite
	CALL loadSprites					; Load sprites to FPGA

setup_sprites:
	; Setup sprite hardware
  	NEXTREG $15, %01000011		; sprite 0 on top, SLU, over border, sprites visible
	; NEXTREG $15, %00000011	 ; sprite 0 on bottom, SLU, over border, sprites visible

	; Setup sprite transparency index
	NEXTREG $4B, %00001111

	; sprite palette in tilemap

	; ; Show single sprite 0 using pattern 0
	; NEXTREG $34, 0			; First sprite
	; NEXTREG $35, 72		; X=100
	; NEXTREG $36, 80			; Y=80
	; NEXTREG $37, %00000000		; Palette offset, no mirror, no rotation
	; NEXTREG $38, %10000000		; Visible, no byte 4, pattern 0

	; ; Show single sprite 1 using pattern 0
	; NEXTREG $34, 1			; Second sprite
	; NEXTREG $35, 80			; X=84
	; NEXTREG $36, 80			; Y=80
	; NEXTREG $37, %00000000		; Palette offset, no mirror, no rotation
	; NEXTREG $38, %10000000		; Visible, no byte 4, pattern 0

	; Show combined sprite 1-4 using patterns 1-4
	; NEXTREG $34, 0			; Select third sprite
	; NEXTREG $35, 200		; X
	; NEXTREG $36, 80			; Y
	; NEXTREG $37, %00000000		; Palette offset, no mirror, no rotation
	; NEXTREG $38, %11000000		; Visible, use byte 4, pattern 1
	; ;               ^^^^^^ PATTERN
	; NEXTREG $39, %10100000		; Anchor with unified relatives, no scaling
	; ;              ^       PATTERN LSB (doc says MSB)

	; NEXTREG $34, 1
	; NEXTREG $35, 16			; X=AnchorX+16
	; NEXTREG $36, 0			; Y=AnchorY+0
	; NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation
	; NEXTREG $38, %11000000		; Visible, use byte 4, pattern 2
	; ;               ^^^^^^ PATTERN
	; NEXTREG $39, %01100000		; Relative sprite
	; ;               ^      PATTERN LSB (doc says MSB)

	; NEXTREG $34, 2
	; NEXTREG $35, 0			; X=AnchorX+0
	; NEXTREG $36, 16			; Y=AnchorY+16
	; NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation
	; NEXTREG $38, %11000001		; Visible, use byte 4, pattern 3
	; ;               ^^^^^^ PATTERN
	; NEXTREG $39, %01000000		; Relative sprite
	; ;               ^      PATTERN LSB (doc says MSB)

	; NEXTREG $34, 3
	; NEXTREG $35, 16			; X=AnchorX+16
	; NEXTREG $36, 16			; Y=AnchorY+16
	; NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation
	; NEXTREG $38, %11000001		; Visible, use byte 4, pattern 4
	; ;               ^^^^^^ PATTERN
	; NEXTREG $39, %01100000		; Relative sprite
	; ;               ^      PATTERN LSB (doc says MSB)

    RET

	; Wait for a while
	call delay

	; Update our relative sprite:
	; - change position
	; - rotate
	; - scale X 2x
	NEXTREG $34, 2			; Select third sprite
	NEXTREG $35, 200		; X=200
	NEXTREG $36, 100		; Y=100
	NEXTREG $37, %00000010		; Palette offset, no mirror, rotate
	NEXTREG $38, %11000001		; Visible, use byte 4, pattern 1
	NEXTREG $39, %00101000		; Anchor with unified relatives, scale X 

	call delay

	; Update our relative sprite again:
	; - change position
	; - mirror X
	; - scale X&Y 2x
	NEXTREG $34, 2			; Select third sprite
	NEXTREG $35, 220		; X=220
	NEXTREG $36, 120		; Y=120
	NEXTREG $37, %00001010		; Palette offset, mirror X, rotate
	NEXTREG $38, %11000001		; Visible, use byte 4, pattern 1
	NEXTREG $39, %00101010		; Anchor with unified relatives, scale X&Y 

    RET

;_param0b: sprite number (using N, N+1, N+2, N+3)
;_param1b: pattern number (using M, M+1, M+2, M+3)
;_param2b: X position
;_param3b: Y position
setup_2x2_sprite:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite 1/4
	INC B

	LD A, (_param2b)
	NEXTREG $35, A		    ; X

	LD A, (_param3b)
	NEXTREG $36, A			; Y

	NEXTREG $37, %00000000		; Palette offset, no mirror, no rotation

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10100000  
 	NEXTREG $39, A		; Anchor with unified relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)
	INC C

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 2/4
	INC B

	NEXTREG $35, 16			; X=AnchorX+16
	NEXTREG $36, 0			; Y=AnchorY+0
	NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation

	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN


	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)
	INC C

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	NEXTREG $35, 0			; X=AnchorX+16
	NEXTREG $36, 16			; Y=AnchorY+0
	NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation

	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN


	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)
	INC C

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	NEXTREG $35, 16			; X=AnchorX+16
	NEXTREG $36, 16			; Y=AnchorY+0
	NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation

	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN


	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)
	INC C


	POP BC
	RET


;_param0b: sprite number (using N, N+1, N+2, N+3)
;_param4b: palette for sprite in top    left  position
update_2x2_sprite_palette:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite 1/4

	LD A, (_param4b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset, no mirror, no rotation
	NEXTREG $37, A		

	RET
	

;_param0b: sprite number (using N, N+1)
;_param1b: pattern number (using M, M+1)
;_param2b: X position
;_param3b: Y position
;_param4b: palette/mirror/rotation
;_param5b: second subsprite X offset
;_param6b: second subsprite Y offset
setup_2x1_sprite:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite 1/2
	INC B

	LD A, (_param2b)
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y
	LD A, (_param4b)
	NEXTREG $37, A			; Palette offset, mirror, rotation

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10100000  
 	NEXTREG $39, A		; Anchor with unified relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)
	INC C

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 2/2
	INC B

	LD A, (_param5b)
	NEXTREG $35, A			; X=AnchorX+dx
	LD A, (_param6b)
	NEXTREG $36, A			; Y=AnchorY+dy
	NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation

	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN


	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)
	INC C

	POP BC
	RET


;_param0b: sprite number (using N, N+1)
;_param1b: pattern number (using M, M+1)
;_param2w: X position
;_param3b: Y position
;_param4b: palette/mirror/rotation
;_param5b: second subsprite X offset
;_param6b: second subsprite Y offset
_setup_2x1_sprite_ex:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite 1/2
	INC B

	LD  HL, (_param2w)
	LD  A, H
	AND A, %00000001
	LD  H, A
	LD  A, L
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y
	LD A, (_param4b)
	OR A, H
	NEXTREG $37, A			; Palette offset, mirror, rotation, X high bit

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10100000  
 	NEXTREG $39, A		; Anchor with unified relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)
	INC C

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 2/2
	INC B

	LD A, (_param5b)
	NEXTREG $35, A			; X=AnchorX+dx
	LD A, (_param6b)
	NEXTREG $36, A			; Y=AnchorY+dy
	NEXTREG $37, %00000001		; Palette offset, no mirror, no rotation

	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN


	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)
	INC C

	POP BC
	RET


;_param0b: sprite number (using N)
;_param1b: pattern number (using M)
;_param2b: X position
;_param3b: Y position
setup_scale2x_sprite:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite
	INC B

	LD A, (_param2b)
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y
	LD A,        %00000000
	NEXTREG $37, A			; Palette offset, mirror, rotation

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10001010  
 	NEXTREG $39, A		; Anchor with composite relatives, 2x scaling in X and Y
	;              ^       PATTERN LSB (doc says MSB)
	INC C

	POP BC
	RET


;_param0b: sprite number (using N, N+1, N+2, N+3)
;_param1b: pattern number (using M)
;_param2b: X position
;_param3b: Y position
;_param4b: palette for sprite in top    left  position
;_param5b: palette for sprite in top    right position
;_param6b: palette for sprite in bottom left  position
;_param7b: palette for sprite in bottom right position
setup_2x2_quad_sprite:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite 1/4
	INC B

	LD A, (_param2b)
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y
	LD A, (_param4b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset, no mirror, no rotation
	NEXTREG $37, A		

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10000000  
 	NEXTREG $39, A		; Anchor with composite relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 2/4
	INC B

	NEXTREG $35, 16			; X=AnchorX + 16
	NEXTREG $36, 0			; Y=AnchorY + 0
	LD A, (_param5b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset
	OR  A,       %00001000  ; mirrorX, no rotation
	NEXTREG $37, A		    
	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN

	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	NEXTREG $35, 0			; X=AnchorX + 0
	NEXTREG $36, 16			; Y=AnchorY + 16
	LD A, (_param6b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset
	OR  A,       %00000100  ; mirrorY, no rotation
	NEXTREG $37, A		    
	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN

	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	NEXTREG $35, 16			; X=AnchorX + 16
	NEXTREG $36, 16			; Y=AnchorY + 16
	LD A, (_param7b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset
	OR  A,       %00001100  ; mirrorXY, no rotation
	NEXTREG $37, A		    
	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN

	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)

	POP BC
	RET










;_param0b: sprite number (using N)
;_param1b: pattern number (using M)
;_param2b: X position
;_param3b: Y position
init_1x1_sprite:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite 1
	INC B

	LD A, (_param2b)
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y

	LD A, (_param3b)
	NEXTREG $37, 0			

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10000000  
 	NEXTREG $39, A		; Anchor with composite relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)

	POP BC

	RET





;_param0b: sprite number (using N)
;_param1b: pattern number (using M)
init_cursor_sprite:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD A, (_param1b)
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, (_param1b)
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10000000  
 	NEXTREG $39, A		; Anchor with composite relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)

	RET


;_param0b: sprite number (using N)
;_param0w: x (0-511)
;_param1b: y (0-255)
update_cursor_sprite:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD HL, (_param0w)
	LD A, L
	NEXTREG $35, A			; X low byte

	LD A, (_param1b)
	NEXTREG $36, A			; Y

	LD A, H
	AND A, %00000001		; X high bit
	NEXTREG $37, A

	RET


;_param0b: sprite number (using N)
;_param0w: x (0-511)
;_param1b: y (0-255)
;_param2b: attribute
update_cursor_sprite_ex:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD HL, (_param0w)
	LD  A, H
	AND A, %00000001
	LD  H, A
	LD A, L
	NEXTREG $35, A			; X low byte

	LD A, (_param1b)
	NEXTREG $36, A			; Y

	LD A, (_param2b)
	AND A, %11111110		
	OR A, H
	NEXTREG $37, A			; palette, mirror, rotate, X high bit

	RET


;_param0b: sprite number (using N)
;_param1b: pattern number (using M)
hide_cursor_sprite:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD A, (_param1b)
	RRCA
	AND A,       %00111111
	OR  A,       %01000000
	NEXTREG $38, A            ; %01000000		; NOT isible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^
	RET


show_cursor_sprite:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD A, (_param1b)
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^
	RET










;_param0b: sprite number (using N, N+1, N+2, N+3)
;_param1b: pattern number (using M)
;_param2b: X position
;_param3b: Y position
init_2x2_quad_sprite:
	PUSH BC

	LD A, (_param0b)
	LD B, A
	LD A, (_param1b)
	LD C, A

	LD A, B
	NEXTREG $34, A			; sprite 1/4
	INC B

	LD A, (_param2b)
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y

	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, C
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10000000  
 	NEXTREG $39, A		; Anchor with composite relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 2/4
	INC B

	NEXTREG $35, 16			; X=AnchorX + 16
	NEXTREG $36, 0			; Y=AnchorY + 0
	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN

	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	NEXTREG $35, 0			; X=AnchorX + 0
	NEXTREG $36, 16			; Y=AnchorY + 16
	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN

	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	NEXTREG $35, 16			; X=AnchorX + 16
	NEXTREG $36, 16			; Y=AnchorY + 16
	
	LD A, C
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A		; Visible, use byte 4, pattern 2
	;               ^^^^^^ PATTERN

	LD A, C
	RRCA
	RRCA
	RRCA
	AND A,       %00100000
	OR  A,       %01000000  
	NEXTREG $39, A  		; Relative sprite
	;               ^      PATTERN LSB (doc says MSB)

	POP BC
	RET



















;_param0b: sprite number (using N, N+1, N+2, N+3)
;_param4b: palette for sprite in top    left  position
;_param5b: palette for sprite in top    right position
;_param6b: palette for sprite in bottom left  position
;_param7b: palette for sprite in bottom right position
update_2x2_quad_sprite_palettes:
	PUSH BC

	LD A, (_param0b)
	LD B, A

	LD A, B
	NEXTREG $34, A			; sprite 1/4
	INC B

	LD A, (_param4b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset, no mirror, no rotation
	NEXTREG $37, A		

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 2/4
	INC B

	LD A, (_param5b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset
	OR  A,       %00001000  ; mirrorX, no rotation
	NEXTREG $37, A		    
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	LD A, (_param6b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset
	OR  A,       %00000100  ; mirrorY, no rotation
	NEXTREG $37, A		    
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	LD A, B
	NEXTREG $34, A			; sprite 3/4
	INC B

	LD A, (_param7b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset
	OR  A,       %00001100  ; mirrorXY, no rotation
	NEXTREG $37, A		    
	
	POP BC
	RET





;_param0b: sprite number
;_param1b: pattern number
update_2x2_quad_sprite_hide:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite 1/4

	LD A, (_param1b)
	RRCA
	AND A,       %00111111
	OR  A,       %01000000
	NEXTREG $38, A            ; %11000000		; Not visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^
	RET

;_param0b: sprite number
;_param1b: pattern number
update_2x2_quad_sprite_show:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite 1/4

	LD A, (_param1b)
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^
	RET

;_param0b: sprite number
;_param2b: X position
;_param3b: Y position
update_2x2_quad_sprite_pos:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite

	LD A, (_param2b)
	NEXTREG $35, A		    ; X
	LD A, (_param3b)
	NEXTREG $36, A			; Y

	RET



; param0b: first sprite to clear
clear_sprites:
	PUSH BC

	LD A, (_param0b)
	LD C, A
clear_sprites_loop:
	LD A, C
	NEXTREG $34, A
	NEXTREG $38, 0

	INC C
	CP  A, 127
	JR  NZ, clear_sprites_loop

	POP BC
	RET

update_sprite3:
    LD A, (_param0b)
	NEXTREG $34, A
    LD A, (_param1b)
	NEXTREG $35, A
    LD A, (_param2b)
	NEXTREG $36, A

    RET

update_sprite4:
    LD A, (_param0b)
	NEXTREG $34, A
    LD A, (_param1b)
	NEXTREG $35, A
    LD A, (_param2b)
	NEXTREG $36, A
    LD A, (_param3b)
	NEXTREG $37, A

    RET

update_sprite5:
    LD A, (_param0b)
	NEXTREG $34, A
    LD A, (_param1b)
	NEXTREG $35, A
    LD A, (_param2b)
	NEXTREG $36, A
    LD A, (_param3b)
	NEXTREG $37, A
    LD A, (_param4b)
	NEXTREG $38, A

    RET

update_sprite6:
    LD A, (_param0b)
	NEXTREG $34, A
    LD A, (_param1b)
	NEXTREG $35, A
    LD A, (_param2b)
	NEXTREG $36, A
    LD A, (_param3b)
	NEXTREG $37, A
    LD A, (_param4b)
	NEXTREG $38, A
    LD A, (_param5b)
	NEXTREG $39, A

    RET

disable_sprite:
	LD A, (_param0b)
	NEXTREG $34, A
	NEXTREG $38, 0

	RET

;_param0b: sprite number (using N)
;_param1b: pattern number (using M)
;_param2b: x
;_param3b: y
init_src9snk_sprite:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD A, (_param2b)
	NEXTREG $35, A			; X

	LD A, (_param3b)
	NEXTREG $36, A			; Y

	LD A, 0
	NEXTREG $37, A

	LD A, (_param1b)
	RRCA
	AND A,       %00111111
	OR  A,       %11000000
	NEXTREG $38, A            ; %11000000		; Visible, use Byte 4, pattern
	;               ^^^^^^ PATTERN ^^^^^^

	LD A, (_param1b)
	RRCA
	RRCA
	AND A,       %01000000
	OR  A,       %10000000  
 	NEXTREG $39, A		; Anchor with composite relatives, no scaling
	;              ^       PATTERN LSB (doc says MSB)

	RET


;_param0b: sprite number (using N)
;_param4b: palette index
update_src9snk_sprite_color:
	LD A, (_param0b)
	NEXTREG $34, A			; sprite N

	LD A, (_param4b)
	SWAPNIB
	AND A,       %11110000  ; Palette offset, no mirror, no rotation
	NEXTREG $37, A		


	RET








delay:
	LD B, 5
delay_outer:
	LD HL, $FFFF
delay_inner:
	DEC HL
	LD A, H
	OR L
	JR NZ, delay_inner
	DJNZ delay_outer
	RET

;---------------------------------------------------------------------
; HL = address of sprite sheet in memory
; BC = number of bytes to load
; A  = index of first sprite to load int5o
loadSprites:
	LD (dmaSource), HL	; Copy sprite sheet address from HL
	LD (dmaLength), BC	; Copy length in bytes from BC
	LD BC, $303B		; Prepare port for sprite index
	OUT (C), A		; Load index of first sprite
	LD HL, dmaProgram	; Setup source for OTIR
	LD B, dmaProgramLength	; Setup length for OTIR
	LD C, $6B		; Setup DMA port
	OTIR			; Invoke DMA code
	RET
dmaProgram:
	DB %10000011		; WR6 - Disable DMA
	DB %01111101		; WR0 - append length + port A address, A->B
dmaSource:
	DW 0			; WR0 par 1&2 - port A start address
dmaLength:
	DW 0			; WR0 par 3&4 - transfer length
	DB %00010100		; WR1 - A incr, A=memory
	DB %00101000		; WR2 - B fixed, B=I/O
	DB %10101101		; WR4 - continuous, append port B address
	DW $005B		; WR4 par 1&2 - port B address
	DB %10000010		; WR5 - stop on end of block, CE only
	DB %11001111		; WR6 - load addresses into DMA counters
	DB %10000111		; WR6 - enable DMA
dmaProgramLength = $-dmaProgram

;;--------------------------------------------------------------------
;; data
;;--------------------------------------------------------------------

; NOT NEEDED, loaded from sprites.asm
; sprites:
;	INCBIN "train_sprites.spr"

