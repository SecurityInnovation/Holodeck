//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ScheduledTestDataNode.h
//
// DESCRIPTION: Contains definition for the class ScheduledTestDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "ScheduledTestDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ScheduledTestDataNode
	// Description: Constructor for the ScheduledTestDataNode class
	//
	// Parameters:
	//	Name - the name of the node
	//	pID - the process id of the process this node is associated with
	//	tID - the thread id of the thread this node is associated with
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestDataNode::ScheduledTestDataNode(String *Name, int pID, int tID) : 
		DisplayableDataNode(Name, UIData::NodeType::Test, pID, tID)
	{
		specifyErrorCode = true;
		
		//Default values for test firing
		//Run test 100% of the time and do not pause
		firingOption = 1;
		firingDetails = dynamic_cast <Object *> (S"100");
		pauseAppOnTestFire = false;

		isTestOn = true;
	}

	//*************************************************************************
	// Method:		ScheduledTestDataNode
	// Description: Copy constructor for the ScheduledTestDataNode class
	//
	// Parameters:
	//	testNode - Node to copy
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestDataNode::ScheduledTestDataNode(ScheduledTestDataNode *testNode) : 
		DisplayableDataNode(testNode)
	{
		category = testNode->Category;
		function = testNode->Function;
		returnValue = testNode->ReturnValue;
		errorCode = testNode->ErrorCode;
		specifyErrorCode = testNode->SpecifyErrorCode;
		matchParameters = testNode->MatchParameters;
		changeParameters = testNode->ChangeParameters;
		testID = testNode->TestID;
		testOwner = testNode->TestOwner;
		firingOption = testNode->FiringOption;
		firingDetails = testNode->FiringDetails;
		pauseAppOnTestFire = testNode->PauseAppOnTestFire;
		isTestOn = testNode->IsTestOn;
	}

	//*************************************************************************
	// Method:		~ScheduledTestDataNode
	// Description: Destructor for the ScheduledTestDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestDataNode::~ScheduledTestDataNode()
	{
	}

	//*************************************************************************
	// Method:		ConvertToTextFormat
	// Description: Converts the object to a string, used for pasting item into
	//				the clipboard
	//
	// Parameters:
	//	None
	//
	// Return Value: string that represents the object
	//*************************************************************************
	String * ScheduledTestDataNode::ConvertToTextFormat()
	{
		String * Text = "";
		Text = String::Concat (Text, S"Test On: ", this->isTestOn ? "True":"False", S"\r\n");
		Text = String::Concat (Text, S"Target Thread: ", Convert::ToString(this->threadID), S"\r\n");
		Text = String::Concat (Text, S"Test Function: ", this->function, S"\r\n");
		Text = String::Concat (Text, S"Return Value: ", this->returnValue, S"\r\n");
		if (this->specifyErrorCode)
			Text = String::Concat (Text, S"ErrorCode Value: ", this->errorCode, S"\r\n");
		
		//Add match param information
		Text = String::Concat (Text, S"Parameters values to match before executing test: ", S"\r\n");
		String * matchParamString = "";
		for (int i=0; i < matchParameters->Count; i++)
		{
			InterceptedFunctionParameter * ifp = dynamic_cast <InterceptedFunctionParameter*> (matchParameters->Item[i]);
			if (ifp->TestCasesCount > 0)
			{
				matchParamString = String::Concat (matchParamString, S"\t", ifp->Name);
				
				for (int tc=0; tc < ifp->TestCasesCount; tc++)
				{
					ParamTestCase * testCase = ifp->TestCase (tc);
					String * tcText = String::Concat (S" ", testCase->TestOperator, S" ", testCase->TestValue);
					if (ifp->TestCasesCount != 1 && tc != ifp->TestCasesCount - 1)
						tcText = String::Concat (tcText, S" or ");
					matchParamString = String::Concat (matchParamString, tcText);
				}

				matchParamString = String::Concat (matchParamString, S"\r\n");
			}
		}
		if (matchParamString->Length == 0)
			Text = String::Concat(Text, "\tNone\r\n");
		else
			Text = String::Concat (Text, matchParamString);
		Text = String::Concat(Text, "\r\n");

		//Add change param information
		Text = String::Concat (Text, S"Parameters values to change while executing test: ", S"\r\n");
		String *changeParamString = "";
		for (int i=0; i < changeParameters->Count; i++)
		{
			InterceptedFunctionParameter * ifp = dynamic_cast <InterceptedFunctionParameter*> (changeParameters->Item[i]);
			changeParamString = String::Concat (S"\t", ifp->Name, S": ");
			changeParamString = String::Concat (changeParamString, S"Change value to: ", ifp->ChangeValue, S"\r\n");
		}
		if (changeParamString->Length == 0)
			Text = String::Concat(Text, "\tNone\r\n");
		else
			Text = String::Concat (Text, changeParamString);
		Text = String::Concat(Text, "\r\n");

		Text = String::Concat (Text, S"Test Owner: ", this->testOwner, S"\r\n");
		Text = String::Concat (Text, S"Test will pause AUT after execution: ", this->pauseAppOnTestFire? S"Yes\r\n":S"No\r\n");

		//Add firing details information
		Text = String::Concat (Text, S"Additional test firing options:\r\n");
		switch (this->firingOption)
		{
			case 1:
				Text = String::Concat (Text, "\tTest will fire ", dynamic_cast <String *> (this->firingDetails), S"% of the time");
				break;
			case 2:
				Text = String::Concat (Text, S"\tTest will be run when function is called by one of the following:\r\n");
				break;
			case 3:
				Text = String::Concat (Text, S"\tTest will be run when callstack matches the following pattern:\r\n");
				break;
			case 4:
				Text = String::Concat (Text, S"\tTest will be run when any of the following are on the callstack:\r\n");
				break;
			default:
				break;
		}

		if (this->firingOption != 1)
		{
			ArrayList * list = dynamic_cast <ArrayList *> (this->firingDetails);
			for (int lst=0; lst <list->Count; lst++)
			{
				Text = String::Concat (Text, S"\t\t- ", dynamic_cast <String *> (list->Item[lst]));
				Text = String::Concat (Text, S"\r\n");
			}

		}
		Text = String::Concat (Text, S"\r\n");
		return Text;
	}


	//*************************************************************************
	// Method:		CopyObject
	// Description: Creates a clone of the current instance
	//
	// Parameters:
	//	None
	//
	// Return Value: A clone of the current instance
	//*************************************************************************
	ScheduledTestDataNode* ScheduledTestDataNode::GetCloneObject()
	{
		//Create a shallow copy of the members from the current instance
		ScheduledTestDataNode * copyObj = dynamic_cast <ScheduledTestDataNode *> (this->MemberwiseClone());
		
		//Overwrite properties which need deep-copying
		//Overwrite the MatchParams
		ArrayList * matchParams = this->MatchParameters ? dynamic_cast <ArrayList *> (this->MatchParameters) : NULL;
		if (matchParams)
		{
			copyObj->MatchParameters = new ArrayList (matchParams->Count);
			for (int mp=0; mp < matchParams->Count; mp++)
			{
				InterceptedFunctionParameter * ifp = (dynamic_cast <InterceptedFunctionParameter *> (matchParams->Item[mp]));
				copyObj->MatchParameters->Add (ifp->CopyObject());
			}
		}
		else
			copyObj->MatchParameters = NULL;

		//Overwrite the ChangeParams
		ArrayList * changeParams = this->ChangeParameters ? dynamic_cast <ArrayList *> (this->ChangeParameters) : NULL;
		if (changeParams)
		{
			copyObj->ChangeParameters = new ArrayList (changeParams->Count);
			for (int cp=0; cp < changeParams->Count; cp++)
			{
				InterceptedFunctionParameter * ifp = (dynamic_cast <InterceptedFunctionParameter *> (changeParams->Item[cp]));
				copyObj->ChangeParameters->Add (ifp->CopyObject());
			}
		}
		else
			copyObj->ChangeParameters = NULL;

		//Overwrite the FiringDetails
		if (this->FiringOption == 1)
			copyObj->FiringDetails = dynamic_cast <String *> (this->FiringDetails);
		else
		{
			copyObj->FiringDetails = (dynamic_cast<ArrayList *> (this->FiringDetails))->Clone();
		}

		return copyObj;
	}
}