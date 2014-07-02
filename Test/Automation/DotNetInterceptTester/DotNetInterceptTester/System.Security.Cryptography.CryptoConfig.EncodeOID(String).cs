namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoConfig
{
public class EncodeOID_System_String
{
public static bool _EncodeOID_System_String( )
{
   //Parameters
   System.String str = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoConfig.EncodeOID(str);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoConfig.EncodeOID(str);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
