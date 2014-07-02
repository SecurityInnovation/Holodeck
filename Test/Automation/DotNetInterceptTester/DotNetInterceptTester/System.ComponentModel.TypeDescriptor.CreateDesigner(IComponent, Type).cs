using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeDescriptor
{
public class CreateDesigner_System_ComponentModel_IComponent_System_Type
{
public static bool _CreateDesigner_System_ComponentModel_IComponent_System_Type( )
{

   //class object
    System.ComponentModel.TypeDescriptor _System_ComponentModel_TypeDescriptor = new System.ComponentModel.TypeDescriptor();

   //Parameters
   System.ComponentModel.IComponent component = null;
   System.Type designerBaseType = null;

   //ReturnType/Value
   System.ComponentModel.Design.IDesigner returnVal_Real = null;
   System.ComponentModel.Design.IDesigner returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_TypeDescriptor.CreateDesigner(component,designerBaseType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_TypeDescriptor.CreateDesigner(component,designerBaseType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
