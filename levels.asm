; index:  0  0
; prov: "British Columbia"
; name: "Welcome!"
; stars: 0
_level00:
    DEFB 0    ; stars
    DEFB 0    ; index of province
    DEFB "Welcome!", 0
    DEFB $00

; index:  1  1
; prov: "British Columbia"
; name: "Green Line"
; stars: 1
_level01:
    DEFB 1    ; stars
    DEFB 0    ; index of province
    DEFB "Green Line", 0
    DEFB $F8, $13, $15
    DEFB $E2, $53, $15
    DEFB $00

; index:  2  2
; prov: "British Columbia"
; name: "The Three"
; stars: 1
_level02:
    DEFB 1    ; stars
    DEFB 0    ; index of province
    DEFB "The Three", 0
    DEFB $F4, $30, $14
    DEFB $F4, $21, $12
    DEFB $F4, $41, $11
    DEFB $E1, $25, $12
    DEFB $E1, $45, $11
    DEFB $E1, $36, $14
    DEFB $00

; index:  3  3
; prov: "British Columbia"
; name: "Pure Magic"
; stars: 1
_level03:
    DEFB 1    ; stars
    DEFB 0    ; index of province
    DEFB "Pure Magic", 0
    DEFB $F4, $30, $15
    DEFB $F8, $03, $15
    DEFB $E2, $63, $15
    DEFB $E1, $36, $15
    DEFB $00

; index:  4  4
; prov: "British Columbia"
; name: "Drawing Corners"
; stars: 0
_level04:
    DEFB 0    ; stars
    DEFB 0    ; index of province
    DEFB "Drawing Corners", 0
    DEFB $00

; index:  5  5
; prov: "British Columbia"
; name: "Two Liner"
; stars: 1
_level05:
    DEFB 1    ; stars
    DEFB 0    ; index of province
    DEFB "Two Liner", 0
    DEFB $E2, $31, $15
    DEFB $F1, $13, $15
    DEFB $E4, $53, $12
    DEFB $F8, $35, $12
    DEFB $00

; index:  6  6
; prov: "British Columbia"
; name: "The Little One"
; stars: 1
_level06:
    DEFB 1    ; stars
    DEFB 0    ; index of province
    DEFB "The Little One", 0
    DEFB $F1, $33, $13
    DEFB $E4, $34, $13
    DEFB $00

; index:  7  7
; prov: "Alberta"
; name: "A Hard Place"
; stars: 1
_level10:
    DEFB 1    ; stars
    DEFB 1    ; index of province
    DEFB "A Hard Place", 0
    DEFB $F8, $03, $12
    DEFB $D0, $33
    DEFB $E2, $63, $12
    DEFB $00

; index:  8  8
; prov: "Alberta"
; name: "You Did"
; stars: 1
_level11:
    DEFB 1    ; stars
    DEFB 1    ; index of province
    DEFB "You Did", 0
    DEFB $E8, $00, $14
    DEFB $F8, $02, $14
    DEFB $E2, $64, $12
    DEFB $F2, $66, $12
    DEFB $D0, $01
    DEFB $D0, $11
    DEFB $D0, $21
    DEFB $D0, $31
    DEFB $D0, $41
    DEFB $D0, $51
    DEFB $D0, $15
    DEFB $D0, $25
    DEFB $D0, $35
    DEFB $D0, $45
    DEFB $D0, $55
    DEFB $D0, $65
    DEFB $00

; index:  9  9
; prov: "Alberta"
; name: "Traveller"
; stars: 1
_level12:
    DEFB 1    ; stars
    DEFB 1    ; index of province
    DEFB "Traveller", 0
    DEFB $F8, $00, $11
    DEFB $E3, $66, $11
    DEFB $D0, $30
    DEFB $D0, $51
    DEFB $D0, $12
    DEFB $D0, $43
    DEFB $D0, $54
    DEFB $D0, $25
    DEFB $00

; index: 10  A
; prov: "Alberta"
; name: "Fixing Mistakes"
; stars: 0
_level13:
    DEFB 0    ; stars
    DEFB 1    ; index of province
    DEFB "Fixing Mistakes", 0
    DEFB $00

; index: 11  B
; prov: "Alberta"
; name: "In The Middle"
; stars: 1
_level14:
    DEFB 1    ; stars
    DEFB 1    ; index of province
    DEFB "In The Middle", 0
    DEFB $F1, $11, $13
    DEFB $F8, $22, $14
    DEFB $E2, $44, $14
    DEFB $E4, $55, $13
    DEFB $00

; index: 12  C
; prov: "Alberta"
; name: "Foray"
; stars: 2
_level15:
    DEFB 2    ; stars
    DEFB 1    ; index of province
    DEFB "Foray", 0
    DEFB $F8, $00, $14
    DEFB $F2, $60, $12
    DEFB $EF, $33, $44, $21, $50
    DEFB $F8, $06, $11
    DEFB $F2, $66, $15
    DEFB $00

; index: 13  D
; prov: "Alberta"
; name: "No Touching"
; stars: 2
_level16:
    DEFB 2    ; stars
    DEFB 1    ; index of province
    DEFB "No Touching", 0
    DEFB $F4, $30, $12
    DEFB $E8, $03, $14
    DEFB $F2, $63, $14
    DEFB $E1, $36, $12
    DEFB $00

