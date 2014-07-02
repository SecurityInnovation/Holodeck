namespace DotNetInterceptTester.My_System.ComponentModel.StringConverter
{
public class GetProperties_System_ComponentModel_StringConverter_System_Object
{
public static bool _GetProperties_System_ComponentModel_StringConverter_System_Object( )
{
   //Parameters
   System.Object _value = null;

   //ReturnType/Value
   System.ComponentModel.PropertyDescriptorCollection returnVal_Real = null;
   System.ComponentModel.PropertyDescriptorCollection returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.StringConverter.GetProperties(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.StringConverter.GetProperties(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
