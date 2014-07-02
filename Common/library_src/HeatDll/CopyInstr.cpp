#include <windows.h>
#include "CopyInstr.h"
#include "InstrTable.h"

#define TWO_BYTE_OPCODE 0xf
#define ADDRESS_SIZE_OPCODE 0x67
#define CALL_NEAR_OPCODE 0xe8
#define JUMP_NEAR_OPCODE 0xe9
#define JUMP_SHORT_OPCODE 0xeb
#define JUMP_COND_NEAR_OPCODE 0x80
#define JCXZ_OPCODE 0xe3
#define LOOP_OPCODE 0xe2
#define LOOPZ_OPCODE 0xe1
#define LOOPNZ_OPCODE 0xe0

inline BYTE readByte(PBYTE& src)
{
	return *(src++);
}

inline WORD readWord(PBYTE& src)
{
	WORD val=(WORD)readByte(src);
	val|=((WORD)readByte(src))<<8;
	return val;
}

inline DWORD readDWord(PBYTE& src)
{
	DWORD val=(DWORD)readWord(src);
	val|=((DWORD)readWord(src))<<16;
	return val;
}

inline void writeByte(PBYTE& dest,BYTE n)
{
	if (dest)
		*(dest++)=n;
}

inline void writeWord(PBYTE& dest,WORD n)
{
	writeByte(dest,n&0xff);
	writeByte(dest,(n>>8)&0xff);
}

inline void writeDWord(PBYTE& dest,DWORD n)
{
	writeWord(dest,(WORD)(n&0xffff));
	writeWord(dest,(WORD)((n>>16)&0xffff));
}

inline void copyByte(PBYTE& dest, PBYTE& src)
{
	writeByte(dest,readByte(src));
}

inline void copyWord(PBYTE& dest, PBYTE& src)
{
	writeWord(dest,readWord(src));
}

inline void copyDWord(PBYTE& dest, PBYTE& src)
{
	writeDWord(dest,readDWord(src));
}

inline void copyOpSize(PBYTE& dest, PBYTE& src, int opSize)
{
	copyWord(dest,src);
	if (opSize==32)
		copyWord(dest,src);
}

// Handler to relocate near jump instructions
void JumpNear(PBYTE& dest, PBYTE& src, int addrSize)
{
	long offset=(long)readDWord(src);
	PBYTE destAddr=src+offset;
	// Output jump near instruction
	if (addrSize==16) writeByte(dest,ADDRESS_SIZE_OPCODE);
	writeByte(dest,JUMP_NEAR_OPCODE);
	writeDWord(dest,destAddr-(dest+4));
}

// Handler to relocate short jump instructions
void JumpShort(PBYTE& dest, PBYTE& src, int addrSize)
{
	long offset=(long)((char)readByte(src));
	PBYTE destAddr=src+offset;
	// Output jump near instruction
	if (addrSize==16) writeByte(dest,ADDRESS_SIZE_OPCODE);
	writeByte(dest,JUMP_NEAR_OPCODE);
	writeDWord(dest,destAddr-(dest+4));
}

// Handler to relocate conditional near jump instructions
void JumpCondNear(PBYTE& dest, PBYTE& src, int addrSize)
{
	int conditionCode=(*(src-1))&0xf;
	long offset=(long)readDWord(src);
	PBYTE destAddr=src+offset;
	// Output conditional jump near instruction
	if (addrSize==16) writeByte(dest,ADDRESS_SIZE_OPCODE);
	writeByte(dest,TWO_BYTE_OPCODE);
	writeByte(dest,JUMP_COND_NEAR_OPCODE|conditionCode);
	writeDWord(dest,destAddr-(dest+4));
}

// Handler to relocate conditional short jump instructions
void JumpCondShort(PBYTE& dest, PBYTE& src, int addrSize)
{
	int conditionCode=(*(src-1))&0xf;
	long offset=(long)((char)readByte(src));
	PBYTE destAddr=src+offset;
	// Output conditional jump near instruction
	if (addrSize==16) writeByte(dest,ADDRESS_SIZE_OPCODE);
	writeByte(dest,TWO_BYTE_OPCODE);
	writeByte(dest,JUMP_COND_NEAR_OPCODE|conditionCode);
	writeDWord(dest,destAddr-(dest+4));
}

// Handler to relocate near call instructions
void CallNear(PBYTE& dest, PBYTE& src, int addrSize)
{
	long offset=(long)readDWord(src);
	PBYTE destAddr=src+offset;
	// Output call near instruction
	if (addrSize==16) writeByte(dest,ADDRESS_SIZE_OPCODE);
	writeByte(dest,CALL_NEAR_OPCODE);
	writeDWord(dest,destAddr-(dest+4));
}

inline void LoopingInstructionRelocate(PBYTE& dest, PBYTE& src, int addrSize, int opcode)
{
	long offset=(long)((char)readByte(src));
	PBYTE destAddr=src+offset;
	// Ouput following code to simulate a near form of the looping instruction
	//   [Loop instr] JumpTaken
	//   JMP  JumpNotTaken  (short jump)
	// JumpTaken:
	//   JMP  [Actual JCXZ destination]  (near jump)
	// JumpNotTaken:
	writeByte(dest,opcode);
	writeByte(dest,2); // JumpTaken label is 2 bytes after loop instruction
	writeByte(dest,JUMP_SHORT_OPCODE);
	writeByte(dest,5); // Near jump is 5 bytes long
	writeByte(dest,JUMP_NEAR_OPCODE);
	writeDWord(dest,destAddr-(dest+4));
}

// Handler to relocate JCXZ instructions
void JCXZShort(PBYTE& dest, PBYTE& src, int addrSize)
{
	LoopingInstructionRelocate(dest,src,addrSize,JCXZ_OPCODE);
}

