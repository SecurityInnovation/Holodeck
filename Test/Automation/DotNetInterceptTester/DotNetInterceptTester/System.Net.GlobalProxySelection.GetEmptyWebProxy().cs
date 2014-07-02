namespace DotNetInterceptTester.My_System.Net.GlobalProxySelection
{
public class GetEmptyWebProxy
{
public static bool _GetEmptyWebProxy( )
{
   //Parameters

   //ReturnType/Value
   System.Net.IWebProxy returnVal_Real = null;
   System.Net.IWebProxy returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.GlobalProxySelection.GetEmptyWebProxy(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.GlobalProxySelection.GetEmptyWebProxy(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
