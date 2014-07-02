namespace DotNetInterceptTester.My_System.Security.Cryptography.CspParameters
{
public class ctor_System_Security_Cryptography_CspParameters_System_Int32_System_String_System_String
{
public static bool _ctor_System_Security_Cryptography_CspParameters_System_Int32_System_String_System_String( )
{
   //Parameters
   System.Int32 dwTypeIn = null;
   System.String strProviderNameIn = null;
   System.String strContainerNameIn = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CspParameters.ctor(dwTypeIn,strProviderNameIn,strContainerNameIn);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CspParameters.ctor(dwTypeIn,strProviderNameIn,strContainerNameIn);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
