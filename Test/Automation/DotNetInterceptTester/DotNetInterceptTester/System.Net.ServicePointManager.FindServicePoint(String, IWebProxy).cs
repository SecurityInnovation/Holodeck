namespace DotNetInterceptTester.My_System.Net.ServicePointManager
{
public class FindServicePoint_System_String_System_Net_IWebProxy
{
public static bool _FindServicePoint_System_String_System_Net_IWebProxy( )
{
   //Parameters
   System.String uriString = null;
   System.Net.IWebProxy proxy = null;

   //ReturnType/Value
   System.Net.ServicePoint returnVal_Real = null;
   System.Net.ServicePoint returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.ServicePointManager.FindServicePoint(uriString,proxy);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.ServicePointManager.FindServicePoint(uriString,proxy);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
