//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		BinaryDataEditor.h
//
// DESCRIPTION: Contains definition for the class BinaryDataEditor
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 6 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "HoverBalloon.h"
#include "SIContextMenu.h"

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>
#using <SiIconLib.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Text;
using namespace System::Windows::Forms;
using namespace SiIconLib;

namespace SIControls
{
	public __delegate Byte QueryDataRangeDelegate(unsigned long startAddress, unsigned long endAddress)[];
	public __delegate void BytesOverwrittenDelegate(unsigned long startAddress, unsigned long endAddress, Byte oldValue[], Byte newValue[]);
	public __delegate void BytesInsertedDelegate(unsigned long startAddress, unsigned long endAddress, Byte insertedData[]);
	public __delegate void BytesDeletedDelegate(unsigned long startAddress, unsigned long endAddress);

	//*********************************************************************
	// Represents edit modes for the editor
	//*********************************************************************
	public __value enum EditMode
	{
		Overwrite,
		Insert,
		Delete,
		None
	};

	//*********************************************************************
	// Represents an undo-able operation in the editor
	//*********************************************************************
	__gc struct UndoableOperation
	{
		unsigned long startAddress;
		unsigned long endAddress;
		EditMode editMode;
		ArrayList *changedDataBytes;
	};

	//*********************************************************************
	// Represents a set of operations that get undone/redone in a single
	// sequence
	//*********************************************************************
	__gc struct UndoableOperationSet
	{
		ArrayList *operations;
	};

	//*********************************************************************
	// Represents an onwer-drawn binary data editor
	//*********************************************************************
	__gc struct ChangedDataByte
	{
		unsigned long address;
		unsigned char originalByte;
		unsigned char currentByte;
	};

