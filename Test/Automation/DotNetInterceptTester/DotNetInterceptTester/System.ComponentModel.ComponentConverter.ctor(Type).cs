using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentConverter
{
public class ctor_System_ComponentModel_ComponentConverter_System_Type
{
public static bool _ctor_System_ComponentModel_ComponentConverter_System_Type( )
{

   //class object
    System.ComponentModel.ComponentConverter _System_ComponentModel_ComponentConverter = new System.ComponentModel.ComponentConverter();

   //Parameters
   System.Type type = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_ComponentConverter.ctor(type);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_ComponentConverter.ctor(type);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
