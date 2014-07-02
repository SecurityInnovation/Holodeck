//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PropertiesDescriptor.h
//
// DESCRIPTION: Contains definition for the class PropertiesDescriptor
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 03 Apr 2003		 A. Kakrania	File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include "defines.h"

#using <System.dll>
#using <mscorlib.dll>
#using <FunctionDatabase.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace FunctionDatabase;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace Microsoft::Win32;

namespace UIData
{
	public __gc class PropertiesData: public PropertyDescriptor
	{
	public:
		PropertiesData(PropertyDescriptor *);
		PropertiesData(PropertyDescriptor *, String * newName, String * newDescription);
		~PropertiesData() {};

		virtual System::Boolean CanResetValue(System::Object * component);
		__property virtual Type *get_ComponentType() { return basePropertyDescriptor->ComponentType; }
		__property virtual String *get_DisplayName() { return displayname; }
		__property virtual String *get_Description() { return description; }
		virtual System::Object *GetValue(System::Object * component);
		//get_IsReadOnly always returns true; this makes the items greyed out and read only
		__property virtual System::Boolean get_IsReadOnly() { return true; }
		__property virtual String *get_Name() { return basePropertyDescriptor->Name; }
		__property virtual Type *get_PropertyType() { return basePropertyDescriptor->PropertyType; }
		virtual void ResetValue(System::Object * component);
		virtual System::Boolean ShouldSerializeValue(System::Object * component);
		virtual void SetValue(System::Object * component, System::Object * value);

	private:
		PropertyDescriptor * basePropertyDescriptor; 
		String * displayname;
		String * description;
	};

public __gc class PropertiesDescriptor : public ICustomTypeDescriptor
	{
	public:
		PropertiesDescriptor(Object * pObject);

		virtual String * GetClassName();
		virtual AttributeCollection * GetAttributes();
		virtual String * GetComponentName();
		virtual TypeConverter * GetConverter();
		virtual EventDescriptor * GetDefaultEvent();
		virtual PropertyDescriptor * GetDefaultProperty();
		virtual System::Object * GetEditor(Type * editorBaseType);
		virtual EventDescriptorCollection * GetEvents(Attribute * attributes[]);
		virtual EventDescriptorCollection * GetEvents();
		virtual PropertyDescriptorCollection * GetProperties(Attribute * attributes[]);
		virtual PropertyDescriptorCollection * GetProperties();
		virtual System::Object * GetPropertyOwner(PropertyDescriptor * pd);
	private:
		PropertyDescriptorCollection * mProperties;
		PropertiesData * filterAndDecorateProperty (PropertiesData * pDescriptor);

		Object * propObject;
		FunctionDatabase::InterceptedFunctionDB * iNativeFuncDB;
		FunctionDatabase::InterceptedFunctionDB * iDotNetFuncDB;
	};

}