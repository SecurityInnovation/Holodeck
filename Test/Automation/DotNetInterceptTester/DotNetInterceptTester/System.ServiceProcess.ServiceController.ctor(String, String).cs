namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceController
{
public class ctor_System_ServiceProcess_ServiceController_System_String_System_String
{
public static bool _ctor_System_ServiceProcess_ServiceController_System_String_System_String( )
{
   //Parameters
   System.String name = null;
   System.String machineName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceController.ctor(name,machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceController.ctor(name,machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
