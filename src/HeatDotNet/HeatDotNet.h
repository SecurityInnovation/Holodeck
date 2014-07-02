// HeatDotNet.h

#pragma once

using namespace System;

namespace HeatDotNet
{
	[System::AttributeUsageAttribute(System::AttributeTargets::Class)]
	public __gc class InterceptTypeName
	{
		System::String* typeName;

	public:
		InterceptTypeName(System::String* name);
	};


	public __gc class Interception
	{
	public:
		[System::ThreadStaticAttribute]
		static int InInterceptionHandler = 0;

		[System::ThreadStaticAttribute]
		static int ForceOriginalCode = 0;

		static void DisableInterceptionInCurrentThread();
		static void EnableInterceptionInCurrentThread();
		static void BeginForceOriginalCode();
		static void ThrowException(System::Exception* e);
	};


	public __gc class MethodInterceptInfo
	{
	protected:
		System::Object* thisObj;

		MethodInterceptInfo(System::Object* obj);

	public:
		__property System::Object* get_ThisObject() { return thisObj; }

		virtual System::Object* Invoke(System::Object* paramObjs __gc[]) = 0;
	};
}
