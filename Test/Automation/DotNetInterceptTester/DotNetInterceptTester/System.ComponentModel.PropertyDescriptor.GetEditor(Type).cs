namespace DotNetInterceptTester.My_System.ComponentModel.PropertyDescriptor
{
public class GetEditor_System_ComponentModel_PropertyDescriptor_System_Type
{
public static bool _GetEditor_System_ComponentModel_PropertyDescriptor_System_Type( )
{
   //Parameters
   System.Type editorBaseType = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.PropertyDescriptor.GetEditor(editorBaseType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.PropertyDescriptor.GetEditor(editorBaseType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
