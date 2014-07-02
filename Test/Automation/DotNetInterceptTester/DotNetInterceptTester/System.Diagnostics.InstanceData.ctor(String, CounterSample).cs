namespace DotNetInterceptTester.My_System.Diagnostics.InstanceData
{
public class ctor_System_Diagnostics_InstanceData_System_String_System_Diagnostics_CounterSample
{
public static bool _ctor_System_Diagnostics_InstanceData_System_String_System_Diagnostics_CounterSample( )
{
   //Parameters
   System.String instanceName = null;
   System.Diagnostics.CounterSample sample = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.InstanceData.ctor(instanceName,sample);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.InstanceData.ctor(instanceName,sample);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