; index: 14  E
; prov: "Alberta"
; name: "Two For Two"
; stars: 1
_level17:
    DEFB 1    ; stars
    DEFB 1    ; index of province
    DEFB "Two For Two", 0
    DEFB $E1, $06, $25, $50
    DEFB $F1, $66, $25, $50
    DEFB $D0, $60
    DEFB $D0, $01
    DEFB $D0, $21
    DEFB $D0, $42
    DEFB $D0, $03
    DEFB $D0, $33
    DEFB $D0, $24
    DEFB $D0, $64
    DEFB $D0, $25
    DEFB $D0, $36
    DEFB $00

; index: 15  F
; prov: "Saskatchewan"
; name: "Switching Track"
; stars: 0
_level20:
    DEFB 0    ; stars
    DEFB 2    ; index of province
    DEFB "Switching Track", 0
    DEFB $00

; index: 16 10
; prov: "Saskatchewan"
; name: "Bluesy"
; stars: 2
_level21:
    DEFB 2    ; stars
    DEFB 2    ; index of province
    DEFB "Bluesy", 0
    DEFB $E2, $11, $11
    DEFB $E8, $51, $11
    DEFB $F1, $35, $21, $10
    DEFB $00

; index: 17 11
; prov: "Saskatchewan"
; name: "Lantern"
; stars: 2
_level22:
    DEFB 2    ; stars
    DEFB 2    ; index of province
    DEFB "Lantern", 0
    DEFB $E4, $31, $16
    DEFB $E1, $33, $15
    DEFB $F4, $34, $26, $50
    DEFB $00

; index: 18 12
; prov: "Saskatchewan"
; name: "Boom Bah"
; stars: 3
_level23:
    DEFB 3    ; stars
    DEFB 2    ; index of province
    DEFB "Boom Bah", 0
    DEFB $E4, $60, $12
    DEFB $F4, $03, $24, $20
    DEFB $EF, $33, $24, $40
    DEFB $F1, $63, $22, $40
    DEFB $E1, $06, $12
    DEFB $00

; index: 19 13
; prov: "Saskatchewan"
; name: "Red Green"
; stars: 3
_level24:
    DEFB 3    ; stars
    DEFB 2    ; index of province
    DEFB "Red Green", 0
    DEFB $E4, $00, $12
    DEFB $E4, $20, $15
    DEFB $E4, $40, $12
    DEFB $E4, $60, $15
    DEFB $F1, $36, $42, $55, $20
    DEFB $00

; index: 20 14
; prov: "Manitoba"
; name: "Merging Trains"
; stars: 0
_level30:
    DEFB 0    ; stars
    DEFB 3    ; index of province
    DEFB "Merging Trains", 0
    DEFB $00

; index: 21 15
; prov: "Manitoba"
; name: "Yellow Triangle"
; stars: 3
_level31:
    DEFB 3    ; stars
    DEFB 3    ; index of province
    DEFB "Yellow Triangle", 0
    DEFB $F4, $00, $14
    DEFB $F4, $20, $14
    DEFB $E4, $60, $14
    DEFB $00

; index: 22 16
; prov: "Manitoba"
; name: "Purpeller"
; stars: 3
_level32:
    DEFB 3    ; stars
    DEFB 3    ; index of province
    DEFB "Purpeller", 0
    DEFB $F4, $00, $13
    DEFB $F2, $64, $13
    DEFB $E4, $65, $13
    DEFB $D0, $50
    DEFB $D0, $60
    DEFB $D0, $61
    DEFB $D0, $05
    DEFB $D0, $06
    DEFB $D0, $16
    DEFB $00

; index: 23 17
; prov: "Manitoba"
; name: "Reflection"
; stars: 3
_level33:
    DEFB 3    ; stars
    DEFB 3    ; index of province
    DEFB "Reflection", 0
    DEFB $F4, $00, $12
    DEFB $E6, $60, $11
    DEFB $F4, $02, $12
    DEFB $F1, $64, $11
    DEFB $E9, $06, $12
    DEFB $F1, $66, $11
    DEFB $00

; index: 24 18
; prov: "Manitoba"
; name: "Combinellow"
; stars: 3
_level34:
    DEFB 3    ; stars
    DEFB 3    ; index of province
    DEFB "Combinellow", 0
    DEFB $E4, $60, $14
    DEFB $F8, $02, $14
    DEFB $F8, $04, $14
    DEFB $F1, $26, $14
    DEFB $F1, $46, $14
    DEFB $D0, $00
    DEFB $D0, $01
    DEFB $D0, $03
    DEFB $D0, $05
    DEFB $D0, $06
    DEFB $D0, $16
    DEFB $D0, $36
    DEFB $D0, $56
    DEFB $D0, $66
    DEFB $00

; index: 25 19
; prov: "Manitoba"
; name: "Colourize"
; stars: 3
_level35:
    DEFB 3    ; stars
    DEFB 3    ; index of province
    DEFB "Colourize", 0
    DEFB $F4, $00, $12
    DEFB $F1, $11, $11
    DEFB $E3, $66, $13
    DEFB $00

