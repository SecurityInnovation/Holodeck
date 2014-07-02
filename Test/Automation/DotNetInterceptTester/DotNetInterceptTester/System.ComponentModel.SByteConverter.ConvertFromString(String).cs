namespace DotNetInterceptTester.My_System.ComponentModel.SByteConverter
{
public class ConvertFromString_System_ComponentModel_SByteConverter_System_String
{
public static bool _ConvertFromString_System_ComponentModel_SByteConverter_System_String( )
{
   //Parameters
   System.String text = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.SByteConverter.ConvertFromString(text);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.SByteConverter.ConvertFromString(text);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
