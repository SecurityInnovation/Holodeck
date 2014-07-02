
//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PropertiesDescriptor.cpp
//
// DESCRIPTION: Contains implementation for the class PropertiesDescriptor
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 03 Apr 2003		 A. Kakrania	File created.
//*************************************************************************
#include "PropertiesDescriptor.h"
#include "LogPaneDataNode.h"
#include "defines.h"

namespace UIData
{
	//*************************************************************************
	// Class:		PropertiesData
	//*************************************************************************
	
	//*************************************************************************
	// Method:		PropertiesData
	// Description: Constructor for the PropertiesData class
	//
	// Parameters:
	//	pObject		- The object around which PropertiesData should wrap itself
	//*************************************************************************
	PropertiesData::PropertiesData(PropertyDescriptor * bPD): PropertyDescriptor (bPD)
	{
		basePropertyDescriptor = bPD;
		displayname = basePropertyDescriptor->DisplayName;
		description = basePropertyDescriptor->Description;
	}

	//*************************************************************************
	// Method:		PropertiesData
	// Description: Constructor for the PropertiesData class
	//
	// Parameters:
	//	pObject			- The object around which PropertiesData should wrap itself
	//	newName			- The new display name that this property should have
	//	newDescription	- The new description that this property should have
	//*************************************************************************
	PropertiesData::PropertiesData(PropertyDescriptor * bPD, String * newName, String * newDescription): PropertyDescriptor (bPD)
	{
		basePropertyDescriptor = bPD;
		displayname = newName;
		description = newDescription;
	}

	//*************************************************************************
	// Method:		CanResetValue
	// Description: See PropertyDescriptor::CanResetValue in MSDN
	//*************************************************************************
	System::Boolean PropertiesData::CanResetValue(System::Object * component)
	{
		return this->basePropertyDescriptor->CanResetValue (component);
	}

	//*************************************************************************
	// Method:		GetValue
	// Description: See PropertyDescriptor::GetValue in MSDN
	//*************************************************************************
	System::Object * PropertiesData::GetValue(System::Object * component)
	{
		return this->basePropertyDescriptor->GetValue(component);
	}

	//*************************************************************************
	// Method:		ResetValue
	// Description: See PropertyDescriptor::ResetValue in MSDN
	//*************************************************************************
	void PropertiesData::ResetValue(System::Object * component)
	{
		this->basePropertyDescriptor->ResetValue(component);
	}

	//*************************************************************************
	// Method:		ShouldSerializeValue
	// Description: See PropertyDescriptor::ShouldSerializeValue in MSDN
	//*************************************************************************
	System::Boolean PropertiesData::ShouldSerializeValue(System::Object * component)
	{
		return this->basePropertyDescriptor->ShouldSerializeValue(component);
	}
	
	//*************************************************************************
	// Method:		SetValue
	// Description: See PropertyDescriptor::SetValue in MSDN
	//*************************************************************************
	void PropertiesData::SetValue(System::Object * component, System::Object * value)
	{
		this->basePropertyDescriptor->SetValue(component, value);
	}

	//*************************************************************************
	// Class:		PropertiesData
	//*************************************************************************

	//*************************************************************************
	// Method:		PropertiesDescriptor
	// Description: Constructor for the PropertiesDescriptor class
	//
	// Parameters:
	//	pObject		- The object around which PropertiesDescriptor should wrap itself
	//
	// Return Value: None
	//*************************************************************************
	PropertiesDescriptor::PropertiesDescriptor(Object * pObject)
	{
		propObject = pObject;
		iNativeFuncDB = FunctionDatabase::InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		iDotNetFuncDB = FunctionDatabase::InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
	}

