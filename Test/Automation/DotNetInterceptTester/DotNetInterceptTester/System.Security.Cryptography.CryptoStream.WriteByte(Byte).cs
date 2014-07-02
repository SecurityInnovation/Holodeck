namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoStream
{
public class WriteByte_System_Security_Cryptography_CryptoStream_System_Byte
{
public static bool _WriteByte_System_Security_Cryptography_CryptoStream_System_Byte( )
{
   //Parameters
   System.Byte _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
