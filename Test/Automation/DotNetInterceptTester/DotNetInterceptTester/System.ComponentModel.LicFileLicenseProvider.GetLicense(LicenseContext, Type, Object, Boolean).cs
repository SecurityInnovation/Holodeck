namespace DotNetInterceptTester.My_System.ComponentModel.LicFileLicenseProvider
{
public class GetLicense_System_ComponentModel_LicFileLicenseProvider_System_ComponentModel_LicenseContext_System_Type_System_Object_System_Boolean
{
public static bool _GetLicense_System_ComponentModel_LicFileLicenseProvider_System_ComponentModel_LicenseContext_System_Type_System_Object_System_Boolean( )
{
   //Parameters
   System.ComponentModel.LicenseContext context = null;
   System.Type type = null;
   System.Object instance = null;
   System.Boolean allowExceptions = null;

   //ReturnType/Value
   System.ComponentModel.License returnVal_Real = null;
   System.ComponentModel.License returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicFileLicenseProvider.GetLicense(context,type,instance,allowExceptions);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicFileLicenseProvider.GetLicense(context,type,instance,allowExceptions);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
