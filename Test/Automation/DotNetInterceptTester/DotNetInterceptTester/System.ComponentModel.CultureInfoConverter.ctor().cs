using System;

namespace DotNetInterceptTester.My_System.ComponentModel.CultureInfoConverter
{
public class ctor_System_ComponentModel_CultureInfoConverter
{
public static bool _ctor_System_ComponentModel_CultureInfoConverter( )
{

   //class object
    System.ComponentModel.CultureInfoConverter _System_ComponentModel_CultureInfoConverter = new System.ComponentModel.CultureInfoConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_CultureInfoConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_CultureInfoConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
