using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Component
{
public class GetLifetimeService_System_ComponentModel_Component
{
public static bool _GetLifetimeService_System_ComponentModel_Component( )
{

   //class object
    System.ComponentModel.Component _System_ComponentModel_Component = new System.ComponentModel.Component();

   //Parameters

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_Component.GetLifetimeService();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_Component.GetLifetimeService();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