	//*********************************************************************
	// Represents an onwer-drawn binary data editor
	//*********************************************************************
	public __gc class BinaryDataEditor : public Control
	{
	public:
		BinaryDataEditor();
		~BinaryDataEditor();

		__property void set_ChangedTextColor(Color value) { changedTextColor = value; }
		__property Color get_ChangedTextColor() { return changedTextColor; }

		__property void set_InsertedTextColor(Color value) { insertedTextColor = value; }
		__property Color get_InsertedTextColor() { return insertedTextColor; }

		__property void set_EditingMode(EditMode value) { editMode = value; }
		__property EditMode get_EditingMode() { return editMode; }

		__property void set_Columns(int value);
		__property int get_Columns() { return columns; }

		__property void set_StartAddress(unsigned long value);
		__property unsigned long get_StartAddress() { return startAddress; }

		__property void set_EndAddress(unsigned long value);
		__property unsigned long get_EndAddress() { return endAddress; }

		__property void set_Visible(bool value);
		__property bool get_Visible();

		__property void set_AutoCalculateColumns(bool value);
		__property bool get_AutoCalculateColumns() { return autoCalculateNumColumns; }

		__property SIContextMenu *get_EditorContextMenu() { return contextMenu; }

		__property void set_AllowEditingModeSwitch(bool value) { allowModeSwitch = value; }
		__property bool get_AllowEditingModeSwitch() { return allowModeSwitch; }

		void MoveToCaret();
		void ScrollToAddress(unsigned long address);
		void OverwriteByte(unsigned long address, unsigned char oldData, unsigned char currentData);
		void OverwriteBytes(unsigned long startAddress, Byte oldData[], Byte currentData[]);
		void InsertByte(unsigned long address, Byte insertedData);
		void InsertBytes(unsigned long startAddress, Byte insertedData[]);
		void DeleteByte(unsigned long address);
		void DeleteBytes(unsigned long startAddress, unsigned long endAddress);
		void UndoLastOperation();
		void RedoLastOperation();
		void CutSelectedDataToClipboard();
		void CopySelectedDataToClipboard();
		void PasteDataFromClipboard();
		void DeleteSelectedData();
		Byte GetDataInRange(unsigned long startAddress, unsigned long endAddress)[];
		void SwitchEditMode();
		void SetInvalidAddressRange(unsigned long startAddress, unsigned long endAddress);
		void RemoveRangeFromInvalidAddresses(unsigned long startAddress, unsigned long endAddress);

		QueryDataRangeDelegate *OnQueryDataRange;
		BytesOverwrittenDelegate *OnBytesOverwritten;
		BytesInsertedDelegate *OnBytesInserted;
		BytesDeletedDelegate *OnBytesDeleted;

	protected:
		__property virtual unsigned long get_RowCount() { return rowCount; }
		__property virtual void set_RowCount(unsigned long value);

		static IntPtr currentCursorWindow = IntPtr::Zero;

		void OnPaint(PaintEventArgs *args);
		void OnResize(EventArgs *e);
		void OnScroll(Object *sender, EventArgs *args);
		void OnMouseMove(MouseEventArgs *args);
		void OnMouseDown(MouseEventArgs *args);
		void OnMouseUp(MouseEventArgs *args);
		void OnMouseWheel(MouseEventArgs *args);
		void OnDoubleClick(EventArgs *args);

		void OnGotFocus(EventArgs *args);
		void OnLostFocus(EventArgs *args);

		void OnKeyDown(KeyEventArgs *args);
		void OnPageKeys(KeyEventArgs *args);
		void OnUpDownKeys(KeyEventArgs *args);
		void OnLeftRightKeys(KeyEventArgs *args);
		void OnEditDataKeys(KeyEventArgs *args);
		void OnUndoKey(KeyEventArgs *args);
		void OnRedoKey(KeyEventArgs *args);
		void OnCopyKey(KeyEventArgs *args);
		void OnPasteKey(KeyEventArgs *args);
		void OnDeleteKey(KeyEventArgs *args);
		void OnBackSpaceKey(KeyEventArgs *args);

		void HideCursor();
		void ShowCursor();
		void DisableCursor();
		void EnableCursor();

		void MakeCaretVisible();

		void WndProc(Message *msg);

		unsigned int GetTopVisibleAddress();
		unsigned int GetBottomVisibleAddress();
		virtual void AdjustScrollBars();
		void RecalculateNumberOfColumns(Graphics *g);
		void DetermineSelectionAddresses(int startX, __int64 startY, int endX, __int64 endY);

		SIMenuItemState OnQueryCutMenuItemState(String *itemName, Object *tag);
		void OnCutMenuItemClicked(String *itemName, Object *tag);

		SIMenuItemState OnQueryCopyMenuItemState(String *itemName, Object *tag);
		void OnCopyMenuItemClicked(String *itemName, Object *tag);

		SIMenuItemState OnQueryPasteMenuItemState(String *itemName, Object *tag);
		void OnPasteMenuItemClicked(String *itemName, Object *tag);

		HScrollBar *hScrollBar;
		VScrollBar *vScrollBar;

		ArrayList *overwrittenAddresses;
		ArrayList *insertedAddresses;
		Stack *undoableOperationsHistory;
		Stack *redoableOperationsHistory;
		Hashtable *invalidByteAddresses;

		SIContextMenu *contextMenu;
		MenuItem *cutContextMenuItem;
		MenuItem *copyContextMenuItem;
		MenuItem *pasteContextMenuItem;

		Color changedTextColor;
		Color insertedTextColor;
		EditMode editMode;
		unsigned long selectionStartAddress;
		unsigned long selectionEndAddress;
		unsigned long caretAddress;
		unsigned long rowCount;
		int mouseDownPointForSelectionX;
		__int64 mouseDownPointForSelectionY;
		HoverBalloon *selectionSizeToolTip;
		bool selectionActive;
		bool selectionOccurring;
		bool somethingSelected;
		bool visible;
		bool autoCalculateNumColumns;
		bool caretVisible;
		bool caretEnabled;
		bool inPartialEditMode;
		bool allowModeSwitch;
		int columns;
		int rowHeight;

		int fieldSeparationWidth;
		int columnSeparationWidth;
		int dataByteWidth;
		int asciiByteWidth;
		int dataBytesFieldXOffset;
		int asciiBytesFieldXOffset;
		int totalFieldsWidth;
		int hSize;
		int vSize;
		
		unsigned long startAddress;
		unsigned long endAddress;

	private:
		UndoableOperation *partialEditOperation;

		void DoOverwrite(UndoableOperation *operation);
		void UndoOverwrite(UndoableOperation *operation);
		void DoInsert(UndoableOperation *operation);
		void UndoInsert(UndoableOperation *operation);
		void DoDelete(UndoableOperation *operation);
		void UndoDelete(UndoableOperation *operation);
	};
}