	//*************************************************************************
	// Method:		filterAndDecorateProperty 
	// Description: Filter unwanted properties and decorates others with dynamic DisplayNames
	//				and descriptions
	//
	// Parameters:
	//	pDescriptor	- The descriptor for the property to check
	//
	// Return Value: (PropertiesData*) if property should be included, returns an (possibly updated) object 
	//				 else returns NULL
	//*************************************************************************
	PropertiesData * PropertiesDescriptor::filterAndDecorateProperty (PropertiesData * pDescriptor)
	{
		//type DisplayableDataNode
		/*
		if (propObject->GetType()->ToString()->Equals (new String("UIData.DisplayableDataNode")))
		{
			
			DisplayableDataNode * dataNode = dynamic_cast <DisplayableDataNode *> (propObject);
			
			//ExtraInfo properties
			if (pDescriptor->Name->StartsWith (new String("extraInfo")))
			{
				String * eiIndexStr = "";
				for (int i=0; i< pDescriptor->Name->Length; i++)
				{
					if (Char::IsNumber(pDescriptor->Name->Chars [i]))
						eiIndexStr = String::Concat (eiIndexStr, pDescriptor->Name->Chars [i].ToString());
				}

				int eiIndex = Int32::Parse (eiIndexStr);
				
				//type faults
				if ((dataNode->Type == NodeType::MemoryFault)||(dataNode->Type == NodeType::DiskFault)||(dataNode->Type == NodeType::NetworkFault))
				{
					FaultType faultID = (FaultType) (Int32::Parse(dataNode->ID));
					RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
					
					String *installPath, *functionFolder;
					installPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
					functionFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
					functionFolder = String::Concat(installPath, "\\", functionFolder);

					InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
					if (!db->IsLoaded)
						db->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

					FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
					if (!faultDB->IsLoaded)
						faultDB->LoadDatabase(functionFolder);

					// get the fault functions
					Array *faultFunctions = faultDB->GetFunctionsWithFaultID(faultID);
					
					//If all fault functions have been processed, then hide the rest of the props.
					if (eiIndex > faultFunctions->Length)
						return NULL;

					FaultFunction *function = __try_cast<FaultFunction *> (faultFunctions->Item[eiIndex]);
					PropertiesData * newPD = new PropertiesData (pDescriptor, function->Name, "This function is modified to create this fault");
				}
				else
					return NULL;
			}
		}*/

		//type LogPaneDataNode
		if (pDescriptor->ComponentType->ToString()->Equals (new String("UIData.LogPaneDataNode")))
		{
			//Parameters
			if (pDescriptor->Name->StartsWith (new String("Param")))
			{
				//If the param is not valid then filter it from the list
				if (pDescriptor->GetValue(propObject) == NULL)
					return NULL;
				else
				{
					String * funcName = dynamic_cast <LogPaneDataNode*> (propObject)->Function;

					InterceptedFunction * ifunc = iNativeFuncDB->GetFunctionByName (funcName);
					if (!ifunc)
					{
						// Try .NET database
						ifunc = iDotNetFuncDB->GetFunctionByName(funcName);
					}

					InterceptedFunctionParameter * ifuncparam = static_cast <InterceptedFunctionParameter *> (ifunc->Parameters->Item[Int32::Parse(pDescriptor->Description)]);
					
					String * description = "Param Index: ";
					description = String::Concat (description, pDescriptor->Description);
					description = String::Concat (description, ", Type: ");
					description = String::Concat (description, ifuncparam->Type);

					String * name = pDescriptor->Name;
					name = String::Concat (name, " (");
					name = String::Concat (name, ifuncparam->Name);
					name = String::Concat (name, ")");

					PropertiesData * newPD = new PropertiesData (pDescriptor, name , description);
					return newPD;
				}
			}
		}

		//Any type
		//Hide threadID & processID when vaue == 0
		if ((String::Compare (pDescriptor->Name, "threadid", true) == 0) || (String::Compare (pDescriptor->Name, "processid", true) == 0))
		{
			if (Int32::Parse ((pDescriptor->GetValue(propObject))->ToString()) == 0)
				return NULL;
		}

		
		return pDescriptor;
				
	}

	//*************************************************************************
	// Method:		GetClassName 
	// Description: Extension for TypeDescriptor::GetClassName
	//*************************************************************************
	String * PropertiesDescriptor::GetClassName()
	{
		return TypeDescriptor::GetClassName(propObject,true);
	}

	//*************************************************************************
	// Method:		GetAttributes 
	// Description: Extension for TypeDescriptor::GetAttributes
	//*************************************************************************
	AttributeCollection * PropertiesDescriptor::GetAttributes()
	{
		return TypeDescriptor::GetAttributes(propObject,true);
	}

