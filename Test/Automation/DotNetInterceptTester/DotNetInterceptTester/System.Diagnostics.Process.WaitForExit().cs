namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class WaitForExit_System_Diagnostics_Process
{
public static bool _WaitForExit_System_Diagnostics_Process( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.WaitForExit();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.WaitForExit();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
