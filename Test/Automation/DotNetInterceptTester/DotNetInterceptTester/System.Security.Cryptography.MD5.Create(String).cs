namespace DotNetInterceptTester.My_System.Security.Cryptography.MD5
{
public class Create_System_String
{
public static bool _Create_System_String( )
{
   //Parameters
   System.String algName = null;

   //ReturnType/Value
   System.Security.Cryptography.MD5 returnVal_Real = null;
   System.Security.Cryptography.MD5 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.MD5.Create(algName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.MD5.Create(algName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
