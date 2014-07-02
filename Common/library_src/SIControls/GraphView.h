//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GraphView.h
//
// DESCRIPTION: Contains definition for the class GraphView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 23 Jul 2004		 R. Wagner	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>

#include "GraphNodeCollection.h"

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Reflection;
using namespace System::Collections::Specialized;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	public __gc class GraphView: public Control
	{
	private:
		__gc struct GraphNodeDisplayInfo
		{
			bool dirty;
			int x, y, w, h;
			int colWidth __gc[];
		};

		GraphNodeCollection* nodes;
		Hashtable* dispInfo;

		Color defaultBackColor, defaultForeColor;
		System::Drawing::Size pageSize;
		int borderPad, nodePad, colPad;
		int rowHeight;

		void OnGraphNodeRemoved(GraphNode* node);
		void OnGraphNodeAdded(GraphNode* node);

		void UpdateLayout(bool invalidateInfo);

		void MeasureNode(GraphNode* node, GraphNodeDisplayInfo* info);
		void DrawNode(Graphics* g, GraphNode* node, GraphNodeDisplayInfo* info, int scrollX, int scrollY);
		void AdjustScrollBars();

		int GetScrollPosition(int bar);
		void SetScrollPosition(int bar, int pos);
		int GetScrollTrackPosition(int bar);
		void SetScrollPageSize(int width, int height);
		void SetScrollRange(int minX, int maxX, int minY, int maxY);

		void HScroll(int type);
		void VScroll(int type);

	protected:
		void WndProc(Message *msg);
		void OnPaint(PaintEventArgs* e);
		void OnResize(EventArgs* e);
		void OnFontChanged(EventArgs* e);

	public:
		GraphView();
		~GraphView();

		__property GraphNodeCollection* get_Nodes() { return nodes; }

		__property Color get_DefaultNodeBackColor() { return defaultBackColor; }
		__property void set_DefaultNodeBackColor(Color value) { defaultBackColor = value; }

		__property Color get_DefaultNodeForeColor() { return defaultForeColor; }
		__property void set_DefaultNodeForeColor(Color value) { defaultForeColor = value; }

		__property int get_BorderPadding() { return borderPad; }
		__property void set_BorderPadding(int value) { borderPad = value; }

		__property int get_NodePadding() { return nodePad; }
		__property void set_NodePadding(int value) { nodePad = value; }

		__property int get_ColumnPadding() { return colPad; }
		__property void set_ColumnPadding(int value) { colPad = value; }

		__property int get_NodeRowHeight() { return rowHeight; }
		__property void set_NodeRowHeight(int value) { rowHeight = value; }
	};
}
