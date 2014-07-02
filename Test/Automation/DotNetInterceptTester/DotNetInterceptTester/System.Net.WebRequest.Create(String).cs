namespace DotNetInterceptTester.My_System.Net.WebRequest
{
public class Create_System_String
{
public static bool _Create_System_String( )
{
   //Parameters
   System.String requestUriString = null;

   //ReturnType/Value
   System.Net.WebRequest returnVal_Real = null;
   System.Net.WebRequest returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebRequest.Create(requestUriString);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebRequest.Create(requestUriString);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
