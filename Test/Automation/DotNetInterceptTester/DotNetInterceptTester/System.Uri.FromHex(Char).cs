namespace DotNetInterceptTester.My_System.Uri
{
public class FromHex_System_Char
{
public static bool _FromHex_System_Char( )
{
   //Parameters
   System.Char digit = null;

   //ReturnType/Value
   System.Int32 returnVal_Real = null;
   System.Int32 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Uri.FromHex(digit);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Uri.FromHex(digit);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
