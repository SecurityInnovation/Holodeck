namespace DotNetInterceptTester.My_System.Xml.XmlEntity
{
public class InsertBefore_System_Xml_XmlEntity_System_Xml_XmlNode_System_Xml_XmlNode
{
public static bool _InsertBefore_System_Xml_XmlEntity_System_Xml_XmlNode_System_Xml_XmlNode( )
{
   //Parameters
   System.Xml.XmlNode newChild = null;
   System.Xml.XmlNode refChild = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlEntity.InsertBefore(newChild,refChild);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlEntity.InsertBefore(newChild,refChild);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
