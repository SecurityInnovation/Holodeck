namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class Start_System_Diagnostics_ProcessStartInfo
{
public static bool _Start_System_Diagnostics_ProcessStartInfo( )
{
   //Parameters
   System.Diagnostics.ProcessStartInfo startInfo = null;

   //ReturnType/Value
   System.Diagnostics.Process returnVal_Real = null;
   System.Diagnostics.Process returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.Start(startInfo);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.Start(startInfo);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
