namespace DotNetInterceptTester.My_System.Diagnostics.Debug
{
public class Unindent
{
public static bool _Unindent( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Debug.Unindent(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Debug.Unindent(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
