namespace DotNetInterceptTester.My_System.Net.WebPermission
{
public class AddPermission_System_Net_WebPermission_System_Net_NetworkAccess_System_Text_RegularExpressions_Regex
{
public static bool _AddPermission_System_Net_WebPermission_System_Net_NetworkAccess_System_Text_RegularExpressions_Regex( )
{
   //Parameters
   System.Net.NetworkAccess access = null;
   System.Text.RegularExpressions.Regex uriRegex = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebPermission.AddPermission(access,uriRegex);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebPermission.AddPermission(access,uriRegex);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
