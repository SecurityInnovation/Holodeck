using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.StandardToolWindows
{
public class ctor_System_ComponentModel_Design_StandardToolWindows
{
public static bool _ctor_System_ComponentModel_Design_StandardToolWindows( )
{

   //class object
    System.ComponentModel.Design.StandardToolWindows _System_ComponentModel_Design_StandardToolWindows = new System.ComponentModel.Design.StandardToolWindows();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_StandardToolWindows.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_StandardToolWindows.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
