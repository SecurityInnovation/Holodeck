namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class ctor_System_Diagnostics_PerformanceCounterCategory_System_String_System_String
{
public static bool _ctor_System_Diagnostics_PerformanceCounterCategory_System_String_System_String( )
{
   //Parameters
   System.String categoryName = null;
   System.String machineName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.ctor(categoryName,machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.ctor(categoryName,machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
