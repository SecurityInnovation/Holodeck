namespace DotNetInterceptTester.My_System.ComponentModel.ReferenceConverter
{
public class ctor_System_ComponentModel_ReferenceConverter_System_Type
{
public static bool _ctor_System_ComponentModel_ReferenceConverter_System_Type( )
{
   //Parameters
   System.Type type = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.ReferenceConverter.ctor(type);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.ReferenceConverter.ctor(type);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
