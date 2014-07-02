using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Container
{
public class Add_System_ComponentModel_Container_System_ComponentModel_IComponent_System_String
{
public static bool _Add_System_ComponentModel_Container_System_ComponentModel_IComponent_System_String( )
{

   //class object
    System.ComponentModel.Container _System_ComponentModel_Container = new System.ComponentModel.Container();

   //Parameters
   System.ComponentModel.IComponent component = null;
   System.String name = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Container.Add(component,name);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Container.Add(component,name);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
