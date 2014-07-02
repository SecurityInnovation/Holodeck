#ifndef __INSTRTABLE_H__
#define __INSTRTABLE_H__

typedef void (*DispHandlerFuncPtr)(PBYTE&,PBYTE&,int);

void JumpShort(PBYTE& dest,PBYTE& src,int addrSize);
void JumpNear(PBYTE& dest,PBYTE& src,int addrSize);
void JumpCondShort(PBYTE& dest,PBYTE& src,int addrSize);
void JumpCondNear(PBYTE& dest,PBYTE& src,int addrSize);
void JCXZShort(PBYTE& dest,PBYTE& src,int addrSize);
void CallNear(PBYTE& dest,PBYTE& src,int addrSize);
void LoopShort(PBYTE& dest,PBYTE& src,int addrSize);
void LoopZShort(PBYTE& dest,PBYTE& src,int addrSize);
void LoopNZShort(PBYTE& dest,PBYTE& src,int addrSize);

#define MODRM 1
#define IMMED8 2
#define IMMED16 4
#define IMMED24 8
#define IMMED32 16
#define DISP8 32
#define DISP32 64
#define NEAR_ADDR 128
#define FAR_ADDR 256
#define PREFIX 512
#define ADDR_SIZE_PREFIX 1024
#define OP_SIZE_PREFIX 2048
#define IMMED8_ON_CASE0	 4096
#define IMMED32_ON_CASE0	 8192
#define SIB 16384
WORD mainOpcodeTable[256]={
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x0
	IMMED8,                 IMMED32,                0,                      0,                       // 0x4
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x8
	IMMED8,                 IMMED32,                0,                      0,                       // 0xc
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x10
	IMMED8,                 IMMED32,                0,                      0,                       // 0x14
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x18
	IMMED8,                 IMMED32,                0,                      0,                       // 0x1c
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x20
	IMMED8,                 IMMED32,                PREFIX,                 0,                       // 0x24
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x28
	IMMED8,                 IMMED32,                PREFIX,                 0,                       // 0x2c
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x30
	IMMED8,                 IMMED32,                PREFIX,                 0,                       // 0x34
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x38
	IMMED8,                 IMMED32,                PREFIX,                 0,                       // 0x3c
	0,                      0,                      0,                      0,                       // 0x40
	0,                      0,                      0,                      0,                       // 0x44
	0,                      0,                      0,                      0,                       // 0x48
	0,                      0,                      0,                      0,                       // 0x4c
	0,                      0,                      0,                      0,                       // 0x50
	0,                      0,                      0,                      0,                       // 0x54
	0,                      0,                      0,                      0,                       // 0x58
	0,                      0,                      0,                      0,                       // 0x5c
	0,                      0,                      MODRM,                  MODRM,                   // 0x60
	PREFIX,                 PREFIX,                 OP_SIZE_PREFIX,         ADDR_SIZE_PREFIX,        // 0x64
	IMMED32,                MODRM|IMMED32,          IMMED8,                 MODRM|IMMED8,            // 0x68
	0,                      0,                      0,                      0,                       // 0x6c
	DISP8,                  DISP8,                  DISP8,                  DISP8,                   // 0x70
	DISP8,                  DISP8,                  DISP8,                  DISP8,                   // 0x74
	DISP8,                  DISP8,                  DISP8,                  DISP8,                   // 0x78
	DISP8,                  DISP8,                  DISP8,                  DISP8,                   // 0x7c
	MODRM|IMMED8,           MODRM|IMMED32,          MODRM|IMMED8,           MODRM|IMMED8,            // 0x80
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x84
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x88
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0x8c
	0,                      0,                      0,                      0,                       // 0x90
	0,                      0,                      0,                      0,                       // 0x94
	0,                      0,                      FAR_ADDR,               0,                       // 0x98
	0,                      0,                      0,                      0,                       // 0x9c
	NEAR_ADDR,              NEAR_ADDR,              NEAR_ADDR,              NEAR_ADDR,               // 0xa0
	0,                      0,                      0,                      0,                       // 0xa4
	IMMED8,                 IMMED32,                0,                      0,                       // 0xa8
	0,                      0,                      0,                      0,                       // 0xac
	IMMED8,                 IMMED8,                 IMMED8,                 IMMED8,                  // 0xb0
	IMMED8,                 IMMED8,                 IMMED8,                 IMMED8,                  // 0xb4
	IMMED32,                IMMED32,                IMMED32,                IMMED32,                 // 0xb8
	IMMED32,                IMMED32,                IMMED32,                IMMED32,                 // 0xbc
	MODRM|IMMED8,           MODRM|IMMED8,           IMMED16,                0,                       // 0xc0
	MODRM,                  MODRM,                  MODRM|IMMED8,           MODRM|IMMED32,           // 0xc4
	IMMED24,                0,                      IMMED16,                0,                       // 0xc8
	0,                      IMMED8,                 0,                      0,                       // 0xcc
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0xd0
	IMMED8,                 IMMED8,                 0,                      0,                       // 0xd4
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0xd8
	MODRM,                  MODRM,                  MODRM,                  MODRM,                   // 0xdc
	DISP8,                  DISP8,                  DISP8,                  DISP8,                   // 0xe0
	IMMED8,                 IMMED8,                 IMMED8,                 IMMED8,                  // 0xe4
	DISP32,                 DISP32,                 FAR_ADDR,               DISP8,                   // 0xe8
	0,                      0,                      0,                      0,                       // 0xec
	PREFIX,                 PREFIX,                 PREFIX,                 PREFIX,                  // 0xf0
	0,                      0,                      MODRM|IMMED8_ON_CASE0,  MODRM|IMMED32_ON_CASE0,  // 0xf4
	0,                      0,                      0,                      0,                       // 0xf8
	0,                      0,                      MODRM,                  MODRM                    // 0xfc
	};

