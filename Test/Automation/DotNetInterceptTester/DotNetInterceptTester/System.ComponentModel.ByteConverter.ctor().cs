using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ByteConverter
{
public class ctor_System_ComponentModel_ByteConverter
{
public static bool _ctor_System_ComponentModel_ByteConverter( )
{

   //class object
    System.ComponentModel.ByteConverter _System_ComponentModel_ByteConverter = new System.ComponentModel.ByteConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_ByteConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_ByteConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
