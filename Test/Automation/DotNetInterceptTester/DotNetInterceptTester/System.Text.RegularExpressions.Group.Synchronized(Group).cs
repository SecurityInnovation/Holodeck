namespace DotNetInterceptTester.My_System.Text.RegularExpressions.Group
{
public class Synchronized_System_Text_RegularExpressions_Group
{
public static bool _Synchronized_System_Text_RegularExpressions_Group( )
{
   //Parameters
   System.Text.RegularExpressions.Group inner = null;

   //ReturnType/Value
   System.Text.RegularExpressions.Group returnVal_Real = null;
   System.Text.RegularExpressions.Group returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Text.RegularExpressions.Group.Synchronized(inner);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Text.RegularExpressions.Group.Synchronized(inner);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