WORD twoByteOpcodeTable[256]={
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x0
	MODRM,        0,            0,            0,             // 0x4
	0,            0,            MODRM,        MODRM,         // 0x8
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xc
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x10
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x14
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x18
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x1c
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x20
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x24
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x28
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x2c
	0,            0,            0,            0,             // 0x30
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x34
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x38
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x3c
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x40
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x44
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x48
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x4c
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x50
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x54
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x58
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x5c
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x60
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x64
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x68
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x6c
	MODRM|IMMED8, MODRM|IMMED8, MODRM|IMMED8, MODRM|IMMED8,  // 0x70
	MODRM,        MODRM,        MODRM,        0,             // 0x74
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x78
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x7c
	DISP32,       DISP32,       DISP32,       DISP32,        // 0x80
	DISP32,       DISP32,       DISP32,       DISP32,        // 0x84
	DISP32,       DISP32,       DISP32,       DISP32,        // 0x88
	DISP32,       DISP32,       DISP32,       DISP32,        // 0x8c
	0,            MODRM,        MODRM,        MODRM,         // 0x90
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x94
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x98
	MODRM,        MODRM,        MODRM,        MODRM,         // 0x9c
	0,            0,            0,            MODRM,         // 0xa0
	MODRM|IMMED8, MODRM,        MODRM,        MODRM,         // 0xa4
	0,            0,            MODRM,        MODRM,         // 0xa8
	MODRM|IMMED8, MODRM,        MODRM,        MODRM,         // 0xac
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xb0
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xb4
	MODRM,        MODRM,        MODRM|IMMED8, MODRM,         // 0xb8
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xbc
	MODRM,        MODRM,        MODRM|IMMED8, MODRM,         // 0xc0
	MODRM|IMMED8, MODRM|IMMED8, MODRM|IMMED8, MODRM,         // 0xc4
	0,            0,            0,            0,             // 0xc8
	0,            0,            0,            0,             // 0xcc
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xd0
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xd4
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xd8
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xdc
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xe0
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xe4
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xe8
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xec
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xf0
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xf4
	MODRM,        MODRM,        MODRM,        MODRM,         // 0xf8
	MODRM,        MODRM,        MODRM,        MODRM          // 0xfc
	};

