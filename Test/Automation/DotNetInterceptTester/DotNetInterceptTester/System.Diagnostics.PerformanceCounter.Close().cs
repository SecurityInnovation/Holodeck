namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounter
{
public class Close_System_Diagnostics_PerformanceCounter
{
public static bool _Close_System_Diagnostics_PerformanceCounter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounter.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounter.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
