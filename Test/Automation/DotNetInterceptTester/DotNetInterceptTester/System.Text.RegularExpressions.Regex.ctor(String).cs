namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Regex
{
public class ctor_System_Text_RegularExpressions_Regex_System_String
{
public static bool _ctor_System_Text_RegularExpressions_Regex_System_String( )
{
   //Parameters
   System.String pattern = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Regex.ctor(pattern);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Regex.ctor(pattern);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
