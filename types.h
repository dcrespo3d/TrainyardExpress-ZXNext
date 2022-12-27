////////////////////////////////////////////////////////////////////////////////
//
// Trainyard for ZX Spectrum Next
//
// (C) 2022 David Crespo - https://github.com/dcrespo3d
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __types_h__
#define __types_h__

// unsigned byte
typedef unsigned char ubyte;
// unsigned word
typedef unsigned short uword;

// signed byte
typedef char sbyte;
// signed word
typedef short sword;

// extract high nibble of a byte
#define HINIB(a_byte) (a_byte >> 4)

// extract low nibble of a byte
#define LONIB(a_byte) (a_byte & 0xF)

// join two nibbles into a byte (unchecked)
#define JOINIB(hi, lo) ((hi << 4) | lo)

#endif // __types_h__