WORD modRM16Table[256]={
	0,       0,       0,       0,        // 0x0
	0,       0,       IMMED16, 0,        // 0x4
	0,       0,       0,       0,        // 0x8
	0,       0,       IMMED16, 0,        // 0xc
	0,       0,       0,       0,        // 0x10
	0,       0,       IMMED16, 0,        // 0x14
	0,       0,       0,       0,        // 0x18
	0,       0,       IMMED16, 0,        // 0x1c
	0,       0,       0,       0,        // 0x20
	0,       0,       IMMED16, 0,        // 0x24
	0,       0,       0,       0,        // 0x28
	0,       0,       IMMED16, 0,        // 0x2c
	0,       0,       0,       0,        // 0x30
	0,       0,       IMMED16, 0,        // 0x34
	0,       0,       0,       0,        // 0x38
	0,       0,       IMMED16, 0,        // 0x3c
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x40
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x44
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x48
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x4c
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x50
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x54
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x58
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x5c
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x60
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x64
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x68
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x6c
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x70
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x74
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x78
	IMMED8,  IMMED8,  IMMED8,  IMMED8,   // 0x7c
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x80
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x84
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x88
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x8c
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x90
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x94
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x98
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0x9c
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xa0
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xa4
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xa8
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xac
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xb0
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xb4
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xb8
	IMMED16, IMMED16, IMMED16, IMMED16,  // 0xbc
	0,       0,       0,       0,        // 0xc0
	0,       0,       0,       0,        // 0xc4
	0,       0,       0,       0,        // 0xc8
	0,       0,       0,       0,        // 0xcc
	0,       0,       0,       0,        // 0xd0
	0,       0,       0,       0,        // 0xd4
	0,       0,       0,       0,        // 0xd8
	0,       0,       0,       0,        // 0xdc
	0,       0,       0,       0,        // 0xe0
	0,       0,       0,       0,        // 0xe4
	0,       0,       0,       0,        // 0xe8
	0,       0,       0,       0,        // 0xec
	0,       0,       0,       0,        // 0xf0
	0,       0,       0,       0,        // 0xf4
	0,       0,       0,       0,        // 0xf8
	0,       0,       0,       0         // 0xfc
	};

WORD modRM32Table[256]={
	0,           0,           0,           0,            // 0x0
	SIB,         IMMED32,     0,           0,            // 0x4
	0,           0,           0,           0,            // 0x8
	SIB,         IMMED32,     0,           0,            // 0xc
	0,           0,           0,           0,            // 0x10
	SIB,         IMMED32,     0,           0,            // 0x14
	0,           0,           0,           0,            // 0x18
	SIB,         IMMED32,     0,           0,            // 0x1c
	0,           0,           0,           0,            // 0x20
	SIB,         IMMED32,     0,           0,            // 0x24
	0,           0,           0,           0,            // 0x28
	SIB,         IMMED32,     0,           0,            // 0x2c
	0,           0,           0,           0,            // 0x30
	SIB,         IMMED32,     0,           0,            // 0x34
	0,           0,           0,           0,            // 0x38
	SIB,         IMMED32,     0,           0,            // 0x3c
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x40
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x44
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x48
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x4c
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x50
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x54
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x58
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x5c
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x60
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x64
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x68
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x6c
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x70
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x74
	IMMED8,      IMMED8,      IMMED8,      IMMED8,       // 0x78
	SIB|IMMED8,  IMMED8,      IMMED8,      IMMED8,       // 0x7c
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0x80
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0x84
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0x88
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0x8c
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0x90
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0x94
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0x98
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0x9c
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0xa0
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0xa4
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0xa8
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0xac
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0xb0
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0xb4
	IMMED32,     IMMED32,     IMMED32,     IMMED32,      // 0xb8
	SIB|IMMED32, IMMED32,     IMMED32,     IMMED32,      // 0xbc
	0,           0,           0,           0,            // 0xc0
	0,           0,           0,           0,            // 0xc4
	0,           0,           0,           0,            // 0xc8
	0,           0,           0,           0,            // 0xcc
	0,           0,           0,           0,            // 0xd0
	0,           0,           0,           0,            // 0xd4
	0,           0,           0,           0,            // 0xd8
	0,           0,           0,           0,            // 0xdc
	0,           0,           0,           0,            // 0xe0
	0,           0,           0,           0,            // 0xe4
	0,           0,           0,           0,            // 0xe8
	0,           0,           0,           0,            // 0xec
	0,           0,           0,           0,            // 0xf0
	0,           0,           0,           0,            // 0xf4
	0,           0,           0,           0,            // 0xf8
	0,           0,           0,           0             // 0xfc
	};

