namespace DotNetInterceptTester.My_System.ComponentModel.LicenseManager
{
public class UnlockContext_System_Object
{
public static bool _UnlockContext_System_Object( )
{
   //Parameters
   System.Object contextUser = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseManager.UnlockContext(contextUser);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseManager.UnlockContext(contextUser);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
