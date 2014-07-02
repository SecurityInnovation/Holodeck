namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class GetCategories
{
public static bool _GetCategories( )
{
   //Parameters

   //ReturnType/Value
   System.Diagnostics.PerformanceCounterCategory[] returnVal_Real = null;
   System.Diagnostics.PerformanceCounterCategory[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.GetCategories(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.GetCategories(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
