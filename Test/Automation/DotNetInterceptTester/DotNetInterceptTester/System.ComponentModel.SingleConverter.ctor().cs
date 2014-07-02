namespace DotNetInterceptTester.My_System.ComponentModel.SingleConverter
{
public class ctor_System_ComponentModel_SingleConverter
{
public static bool _ctor_System_ComponentModel_SingleConverter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.SingleConverter.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.SingleConverter.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
