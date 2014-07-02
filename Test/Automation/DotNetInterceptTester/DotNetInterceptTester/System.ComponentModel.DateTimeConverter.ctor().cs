using System;

namespace DotNetInterceptTester.My_System.ComponentModel.DateTimeConverter
{
public class ctor_System_ComponentModel_DateTimeConverter
{
public static bool _ctor_System_ComponentModel_DateTimeConverter( )
{

   //class object
    System.ComponentModel.DateTimeConverter _System_ComponentModel_DateTimeConverter = new System.ComponentModel.DateTimeConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_DateTimeConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_DateTimeConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