// Handler to relocate LOOP instructions
void LoopShort(PBYTE& dest, PBYTE& src, int addrSize)
{
	LoopingInstructionRelocate(dest,src,addrSize,LOOP_OPCODE);
}

// Handler to relocate LOOPZ instructions
void LoopZShort(PBYTE& dest, PBYTE& src, int addrSize)
{
	LoopingInstructionRelocate(dest,src,addrSize,LOOPZ_OPCODE);
}

// Handler to relocate LOOPNZ instructions
void LoopNZShort(PBYTE& dest, PBYTE& src, int addrSize)
{
	LoopingInstructionRelocate(dest,src,addrSize,LOOPNZ_OPCODE);
}

PBYTE copyNextOpcode(PBYTE dest, PBYTE src, PBYTE* updatedSrcPtr, int addrSize=32, int opSize=32, BOOL twoByteOp=false)
{
	int opcode=readByte(src);
	if (updatedSrcPtr) *(updatedSrcPtr)=src;
	WORD flags=twoByteOp ? twoByteOpcodeTable[opcode] : mainOpcodeTable[opcode];

	// Check for two byte opcode prefix, if not already found
	if ((!twoByteOp) && (opcode==TWO_BYTE_OPCODE))
		return copyNextOpcode(dest,src,updatedSrcPtr,addrSize,opSize,true);

	// Check for relative displacement instructions first, and execute handler function
	// if needed
	if (flags&DISP8)
	{
		if (twoByteOp)
		{
			if (twoByteDispHandlerTable[opcode])
			{
				twoByteDispHandlerTable[opcode](dest,src,addrSize);
				if (updatedSrcPtr) *(updatedSrcPtr)=src;
				return dest;
			}
		}
		else
		{
			if (mainDispHandlerTable[opcode])
			{
				mainDispHandlerTable[opcode](dest,src,addrSize);
				if (updatedSrcPtr) *(updatedSrcPtr)=src;
				return dest;
			}
		}
		// Unhandled 8-bit relative displacement opcode
		writeByte(dest,opcode);
		copyByte(dest,src);
		if (updatedSrcPtr) *(updatedSrcPtr)=src;
		return dest;
	}
	else if (flags&DISP32)
	{
		if (twoByteOp)
		{
			if (twoByteDispHandlerTable[opcode])
			{
				twoByteDispHandlerTable[opcode](dest,src,addrSize);
				if (updatedSrcPtr) *(updatedSrcPtr)=src;
				return dest;
			}
		}
		else
		{
			if (mainDispHandlerTable[opcode])
			{
				mainDispHandlerTable[opcode](dest,src,addrSize);
				if (updatedSrcPtr) *(updatedSrcPtr)=src;
				return dest;
			}
		}
		// Unhandled 32-bit relative displacement opcode
		writeByte(dest,opcode);
		copyOpSize(dest,src,addrSize);
		if (updatedSrcPtr) *(updatedSrcPtr)=src;
		return dest;
	}

	// Check for prefixes
	if (flags&ADDR_SIZE_PREFIX)
	{
		writeByte(dest,opcode);
		addrSize=(addrSize==32)?16:32;
		return copyNextOpcode(dest,src,updatedSrcPtr,addrSize,opSize,twoByteOp);
	}
	else if (flags&OP_SIZE_PREFIX)
	{
		writeByte(dest,opcode);
		opSize=(opSize==32)?16:32;
		return copyNextOpcode(dest,src,updatedSrcPtr,addrSize,opSize,twoByteOp);
	}
	else if (flags&PREFIX)
	{
		writeByte(dest,opcode);
		return copyNextOpcode(dest,src,updatedSrcPtr,addrSize,opSize,twoByteOp);
	}

	// Output two byte prefix if needed, and then opcode
	if (twoByteOp)
		writeByte(dest,TWO_BYTE_OPCODE);
	writeByte(dest,opcode);

	// Check for Mod/RM byte
	if (flags&MODRM)
	{
		WORD rmFlags;
		opcode=readByte(src);
		writeByte(dest,opcode);
		if (addrSize==32)
			rmFlags=modRM32Table[opcode];
		else
			rmFlags=modRM16Table[opcode];
		if (rmFlags&SIB)
			copyByte(dest,src);
		if (rmFlags&IMMED8)
			copyByte(dest,src);
		else if (rmFlags&IMMED16)
			copyWord(dest,src);
		else if (rmFlags&IMMED32)
			copyDWord(dest,src);
		else if ((rmFlags&SIB) && (((*(src-1))&7) == 5)) // special immed32 for mod=0 sib byte
			copyDWord(dest,src);
		if ((flags&IMMED8_ON_CASE0) && (((opcode>>3)&7) == 0))
			copyByte(dest,src);
		else if ((flags&IMMED32_ON_CASE0) && (((opcode>>3)&7) == 0))
			copyOpSize(dest,src,opSize);
	}

	// Check for immediate values
	if (flags&IMMED8)
		copyByte(dest,src);
	else if (flags&IMMED16)
		copyByte(dest,src);
	else if (flags&IMMED24)
	{
		copyWord(dest,src);
		copyByte(dest,src);
	}
	else if (flags&IMMED32)
		copyOpSize(dest,src,opSize);
	else if (flags&NEAR_ADDR)
		copyOpSize(dest,src,addrSize);
	else if (flags&FAR_ADDR)
	{
		copyWord(dest,src);
		copyOpSize(dest,src,addrSize);
	}
	if (updatedSrcPtr)
		*(updatedSrcPtr)=src;
	return dest;
}

PBYTE copyInstruction(PBYTE dest, PBYTE src, PBYTE* updatedSrcPtr)
{
	return copyNextOpcode(dest,src,updatedSrcPtr);
}

