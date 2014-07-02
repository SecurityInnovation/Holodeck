using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.Serialization.ContextStack
{
public class ctor_System_ComponentModel_Design_Serialization_ContextStack
{
public static bool _ctor_System_ComponentModel_Design_Serialization_ContextStack( )
{

   //class object
    System.ComponentModel.Design.Serialization.ContextStack _System_ComponentModel_Design_Serialization_ContextStack = new System.ComponentModel.Design.Serialization.ContextStack();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_Serialization_ContextStack.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_Serialization_ContextStack.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
