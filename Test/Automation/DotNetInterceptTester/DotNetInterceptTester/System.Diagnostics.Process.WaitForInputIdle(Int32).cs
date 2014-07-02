namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class WaitForInputIdle_System_Diagnostics_Process_System_Int32
{
public static bool _WaitForInputIdle_System_Diagnostics_Process_System_Int32( )
{
   //Parameters
   System.Int32 milliseconds = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.WaitForInputIdle(milliseconds);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.WaitForInputIdle(milliseconds);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
