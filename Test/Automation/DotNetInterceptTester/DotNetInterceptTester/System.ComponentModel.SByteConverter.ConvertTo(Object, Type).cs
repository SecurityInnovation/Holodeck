namespace DotNetInterceptTester.My_System.ComponentModel.SByteConverter
{
public class ConvertTo_System_ComponentModel_SByteConverter_System_Object_System_Type
{
public static bool _ConvertTo_System_ComponentModel_SByteConverter_System_Object_System_Type( )
{
   //Parameters
   System.Object _value = null;
   System.Type destinationType = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.SByteConverter.ConvertTo(_value,destinationType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.SByteConverter.ConvertTo(_value,destinationType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
