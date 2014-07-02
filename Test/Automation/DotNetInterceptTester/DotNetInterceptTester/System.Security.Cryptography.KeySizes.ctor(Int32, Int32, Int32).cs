namespace DotNetInterceptTester.My_System.Security.Cryptography.KeySizes
{
public class ctor_System_Security_Cryptography_KeySizes_System_Int32_System_Int32_System_Int32
{
public static bool _ctor_System_Security_Cryptography_KeySizes_System_Int32_System_Int32_System_Int32( )
{
   //Parameters
   System.Int32 minSize = null;
   System.Int32 maxSize = null;
   System.Int32 skipSize = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.KeySizes.ctor(minSize,maxSize,skipSize);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.KeySizes.ctor(minSize,maxSize,skipSize);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
