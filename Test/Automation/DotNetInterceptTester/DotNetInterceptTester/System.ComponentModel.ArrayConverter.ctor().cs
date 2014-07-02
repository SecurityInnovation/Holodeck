using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ArrayConverter
{
public class ctor_System_ComponentModel_ArrayConverter
{
public static bool _ctor_System_ComponentModel_ArrayConverter( )
{

   //class object
    System.ComponentModel.ArrayConverter _System_ComponentModel_ArrayConverter = new System.ComponentModel.ArrayConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_ArrayConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_ArrayConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
