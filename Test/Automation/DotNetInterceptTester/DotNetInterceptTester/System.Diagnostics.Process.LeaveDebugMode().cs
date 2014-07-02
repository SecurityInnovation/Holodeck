namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class LeaveDebugMode
{
public static bool _LeaveDebugMode( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.LeaveDebugMode(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.LeaveDebugMode(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
