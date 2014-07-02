namespace DotNetInterceptTester.My_System.ComponentModel.Int64Converter
{
public class GetStandardValues_System_ComponentModel_Int64Converter
{
public static bool _GetStandardValues_System_ComponentModel_Int64Converter( )
{
   //Parameters

   //ReturnType/Value
   System.Collections.ICollection returnVal_Real = null;
   System.Collections.ICollection returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.Int64Converter.GetStandardValues();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.Int64Converter.GetStandardValues();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
