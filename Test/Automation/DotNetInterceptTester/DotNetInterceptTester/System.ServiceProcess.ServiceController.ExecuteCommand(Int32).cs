namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceController
{
public class ExecuteCommand_System_ServiceProcess_ServiceController_System_Int32
{
public static bool _ExecuteCommand_System_ServiceProcess_ServiceController_System_Int32( )
{
   //Parameters
   System.Int32 command = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceController.ExecuteCommand(command);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceController.ExecuteCommand(command);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
