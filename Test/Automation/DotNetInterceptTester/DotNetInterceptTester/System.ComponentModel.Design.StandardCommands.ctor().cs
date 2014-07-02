using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.StandardCommands
{
public class ctor_System_ComponentModel_Design_StandardCommands
{
public static bool _ctor_System_ComponentModel_Design_StandardCommands( )
{

   //class object
    System.ComponentModel.Design.StandardCommands _System_ComponentModel_Design_StandardCommands = new System.ComponentModel.Design.StandardCommands();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_StandardCommands.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_StandardCommands.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