; index: 26 1A
; prov: "Manitoba"
; name: "Weighted Top"
; stars: 3
_level36:
    DEFB 3    ; stars
    DEFB 3    ; index of province
    DEFB "Weighted Top", 0
    DEFB $E4, $30, $15
    DEFB $F2, $23, $21, $10
    DEFB $F8, $43, $24, $40
    DEFB $E1, $36, $15
    DEFB $D0, $33
    DEFB $00

; index: 27 1B
; prov: "Ontario"
; name: "Mixing"
; stars: 0
_level40:
    DEFB 0    ; stars
    DEFB 4    ; index of province
    DEFB "Mixing", 0
    DEFB $00

; index: 28 1C
; prov: "Ontario"
; name: "Embrace"
; stars: 3
_level41:
    DEFB 3    ; stars
    DEFB 4    ; index of province
    DEFB "Embrace", 0
    DEFB $F4, $00, $11
    DEFB $E4, $30, $13
    DEFB $E1, $36, $13
    DEFB $F1, $66, $12
    DEFB $00

; index: 29 1D
; prov: "Ontario"
; name: "Pee Gee"
; stars: 4
_level42:
    DEFB 4    ; stars
    DEFB 4    ; index of province
    DEFB "Pee Gee", 0
    DEFB $F8, $00, $14
    DEFB $F2, $60, $11
    DEFB $E8, $02, $13
    DEFB $E2, $63, $15
    DEFB $E8, $04, $13
    DEFB $F8, $06, $11
    DEFB $F2, $66, $12
    DEFB $00

; index: 30 1E
; prov: "Ontario"
; name: "Autumn"
; stars: 4
_level43:
    DEFB 4    ; stars
    DEFB 4    ; index of province
    DEFB "Autumn", 0
    DEFB $F4, $20, $12
    DEFB $E4, $40, $16
    DEFB $F1, $26, $14
    DEFB $F1, $46, $16
    DEFB $00

; index: 31 1F
; prov: "Ontario"
; name: "Unscathed"
; stars: 4
_level44:
    DEFB 4    ; stars
    DEFB 4    ; index of province
    DEFB "Unscathed", 0
    DEFB $E8, $01, $12
    DEFB $F8, $02, $11
    DEFB $F8, $04, $12
    DEFB $E8, $05, $11
    DEFB $D0, $03
    DEFB $D0, $13
    DEFB $D0, $23
    DEFB $D0, $43
    DEFB $D0, $53
    DEFB $D0, $63
    DEFB $00

; index: 32 20
; prov: "Ontario"
; name: "Challenger"
; stars: 5
_level45:
    DEFB 5    ; stars
    DEFB 4    ; index of province
    DEFB "Challenger", 0
    DEFB $F4, $30, $21, $20
    DEFB $E8, $03, $15
    DEFB $E2, $63, $16
    DEFB $F1, $36, $24, $40
    DEFB $00

; index: 33 21
; prov: "Ontario"
; name: "Please Wait"
; stars: 5
_level46:
    DEFB 5    ; stars
    DEFB 4    ; index of province
    DEFB "Please Wait", 0
    DEFB $F2, $11, $12
    DEFB $E2, $46, $13
    DEFB $F1, $66, $11
    DEFB $D0, $21
    DEFB $D0, $31
    DEFB $D0, $41
    DEFB $D0, $51
    DEFB $D0, $52
    DEFB $D0, $53
    DEFB $D0, $54
    DEFB $D0, $55
    DEFB $D0, $56
    DEFB $00

; index: 34 22
; prov: "Quebec"
; name: "Berlin"
; stars: 5
_level50:
    DEFB 5    ; stars
    DEFB 5    ; index of province
    DEFB "Berlin", 0
    DEFB $E4, $11, $13
    DEFB $E4, $31, $12
    DEFB $E4, $51, $15
    DEFB $F1, $15, $15
    DEFB $F1, $35, $13
    DEFB $F1, $55, $12
    DEFB $D0, $03
    DEFB $D0, $13
    DEFB $D0, $23
    DEFB $D0, $43
    DEFB $D0, $53
    DEFB $D0, $63
    DEFB $00

; index: 35 23
; prov: "Quebec"
; name: "Golem"
; stars: 5
_level51:
    DEFB 5    ; stars
    DEFB 5    ; index of province
    DEFB "Golem", 0
    DEFB $F1, $32, $14
    DEFB $F2, $23, $15
    DEFB $E8, $43, $15
    DEFB $F4, $34, $11
    DEFB $D0, $33
    DEFB $00

; index: 36 24
; prov: "Quebec"
; name: "Sunburst"
; stars: 5
_level52:
    DEFB 5    ; stars
    DEFB 5    ; index of province
    DEFB "Sunburst", 0
    DEFB $F4, $31, $14
    DEFB $F8, $13, $14
    DEFB $EF, $33, $14
    DEFB $F2, $53, $14
    DEFB $F1, $35, $14
    DEFB $00

; index: 37 25
; prov: "Quebec"
; name: "Pea Shooter"
; stars: 6
_level53:
    DEFB 6    ; stars
    DEFB 5    ; index of province
    DEFB "Pea Shooter", 0
    DEFB $F1, $03, $11
    DEFB $F1, $13, $11
    DEFB $F4, $23, $14
    DEFB $F4, $33, $14
    DEFB $E2, $63, $25, $50
    DEFB $00

