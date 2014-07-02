namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounter
{
public class ctor_System_Diagnostics_PerformanceCounter_System_String_System_String_System_String
{
public static bool _ctor_System_Diagnostics_PerformanceCounter_System_String_System_String_System_String( )
{
   //Parameters
   System.String categoryName = null;
   System.String counterName = null;
   System.String instanceName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounter.ctor(categoryName,counterName,instanceName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounter.ctor(categoryName,counterName,instanceName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
