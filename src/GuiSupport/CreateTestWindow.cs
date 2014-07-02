using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using FunctionDatabase;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for CreateTestWindow.
	/// </summary>
	public class CreateTestWindow : System.Windows.Forms.Form
	{
		private static string IgnoreParamString = "Ignore";

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label returnTypeLabel;
		private System.Windows.Forms.Label modifiersLabel;
		private System.Windows.Forms.Label functionGroupLabel;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.TextBox returnValueTextBox;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.ListView paramListView;
		private System.Windows.Forms.ColumnHeader nameHeader;
		private System.Windows.Forms.ColumnHeader typeHeader;
		private System.Windows.Forms.ColumnHeader valueHeader;
		private System.Windows.Forms.ComboBox cmbBox;
		private System.Windows.Forms.Label label8;
		private InterceptedFunction function;
		private ListViewItem li;
		private Point firstMouseClickPoint;
		private string subItemText;
		private System.Windows.Forms.ComboBox functionNameComboBox;
		private int subItemSelected = 0;
		private System.Windows.Forms.ComboBox errorCodeComboBox;
		private bool hasLoaded = false;

		#region properties

		public string Section
		{
			get { return functionGroupLabel.Text; }
		}

		public string FunctionName
		{
			get 
			{ 
				if (functionNameComboBox.SelectedItem == null)
					return null;
				else
					return functionNameComboBox.SelectedItem.ToString(); 
			}
		}

		public string Action
		{
			get { return returnValueTextBox.Text + " " + errorCodeComboBox.Text; }
		}

		public int NumApplicableParams
		{
			get
			{
				return paramListView.Items.Count;
			}
		}

		public Array ApplicableParams
		{
			get
			{
				int arraySize = this.NumApplicableParams;
				Array retVal = Array.CreateInstance(typeof(string), arraySize);

				int index = 0;
				foreach (ListViewItem item in this.paramListView.Items)
				{
					retVal.SetValue(item.SubItems[0].Text, index++);
				}

				return retVal;
			}
		}

		public Array ApplicableParamValues
		{
			get
			{
				int arraySize = this.NumApplicableParams;
				Array retVal = Array.CreateInstance(typeof(string), arraySize);
				Array appParamPos = this.ApplicableParamPos;

				int index = 0;
				foreach (int pos in appParamPos)
					retVal.SetValue(this.paramListView.Items[pos].SubItems[2].Text, index++);

				return retVal;
			}
		}

		public Array ApplicableParamTypes
		{
			get
			{
				int arraySize = this.NumApplicableParams;
				Array retVal = Array.CreateInstance(typeof(int), arraySize);
				Array appParams = this.ApplicableParams;
				Array appParamValues = this.ApplicableParamValues;

				InterceptedFunction f = InterceptedFunctionDB.GetInstance("functions.xml").GetFunctionByName(this.FunctionName);
				int index = 0;
				if (f != null)
				{
					foreach(string s in appParams)
					{
						foreach(InterceptedFunctionParameter p in f.Parameters)
						{
							if (p.Name == s)
							{
								retVal.SetValue(p.CompareAsType, index);
								index++;
								break;
							}
						}
					}
					//return ignoretype where necessary
					index = 0;
					foreach(string s in appParamValues)
					{
						if (s == IgnoreParamString)
							retVal.SetValue(InterceptedFunctionParameter.IgnoreParameterType, index);

						index++;
					}
				}

				return retVal;
			}
		}

		public Array ApplicableParamPos
		{
			get
			{
				int arraySize = this.NumApplicableParams;
				Array retVal = Array.CreateInstance(typeof(int), arraySize);
				Array appParams = this.ApplicableParams;

				InterceptedFunction f = InterceptedFunctionDB.GetInstance("functions.xml").GetFunctionByName(this.FunctionName);
				int index = 0;
				if (f != null)
				{
					foreach(string s in appParams)
					{
						foreach(InterceptedFunctionParameter p in f.Parameters)
						{
							if (p.Name == s)
							{
								retVal.SetValue(p.ID, index);
								index++;
								break;
							}
						}
					}
				}

				return retVal;
			}
		}

		public string ReturnValue
		{
			get { return returnValueTextBox.Text; }
			set { returnValueTextBox.Text = value; }
		}

		public string ErrorCode
		{
			get { return errorCodeComboBox.Text; }
			set { errorCodeComboBox.Text = value; }
		}

		#endregion

		public CreateTestWindow(InterceptedFunction func)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			function = func;
			firstMouseClickPoint = new Point();
			paramListView.Focus();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(CreateTestWindow));
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.functionNameComboBox = new System.Windows.Forms.ComboBox();
			this.functionGroupLabel = new System.Windows.Forms.Label();
			this.modifiersLabel = new System.Windows.Forms.Label();
			this.returnTypeLabel = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.label8 = new System.Windows.Forms.Label();
			this.paramListView = new System.Windows.Forms.ListView();
			this.nameHeader = new System.Windows.Forms.ColumnHeader();
			this.typeHeader = new System.Windows.Forms.ColumnHeader();
			this.valueHeader = new System.Windows.Forms.ColumnHeader();
			this.cmbBox = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.errorCodeComboBox = new System.Windows.Forms.ComboBox();
			this.returnValueTextBox = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.okButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.paramListView.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.functionNameComboBox,
																					this.functionGroupLabel,
																					this.modifiersLabel,
																					this.returnTypeLabel,
																					this.label4,
																					this.label3,
																					this.label2,
																					this.label1});
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(472, 80);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Function to Create A Test For:";
			// 
			// functionNameComboBox
			// 
			this.functionNameComboBox.Location = new System.Drawing.Point(88, 16);
			this.functionNameComboBox.Name = "functionNameComboBox";
			this.functionNameComboBox.Size = new System.Drawing.Size(136, 21);
			this.functionNameComboBox.TabIndex = 8;
			this.functionNameComboBox.SelectedValueChanged += new System.EventHandler(this.functionNameComboBox_SelectedValueChanged);
			// 
			// functionGroupLabel
			// 
			this.functionGroupLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.functionGroupLabel.Location = new System.Drawing.Point(320, 48);
			this.functionGroupLabel.Name = "functionGroupLabel";
			this.functionGroupLabel.Size = new System.Drawing.Size(136, 16);
			this.functionGroupLabel.TabIndex = 7;
			// 
			// modifiersLabel
			// 
			this.modifiersLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.modifiersLabel.Location = new System.Drawing.Point(320, 24);
			this.modifiersLabel.Name = "modifiersLabel";
			this.modifiersLabel.Size = new System.Drawing.Size(136, 16);
			this.modifiersLabel.TabIndex = 6;
			// 
			// returnTypeLabel
			// 
			this.returnTypeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.returnTypeLabel.Location = new System.Drawing.Point(88, 48);
			this.returnTypeLabel.Name = "returnTypeLabel";
			this.returnTypeLabel.Size = new System.Drawing.Size(136, 16);
			this.returnTypeLabel.TabIndex = 5;
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(256, 48);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(48, 16);
			this.label4.TabIndex = 3;
			this.label4.Text = "Group:";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(256, 24);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(56, 16);
			this.label3.TabIndex = 2;
			this.label3.Text = "Modifiers:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(16, 48);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(72, 16);
			this.label2.TabIndex = 1;
			this.label2.Text = "Return Type:";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(64, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "Name:";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.label8,
																					this.paramListView,
																					this.label5});
			this.groupBox2.Location = new System.Drawing.Point(8, 96);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(472, 224);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Function Parameters";
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(148, 32);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(176, 16);
			this.label8.TabIndex = 2;
			this.label8.Text = "(Double Click the Value to Edit It)";
			// 
			// paramListView
			// 
			this.paramListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																							this.nameHeader,
																							this.typeHeader,
																							this.valueHeader});
			this.paramListView.Controls.AddRange(new System.Windows.Forms.Control[] {
																						this.cmbBox});
			this.paramListView.FullRowSelect = true;
			this.paramListView.Location = new System.Drawing.Point(16, 56);
			this.paramListView.Name = "paramListView";
			this.paramListView.Size = new System.Drawing.Size(440, 152);
			this.paramListView.TabIndex = 1;
			this.paramListView.View = System.Windows.Forms.View.Details;
			this.paramListView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.paramListView_MouseDown);
			this.paramListView.DoubleClick += new System.EventHandler(this.paramListView_DoubleClick);
			// 
			// nameHeader
			// 
			this.nameHeader.Text = "Name";
			this.nameHeader.Width = 150;
			// 
			// typeHeader
			// 
			this.typeHeader.Text = "Type";
			this.typeHeader.Width = 100;
			// 
			// valueHeader
			// 
			this.valueHeader.Text = "Value";
			this.valueHeader.Width = 186;
			// 
			// cmbBox
			// 
			this.cmbBox.Name = "cmbBox";
			this.cmbBox.Size = new System.Drawing.Size(0, 21);
			this.cmbBox.TabIndex = 0;
			this.cmbBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.CmbKeyPress);
			this.cmbBox.SelectedIndexChanged += new System.EventHandler(this.CmbSelected);
			this.cmbBox.LostFocus += new System.EventHandler(this.CmbFocusOver);
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(52, 16);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(368, 16);
			this.label5.TabIndex = 0;
			this.label5.Text = "The test will be executed when the parameters match the values below:";
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.errorCodeComboBox,
																					this.returnValueTextBox,
																					this.label7,
																					this.label6});
			this.groupBox3.Location = new System.Drawing.Point(8, 328);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(472, 88);
			this.groupBox3.TabIndex = 2;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Specify the following return value and error code for the test:";
			// 
			// errorCodeComboBox
			// 
			this.errorCodeComboBox.Location = new System.Drawing.Point(288, 48);
			this.errorCodeComboBox.Name = "errorCodeComboBox";
			this.errorCodeComboBox.Size = new System.Drawing.Size(168, 21);
			this.errorCodeComboBox.TabIndex = 3;
			// 
			// returnValueTextBox
			// 
			this.returnValueTextBox.Location = new System.Drawing.Point(16, 48);
			this.returnValueTextBox.Name = "returnValueTextBox";
			this.returnValueTextBox.Size = new System.Drawing.Size(232, 20);
			this.returnValueTextBox.TabIndex = 2;
			this.returnValueTextBox.Text = "";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(288, 24);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(100, 16);
			this.label7.TabIndex = 1;
			this.label7.Text = "Error Code:";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(16, 24);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(88, 16);
			this.label6.TabIndex = 0;
			this.label6.Text = "Return Value:";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(160, 432);
			this.okButton.Name = "okButton";
			this.okButton.TabIndex = 3;
			this.okButton.Text = "OK";
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(256, 432);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.TabIndex = 4;
			this.cancelButton.Text = "Cancel";
			// 
			// CreateTestWindow
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(490, 464);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.cancelButton,
																		  this.okButton,
																		  this.groupBox3,
																		  this.groupBox2,
																		  this.groupBox1});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CreateTestWindow";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Create a Test";
			this.Load += new System.EventHandler(this.CreateTestWindow_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.paramListView.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void CreateTestWindow_Load(object sender, System.EventArgs e)
		{
			InterceptedFunctionDB db = InterceptedFunctionDB.GetInstance("functions.xml");
			foreach (InterceptedFunction f in db.Functions)
			{
				this.functionNameComboBox.Items.Add(f.Name);
			}

			// add error codes to the error code drop down
			IEnumerator enumerator = ErrorCodeDB.GetInstance().GetAllValues().GetEnumerator();
			while (enumerator.MoveNext())
				this.errorCodeComboBox.Items.Add(enumerator.Current);

			if (function != null)
			{
				this.functionNameComboBox.SelectedItem = function.Name;
				this.functionGroupLabel.Text = function.Category;
				this.returnTypeLabel.Text = function.ReturnType;

				string modifier = null;
				foreach (string s in function.Modifiers)
				{
					if (modifier != null)
					{
						modifier += ", ";
					}

					modifier += s;
				}

				this.modifiersLabel.Text = modifier;

				// fill in the parameters
				foreach (InterceptedFunctionParameter param in function.Parameters)
				{
					ListViewItem item = new ListViewItem(param.Name);
					item.SubItems.Add(param.Type);
					item.SubItems.Add(param.TestValue);

					paramListView.Items.Add(item);
				}
			}

			hasLoaded = true;
		}

		private void CmbKeyPress(object sender , System.Windows.Forms.KeyPressEventArgs e)
		{
			if ((e.KeyChar == 13) || (e.KeyChar == 27))
			{
				cmbBox.Hide();
			}
		}

		private void CmbSelected(object sender , System.EventArgs e)
		{
			int sel = cmbBox.SelectedIndex;
			if (sel >= 0)
			{
				string itemSel = cmbBox.Items[sel].ToString();
				li.SubItems[subItemSelected].Text = itemSel;
			}
		}

		private void CmbFocusOver(object sender , System.EventArgs e)
		{
			if (!cmbBox.Focused)
			{
				// set the text for the value field
				li.SubItems[2].Text = cmbBox.Text;

				cmbBox.Hide();
				cmbBox.Items.Clear();
			}
		}

		public  void paramListView_DoubleClick(object sender, System.EventArgs e)
		{
			// Check the subitem clicked .
			int colStartX = 0; 
			int colEndX = paramListView.Columns[0].Width;

			for (int i = 0; i < paramListView.Columns.Count; i++)
			{
				if ((firstMouseClickPoint.X > colStartX) && (firstMouseClickPoint.X < colEndX))
				{
					subItemSelected = i;
					break; 
				}
				
				colStartX = colEndX;

				if (i < (paramListView.Columns.Count - 1))
				{
					colEndX += paramListView.Columns[i + 1].Width;
				}
			}

			subItemText = li.SubItems[subItemSelected].Text;

			foreach (InterceptedFunctionParameter fp in function.Parameters)
			{
				if(fp.Name == li.SubItems[0].Text)
				{
					cmbBox.Items.Add(fp.TestValue);
				}
			}
			if ((string) cmbBox.Items[0] != IgnoreParamString)
				cmbBox.Items.Add(IgnoreParamString);

			string colName = paramListView.Columns[subItemSelected].Text;
			if (colName == "Value")
			{
				Rectangle r = new Rectangle(colStartX , li.Bounds.Y , colEndX , li.Bounds.Bottom);
				cmbBox.Width = colEndX - colStartX;
				cmbBox.Height = li.Bounds.Bottom - li.Bounds.Top;
				cmbBox.Location = new System.Drawing.Point(colStartX , li.Bounds.Y);

				cmbBox.Show();

				cmbBox.Text = subItemText;
				cmbBox.SelectAll();
				cmbBox.Focus();
			}
		}

		public void paramListView_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			li = paramListView.GetItemAt(e.X , e.Y);
			firstMouseClickPoint.X = e.X ;
			firstMouseClickPoint.Y = e.Y ;
		}

		private void functionNameComboBox_SelectedValueChanged(object sender, System.EventArgs e)
		{
			if (hasLoaded)
			{
				InterceptedFunctionDB db = InterceptedFunctionDB.GetInstance("functions.xml");
				InterceptedFunction f = db.GetFunctionByName(functionNameComboBox.SelectedItem.ToString());
				if (f != null)
				{
					this.functionGroupLabel.Text = f.Category;
					this.returnTypeLabel.Text = f.ReturnType;

					string modifier = null;
					foreach (string s in f.Modifiers)
					{
						if (modifier != null)
						{
							modifier += ", ";
						}

						modifier += s;
					}

					this.modifiersLabel.Text = modifier;

					// fill in the parameters
					paramListView.Items.Clear();
					foreach (InterceptedFunctionParameter param in f.Parameters)
					{
						ListViewItem item = new ListViewItem(param.Name);
						item.SubItems.Add(param.Type);
						param.TestValue = IgnoreParamString;
						item.SubItems.Add(param.TestValue);

						paramListView.Items.Add(item);
					}
				}
				function = f;
			}
		}
	}
}
