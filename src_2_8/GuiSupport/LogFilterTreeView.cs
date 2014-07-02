using System;
using System.Collections;
using System.Windows.Forms;
using FunctionDatabase;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for LogFilterTreeView.
	/// </summary>
	public class LogFilterTreeView : System.Windows.Forms.TreeView
	{
		private TreeNode regNode;
		private TreeNode libNode;
		private TreeNode procNode;
		private TreeNode netNode;
		private TreeNode fileNode;
		private TreeNode memNode;
		private Array registryOperations;
		private Array libraryOperations;
		private Array processOperations;
		private Array networkOperations;
		private Array fileOperations;
		private Array memoryOperations;
		private ArrayList filteredOutFunctions;
		private TreeViewEventHandler eventHandler;

		public ArrayList FilteredOutFunctionNames
		{
			get	{ return filteredOutFunctions; }
			set
			{
				ArrayList tempFilteredOutFunctions = (ArrayList)value.Clone();

				bool checkRegCategory = true;
				bool checkLibCategory = true;
				bool checkProcCategory = true;
				bool checkNetCategory = true;
				bool checkFileCategory = true;
				bool checkMemCategory = true;

				// determine whether or not the category nodes should be checked
				foreach (TreeNode n in regNode.Nodes)
					checkRegCategory &= (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in libNode.Nodes)
					checkLibCategory &= (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in procNode.Nodes)
					checkProcCategory &= (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in netNode.Nodes)
					checkNetCategory &= (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in fileNode.Nodes)
					checkFileCategory &= (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in memNode.Nodes)
					checkMemCategory &= (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				regNode.Checked = checkRegCategory;
				libNode.Checked = checkLibCategory;
				procNode.Checked = checkProcCategory;
				netNode.Checked = checkNetCategory;
				fileNode.Checked = checkFileCategory;
				memNode.Checked = checkMemCategory;

				// set the nodes' check state
				foreach (TreeNode n in regNode.Nodes)
					n.Checked = (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in libNode.Nodes)
					n.Checked = (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				foreach (TreeNode n in procNode.Nodes)
					n.Checked = (tempFilteredOutFunctions.IndexOf(n.Text) == -1);
				
				foreach (TreeNode n in netNode.Nodes)
					n.Checked = (tempFilteredOutFunctions.IndexOf(n.Text) == -1);
				
				foreach (TreeNode n in fileNode.Nodes)
					n.Checked = (tempFilteredOutFunctions.IndexOf(n.Text) == -1);
				
				foreach (TreeNode n in memNode.Nodes)
					n.Checked = (tempFilteredOutFunctions.IndexOf(n.Text) == -1);

				filteredOutFunctions = tempFilteredOutFunctions;
			}
		}

		public LogFilterTreeView()
		{
			this.CheckBoxes = true;
			this.ImageIndex = -1;
			this.Name = "filterTreeView";
			this.SelectedImageIndex = -1;
			this.Size = new System.Drawing.Size(424, 360);
			this.eventHandler = new System.Windows.Forms.TreeViewEventHandler(this.filterTreeView_AfterCheck);
			this.AfterCheck += eventHandler;

			filteredOutFunctions = new ArrayList();

			InterceptedFunctionDB db = InterceptedFunctionDB.GetInstance("functions.xml");

			registryOperations = db.GetFunctionCategoryArray("REGISTRY");
			libraryOperations = db.GetFunctionCategoryArray("LIBRARY");
			processOperations = db.GetFunctionCategoryArray("PROCESS");
			networkOperations = db.GetFunctionCategoryArray("NETWORK");
			fileOperations = db.GetFunctionCategoryArray("FILE");
			memoryOperations = db.GetFunctionCategoryArray("MEMORY");

			regNode = new TreeNode("Registry Operations");
			libNode = new TreeNode("Library Operations");
			procNode = new TreeNode("Process Operations");
			netNode = new TreeNode("Network Operations");
			fileNode = new TreeNode("File Operations");
			memNode = new TreeNode("Memory Operations");

			regNode.Checked = true;
			libNode.Checked = true;
			procNode.Checked = true;
			netNode.Checked = true;
			fileNode.Checked = true;
			memNode.Checked = true;

			this.Nodes.Add(fileNode);
			this.Nodes.Add(libNode);
			this.Nodes.Add(memNode);
			this.Nodes.Add(netNode);
			this.Nodes.Add(procNode);
			this.Nodes.Add(regNode);

			foreach(InterceptedFunction f in registryOperations)
			{
				TreeNode node = new TreeNode(f.Name);
				node.Checked = true;
				regNode.Nodes.Add(node);
			}

			foreach(InterceptedFunction f in libraryOperations)
			{
				TreeNode node = new TreeNode(f.Name);
				node.Checked = true;
				libNode.Nodes.Add(node);
			}

			foreach(InterceptedFunction f in processOperations)
			{
				TreeNode node = new TreeNode(f.Name);
				node.Checked = true;
				procNode.Nodes.Add(node);
			}

			foreach(InterceptedFunction f in networkOperations)
			{
				TreeNode node = new TreeNode(f.Name);
				node.Checked = true;
				netNode.Nodes.Add(node);
			}

			foreach(InterceptedFunction f in fileOperations)
			{
				TreeNode node = new TreeNode(f.Name);
				node.Checked = true;
				fileNode.Nodes.Add(node);
			}

			foreach(InterceptedFunction f in memoryOperations)
			{
				TreeNode node = new TreeNode(f.Name);
				node.Checked = true;
				memNode.Nodes.Add(node);
			}
		}

		private void setNodeChecked(TreeNode node, bool check)
		{
			foreach (TreeNode n in node.Nodes)
			{
				n.Checked = check;
				
				if (!check)
				{
					filteredOutFunctions.Add(node.Text);
				}
				else
				{
					filteredOutFunctions.Remove(node.Text);
				}
			}
		}

		private void filterTreeView_AfterCheck(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			if (!e.Node.Checked)
			{
				filteredOutFunctions.Add(e.Node.Text);
			}
			else
			{
				filteredOutFunctions.Remove(e.Node.Text);
			}

			setNodeChecked(e.Node, e.Node.Checked);

			if (e.Node.Parent != null)
			{
				// disable event handler for setting the category check state
				this.AfterCheck -= eventHandler;

				bool allChecked = true;
				foreach (TreeNode n in e.Node.Parent.Nodes)
					allChecked &= n.Checked;
				
				e.Node.Parent.Checked = allChecked;

				// reenable event handler
				this.AfterCheck += eventHandler;
			}
		}
	}
}
