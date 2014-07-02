namespace DotNetInterceptTester.My_System.IO.StreamWriter
{
public class ctor_System_IO_StreamWriter_System_IO_Stream_System_Text_Encoding_System_Int32
{
public static bool _ctor_System_IO_StreamWriter_System_IO_Stream_System_Text_Encoding_System_Int32( )
{
   //Parameters
   System.IO.Stream stream = null;
   System.Text.Encoding encoding = null;
   System.Int32 bufferSize = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamWriter.ctor(stream,encoding,bufferSize);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamWriter.ctor(stream,encoding,bufferSize);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
