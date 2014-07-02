namespace DotNetInterceptTester.My_System.ComponentModel.UInt64Converter
{
public class ctor_System_ComponentModel_UInt64Converter
{
public static bool _ctor_System_ComponentModel_UInt64Converter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.UInt64Converter.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.UInt64Converter.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
