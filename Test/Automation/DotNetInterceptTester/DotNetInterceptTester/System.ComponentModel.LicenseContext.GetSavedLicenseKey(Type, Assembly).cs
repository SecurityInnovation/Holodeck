namespace DotNetInterceptTester.My_System.ComponentModel.LicenseContext
{
public class GetSavedLicenseKey_System_ComponentModel_LicenseContext_System_Type_System_Reflection_Assembly
{
public static bool _GetSavedLicenseKey_System_ComponentModel_LicenseContext_System_Type_System_Reflection_Assembly( )
{
   //Parameters
   System.Type type = null;
   System.Reflection.Assembly resourceAssembly = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseContext.GetSavedLicenseKey(type,resourceAssembly);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseContext.GetSavedLicenseKey(type,resourceAssembly);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
