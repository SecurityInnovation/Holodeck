namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Match
{
public class NextMatch_System_Text_RegularExpressions_Match
{
public static bool _NextMatch_System_Text_RegularExpressions_Match( )
{
   //Parameters

   //ReturnType/Value
   System.Text.RegularExpressions.Match returnVal_Real = null;
   System.Text.RegularExpressions.Match returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Match.NextMatch();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Match.NextMatch();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
