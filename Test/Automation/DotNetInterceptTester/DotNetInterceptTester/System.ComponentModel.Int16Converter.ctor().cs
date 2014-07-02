using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Int16Converter
{
public class ctor_System_ComponentModel_Int16Converter
{
public static bool _ctor_System_ComponentModel_Int16Converter( )
{

   //class object
    System.ComponentModel.Int16Converter _System_ComponentModel_Int16Converter = new System.ComponentModel.Int16Converter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Int16Converter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Int16Converter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