; index: 38 26
; prov: "Quebec"
; name: "Scarab"
; stars: 6
_level54:
    DEFB 6    ; stars
    DEFB 5    ; index of province
    DEFB "Scarab", 0
    DEFB $E4, $00, $15
    DEFB $E4, $60, $15
    DEFB $F4, $32, $11
    DEFB $F8, $13, $14
    DEFB $F2, $53, $12
    DEFB $F1, $34, $14
    DEFB $E1, $06, $16
    DEFB $E1, $66, $16
    DEFB $00

; index: 39 27
; prov: "New Brunswick"
; name: "Prancing"
; stars: 6
_level60:
    DEFB 6    ; stars
    DEFB 6    ; index of province
    DEFB "Prancing", 0
    DEFB $F8, $00, $11
    DEFB $F2, $60, $12
    DEFB $F8, $51, $13
    DEFB $EA, $33, $23, $50
    DEFB $F2, $15, $15
    DEFB $F8, $06, $14
    DEFB $F2, $66, $11
    DEFB $00

; index: 40 28
; prov: "New Brunswick"
; name: "Box Seven"
; stars: 7
_level61:
    DEFB 7    ; stars
    DEFB 6    ; index of province
    DEFB "Box Seven", 0
    DEFB $F8, $00, $22, $40
    DEFB $F8, $01, $21, $10
    DEFB $E8, $05, $15
    DEFB $E8, $06, $13
    DEFB $D0, $02
    DEFB $D0, $12
    DEFB $D0, $22
    DEFB $D0, $03
    DEFB $D0, $13
    DEFB $D0, $23
    DEFB $D0, $33
    DEFB $D0, $04
    DEFB $D0, $14
    DEFB $D0, $24
    DEFB $00

; index: 41 29
; prov: "New Brunswick"
; name: "Sapling"
; stars: 7
_level62:
    DEFB 7    ; stars
    DEFB 6    ; index of province
    DEFB "Sapling", 0
    DEFB $E8, $00, $12
    DEFB $E2, $60, $11
    DEFB $E8, $06, $12
    DEFB $F2, $66, $62, $12, $12, $10
    DEFB $00

; index: 42 2A
; prov: "New Brunswick"
; name: "Match Up"
; stars: 8
_level63:
    DEFB 8    ; stars
    DEFB 6    ; index of province
    DEFB "Match Up", 0
    DEFB $E4, $00, $13
    DEFB $E4, $60, $16
    DEFB $F2, $33, $11
    DEFB $F4, $43, $22, $20
    DEFB $E1, $06, $13
    DEFB $F1, $66, $14
    DEFB $00

; index: 43 2B
; prov: "New Brunswick"
; name: "Being Green"
; stars: 7
_level64:
    DEFB 7    ; stars
    DEFB 6    ; index of province
    DEFB "Being Green", 0
    DEFB $F1, $11, $11
    DEFB $E1, $51, $15
    DEFB $F1, $02, $11
    DEFB $E1, $62, $15
    DEFB $F4, $04, $14
    DEFB $E4, $64, $15
    DEFB $F4, $15, $14
    DEFB $E4, $55, $15
    DEFB $D0, $12
    DEFB $D0, $52
    DEFB $D0, $03
    DEFB $D0, $13
    DEFB $D0, $53
    DEFB $D0, $63
    DEFB $D0, $14
    DEFB $D0, $54
    DEFB $00

; index: 44 2C
; prov: "New Brunswick"
; name: "Casualty"
; stars: 8
_level65:
    DEFB 8    ; stars
    DEFB 6    ; index of province
    DEFB "Casualty", 0
    DEFB $E4, $40, $14
    DEFB $F8, $02, $12
    DEFB $F2, $62, $11
    DEFB $E8, $03, $13
    DEFB $E2, $63, $23, $30
    DEFB $F8, $04, $12
    DEFB $F2, $64, $11
    DEFB $F1, $46, $14
    DEFB $00

; index: 45 2D
; prov: "Prince Edward Isl."
; name: "Painter Pieces"
; stars: 0
_level70:
    DEFB 0    ; stars
    DEFB 7    ; index of province
    DEFB "Painter Pieces", 0
    DEFB $00

; index: 46 2E
; prov: "Prince Edward Isl."
; name: "Niner"
; stars: 8
_level71:
    DEFB 8    ; stars
    DEFB 7    ; index of province
    DEFB "Niner", 0
    DEFB $F1, $32, $12
    DEFB $E8, $03, $15
    DEFB $B5, $23, $14
    DEFB $B5, $43, $11
    DEFB $E2, $63, $15
    DEFB $F4, $34, $12
    DEFB $D0, $33
    DEFB $00

; index: 47 2F
; prov: "Prince Edward Isl."
; name: "Teamwork"
; stars: 8
_level72:
    DEFB 8    ; stars
    DEFB 7    ; index of province
    DEFB "Teamwork", 0
    DEFB $BA, $30, $13
    DEFB $EA, $31, $13
    DEFB $F1, $35, $15
    DEFB $F2, $26, $11
    DEFB $F8, $46, $12
    DEFB $D0, $36
    DEFB $00

; index: 48 30
; prov: "Prince Edward Isl."
; name: "Return Policing"
; stars: 8
_level73:
    DEFB 8    ; stars
    DEFB 7    ; index of province
    DEFB "Return Policing", 0
    DEFB $D0, $60
    DEFB $D0, $51
    DEFB $D0, $42
    DEFB $BA, $33, $11
    DEFB $D0, $24
    DEFB $D0, $15
    DEFB $D0, $06
    DEFB $E2, $56, $41, $11, $10
    DEFB $F1, $66, $42, $22, $20
    DEFB $00

