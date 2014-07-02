using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.Serialization.ContextStack
{
public class Push_System_ComponentModel_Design_Serialization_ContextStack_System_Object
{
public static bool _Push_System_ComponentModel_Design_Serialization_ContextStack_System_Object( )
{

   //class object
    System.ComponentModel.Design.Serialization.ContextStack _System_ComponentModel_Design_Serialization_ContextStack = new System.ComponentModel.Design.Serialization.ContextStack();

   //Parameters
   System.Object context = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Design_Serialization_ContextStack.Push(context);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Design_Serialization_ContextStack.Push(context);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
