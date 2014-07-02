using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeConverter
{
public class ctor_System_ComponentModel_TypeConverter
{
public static bool _ctor_System_ComponentModel_TypeConverter( )
{

   //class object
    System.ComponentModel.TypeConverter _System_ComponentModel_TypeConverter = new System.ComponentModel.TypeConverter();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_TypeConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_TypeConverter.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
