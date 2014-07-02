namespace DotNetInterceptTester.My_System.Security.Cryptography.FromBase64Transform
{
public class ctor_System_Security_Cryptography_FromBase64Transform_System_Security_Cryptography_FromBase64TransformMode
{
public static bool _ctor_System_Security_Cryptography_FromBase64Transform_System_Security_Cryptography_FromBase64TransformMode( )
{
   //Parameters
   System.Security.Cryptography.FromBase64TransformMode whitespaces = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.FromBase64Transform.ctor(whitespaces);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.FromBase64Transform.ctor(whitespaces);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
