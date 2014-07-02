namespace DotNetInterceptTester.My_System.ComponentModel.LicenseManager
{
public class Validate_System_Type_System_Object
{
public static bool _Validate_System_Type_System_Object( )
{
   //Parameters
   System.Type type = null;
   System.Object instance = null;

   //ReturnType/Value
   System.ComponentModel.License returnVal_Real = null;
   System.ComponentModel.License returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseManager.Validate(type,instance);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseManager.Validate(type,instance);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
