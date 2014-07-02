using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Component
{
public class CreateObjRef_System_ComponentModel_Component_System_Type
{
public static bool _CreateObjRef_System_ComponentModel_Component_System_Type( )
{

   //class object
    System.ComponentModel.Component _System_ComponentModel_Component = new System.ComponentModel.Component();

   //Parameters
   System.Type requestedType = null;

   //ReturnType/Value
   System.Runtime.Remoting.ObjRef returnVal_Real = null;
   System.Runtime.Remoting.ObjRef returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_Component.CreateObjRef(requestedType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_Component.CreateObjRef(requestedType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
