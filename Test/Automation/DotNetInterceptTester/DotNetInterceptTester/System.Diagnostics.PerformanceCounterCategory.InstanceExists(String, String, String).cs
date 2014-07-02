namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class InstanceExists_System_String_System_String_System_String
{
public static bool _InstanceExists_System_String_System_String_System_String( )
{
   //Parameters
   System.String instanceName = null;
   System.String categoryName = null;
   System.String machineName = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.InstanceExists(instanceName,categoryName,machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.InstanceExists(instanceName,categoryName,machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
