namespace DotNetInterceptTester.My_System.ComponentModel.LicenseManager
{
public class CreateWithContext_System_Type_System_ComponentModel_LicenseContext
{
public static bool _CreateWithContext_System_Type_System_ComponentModel_LicenseContext( )
{
   //Parameters
   System.Type type = null;
   System.ComponentModel.LicenseContext creationContext = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseManager.CreateWithContext(type,creationContext);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseManager.CreateWithContext(type,creationContext);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
