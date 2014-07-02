namespace DotNetInterceptTester.My_System.Uri
{
public class IsHexEncoding_System_String_System_Int32
{
public static bool _IsHexEncoding_System_String_System_Int32( )
{
   //Parameters
   System.String pattern = null;
   System.Int32 index = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Uri.IsHexEncoding(pattern,index);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Uri.IsHexEncoding(pattern,index);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
