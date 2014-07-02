namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounter
{
public class CloseSharedResources
{
public static bool _CloseSharedResources( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounter.CloseSharedResources(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounter.CloseSharedResources(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
