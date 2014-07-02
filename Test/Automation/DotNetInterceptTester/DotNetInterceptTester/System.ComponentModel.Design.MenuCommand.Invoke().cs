using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.MenuCommand
{
public class Invoke_System_ComponentModel_Design_MenuCommand
{
public static bool _Invoke_System_ComponentModel_Design_MenuCommand( )
{

   //class object
    System.ComponentModel.Design.MenuCommand _System_ComponentModel_Design_MenuCommand = new System.ComponentModel.Design.MenuCommand();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_MenuCommand.Invoke();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_MenuCommand.Invoke();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
