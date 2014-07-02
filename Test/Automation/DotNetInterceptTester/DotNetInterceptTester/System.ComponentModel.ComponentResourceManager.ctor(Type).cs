using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentResourceManager
{
public class ctor_System_ComponentModel_ComponentResourceManager_System_Type
{
public static bool _ctor_System_ComponentModel_ComponentResourceManager_System_Type( )
{

   //class object
    System.ComponentModel.ComponentResourceManager _System_ComponentModel_ComponentResourceManager = new System.ComponentModel.ComponentResourceManager();

   //Parameters
   System.Type t = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_ComponentResourceManager.ctor(t);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_ComponentResourceManager.ctor(t);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
