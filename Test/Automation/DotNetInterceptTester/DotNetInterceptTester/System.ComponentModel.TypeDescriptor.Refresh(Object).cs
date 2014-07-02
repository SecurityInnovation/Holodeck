using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeDescriptor
{
public class Refresh_System_Object
{
public static bool _Refresh_System_Object( )
{

   //class object
    System.ComponentModel.TypeDescriptor _System_ComponentModel_TypeDescriptor = new System.ComponentModel.TypeDescriptor();

   //Parameters
   System.Object component = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_TypeDescriptor.Refresh(component);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_TypeDescriptor.Refresh(component);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