; index: 49 31
; prov: "Prince Edward Isl."
; name: "Formatic"
; stars: 8
_level74:
    DEFB 8    ; stars
    DEFB 7    ; index of province
    DEFB "Formatic", 0
    DEFB $B5, $23, $14
    DEFB $F1, $33, $42, $22, $20
    DEFB $B5, $43, $11
    DEFB $EE, $34, $25, $50
    DEFB $00

; index: 50 32
; prov: "Prince Edward Isl."
; name: "Rainbow Arrow"
; stars: 9
_level75:
    DEFB 9    ; stars
    DEFB 7    ; index of province
    DEFB "Rainbow Arrow", 0
    DEFB $BA, $31, $12
    DEFB $E2, $61, $11
    DEFB $BA, $32, $14
    DEFB $E2, $62, $15
    DEFB $D0, $33
    DEFB $F2, $63, $43, $33, $30
    DEFB $BA, $34, $15
    DEFB $E2, $64, $14
    DEFB $BA, $35, $11
    DEFB $E2, $65, $12
    DEFB $00

; index: 51 33
; prov: "Nova Scotia"
; name: "One Way"
; stars: 8
_level80:
    DEFB 8    ; stars
    DEFB 8    ; index of province
    DEFB "One Way", 0
    DEFB $F2, $61, $11
    DEFB $E8, $03, $15
    DEFB $BA, $23, $12
    DEFB $BA, $33, $14
    DEFB $F2, $63, $11
    DEFB $F2, $65, $11
    DEFB $00

; index: 52 34
; prov: "Nova Scotia"
; name: "Roundabout"
; stars: 9
_level81:
    DEFB 9    ; stars
    DEFB 8    ; index of province
    DEFB "Roundabout", 0
    DEFB $F1, $31, $14
    DEFB $F2, $13, $12
    DEFB $E4, $33, $45, $53, $30
    DEFB $F8, $53, $11
    DEFB $F4, $35, $11
    DEFB $D0, $24
    DEFB $D0, $15
    DEFB $D0, $06
    DEFB $00

; index: 53 35
; prov: "Nova Scotia"
; name: "Chromashift"
; stars: 9
_level82:
    DEFB 9    ; stars
    DEFB 8    ; index of province
    DEFB "Chromashift", 0
    DEFB $F1, $12, $15
    DEFB $BA, $32, $12
    DEFB $F1, $52, $11
    DEFB $F2, $13, $15
    DEFB $EA, $33, $22, $20
    DEFB $F8, $53, $11
    DEFB $F4, $14, $15
    DEFB $BA, $34, $12
    DEFB $F4, $54, $11
    DEFB $00

; index: 54 36
; prov: "Nova Scotia"
; name: "Balsam"
; stars: 9
_level83:
    DEFB 9    ; stars
    DEFB 8    ; index of province
    DEFB "Balsam", 0
    DEFB $E1, $32, $23, $30
    DEFB $F4, $33, $82, $22, $21, $11, $10
    DEFB $00

; index: 55 37
; prov: "Nova Scotia"
; name: "Boomerang"
; stars: 9
_level84:
    DEFB 9    ; stars
    DEFB 8    ; index of province
    DEFB "Boomerang", 0
    DEFB $BA, $50, $14
    DEFB $BA, $51, $15
    DEFB $BA, $52, $11
    DEFB $F4, $63, $32, $22
    DEFB $E1, $66, $34, $51
    DEFB $00

; index: 56 38
; prov: "Newfoundland"
; name: "Taxing"
; stars: 8
_level90:
    DEFB 8    ; stars
    DEFB 9    ; index of province
    DEFB "Taxing", 0
    DEFB $F8, $00, $11
    DEFB $E6, $60, $15
    DEFB $F4, $01, $14
    DEFB $F1, $65, $11
    DEFB $E9, $06, $15
    DEFB $F2, $66, $14
    DEFB $00

; index: 57 39
; prov: "Newfoundland"
; name: "Dousing The Flame"
; stars: 9
_level91:
    DEFB 9    ; stars
    DEFB 9    ; index of province
    DEFB "Dousing The Flame", 0
    DEFB $F4, $00, $12
    DEFB $D0, $10
    DEFB $F4, $20, $12
    DEFB $E4, $30, $11
    DEFB $F4, $40, $12
    DEFB $D0, $50
    DEFB $F4, $60, $12
    DEFB $F4, $11, $12
    DEFB $F4, $51, $12
    DEFB $B5, $35, $11
    DEFB $00

; index: 58 3A
; prov: "Newfoundland"
; name: "Duplex"
; stars: 9
_level92:
    DEFB 9    ; stars
    DEFB 9    ; index of province
    DEFB "Duplex", 0
    DEFB $D0, $00
    DEFB $D0, $60
    DEFB $F8, $01, $11
    DEFB $F2, $61, $14
    DEFB $D0, $02
    DEFB $F2, $62, $14
    DEFB $F8, $03, $11
    DEFB $D0, $63
    DEFB $F8, $04, $11
    DEFB $F2, $64, $14
    DEFB $D0, $05
    DEFB $D0, $65
    DEFB $E8, $06, $15
    DEFB $E2, $66, $15
    DEFB $00

