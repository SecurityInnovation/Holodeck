namespace DotNetInterceptTester.My_System.ComponentModel.LicenseContext
{
public class SetSavedLicenseKey_System_ComponentModel_LicenseContext_System_Type_System_String
{
public static bool _SetSavedLicenseKey_System_ComponentModel_LicenseContext_System_Type_System_String( )
{
   //Parameters
   System.Type type = null;
   System.String key = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseContext.SetSavedLicenseKey(type,key);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseContext.SetSavedLicenseKey(type,key);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
