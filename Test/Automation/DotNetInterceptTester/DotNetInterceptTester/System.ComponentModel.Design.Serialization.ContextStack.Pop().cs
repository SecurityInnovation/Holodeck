using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.Serialization.ContextStack
{
public class Pop_System_ComponentModel_Design_Serialization_ContextStack
{
public static bool _Pop_System_ComponentModel_Design_Serialization_ContextStack( )
{

   //class object
    System.ComponentModel.Design.Serialization.ContextStack _System_ComponentModel_Design_Serialization_ContextStack = new System.ComponentModel.Design.Serialization.ContextStack();

   //Parameters

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_Design_Serialization_ContextStack.Pop();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_Design_Serialization_ContextStack.Pop();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
