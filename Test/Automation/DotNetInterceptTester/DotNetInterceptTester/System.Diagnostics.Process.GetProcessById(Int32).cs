namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class GetProcessById_System_Int32
{
public static bool _GetProcessById_System_Int32( )
{
   //Parameters
   System.Int32 processId = null;

   //ReturnType/Value
   System.Diagnostics.Process returnVal_Real = null;
   System.Diagnostics.Process returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.GetProcessById(processId);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.GetProcessById(processId);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
