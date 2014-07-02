using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentEditor
{
public class EditComponent_System_ComponentModel_ComponentEditor_System_Object
{
public static bool _EditComponent_System_ComponentModel_ComponentEditor_System_Object( )
{

   //class object
    System.ComponentModel.ComponentEditor _System_ComponentModel_ComponentEditor = new System.ComponentModel.ComponentEditor();

   //Parameters
   System.Object component = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = false;
   System.Boolean returnVal_Intercepted = false;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ComponentEditor.EditComponent(component);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ComponentEditor.EditComponent(component);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
