using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentResourceManager
{
public class GetResourceSet_System_ComponentModel_ComponentResourceManager_System_Globalization_CultureInfo_System_Boolean_System_Boolean
{
public static bool _GetResourceSet_System_ComponentModel_ComponentResourceManager_System_Globalization_CultureInfo_System_Boolean_System_Boolean( )
{

   //class object
    System.ComponentModel.ComponentResourceManager _System_ComponentModel_ComponentResourceManager = new System.ComponentModel.ComponentResourceManager();

   //Parameters
   System.Globalization.CultureInfo culture = null;
   System.Boolean createIfNotExists = false;
   System.Boolean tryParents = false;

   //ReturnType/Value
   System.Resources.ResourceSet returnVal_Real = null;
   System.Resources.ResourceSet returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ComponentResourceManager.GetResourceSet(culture,createIfNotExists,tryParents);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ComponentResourceManager.GetResourceSet(culture,createIfNotExists,tryParents);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
