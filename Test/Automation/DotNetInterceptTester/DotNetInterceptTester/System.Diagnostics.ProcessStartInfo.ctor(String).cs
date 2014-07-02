namespace DotNetInterceptTester.My_System.Diagnostics.ProcessStartInfo
{
public class ctor_System_Diagnostics_ProcessStartInfo_System_String
{
public static bool _ctor_System_Diagnostics_ProcessStartInfo_System_String( )
{
   //Parameters
   System.String fileName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.ProcessStartInfo.ctor(fileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.ProcessStartInfo.ctor(fileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
