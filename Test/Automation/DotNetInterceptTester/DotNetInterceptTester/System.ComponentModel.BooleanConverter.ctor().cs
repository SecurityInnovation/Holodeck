using System;

namespace DotNetInterceptTester.My_System.ComponentModel.BooleanConverter
{
public class ctor_System_ComponentModel_BooleanConverter
{
public static bool _ctor_System_ComponentModel_BooleanConverter( )
{

   //class object
    System.ComponentModel.BooleanConverter _System_ComponentModel_BooleanConverter = new System.ComponentModel.BooleanConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_BooleanConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_BooleanConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
