namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Regex
{
public class GetGroupNumbers_System_Text_RegularExpressions_Regex
{
public static bool _GetGroupNumbers_System_Text_RegularExpressions_Regex( )
{
   //Parameters

   //ReturnType/Value
   System.Int32[] returnVal_Real = null;
   System.Int32[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Regex.GetGroupNumbers();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Regex.GetGroupNumbers();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