; index: 59 3B
; prov: "Newfoundland"
; name: "Pals"
; stars: 9
_level93:
    DEFB 9    ; stars
    DEFB 9    ; index of province
    DEFB "Pals", 0
    DEFB $E8, $00, $25, $60
    DEFB $E2, $60, $23, $60
    DEFB $F2, $13, $24, $40
    DEFB $F8, $53, $22, $20
    DEFB $F4, $35, $21, $10
    DEFB $00

; index: 60 3C
; prov: "Newfoundland"
; name: "Refresh"
; stars: 10
_level94:
    DEFB 10    ; stars
    DEFB 9    ; index of province
    DEFB "Refresh", 0
    DEFB $E8, $02, $13
    DEFB $BA, $32, $12
    DEFB $F2, $62, $14
    DEFB $E8, $03, $13
    DEFB $F2, $63, $14
    DEFB $E8, $04, $13
    DEFB $BA, $34, $11
    DEFB $F2, $64, $14
    DEFB $00

; index: 61 3D
; prov: "Yukon"
; name: "Partnership"
; stars: 8
_levelA0:
    DEFB 8    ; stars
    DEFB 10    ; index of province
    DEFB "Partnership", 0
    DEFB $E2, $10, $15
    DEFB $E4, $30, $13
    DEFB $E8, $50, $16
    DEFB $F2, $16, $31, $14
    DEFB $F8, $56, $32, $24
    DEFB $00

; index: 62 3E
; prov: "Yukon"
; name: "Impact"
; stars: 9
_levelA1:
    DEFB 9    ; stars
    DEFB 10    ; index of province
    DEFB "Impact", 0
    DEFB $E4, $30, $43, $33, $30
    DEFB $F8, $03, $42, $22, $20
    DEFB $F2, $63, $41, $11, $10
    DEFB $E1, $36, $43, $33, $30
    DEFB $00

; index: 63 3F
; prov: "Yukon"
; name: "Reciprocate"
; stars: 10
_levelA2:
    DEFB 10    ; stars
    DEFB 10    ; index of province
    DEFB "Reciprocate", 0
    DEFB $F1, $02, $16
    DEFB $E1, $12, $13
    DEFB $E1, $52, $15
    DEFB $F1, $62, $16
    DEFB $F8, $03, $13
    DEFB $F2, $63, $13
    DEFB $F4, $04, $15
    DEFB $E4, $14, $13
    DEFB $E4, $54, $16
    DEFB $F4, $64, $15
    DEFB $D0, $33
    DEFB $00

; index: 64 40
; prov: "Yukon"
; name: "Sibilant"
; stars: 10
_levelA3:
    DEFB 10    ; stars
    DEFB 10    ; index of province
    DEFB "Sibilant", 0
    DEFB $E8, $00, $15
    DEFB $F4, $40, $11
    DEFB $F4, $50, $11
    DEFB $F4, $60, $11
    DEFB $E8, $02, $25, $50
    DEFB $E8, $04, $25, $50
    DEFB $E8, $06, $15
    DEFB $F1, $46, $14
    DEFB $F1, $56, $14
    DEFB $F1, $66, $14
    DEFB $00

; index: 65 41
; prov: "Yukon"
; name: "Moonbeams"
; stars: 10
_levelA4:
    DEFB 10    ; stars
    DEFB 10    ; index of province
    DEFB "Moonbeams", 0
    DEFB $BA, $20, $15
    DEFB $BA, $40, $11
    DEFB $D0, $21
    DEFB $D0, $31
    DEFB $D0, $41
    DEFB $F4, $14, $43, $33, $30
    DEFB $E4, $54, $45, $15, $10
    DEFB $00

; index: 66 42
; prov: "N.W. Territories"
; name: "Snowracer"
; stars: 5
_levelB0:
    DEFB 5    ; stars
    DEFB 11    ; index of province
    DEFB "Snowracer", 0
    DEFB $E2, $60, $12
    DEFB $E2, $61, $14
    DEFB $E2, $62, $11
    DEFB $F8, $04, $11
    DEFB $F8, $05, $14
    DEFB $F8, $06, $12
    DEFB $D0, $21
    DEFB $D0, $32
    DEFB $D0, $42
    DEFB $D0, $13
    DEFB $D0, $53
    DEFB $D0, $24
    DEFB $D0, $34
    DEFB $D0, $45
    DEFB $00

; index: 67 43
; prov: "N.W. Territories"
; name: "Right of Passage"
; stars: 9
_levelB1:
    DEFB 9    ; stars
    DEFB 11    ; index of province
    DEFB "Right of Passage", 0
    DEFB $F4, $10, $11
    DEFB $D0, $20
    DEFB $E4, $30, $25, $30
    DEFB $D0, $40
    DEFB $F4, $50, $11
    DEFB $F1, $32, $15
    DEFB $F4, $34, $14
    DEFB $F1, $36, $12
    DEFB $00

