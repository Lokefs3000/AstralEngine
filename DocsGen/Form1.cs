using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DocsGen
{
    public partial class Form1 : Form
    {
        Dictionary<TreeNode, string> Entries;
        TreeNode TargetNode = null;

        public Form1()
        {
            InitializeComponent();

            Entries = new Dictionary<TreeNode, string>();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void exportToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void saveToolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode node = new TreeNode("New document");
            treeView1.Nodes.Add(node);
            Entries.Add(node, "");
        }

        private void duplicateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;

            TreeNode copy = (TreeNode)TargetNode.Clone();
            copy.Text += " - copy";

            Entries.Add(copy, "");
            treeView1.Nodes.Add(copy);
        }

        private void removeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;

            Entries.Remove(TargetNode);
            treeView1.Nodes.Remove(TargetNode);
        }

        private void upToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;

            for (int i = 0; i < treeView1.Nodes.Count; i++)
            {
                if (treeView1.Nodes[i] == TargetNode)
                {
                    if (i == 0)
                        break;

                    treeView1.Nodes.RemoveAt(i);

                    TargetNode.Nodes.Insert(i - 1, treeView1.Nodes[i]);

                    break;
                }
            }
        }

        private void downToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;
        }

        private void leftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;
        }

        private void rightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;
        }

        private void changeLabelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;

            TextEdit te = new TextEdit();
            te.ShowDialog();

            if (te.NewLabel != string.Empty)
            {
                TargetNode.Text = te.NewLabel;
            }
        }

        private void yesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;
        }

        private void noToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (TargetNode == null)
                return;
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            TargetNode = e.Node;
        }
    }
}
