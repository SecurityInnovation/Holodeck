namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class Create_System_String_System_String_System_String_System_String
{
public static bool _Create_System_String_System_String_System_String_System_String( )
{
   //Parameters
   System.String categoryName = null;
   System.String categoryHelp = null;
   System.String counterName = null;
   System.String counterHelp = null;

   //ReturnType/Value
   System.Diagnostics.PerformanceCounterCategory returnVal_Real = null;
   System.Diagnostics.PerformanceCounterCategory returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.Create(categoryName,categoryHelp,counterName,counterHelp);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.Create(categoryName,categoryHelp,counterName,counterHelp);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
