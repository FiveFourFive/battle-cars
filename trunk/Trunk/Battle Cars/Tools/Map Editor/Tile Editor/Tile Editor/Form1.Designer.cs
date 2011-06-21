namespace Tile_Editor
{
    partial class MapEditor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadTileSetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.worldSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tileWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.collisionWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.eventsWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.spawnsWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MainMapPanel = new System.Windows.Forms.Panel();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(646, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.loadTileSetToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // loadTileSetToolStripMenuItem
            // 
            this.loadTileSetToolStripMenuItem.Name = "loadTileSetToolStripMenuItem";
            this.loadTileSetToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.loadTileSetToolStripMenuItem.Text = "Load Tile Set";
            this.loadTileSetToolStripMenuItem.Click += new System.EventHandler(this.loadTileSetToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(141, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolsWindowToolStripMenuItem,
            this.worldSettingsToolStripMenuItem,
            this.tileWindowToolStripMenuItem,
            this.collisionWindowToolStripMenuItem,
            this.eventsWindowToolStripMenuItem,
            this.spawnsWindowToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // toolsWindowToolStripMenuItem
            // 
            this.toolsWindowToolStripMenuItem.Name = "toolsWindowToolStripMenuItem";
            this.toolsWindowToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.toolsWindowToolStripMenuItem.Text = "Tools Window";
            this.toolsWindowToolStripMenuItem.Click += new System.EventHandler(this.toolsWindowToolStripMenuItem_Click);
            // 
            // worldSettingsToolStripMenuItem
            // 
            this.worldSettingsToolStripMenuItem.Name = "worldSettingsToolStripMenuItem";
            this.worldSettingsToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.worldSettingsToolStripMenuItem.Text = "World Settings";
            // 
            // tileWindowToolStripMenuItem
            // 
            this.tileWindowToolStripMenuItem.Name = "tileWindowToolStripMenuItem";
            this.tileWindowToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.tileWindowToolStripMenuItem.Text = "Tile Window";
            this.tileWindowToolStripMenuItem.Click += new System.EventHandler(this.tileWindowToolStripMenuItem_Click);
            // 
            // collisionWindowToolStripMenuItem
            // 
            this.collisionWindowToolStripMenuItem.Name = "collisionWindowToolStripMenuItem";
            this.collisionWindowToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.collisionWindowToolStripMenuItem.Text = "Collisions Window";
            // 
            // eventsWindowToolStripMenuItem
            // 
            this.eventsWindowToolStripMenuItem.Name = "eventsWindowToolStripMenuItem";
            this.eventsWindowToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.eventsWindowToolStripMenuItem.Text = "Events Window";
            // 
            // spawnsWindowToolStripMenuItem
            // 
            this.spawnsWindowToolStripMenuItem.Name = "spawnsWindowToolStripMenuItem";
            this.spawnsWindowToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.spawnsWindowToolStripMenuItem.Text = "Spawns Window";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            this.helpToolStripMenuItem.Click += new System.EventHandler(this.helpToolStripMenuItem_Click);
            // 
            // MainMapPanel
            // 
            this.MainMapPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainMapPanel.Location = new System.Drawing.Point(0, 24);
            this.MainMapPanel.MinimumSize = new System.Drawing.Size(1, 1);
            this.MainMapPanel.Name = "MainMapPanel";
            this.MainMapPanel.Size = new System.Drawing.Size(646, 532);
            this.MainMapPanel.TabIndex = 1;
            this.MainMapPanel.MouseClick += new System.Windows.Forms.MouseEventHandler(this.MainMapPanel_MouseClick);
            this.MainMapPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MainMapPanel_MouseMove);
            // 
            // MapEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(646, 556);
            this.Controls.Add(this.MainMapPanel);
            this.Controls.Add(this.menuStrip1);
            this.Name = "MapEditor";
            this.Text = "Battle Cars Map Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MapEditor_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsWindowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem worldSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tileWindowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem collisionWindowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem eventsWindowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem spawnsWindowToolStripMenuItem;
        private System.Windows.Forms.Panel MainMapPanel;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripMenuItem loadTileSetToolStripMenuItem;
    }
}