DispHandlerFuncPtr mainDispHandlerTable[256]={
	NULL,          NULL,          NULL,          NULL,           // 0x0
	NULL,          NULL,          NULL,          NULL,           // 0x4
	NULL,          NULL,          NULL,          NULL,           // 0x8
	NULL,          NULL,          NULL,          NULL,           // 0xc
	NULL,          NULL,          NULL,          NULL,           // 0x10
	NULL,          NULL,          NULL,          NULL,           // 0x14
	NULL,          NULL,          NULL,          NULL,           // 0x18
	NULL,          NULL,          NULL,          NULL,           // 0x1c
	NULL,          NULL,          NULL,          NULL,           // 0x20
	NULL,          NULL,          NULL,          NULL,           // 0x24
	NULL,          NULL,          NULL,          NULL,           // 0x28
	NULL,          NULL,          NULL,          NULL,           // 0x2c
	NULL,          NULL,          NULL,          NULL,           // 0x30
	NULL,          NULL,          NULL,          NULL,           // 0x34
	NULL,          NULL,          NULL,          NULL,           // 0x38
	NULL,          NULL,          NULL,          NULL,           // 0x3c
	NULL,          NULL,          NULL,          NULL,           // 0x40
	NULL,          NULL,          NULL,          NULL,           // 0x44
	NULL,          NULL,          NULL,          NULL,           // 0x48
	NULL,          NULL,          NULL,          NULL,           // 0x4c
	NULL,          NULL,          NULL,          NULL,           // 0x50
	NULL,          NULL,          NULL,          NULL,           // 0x54
	NULL,          NULL,          NULL,          NULL,           // 0x58
	NULL,          NULL,          NULL,          NULL,           // 0x5c
	NULL,          NULL,          NULL,          NULL,           // 0x60
	NULL,          NULL,          NULL,          NULL,           // 0x64
	NULL,          NULL,          NULL,          NULL,           // 0x68
	NULL,          NULL,          NULL,          NULL,           // 0x6c
	JumpCondShort, JumpCondShort, JumpCondShort, JumpCondShort,  // 0x70
	JumpCondShort, JumpCondShort, JumpCondShort, JumpCondShort,  // 0x74
	JumpCondShort, JumpCondShort, JumpCondShort, JumpCondShort,  // 0x78
	JumpCondShort, JumpCondShort, JumpCondShort, JumpCondShort,  // 0x7c
	NULL,          NULL,          NULL,          NULL,           // 0x80
	NULL,          NULL,          NULL,          NULL,           // 0x84
	NULL,          NULL,          NULL,          NULL,           // 0x88
	NULL,          NULL,          NULL,          NULL,           // 0x8c
	NULL,          NULL,          NULL,          NULL,           // 0x90
	NULL,          NULL,          NULL,          NULL,           // 0x94
	NULL,          NULL,          NULL,          NULL,           // 0x98
	NULL,          NULL,          NULL,          NULL,           // 0x9c
	NULL,          NULL,          NULL,          NULL,           // 0xa0
	NULL,          NULL,          NULL,          NULL,           // 0xa4
	NULL,          NULL,          NULL,          NULL,           // 0xa8
	NULL,          NULL,          NULL,          NULL,           // 0xac
	NULL,          NULL,          NULL,          NULL,           // 0xb0
	NULL,          NULL,          NULL,          NULL,           // 0xb4
	NULL,          NULL,          NULL,          NULL,           // 0xb8
	NULL,          NULL,          NULL,          NULL,           // 0xbc
	NULL,          NULL,          NULL,          NULL,           // 0xc0
	NULL,          NULL,          NULL,          NULL,           // 0xc4
	NULL,          NULL,          NULL,          NULL,           // 0xc8
	NULL,          NULL,          NULL,          NULL,           // 0xcc
	NULL,          NULL,          NULL,          NULL,           // 0xd0
	NULL,          NULL,          NULL,          NULL,           // 0xd4
	NULL,          NULL,          NULL,          NULL,           // 0xd8
	NULL,          NULL,          NULL,          NULL,           // 0xdc
	LoopNZShort,   LoopZShort,    LoopShort,     JCXZShort,      // 0xe0
	NULL,          NULL,          NULL,          NULL,           // 0xe4
	CallNear,      JumpNear,      NULL,          JumpShort,      // 0xe8
	NULL,          NULL,          NULL,          NULL,           // 0xec
	NULL,          NULL,          NULL,          NULL,           // 0xf0
	NULL,          NULL,          NULL,          NULL,           // 0xf4
	NULL,          NULL,          NULL,          NULL,           // 0xf8
	NULL,          NULL,          NULL,          NULL            // 0xfc
	};

