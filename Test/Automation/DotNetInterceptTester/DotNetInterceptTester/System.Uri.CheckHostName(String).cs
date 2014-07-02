namespace DotNetInterceptTester.My_System.Uri
{
public class CheckHostName_System_String
{
public static bool _CheckHostName_System_String( )
{
   //Parameters
   System.String name = null;

   //ReturnType/Value
   System.UriHostNameType returnVal_Real = null;
   System.UriHostNameType returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Uri.CheckHostName(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Uri.CheckHostName(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
