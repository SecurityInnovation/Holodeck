namespace DotNetInterceptTester.My_System.Console
{
public class WriteLine_System_String_System_Object_System_Object_System_Object
{
public static bool _WriteLine_System_String_System_Object_System_Object_System_Object( )
{
   //Parameters
   System.String format = null;
   System.Object arg0 = null;
   System.Object arg1 = null;
   System.Object arg2 = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.WriteLine(format,arg0,arg1,arg2);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.WriteLine(format,arg0,arg1,arg2);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
