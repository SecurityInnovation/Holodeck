using System;

namespace ResourcesPaneTest
{
	/// <summary>
	/// Summary description for TestGenerator.
	/// </summary>
	public class TestGenerator
	{
		const string namespaceName = "ResourcePaneTestApps";

		string appName = "";
		System.IO.StreamReader sourceStreamReader = null;
		string sourceCode = "";
		string additionalClasses = "";
		string additionalMethods = "";
		string mainCode = "";
		System.CodeDom.Compiler.CompilerResults compilerResult = null;
		string appPath = "";

		public TestGenerator(string name)
		{
			appName = name;
		}

		public TestGenerator(string name, System.IO.StreamReader source) {
			appName = name;
			sourceStreamReader = source;
		}

		public TestGenerator(string name, string code) {
			appName = name;
			sourceCode = code;
		}

		public TestGenerator(string name, bool withForm) {
			appName = name;
			if (withForm) {
				additionalMethods += 
					"public class Form1 : System.Windows.Forms.Form {\n" + 
					"\tprivate System.ComponentModel.Container components = null;\n" + 
					"\tpublic Form1() {\n" +
					"\t\tInitializeComponent();\n"+
					"\t}\n" +
					"\tprotected override void Dispose(bool disposing) {\n" +
					"\t\tif (disposing) {\n" +
					"\t\t\tif (components != null) {\n" +
					"\t\t\t\tcomponents.Dispose ();\n" +
					"\t\t\t}\n" +
					"\t\t}\n" +
					"\t\tbase.Dispose( disposing );\n" +
					"\t}\n" +
					"#region Windows Form Designer generated code\n" +
					"\tprivate void InitializeComponent () {\n" +
					"\t\tthis.components = new System.ComponentModel.Container ();\n" +
					"\t\tthis.Size = new System.Drawing.Size (300, 300);\n" +
					"\t\tthis.Text = \"Form1\";\n" +
					"\t}\n" +
					"#endregion\n";
			}
		}

		public void AddMainCode (string code) {
			mainCode += code + "System.Threading.Thread.Sleep (sleepTime);\n";
		}

		public string GetCode () {
			string code = "";
			if (sourceStreamReader != null) {
				code = sourceStreamReader.ReadToEnd ();
			} else if (sourceCode != "") {
				code = sourceCode;
			} else {
				code = "using System;\n" + 
					"namespace " + namespaceName + " {\n" + 
					"\tclass " + appName + "{\n" +
					"\t\t\tconst int sleepTime = 50;\n" +
					"\t\t[STAThread]\n" +
					"\t\tstatic void Main(string[] args) {\n" +
					"\t\t\tSystem.Threading.Thread.Sleep (sleepTime);\n" + 
					mainCode + 
					"\t\t}\n" +
					"\t}\n" +
					"}\n";
			}

			return code;
		}

		public string GetOutPutString () {
			string output = "";
			foreach (string outputString in  compilerResult.Output) {
				output += outputString + "\n";
			}

			return output;
		}

		public string FormatErrors (System.CodeDom.Compiler.CompilerErrorCollection errorColl) {
			string retVal = "";
			foreach (System.CodeDom.Compiler.CompilerError error in errorColl) {
				retVal += FormatError (error);
			}

			return retVal;
		}

		public string FormatError (System.CodeDom.Compiler.CompilerError error) {
			string retVal = "";
			if (error.IsWarning) {
				retVal = "Warning: ";
			} else {
				retVal = "Error: ";
			}
			retVal += error.ErrorNumber + " " + error.FileName + "(" + error.Line + "," + error.Column + "):\n";
			retVal += "\t" + error.ErrorText;

			return retVal;
		}

		public System.CodeDom.Compiler.CompilerErrorCollection GetErrors () {
			return compilerResult.Errors;
		}

		public string GetAppPathString () {
			return appPath + "\\" + appName + ".exe";
		}

		public int GenerateTest () {
			System.CodeDom.Compiler.CompilerParameters parameters = new System.CodeDom.Compiler.CompilerParameters ();
			parameters.GenerateExecutable = true;
			parameters.GenerateInMemory = false;
			parameters.IncludeDebugInformation = false;
			parameters.MainClass = namespaceName + "." + appName;
			parameters.OutputAssembly = appName + ".exe";
			string code = GetCode ();
			System.CodeDom.Compiler.ICodeCompiler compiler = new Microsoft.CSharp.CSharpCodeProvider ().CreateCompiler ();
			compilerResult = compiler.CompileAssemblyFromSource (parameters, code);
			if (compilerResult.NativeCompilerReturnValue == 0) {
				appPath = System.IO.Directory.GetCurrentDirectory ();
			} else {
				appPath = "";
			}
			return compilerResult.NativeCompilerReturnValue;
		}
	}
}
