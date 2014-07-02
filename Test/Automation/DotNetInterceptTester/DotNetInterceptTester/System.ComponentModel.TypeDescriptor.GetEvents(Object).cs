using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeDescriptor
{
public class GetEvents_System_Object
{
public static bool _GetEvents_System_Object( )
{

   //class object
    System.ComponentModel.TypeDescriptor _System_ComponentModel_TypeDescriptor = new System.ComponentModel.TypeDescriptor();

   //Parameters
   System.Object component = null;

   //ReturnType/Value
   System.ComponentModel.EventDescriptorCollection returnVal_Real = null;
   System.ComponentModel.EventDescriptorCollection returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_TypeDescriptor.GetEvents(component);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_TypeDescriptor.GetEvents(component);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
