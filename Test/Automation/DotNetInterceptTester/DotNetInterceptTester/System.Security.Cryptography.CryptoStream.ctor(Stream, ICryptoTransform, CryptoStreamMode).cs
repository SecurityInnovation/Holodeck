namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoStream
{
public class ctor_System_Security_Cryptography_CryptoStream_System_IO_Stream_System_Security_Cryptography_ICryptoTransform_System_Security_Cryptography_CryptoStreamMode
{
public static bool _ctor_System_Security_Cryptography_CryptoStream_System_IO_Stream_System_Security_Cryptography_ICryptoTransform_System_Security_Cryptography_CryptoStreamMode( )
{
   //Parameters
   System.IO.Stream stream = null;
   System.Security.Cryptography.ICryptoTransform transform = null;
   System.Security.Cryptography.CryptoStreamMode mode = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoStream.ctor(stream,transform,mode);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoStream.ctor(stream,transform,mode);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
