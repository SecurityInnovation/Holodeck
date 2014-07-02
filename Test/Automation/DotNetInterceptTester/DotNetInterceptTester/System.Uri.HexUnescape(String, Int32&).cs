namespace DotNetInterceptTester.My_System.Uri
{
public class HexUnescape_System_String_System_Int32_
{
public static bool _HexUnescape_System_String_System_Int32_( )
{
   //Parameters
   System.String pattern = null;
   System.Int32& index = null;

   //ReturnType/Value
   System.Char returnVal_Real = null;
   System.Char returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Uri.HexUnescape(pattern,index);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Uri.HexUnescape(pattern,index);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