DispHandlerFuncPtr twoByteDispHandlerTable[256]={
	NULL,         NULL,         NULL,         NULL,          // 0x0
	NULL,         NULL,         NULL,         NULL,          // 0x4
	NULL,         NULL,         NULL,         NULL,          // 0x8
	NULL,         NULL,         NULL,         NULL,          // 0xc
	NULL,         NULL,         NULL,         NULL,          // 0x10
	NULL,         NULL,         NULL,         NULL,          // 0x14
	NULL,         NULL,         NULL,         NULL,          // 0x18
	NULL,         NULL,         NULL,         NULL,          // 0x1c
	NULL,         NULL,         NULL,         NULL,          // 0x20
	NULL,         NULL,         NULL,         NULL,          // 0x24
	NULL,         NULL,         NULL,         NULL,          // 0x28
	NULL,         NULL,         NULL,         NULL,          // 0x2c
	NULL,         NULL,         NULL,         NULL,          // 0x30
	NULL,         NULL,         NULL,         NULL,          // 0x34
	NULL,         NULL,         NULL,         NULL,          // 0x38
	NULL,         NULL,         NULL,         NULL,          // 0x3c
	NULL,         NULL,         NULL,         NULL,          // 0x40
	NULL,         NULL,         NULL,         NULL,          // 0x44
	NULL,         NULL,         NULL,         NULL,          // 0x48
	NULL,         NULL,         NULL,         NULL,          // 0x4c
	NULL,         NULL,         NULL,         NULL,          // 0x50
	NULL,         NULL,         NULL,         NULL,          // 0x54
	NULL,         NULL,         NULL,         NULL,          // 0x58
	NULL,         NULL,         NULL,         NULL,          // 0x5c
	NULL,         NULL,         NULL,         NULL,          // 0x60
	NULL,         NULL,         NULL,         NULL,          // 0x64
	NULL,         NULL,         NULL,         NULL,          // 0x68
	NULL,         NULL,         NULL,         NULL,          // 0x6c
	NULL,         NULL,         NULL,         NULL,          // 0x70
	NULL,         NULL,         NULL,         NULL,          // 0x74
	NULL,         NULL,         NULL,         NULL,          // 0x78
	NULL,         NULL,         NULL,         NULL,          // 0x7c
	JumpCondNear, JumpCondNear, JumpCondNear, JumpCondNear,  // 0x80
	JumpCondNear, JumpCondNear, JumpCondNear, JumpCondNear,  // 0x84
	JumpCondNear, JumpCondNear, JumpCondNear, JumpCondNear,  // 0x88
	JumpCondNear, JumpCondNear, JumpCondNear, JumpCondNear,  // 0x8c
	NULL,         NULL,         NULL,         NULL,          // 0x90
	NULL,         NULL,         NULL,         NULL,          // 0x94
	NULL,         NULL,         NULL,         NULL,          // 0x98
	NULL,         NULL,         NULL,         NULL,          // 0x9c
	NULL,         NULL,         NULL,         NULL,          // 0xa0
	NULL,         NULL,         NULL,         NULL,          // 0xa4
	NULL,         NULL,         NULL,         NULL,          // 0xa8
	NULL,         NULL,         NULL,         NULL,          // 0xac
	NULL,         NULL,         NULL,         NULL,          // 0xb0
	NULL,         NULL,         NULL,         NULL,          // 0xb4
	NULL,         NULL,         NULL,         NULL,          // 0xb8
	NULL,         NULL,         NULL,         NULL,          // 0xbc
	NULL,         NULL,         NULL,         NULL,          // 0xc0
	NULL,         NULL,         NULL,         NULL,          // 0xc4
	NULL,         NULL,         NULL,         NULL,          // 0xc8
	NULL,         NULL,         NULL,         NULL,          // 0xcc
	NULL,         NULL,         NULL,         NULL,          // 0xd0
	NULL,         NULL,         NULL,         NULL,          // 0xd4
	NULL,         NULL,         NULL,         NULL,          // 0xd8
	NULL,         NULL,         NULL,         NULL,          // 0xdc
	NULL,         NULL,         NULL,         NULL,          // 0xe0
	NULL,         NULL,         NULL,         NULL,          // 0xe4
	NULL,         NULL,         NULL,         NULL,          // 0xe8
	NULL,         NULL,         NULL,         NULL,          // 0xec
	NULL,         NULL,         NULL,         NULL,          // 0xf0
	NULL,         NULL,         NULL,         NULL,          // 0xf4
	NULL,         NULL,         NULL,         NULL,          // 0xf8
	NULL,         NULL,         NULL,         NULL           // 0xfc
	};

#endif
