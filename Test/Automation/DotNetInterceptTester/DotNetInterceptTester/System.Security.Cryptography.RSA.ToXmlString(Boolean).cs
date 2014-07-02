namespace DotNetInterceptTester.My_System.Security.Cryptography.RSA
{
public class ToXmlString_System_Security_Cryptography_RSA_System_Boolean
{
public static bool _ToXmlString_System_Security_Cryptography_RSA_System_Boolean( )
{
   //Parameters
   System.Boolean includePrivateParameters = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSA.ToXmlString(includePrivateParameters);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSA.ToXmlString(includePrivateParameters);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
