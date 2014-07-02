namespace DotNetInterceptTester.My_System.Diagnostics.Debug
{
public class Fail_System_String
{
public static bool _Fail_System_String( )
{
   //Parameters
   System.String message = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Debug.Fail(message);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Debug.Fail(message);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
