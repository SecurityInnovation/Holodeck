namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Regex
{
public class GroupNumberFromName_System_Text_RegularExpressions_Regex_System_String
{
public static bool _GroupNumberFromName_System_Text_RegularExpressions_Regex_System_String( )
{
   //Parameters
   System.String name = null;

   //ReturnType/Value
   System.Int32 returnVal_Real = null;
   System.Int32 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Regex.GroupNumberFromName(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Regex.GroupNumberFromName(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
