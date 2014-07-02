namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounter
{
public class Increment_System_Diagnostics_PerformanceCounter
{
public static bool _Increment_System_Diagnostics_PerformanceCounter( )
{
   //Parameters

   //ReturnType/Value
   System.Int64 returnVal_Real = null;
   System.Int64 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounter.Increment();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounter.Increment();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
