namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoStream
{
public class SetLength_System_Security_Cryptography_CryptoStream_System_Int64
{
public static bool _SetLength_System_Security_Cryptography_CryptoStream_System_Int64( )
{
   //Parameters
   System.Int64 _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoStream.SetLength(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoStream.SetLength(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
