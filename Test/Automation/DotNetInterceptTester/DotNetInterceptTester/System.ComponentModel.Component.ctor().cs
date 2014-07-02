using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Component
{
public class ctor_System_ComponentModel_Component
{
public static bool _ctor_System_ComponentModel_Component( )
{

   //class object
    System.ComponentModel.Component _System_ComponentModel_Component = new System.ComponentModel.Component();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Component.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Component.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
