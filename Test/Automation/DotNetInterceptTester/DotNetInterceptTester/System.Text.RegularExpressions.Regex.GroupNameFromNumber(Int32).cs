namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Regex
{
public class GroupNameFromNumber_System_Text_RegularExpressions_Regex_System_Int32
{
public static bool _GroupNameFromNumber_System_Text_RegularExpressions_Regex_System_Int32( )
{
   //Parameters
   System.Int32 i = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Regex.GroupNameFromNumber(i);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Regex.GroupNameFromNumber(i);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
