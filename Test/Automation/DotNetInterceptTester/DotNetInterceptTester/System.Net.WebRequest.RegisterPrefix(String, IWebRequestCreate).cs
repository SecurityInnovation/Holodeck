namespace DotNetInterceptTester.My_System.Net.WebRequest
{
public class RegisterPrefix_System_String_System_Net_IWebRequestCreate
{
public static bool _RegisterPrefix_System_String_System_Net_IWebRequestCreate( )
{
   //Parameters
   System.String prefix = null;
   System.Net.IWebRequestCreate creator = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebRequest.RegisterPrefix(prefix,creator);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebRequest.RegisterPrefix(prefix,creator);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
