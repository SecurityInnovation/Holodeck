using System;

namespace DotNetInterceptTester.My_System.ComponentModel.DoubleConverter
{
public class ctor_System_ComponentModel_DoubleConverter
{
public static bool _ctor_System_ComponentModel_DoubleConverter( )
{

   //class object
    System.ComponentModel.DoubleConverter _System_ComponentModel_DoubleConverter = new System.ComponentModel.DoubleConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_DoubleConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_DoubleConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
