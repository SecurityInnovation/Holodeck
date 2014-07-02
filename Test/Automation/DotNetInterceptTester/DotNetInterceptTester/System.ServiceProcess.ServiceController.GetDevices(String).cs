namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceController
{
public class GetDevices_System_String
{
public static bool _GetDevices_System_String( )
{
   //Parameters
   System.String machineName = null;

   //ReturnType/Value
   System.ServiceProcess.ServiceController[] returnVal_Real = null;
   System.ServiceProcess.ServiceController[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceController.GetDevices(machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceController.GetDevices(machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
