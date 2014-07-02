namespace DotNetInterceptTester.My_System.Xml.XmlImplementation
{
public class HasFeature_System_Xml_XmlImplementation_System_String_System_String
{
public static bool _HasFeature_System_Xml_XmlImplementation_System_String_System_String( )
{
   //Parameters
   System.String strFeature = null;
   System.String strVersion = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlImplementation.HasFeature(strFeature,strVersion);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlImplementation.HasFeature(strFeature,strVersion);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