; index: 68 44
; prov: "N.W. Territories"
; name: "Midland"
; stars: 10
_levelB2:
    DEFB 10    ; stars
    DEFB 11    ; index of province
    DEFB "Midland", 0
    DEFB $E4, $20, $14
    DEFB $E4, $30, $14
    DEFB $E4, $40, $14
    DEFB $F8, $02, $12
    DEFB $F8, $03, $12
    DEFB $F8, $04, $12
    DEFB $F2, $62, $12
    DEFB $F2, $63, $12
    DEFB $F2, $64, $12
    DEFB $E1, $26, $14
    DEFB $E1, $36, $14
    DEFB $E1, $46, $14
    DEFB $B5, $33, $14
    DEFB $00

; index: 69 45
; prov: "N.W. Territories"
; name: "Down Under"
; stars: 10
_levelB3:
    DEFB 10    ; stars
    DEFB 11    ; index of province
    DEFB "Down Under", 0
    DEFB $F1, $12, $12
    DEFB $F1, $22, $13
    DEFB $F1, $32, $11
    DEFB $F1, $42, $15
    DEFB $F1, $52, $14
    DEFB $E4, $14, $14
    DEFB $E4, $24, $15
    DEFB $E4, $34, $11
    DEFB $E4, $44, $13
    DEFB $E4, $54, $12
    DEFB $00

; index: 70 46
; prov: "N.W. Territories"
; name: "Lamport"
; stars: 10
_levelB4:
    DEFB 10    ; stars
    DEFB 11    ; index of province
    DEFB "Lamport", 0
    DEFB $E4, $00, $15
    DEFB $F4, $10, $14
    DEFB $F4, $30, $14
    DEFB $F4, $50, $14
    DEFB $E4, $60, $15
    DEFB $E1, $06, $15
    DEFB $F1, $16, $11
    DEFB $F1, $36, $11
    DEFB $F1, $56, $11
    DEFB $E1, $66, $15
    DEFB $D0, $21
    DEFB $D0, $41
    DEFB $D0, $13
    DEFB $D0, $33
    DEFB $D0, $53
    DEFB $D0, $25
    DEFB $D0, $45
    DEFB $00

; index: 71 47
; prov: "Nunavut"
; name: "Jumping Joy"
; stars: 8
_levelC0:
    DEFB 8    ; stars
    DEFB 12    ; index of province
    DEFB "Jumping Joy", 0
    DEFB $EA, $30, $14
    DEFB $F2, $22, $11
    DEFB $F8, $42, $11
    DEFB $B5, $33, $14
    DEFB $F2, $24, $11
    DEFB $F8, $44, $11
    DEFB $EA, $36, $15
    DEFB $D0, $23
    DEFB $D0, $43
    DEFB $00

; index: 72 48
; prov: "Nunavut"
; name: "Sidewinder"
; stars: 10
_levelC1:
    DEFB 10    ; stars
    DEFB 12    ; index of province
    DEFB "Sidewinder", 0
    DEFB $F1, $02, $12
    DEFB $F1, $04, $12
    DEFB $F1, $06, $12
    DEFB $E1, $62, $14
    DEFB $E1, $64, $14
    DEFB $E1, $66, $14
    DEFB $B6, $33, $14
    DEFB $B9, $34, $12
    DEFB $D0, $21
    DEFB $D0, $31
    DEFB $D0, $41
    DEFB $D0, $32
    DEFB $D0, $35
    DEFB $D0, $36
    DEFB $00

; index: 73 49
; prov: "Nunavut"
; name: "Dufferin Gate"
; stars: 10
_levelC2:
    DEFB 10    ; stars
    DEFB 12    ; index of province
    DEFB "Dufferin Gate", 0
    DEFB $E8, $00, $11
    DEFB $E2, $60, $14
    DEFB $F8, $03, $42, $22, $20
    DEFB $E8, $06, $14
    DEFB $E2, $66, $11
    DEFB $BA, $30, $14
    DEFB $BA, $31, $14
    DEFB $BA, $32, $14
    DEFB $BA, $33, $11
    DEFB $BA, $34, $14
    DEFB $BA, $35, $14
    DEFB $BA, $36, $14
    DEFB $00

; index: 74 4A
; prov: "Nunavut"
; name: "Entanglement"
; stars: 11
_levelC3:
    DEFB 11    ; stars
    DEFB 12    ; index of province
    DEFB "Entanglement", 0
    DEFB $E4, $00, $15
    DEFB $E4, $30, $16
    DEFB $F4, $60, $43, $33, $30
    DEFB $B5, $04, $11
    DEFB $B5, $15, $14
    DEFB $BA, $26, $12
    DEFB $D0, $10
    DEFB $D0, $20
    DEFB $D0, $40
    DEFB $D0, $50
    DEFB $00

