namespace DotNetInterceptTester.My_System.Diagnostics.ProcessThread
{
public class Dispose_System_Diagnostics_ProcessThread
{
public static bool _Dispose_System_Diagnostics_ProcessThread( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.ProcessThread.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.ProcessThread.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
