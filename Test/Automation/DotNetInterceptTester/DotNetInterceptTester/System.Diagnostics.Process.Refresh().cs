namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class Refresh_System_Diagnostics_Process
{
public static bool _Refresh_System_Diagnostics_Process( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.Refresh();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.Refresh();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
