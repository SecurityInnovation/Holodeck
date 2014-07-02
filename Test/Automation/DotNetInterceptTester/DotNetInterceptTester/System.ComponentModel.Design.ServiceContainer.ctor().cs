using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.ServiceContainer
{
public class ctor_System_ComponentModel_Design_ServiceContainer
{
public static bool _ctor_System_ComponentModel_Design_ServiceContainer( )
{

   //class object
    System.ComponentModel.Design.ServiceContainer _System_ComponentModel_Design_ServiceContainer = new System.ComponentModel.Design.ServiceContainer();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_ServiceContainer.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_ServiceContainer.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
