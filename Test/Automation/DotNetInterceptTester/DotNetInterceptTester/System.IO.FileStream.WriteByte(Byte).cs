namespace DotNetInterceptTester.My_System.IO.FileStream
{
public class WriteByte_System_IO_FileStream_System_Byte
{
public static bool _WriteByte_System_IO_FileStream_System_Byte( )
{
   //Parameters
   System.Byte _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
