namespace DotNetInterceptTester.My_System.Diagnostics.Trace
{
public class WriteIf_System_Boolean_System_Object_System_String
{
public static bool _WriteIf_System_Boolean_System_Object_System_String( )
{
   //Parameters
   System.Boolean condition = null;
   System.Object _value = null;
   System.String category = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Trace.WriteIf(condition,_value,category);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Trace.WriteIf(condition,_value,category);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
