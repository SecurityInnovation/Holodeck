namespace DotNetInterceptTester.My_System.Net.WebProxy
{
public class GetDefaultProxy
{
public static bool _GetDefaultProxy( )
{
   //Parameters

   //ReturnType/Value
   System.Net.WebProxy returnVal_Real = null;
   System.Net.WebProxy returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebProxy.GetDefaultProxy(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebProxy.GetDefaultProxy(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
