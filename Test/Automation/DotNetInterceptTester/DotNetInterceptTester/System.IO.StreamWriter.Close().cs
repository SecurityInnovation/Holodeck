namespace DotNetInterceptTester.My_System.IO.StreamWriter
{
public class Close_System_IO_StreamWriter
{
public static bool _Close_System_IO_StreamWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamWriter.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamWriter.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
