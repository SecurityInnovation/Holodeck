namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class GetStringBuilder_System_IO_StringWriter
{
public static bool _GetStringBuilder_System_IO_StringWriter( )
{
   //Parameters

   //ReturnType/Value
   System.Text.StringBuilder returnVal_Real = null;
   System.Text.StringBuilder returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.GetStringBuilder();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.GetStringBuilder();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
