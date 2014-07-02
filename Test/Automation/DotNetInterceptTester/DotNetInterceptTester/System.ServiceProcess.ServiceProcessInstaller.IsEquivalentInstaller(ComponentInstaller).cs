namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceProcessInstaller
{
public class IsEquivalentInstaller_System_ServiceProcess_ServiceProcessInstaller_System_Configuration_Install_ComponentInstaller
{
public static bool _IsEquivalentInstaller_System_ServiceProcess_ServiceProcessInstaller_System_Configuration_Install_ComponentInstaller( )
{
   //Parameters
   System.Configuration.Install.ComponentInstaller otherInstaller = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceProcessInstaller.IsEquivalentInstaller(otherInstaller);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceProcessInstaller.IsEquivalentInstaller(otherInstaller);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
