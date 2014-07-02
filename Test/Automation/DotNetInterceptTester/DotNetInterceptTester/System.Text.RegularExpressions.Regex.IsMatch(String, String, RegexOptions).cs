namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Regex
{
public class IsMatch_System_String_System_String_System_Text_RegularExpressions_RegexOptions
{
public static bool _IsMatch_System_String_System_String_System_Text_RegularExpressions_RegexOptions( )
{
   //Parameters
   System.String input = null;
   System.String pattern = null;
   System.Text.RegularExpressions.RegexOptions options = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Regex.IsMatch(input,pattern,options);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Regex.IsMatch(input,pattern,options);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
