using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Container
{
public class Remove_System_ComponentModel_Container_System_ComponentModel_IComponent
{
public static bool _Remove_System_ComponentModel_Container_System_ComponentModel_IComponent( )
{

   //class object
    System.ComponentModel.Container _System_ComponentModel_Container = new System.ComponentModel.Container();

   //Parameters
   System.ComponentModel.IComponent component = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Container.Remove(component);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Container.Remove(component);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