	//*************************************************************************
	// Method:		GetComponentName 
	// Description: Extension for TypeDescriptor::GetComponentName
	//*************************************************************************
	String * PropertiesDescriptor::GetComponentName()
	{
		return TypeDescriptor::GetComponentName(propObject, true);
	}
	
	//*************************************************************************
	// Method:		GetConverter 
	// Description: Extension for TypeDescriptor::GetConverter
	//*************************************************************************
	TypeConverter * PropertiesDescriptor::GetConverter()
	{
		return TypeDescriptor::GetConverter(propObject, true);
	}
	
	//*************************************************************************
	// Method:		GetDefaultEvent 
	// Description: Extension for TypeDescriptor::GetDefaultEvent
	//*************************************************************************
	EventDescriptor * PropertiesDescriptor::GetDefaultEvent()
	{
		return TypeDescriptor::GetDefaultEvent(propObject, true);
	}
	
	//*************************************************************************
	// Method:		GetDefaultProperty 
	// Description: Extension for TypeDescriptor::GetDefaultProperty
	//*************************************************************************
	PropertyDescriptor * PropertiesDescriptor::GetDefaultProperty()
	{
		return TypeDescriptor::GetDefaultProperty(propObject, true);
	}
	
	//*************************************************************************
	// Method:		GetEditor 
	// Description: Extension for TypeDescriptor::GetEditor
	//*************************************************************************
	System::Object * PropertiesDescriptor::GetEditor(Type * editorBaseType)
	{
		return TypeDescriptor::GetEditor(propObject, editorBaseType, true);
	}
	
	//*************************************************************************
	// Method:		GetEvents 
	// Description: Extension for TypeDescriptor::GetEvents
	//*************************************************************************
	EventDescriptorCollection * PropertiesDescriptor::GetEvents(Attribute * attributes[])
	{
		return TypeDescriptor::GetEvents(propObject, attributes, true);
	}

	//*************************************************************************
	// Method:		GetEvents 
	// Description: Extension for TypeDescriptor::GetEvents
	//*************************************************************************
	EventDescriptorCollection * PropertiesDescriptor::GetEvents()
	{
		return TypeDescriptor::GetEvents(propObject, true);
	}
	
	//*************************************************************************
	// Method:		GetProperties 
	// Description: Extension for TypeDescriptor::GetProperties
	//				Method checks for validity of property and also decorates it
	//*************************************************************************
	PropertyDescriptorCollection * PropertiesDescriptor::GetProperties(Attribute * attributes[])
	{
		if ( mProperties == NULL) 
		{
			PropertyDescriptorCollection * baseProps = TypeDescriptor::GetProperties(propObject, attributes, true);

			mProperties = new PropertyDescriptorCollection(NULL);

			for (int pCounter=0; pCounter < baseProps->Count ; pCounter++)
			{
				PropertiesData * pData = filterAndDecorateProperty (new PropertiesData(baseProps->Item[pCounter]));
				if (pData)
					mProperties->Add (pData);
			}

		}
		return mProperties;
	}
	
	//*************************************************************************
	// Method:		GetProperties 
	// Description: Extension for TypeDescriptor::GetProperties
	//				Method checks for validity of property and also decorates it
	//*************************************************************************
	PropertyDescriptorCollection * PropertiesDescriptor::GetProperties()
	{
		if ( mProperties == NULL) 
		{
			PropertyDescriptorCollection * baseProps = TypeDescriptor::GetProperties(propObject, true);
			mProperties = new PropertyDescriptorCollection(NULL);

			for (int pCounter=0; pCounter < baseProps->Count; pCounter++)
			{
				PropertiesData * pData = filterAndDecorateProperty (new PropertiesData(baseProps->Item[pCounter]));
				if (pData)
					mProperties->Add (pData);
			}

		}
		return mProperties;
	}
	
	//*************************************************************************
	// Method:		GetPropertyOwner 
	// Description: Extension for TypeDescriptor::GetPropertyOwner
	//*************************************************************************
	System::Object * PropertiesDescriptor::GetPropertyOwner(PropertyDescriptor * pd)
	{
		return propObject;
	}
}
