namespace DotNetInterceptTester.My_System.Diagnostics.Trace
{
public class WriteLineIf_System_Boolean_System_Object
{
public static bool _WriteLineIf_System_Boolean_System_Object( )
{
   //Parameters
   System.Boolean condition = null;
   System.Object _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Trace.WriteLineIf(condition,_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Trace.WriteLineIf(condition,_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
