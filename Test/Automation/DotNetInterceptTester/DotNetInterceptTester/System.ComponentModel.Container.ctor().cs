using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Container
{
public class ctor_System_ComponentModel_Container
{
public static bool _ctor_System_ComponentModel_Container( )
{

   //class object
    System.ComponentModel.Container _System_ComponentModel_Container = new System.ComponentModel.Container();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Container.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Container.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
