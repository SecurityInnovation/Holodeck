namespace DotNetInterceptTester.My_System.IO.TextWriter
{
public class Synchronized_System_IO_TextWriter
{
public static bool _Synchronized_System_IO_TextWriter( )
{
   //Parameters
   System.IO.TextWriter writer = null;

   //ReturnType/Value
   System.IO.TextWriter returnVal_Real = null;
   System.IO.TextWriter returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.TextWriter.Synchronized(writer);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.TextWriter.Synchronized(writer);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