_levarr:
    DEFW _level00   ;  0  0
    DEFW _level01   ;  1  1
    DEFW _level02   ;  2  2
    DEFW _level03   ;  3  3
    DEFW _level04   ;  4  4
    DEFW _level05   ;  5  5
    DEFW _level06   ;  6  6
    DEFW _level10   ;  7  7
    DEFW _level11   ;  8  8
    DEFW _level12   ;  9  9
    DEFW _level13   ; 10  A
    DEFW _level14   ; 11  B
    DEFW _level15   ; 12  C
    DEFW _level16   ; 13  D
    DEFW _level17   ; 14  E
    DEFW _level20   ; 15  F
    DEFW _level21   ; 16 10
    DEFW _level22   ; 17 11
    DEFW _level23   ; 18 12
    DEFW _level24   ; 19 13
    DEFW _level30   ; 20 14
    DEFW _level31   ; 21 15
    DEFW _level32   ; 22 16
    DEFW _level33   ; 23 17
    DEFW _level34   ; 24 18
    DEFW _level35   ; 25 19
    DEFW _level36   ; 26 1A
    DEFW _level40   ; 27 1B
    DEFW _level41   ; 28 1C
    DEFW _level42   ; 29 1D
    DEFW _level43   ; 30 1E
    DEFW _level44   ; 31 1F
    DEFW _level45   ; 32 20
    DEFW _level46   ; 33 21
    DEFW _level50   ; 34 22
    DEFW _level51   ; 35 23
    DEFW _level52   ; 36 24
    DEFW _level53   ; 37 25
    DEFW _level54   ; 38 26
    DEFW _level60   ; 39 27
    DEFW _level61   ; 40 28
    DEFW _level62   ; 41 29
    DEFW _level63   ; 42 2A
    DEFW _level64   ; 43 2B
    DEFW _level65   ; 44 2C
    DEFW _level70   ; 45 2D
    DEFW _level71   ; 46 2E
    DEFW _level72   ; 47 2F
    DEFW _level73   ; 48 30
    DEFW _level74   ; 49 31
    DEFW _level75   ; 50 32
    DEFW _level80   ; 51 33
    DEFW _level81   ; 52 34
    DEFW _level82   ; 53 35
    DEFW _level83   ; 54 36
    DEFW _level84   ; 55 37
    DEFW _level90   ; 56 38
    DEFW _level91   ; 57 39
    DEFW _level92   ; 58 3A
    DEFW _level93   ; 59 3B
    DEFW _level94   ; 60 3C
    DEFW _levelA0   ; 61 3D
    DEFW _levelA1   ; 62 3E
    DEFW _levelA2   ; 63 3F
    DEFW _levelA3   ; 64 40
    DEFW _levelA4   ; 65 41
    DEFW _levelB0   ; 66 42
    DEFW _levelB1   ; 67 43
    DEFW _levelB2   ; 68 44
    DEFW _levelB3   ; 69 45
    DEFW _levelB4   ; 70 46
    DEFW _levelC0   ; 71 47
    DEFW _levelC1   ; 72 48
    DEFW _levelC2   ; 73 49
    DEFW _levelC3   ; 74 4A


; provinces:

_prov0:
    DEFW 0    ; stars needed to unlock
    DEFB "British Columbia", 0
    DEFB 7    ; number of levels in province
    DEFB 0    ; index of first province level in _levarr

_prov1:
    DEFW 5    ; stars needed to unlock
    DEFB "Alberta", 0
    DEFB 8    ; number of levels in province
    DEFB 7    ; index of first province level in _levarr

_prov2:
    DEFW 14    ; stars needed to unlock
    DEFB "Saskatchewan", 0
    DEFB 5    ; number of levels in province
    DEFB 15    ; index of first province level in _levarr

_prov3:
    DEFW 24    ; stars needed to unlock
    DEFB "Manitoba", 0
    DEFB 7    ; number of levels in province
    DEFB 20    ; index of first province level in _levarr

_prov4:
    DEFW 42    ; stars needed to unlock
    DEFB "Ontario", 0
    DEFB 7    ; number of levels in province
    DEFB 27    ; index of first province level in _levarr

_prov5:
    DEFW 67    ; stars needed to unlock
    DEFB "Quebec", 0
    DEFB 5    ; number of levels in province
    DEFB 34    ; index of first province level in _levarr

_prov6:
    DEFW 84    ; stars needed to unlock
    DEFB "New Brunswick", 0
    DEFB 6    ; number of levels in province
    DEFB 39    ; index of first province level in _levarr

_prov7:
    DEFW 122    ; stars needed to unlock
    DEFB "Prince Edward Isl.", 0
    DEFB 6    ; number of levels in province
    DEFB 45    ; index of first province level in _levarr

_prov8:
    DEFW 163    ; stars needed to unlock
    DEFB "Nova Scotia", 0
    DEFB 5    ; number of levels in province
    DEFB 51    ; index of first province level in _levarr

_prov9:
    DEFW 207    ; stars needed to unlock
    DEFB "Newfoundland", 0
    DEFB 5    ; number of levels in province
    DEFB 56    ; index of first province level in _levarr

_provA:
    DEFW 267    ; stars needed to unlock
    DEFB "Yukon", 0
    DEFB 5    ; number of levels in province
    DEFB 61    ; index of first province level in _levarr

_provB:
    DEFW 284    ; stars needed to unlock
    DEFB "N.W. Territories", 0
    DEFB 5    ; number of levels in province
    DEFB 66    ; index of first province level in _levarr

_provC:
    DEFW 328    ; stars needed to unlock
    DEFB "Nunavut", 0
    DEFB 4    ; number of levels in province
    DEFB 71    ; index of first province level in _levarr



_provarr:
    DEFW _prov0
    DEFW _prov1
    DEFW _prov2
    DEFW _prov3
    DEFW _prov4
    DEFW _prov5
    DEFW _prov6
    DEFW _prov7
    DEFW _prov8
    DEFW _prov9
    DEFW _provA
    DEFW _provB
    DEFW _provC
