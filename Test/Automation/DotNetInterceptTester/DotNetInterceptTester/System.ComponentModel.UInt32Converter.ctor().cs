using System;

namespace DotNetInterceptTester.My_System.ComponentModel.UInt32Converter
{
public class ctor_System_ComponentModel_UInt32Converter
{
public static bool _ctor_System_ComponentModel_UInt32Converter( )
{

   //class object
    System.ComponentModel.UInt32Converter _System_ComponentModel_UInt32Converter = new System.ComponentModel.UInt32Converter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_UInt32Converter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_UInt32Converter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
