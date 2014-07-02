using System;

namespace DotNetInterceptTester.My_System.ComponentModel.CharConverter
{
public class ctor_System_ComponentModel_CharConverter
{
public static bool _ctor_System_ComponentModel_CharConverter( )
{

   //class object
    System.ComponentModel.CharConverter _System_ComponentModel_CharConverter = new System.ComponentModel.CharConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_CharConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_CharConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
