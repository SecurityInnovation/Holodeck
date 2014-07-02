using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeDescriptor
{
public class SortDescriptorArray_System_Collections_IList
{
public static bool _SortDescriptorArray_System_Collections_IList( )
{

   //class object
    System.ComponentModel.TypeDescriptor _System_ComponentModel_TypeDescriptor = new System.ComponentModel.TypeDescriptor();

   //Parameters
   System.Collections.IList infos = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_TypeDescriptor.SortDescriptorArray(infos);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_TypeDescriptor.SortDescriptorArray(infos);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
