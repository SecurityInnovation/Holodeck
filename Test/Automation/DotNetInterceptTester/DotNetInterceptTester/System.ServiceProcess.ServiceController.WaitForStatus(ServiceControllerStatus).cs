namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceController
{
public class WaitForStatus_System_ServiceProcess_ServiceController_System_ServiceProcess_ServiceControllerStatus
{
public static bool _WaitForStatus_System_ServiceProcess_ServiceController_System_ServiceProcess_ServiceControllerStatus( )
{
   //Parameters
   System.ServiceProcess.ServiceControllerStatus desiredStatus = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceController.WaitForStatus(desiredStatus);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceController.WaitForStatus(desiredStatus);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
