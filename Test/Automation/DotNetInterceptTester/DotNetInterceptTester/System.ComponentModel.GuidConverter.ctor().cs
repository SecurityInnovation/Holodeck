using System;

namespace DotNetInterceptTester.My_System.ComponentModel.GuidConverter
{
public class ctor_System_ComponentModel_GuidConverter
{
public static bool _ctor_System_ComponentModel_GuidConverter( )
{

   //class object
    System.ComponentModel.GuidConverter _System_ComponentModel_GuidConverter = new System.ComponentModel.GuidConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_GuidConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_GuidConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
