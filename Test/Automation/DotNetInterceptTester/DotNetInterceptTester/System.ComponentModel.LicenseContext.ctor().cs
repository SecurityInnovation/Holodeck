namespace DotNetInterceptTester.My_System.ComponentModel.LicenseContext
{
public class ctor_System_ComponentModel_LicenseContext
{
public static bool _ctor_System_ComponentModel_LicenseContext( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseContext.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseContext.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
