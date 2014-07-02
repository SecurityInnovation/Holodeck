namespace DotNetInterceptTester.My_System.IO.StreamReader
{
public class ctor_System_IO_StreamReader_System_String_System_Text_Encoding_System_Boolean_System_Int32
{
public static bool _ctor_System_IO_StreamReader_System_String_System_Text_Encoding_System_Boolean_System_Int32( )
{
   //Parameters
   System.String path = null;
   System.Text.Encoding encoding = null;
   System.Boolean detectEncodingFromByteOrderMarks = null;
   System.Int32 bufferSize = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamReader.ctor(path,encoding,detectEncodingFromByteOrderMarks,bufferSize);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamReader.ctor(path,encoding,detectEncodingFromByteOrderMarks,bufferSize);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
