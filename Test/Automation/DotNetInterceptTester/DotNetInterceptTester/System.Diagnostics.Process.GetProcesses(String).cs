namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class GetProcesses_System_String
{
public static bool _GetProcesses_System_String( )
{
   //Parameters
   System.String machineName = null;

   //ReturnType/Value
   System.Diagnostics.Process[] returnVal_Real = null;
   System.Diagnostics.Process[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.GetProcesses(machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.GetProcesses(machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
