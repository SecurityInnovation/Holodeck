namespace DotNetInterceptTester.My_System.Security.Cryptography.RijndaelManaged
{
public class ValidKeySize_System_Security_Cryptography_RijndaelManaged_System_Int32
{
public static bool _ValidKeySize_System_Security_Cryptography_RijndaelManaged_System_Int32( )
{
   //Parameters
   System.Int32 bitLength = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RijndaelManaged.ValidKeySize(bitLength);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RijndaelManaged.ValidKeySize(bitLength);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
