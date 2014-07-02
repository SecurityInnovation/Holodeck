namespace DotNetInterceptTester.My_System.ComponentModel.SByteConverter
{
public class CanConvertTo_System_ComponentModel_SByteConverter_System_Type
{
public static bool _CanConvertTo_System_ComponentModel_SByteConverter_System_Type( )
{
   //Parameters
   System.Type destinationType = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.SByteConverter.CanConvertTo(destinationType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.SByteConverter.CanConvertTo(destinationType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
