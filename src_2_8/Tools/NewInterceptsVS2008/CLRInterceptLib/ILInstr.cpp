//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		ILInstr.cpp
//
// DESCRIPTION: Contains classes to manage .NET IL instructions
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the IL instruction set.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Jan 2003		 R. Wagner	 File created.
//*************************************************************************

#include "ILInstr.h"


//*************************************************************************
// Function:	CLRReplaceTokenInIL
// Description: Replaces all occurences of a token in an IL stream
// Parameters:
//	pCode - IL code to modify
//	dwLen - Length of IL code
//	table - Translation table to use
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRReplaceTokensInIL(BYTE* pCode, DWORD dwLen, CLRTranslationTable& table)
{
	DWORD count, i;
	for (DWORD offset = 0; offset < dwLen; )
	{
		switch (pCode[offset++])
		{
		case 0x0e:	// ldarg.s
		case 0x0f:	// ldarga.s
		case 0x10:	// starg.s
		case 0x11:	// ldloc.s
		case 0x12:	// ldloca.s
		case 0x13:	// stloc.s
		case 0x1f:	// ldc.i4.s
		case 0x2b:	// br.s
		case 0x2c:	// brfalse.s
		case 0x2d:	// brtrue.s
		case 0x2e:	// beq.s
		case 0x2f:	// bge.s
		case 0x30:	// bgt.s
		case 0x31:	// ble.s
		case 0x32:	// blt.s
		case 0x33:	// bne.un.s
		case 0x34:	// bge.un.s
		case 0x35:	// bgt.un.s
		case 0x36:	// ble.un.s
		case 0x37:	// blt.un.s
		case 0xde:	// leave.s
			// Additional byte follows opcode, but it is not a token
			offset++;
			break;
		case 0x20:	// ldc.i4
		case 0x22:	// ldc.r4
		case 0x38:	// br
		case 0x39:	// brfalse
		case 0x3a:	// brtrue
		case 0x3b:	// beq
		case 0x3c:	// bge
		case 0x3d:	// bgt
		case 0x3e:	// ble
		case 0x3f:	// blt
		case 0x40:	// bne.un
		case 0x41:	// bge.un
		case 0x42:	// bgt.un
		case 0x43:	// ble.un
		case 0x44:	// blt.un
		case 0xdd:	// leave
			// 32-bit integer (or 32-bit float) follows opcode, but it is not a token
			offset += 4;
			break;
		case 0x21:	// ldc.i8
		case 0x23:	// ldc.r8
			// 64-bit integer (or 64-bit float) follows opcode, but it is not a token
			offset += 8;
			break;
		case 0x27:	// jmp
		case 0x28:	// call
		case 0x29:	// calli
		case 0x6f:	// callvirt
		case 0x70:	// cpobj
		case 0x71:	// ldobj
		case 0x72:	// ldstr
		case 0x73:	// newobj
		case 0x74:	// castclass
		case 0x75:	// isinst
		case 0x79:	// unbox
		case 0x7b:	// ldfld
		case 0x7c:	// ldflda
		case 0x7d:	// stfld
		case 0x7e:	// ldsfld
		case 0x7f:	// ldsflda
		case 0x80:	// stsfld
		case 0x81:	// stobj
		case 0x8c:	// box
		case 0x8d:	// newarr
		case 0x8f:	// ldelema
		case 0xa3:  // ldelem
		case 0xa4:  // stelem
		case 0xa5:  // unbox.any
		case 0xc2:	// refanyval
		case 0xc6:	// mkrefany
		case 0xd0:	// ldtoken
			// Token follows opcode, replace the token if it is one of the ones we are looking for
			*((DWORD*)&pCode[offset]) = table.Translate(*((DWORD*)&pCode[offset]));
			offset += 4;
			break;
		case 0x45:	// switch
			// Switch opcode, 32-bit count followed by count number of 32-bit offsets
			count = *((DWORD*)&pCode[offset]);
			offset += 4;

			for (i=0; i<count; i++)
				offset += 4;
			break;
		case 0x00:	// nop
		case 0x01:	// break
		case 0x02:	// ldarg.0
		case 0x03:	// ldarg.1
		case 0x04:	// ldarg.2
		case 0x05:	// ldarg.3
		case 0x06:	// ldloc.0
		case 0x07:	// ldloc.1
		case 0x08:	// ldloc.2
		case 0x09:	// ldloc.3
		case 0x0a:	// stloc.0
		case 0x0b:	// stloc.1
		case 0x0c:	// stloc.2
		case 0x0d:	// stloc.3
		case 0x14:	// ldnull
		case 0x15:	// ld.i4.m1
		case 0x16:	// ld.i4.0
		case 0x17:	// ld.i4.1
		case 0x18:	// ld.i4.2
		case 0x19:	// ld.i4.3
		case 0x1a:	// ld.i4.4
		case 0x1b:	// ld.i4.5
		case 0x1c:	// ld.i4.6
		case 0x1d:	// ld.i4.7
		case 0x1e:	// ld.i4.8
		case 0x25:	// dup
		case 0x26:	// pop
		case 0x2a:	// ret
		case 0x46:	// ldind.i1
		case 0x47:	// ldind.u1
		case 0x48:	// ldind.i2
		case 0x49:	// ldind.u2
		case 0x4a:	// ldind.i4
		case 0x4b:	// ldind.u4
		case 0x4c:	// ldind.i8
		case 0x4d:	// ldind.i
		case 0x4e:	// ldind.r4
		case 0x4f:	// ldind.r8
		case 0x50:	// ldind.ref
		case 0x51:	// stind.ref
		case 0x52:	// stind.i1
		case 0x53:	// stind.i2
		case 0x54:	// stind.i4
		case 0x55:	// stind.i8
		case 0x56:	// stind.r4
		case 0x57:	// stind.r8
		case 0x58:	// add
		case 0x59:	// sub
		case 0x5a:	// mul
		case 0x5b:	// div
		case 0x5c:	// div.un
		case 0x5d:	// rem
		case 0x5e:	// rem.un
		case 0x5f:	// and
		case 0x60:	// or
		case 0x61:	// xor
		case 0x62:	// shl
		case 0x63:	// shr
		case 0x64:	// shr.un
		case 0x65:	// neg
		case 0x66:	// not
		case 0x67:	// conv.i1
		case 0x68:	// conv.i2
		case 0x69:	// conv.i4
		case 0x6a:	// conv.i8
		case 0x6b:	// conv.r4
		case 0x6c:	// conv.r8
		case 0x6d:	// conv.u4
		case 0x6e:	// conv.u8
		case 0x76:	// conv.r.un
		case 0x7a:	// throw
		case 0x82:	// conv.ovf.i1.un
		case 0x83:	// conv.ovf.i2.un
		case 0x84:	// conv.ovf.i4.un
		case 0x85:	// conv.ovf.i8.un
		case 0x86:	// conv.ovf.u1.un
		case 0x87:	// conv.ovf.u2.un
		case 0x88:	// conv.ovf.u4.un
		case 0x89:	// conv.ovf.u8.un
		case 0x8a:	// conv.ovf.i.un
		case 0x8b:	// conv.ovf.u.un
		case 0x8e:	// ldlen
		case 0x90:	// ldelem.i1
		case 0x91:	// ldelem.u1
		case 0x92:	// ldelem.i2
		case 0x93:	// ldelem.u2
		case 0x94:	// ldelem.i4
		case 0x95:	// ldelem.u4
		case 0x96:	// ldelem.i8
		case 0x97:	// ldelem.i
		case 0x98:	// ldelem.r4
		case 0x99:	// ldelem.r8
		case 0x9a:	// ldelem.ref
		case 0x9b:	// stelem.i
		case 0x9c:	// stelem.i1
		case 0x9d:	// stelem.i2
		case 0x9e:	// stelem.i4
		case 0x9f:	// stelem.i8
		case 0xa0:	// stelem.r4
		case 0xa1:	// stelem.r8
		case 0xa2:	// stelem.ref
		case 0xb3:	// conv.ovf.i1
		case 0xb4:	// conv.ovf.u1
		case 0xb5:	// conv.ovf.i2
		case 0xb6:	// conv.ovf.u2
		case 0xb7:	// conv.ovf.i4
		case 0xb8:	// conv.ovf.u4
		case 0xb9:	// conv.ovf.i8
		case 0xba:	// conv.ovf.u8
		case 0xc3:	// ckfinite
		case 0xd1:	// conv.u2
		case 0xd2:	// conv.u1
		case 0xd3:	// conv.i
		case 0xd4:	// conv.ovf.i
		case 0xd5:	// conv.ovf.u
		case 0xd6:	// add.ovf
		case 0xd7:	// add.ovf.un
		case 0xd8:	// mul.ovf
		case 0xd9:	// mul.ovf.un
		case 0xda:	// sub.ovf
		case 0xdb:	// sub.ovf.un
		case 0xdc:	// endfinally
		case 0xdf:	// stind.i
		case 0xe0:	// conv.u
			// Single byte opcode, nothing extra needs to be done
			break;
		case 0xfe:
			// Two byte opcodes
			switch (pCode[offset++])
			{
			case 0x06:	// ldftn
			case 0x07:	// ldvirtftn
			case 0x15:	// initobj
			case 0x16:	// constrained.
			case 0x1c:	// sizeof
				// Token follows opcode, replace the token if it is the one we are looking for
				*((DWORD*)&pCode[offset]) = table.Translate(*((DWORD*)&pCode[offset]));
				offset += 4;
				break;
			case 0x09:	// ldarg
			case 0x0a:	// ldarga
			case 0x0b:	// starg
			case 0x0c:	// ldloc
			case 0x0d:	// ldloca
			case 0x0e:	// stloc
				// 32-bit integer (or 32-bit float) follows opcode, but it is not a token
				offset += 4;
				break;
			case 0x12:	// unaligned
				// Additional byte follows opcode, but it is not a token
				offset++;
				break;
			case 0x00:	// arglist
			case 0x01:	// ceq
			case 0x02:	// cgt
			case 0x03:	// cgt.un
			case 0x04:	// clt
			case 0x05:	// clt.un
			case 0x0f:	// localloc
			case 0x11:	// endfilter
			case 0x13:	// volatile
			case 0x14:	// tail
			case 0x17:	// cpblk
			case 0x18:	// initblk
			case 0x1a:	// rethrow
			case 0x1d:	// refanytype
			case 0x1e:	// readonly
				// Two byte opcode, nothing extra needs to be done
				break;
			default:
				// Unknown opcode
				return false;
			}
			break;
		default:
			// Unknown opcode
			return false;
		}
	}
	return true;
}
