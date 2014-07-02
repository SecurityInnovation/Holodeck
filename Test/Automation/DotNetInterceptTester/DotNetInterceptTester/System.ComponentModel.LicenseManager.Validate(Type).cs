namespace DotNetInterceptTester.My_System.ComponentModel.LicenseManager
{
public class Validate_System_Type
{
public static bool _Validate_System_Type( )
{
   //Parameters
   System.Type type = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseManager.Validate(type);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseManager.Validate(type);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
