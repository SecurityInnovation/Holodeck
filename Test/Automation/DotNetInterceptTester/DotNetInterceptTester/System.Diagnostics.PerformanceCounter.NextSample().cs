namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounter
{
public class NextSample_System_Diagnostics_PerformanceCounter
{
public static bool _NextSample_System_Diagnostics_PerformanceCounter( )
{
   //Parameters

   //ReturnType/Value
   System.Diagnostics.CounterSample returnVal_Real = null;
   System.Diagnostics.CounterSample returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounter.NextSample();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounter.NextSample();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
