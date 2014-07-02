namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Regex
{
public class Matches_System_Text_RegularExpressions_Regex_System_String_System_Int32
{
public static bool _Matches_System_Text_RegularExpressions_Regex_System_String_System_Int32( )
{
   //Parameters
   System.String input = null;
   System.Int32 startat = null;

   //ReturnType/Value
   System.Text.RegularExpressions.MatchCollection returnVal_Real = null;
   System.Text.RegularExpressions.MatchCollection returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Regex.Matches(input,startat);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Regex.Matches(input,startat);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
