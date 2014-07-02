namespace DotNetInterceptTester.My_System.Net.WebRequest
{
public class CreateDefault_System_Uri
{
public static bool _CreateDefault_System_Uri( )
{
   //Parameters
   System.Uri requestUri = null;

   //ReturnType/Value
   System.Net.WebRequest returnVal_Real = null;
   System.Net.WebRequest returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebRequest.CreateDefault(requestUri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebRequest.CreateDefault(requestUri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
