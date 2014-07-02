using System;

namespace DotNetInterceptTester.My_System.ComponentModel.DecimalConverter
{
public class ctor_System_ComponentModel_DecimalConverter
{
public static bool _ctor_System_ComponentModel_DecimalConverter( )
{

   //class object
    System.ComponentModel.DecimalConverter _System_ComponentModel_DecimalConverter = new System.ComponentModel.DecimalConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_DecimalConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_DecimalConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
