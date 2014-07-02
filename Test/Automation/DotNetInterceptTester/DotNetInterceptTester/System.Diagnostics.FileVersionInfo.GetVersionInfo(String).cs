namespace DotNetInterceptTester.My_System.Diagnostics.FileVersionInfo
{
public class GetVersionInfo_System_String
{
public static bool _GetVersionInfo_System_String( )
{
   //Parameters
   System.String fileName = null;

   //ReturnType/Value
   System.Diagnostics.FileVersionInfo returnVal_Real = null;
   System.Diagnostics.FileVersionInfo returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.FileVersionInfo.GetVersionInfo(fileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.FileVersionInfo.GetVersionInfo(fileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
