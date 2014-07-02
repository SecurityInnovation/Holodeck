using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TimeSpanConverter
{
public class ctor_System_ComponentModel_TimeSpanConverter
{
public static bool _ctor_System_ComponentModel_TimeSpanConverter( )
{

   //class object
    System.ComponentModel.TimeSpanConverter _System_ComponentModel_TimeSpanConverter = new System.ComponentModel.TimeSpanConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_TimeSpanConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_TimeSpanConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
