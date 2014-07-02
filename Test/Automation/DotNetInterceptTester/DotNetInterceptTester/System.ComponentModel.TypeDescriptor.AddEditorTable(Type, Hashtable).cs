using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeDescriptor
{
public class AddEditorTable_System_Type_System_Collections_Hashtable
{
public static bool _AddEditorTable_System_Type_System_Collections_Hashtable( )
{

   //class object
    System.ComponentModel.TypeDescriptor _System_ComponentModel_TypeDescriptor = new System.ComponentModel.TypeDescriptor();

   //Parameters
   System.Type editorBaseType = null;
   System.Collections.Hashtable table = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_TypeDescriptor.AddEditorTable(editorBaseType,table);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_TypeDescriptor.AddEditorTable(editorBaseType,table);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
