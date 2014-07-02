namespace DotNetInterceptTester.My_System.Xml.XmlEntityReference
{
public class GetEnumerator_System_Xml_XmlEntityReference
{
public static bool _GetEnumerator_System_Xml_XmlEntityReference( )
{
   //Parameters

   //ReturnType/Value
   System.Collections.IEnumerator returnVal_Real = null;
   System.Collections.IEnumerator returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlEntityReference.GetEnumerator();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlEntityReference.GetEnumerator();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
