namespace DotNetInterceptTester.My_System.IO.StreamReader
{
public class DiscardBufferedData_System_IO_StreamReader
{
public static bool _DiscardBufferedData_System_IO_StreamReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamReader.DiscardBufferedData();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamReader.DiscardBufferedData();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
