namespace Tile_Editor
{
    partial class ToolsWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToolsWindow));
            this.EventBrush = new System.Windows.Forms.Button();
            this.Fill = new System.Windows.Forms.Button();
            this.Eraser = new System.Windows.Forms.Button();
            this.Brush = new System.Windows.Forms.Button();
            this.Grid = new System.Windows.Forms.Button();
            this.SelectionTool = new System.Windows.Forms.Button();
            this.ToolsTab = new System.Windows.Forms.TabControl();
            this.ToolsPage = new System.Windows.Forms.TabPage();
            this.label25 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.EventEraser = new System.Windows.Forms.Button();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.EventsInfo = new System.Windows.Forms.TabPage();
            this.EventsGroupBox = new System.Windows.Forms.GroupBox();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.SpeedRampSpawn = new System.Windows.Forms.RadioButton();
            this.PlayerSpawn = new System.Windows.Forms.RadioButton();
            this.PowerUpSpawn = new System.Windows.Forms.RadioButton();
            this.ObstacleSpawn = new System.Windows.Forms.RadioButton();
            this.label17 = new System.Windows.Forms.Label();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.CCollision = new System.Windows.Forms.RadioButton();
            this.WCollision = new System.Windows.Forms.RadioButton();
            this.EventID = new System.Windows.Forms.TextBox();
            this.SettingsPage = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.PixelWidth = new System.Windows.Forms.NumericUpDown();
            this.PixelHeight = new System.Windows.Forms.NumericUpDown();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.MapNameTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.MapWidth = new System.Windows.Forms.NumericUpDown();
            this.MapHeight = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.TilesWidth = new System.Windows.Forms.NumericUpDown();
            this.TilesHeight = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.ToolsTab.SuspendLayout();
            this.ToolsPage.SuspendLayout();
            this.EventsInfo.SuspendLayout();
            this.EventsGroupBox.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.SettingsPage.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PixelWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PixelHeight)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MapWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.MapHeight)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TilesWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TilesHeight)).BeginInit();
            this.SuspendLayout();
            // 
            // EventBrush
            // 
            this.EventBrush.BackColor = System.Drawing.Color.Yellow;
            this.EventBrush.Image = ((System.Drawing.Image)(resources.GetObject("EventBrush.Image")));
            this.EventBrush.Location = new System.Drawing.Point(103, 227);
            this.EventBrush.Name = "EventBrush";
            this.EventBrush.Size = new System.Drawing.Size(40, 40);
            this.EventBrush.TabIndex = 5;
            this.EventBrush.UseVisualStyleBackColor = false;
            this.EventBrush.Click += new System.EventHandler(this.EventBrush_Click);
            // 
            // Fill
            // 
            this.Fill.Image = ((System.Drawing.Image)(resources.GetObject("Fill.Image")));
            this.Fill.Location = new System.Drawing.Point(167, 48);
            this.Fill.Name = "Fill";
            this.Fill.Size = new System.Drawing.Size(40, 40);
            this.Fill.TabIndex = 4;
            this.Fill.UseVisualStyleBackColor = true;
            this.Fill.Click += new System.EventHandler(this.Fill_Click);
            // 
            // Eraser
            // 
            this.Eraser.Image = ((System.Drawing.Image)(resources.GetObject("Eraser.Image")));
            this.Eraser.Location = new System.Drawing.Point(42, 139);
            this.Eraser.Name = "Eraser";
            this.Eraser.Size = new System.Drawing.Size(40, 40);
            this.Eraser.TabIndex = 3;
            this.Eraser.UseVisualStyleBackColor = true;
            this.Eraser.Click += new System.EventHandler(this.Eraser_Click);
            // 
            // Brush
            // 
            this.Brush.Image = ((System.Drawing.Image)(resources.GetObject("Brush.Image")));
            this.Brush.Location = new System.Drawing.Point(42, 227);
            this.Brush.Name = "Brush";
            this.Brush.Size = new System.Drawing.Size(40, 40);
            this.Brush.TabIndex = 2;
            this.Brush.UseVisualStyleBackColor = true;
            this.Brush.Click += new System.EventHandler(this.Brush_Click);
            // 
            // Grid
            // 
            this.Grid.Image = ((System.Drawing.Image)(resources.GetObject("Grid.Image")));
            this.Grid.Location = new System.Drawing.Point(103, 48);
            this.Grid.Name = "Grid";
            this.Grid.Size = new System.Drawing.Size(40, 40);
            this.Grid.TabIndex = 1;
            this.Grid.UseVisualStyleBackColor = true;
            this.Grid.Click += new System.EventHandler(this.Grid_Click);
            // 
            // SelectionTool
            // 
            this.SelectionTool.Image = ((System.Drawing.Image)(resources.GetObject("SelectionTool.Image")));
            this.SelectionTool.Location = new System.Drawing.Point(42, 48);
            this.SelectionTool.Name = "SelectionTool";
            this.SelectionTool.Size = new System.Drawing.Size(40, 40);
            this.SelectionTool.TabIndex = 0;
            this.SelectionTool.UseVisualStyleBackColor = true;
            this.SelectionTool.Click += new System.EventHandler(this.Selection_Click);
            // 
            // ToolsTab
            // 
            this.ToolsTab.Controls.Add(this.ToolsPage);
            this.ToolsTab.Controls.Add(this.EventsInfo);
            this.ToolsTab.Controls.Add(this.SettingsPage);
            this.ToolsTab.Location = new System.Drawing.Point(12, 12);
            this.ToolsTab.Name = "ToolsTab";
            this.ToolsTab.SelectedIndex = 0;
            this.ToolsTab.Size = new System.Drawing.Size(506, 373);
            this.ToolsTab.TabIndex = 9;
            // 
            // ToolsPage
            // 
            this.ToolsPage.Controls.Add(this.label25);
            this.ToolsPage.Controls.Add(this.label26);
            this.ToolsPage.Controls.Add(this.label23);
            this.ToolsPage.Controls.Add(this.label24);
            this.ToolsPage.Controls.Add(this.label22);
            this.ToolsPage.Controls.Add(this.EventEraser);
            this.ToolsPage.Controls.Add(this.label21);
            this.ToolsPage.Controls.Add(this.label20);
            this.ToolsPage.Controls.Add(this.Eraser);
            this.ToolsPage.Controls.Add(this.SelectionTool);
            this.ToolsPage.Controls.Add(this.Grid);
            this.ToolsPage.Controls.Add(this.Brush);
            this.ToolsPage.Controls.Add(this.EventBrush);
            this.ToolsPage.Controls.Add(this.Fill);
            this.ToolsPage.Location = new System.Drawing.Point(4, 22);
            this.ToolsPage.Name = "ToolsPage";
            this.ToolsPage.Padding = new System.Windows.Forms.Padding(3);
            this.ToolsPage.Size = new System.Drawing.Size(498, 347);
            this.ToolsPage.TabIndex = 0;
            this.ToolsPage.Text = "Tool Tab";
            this.ToolsPage.UseVisualStyleBackColor = true;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(94, 211);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(65, 13);
            this.label25.TabIndex = 15;
            this.label25.Text = "Event Brush";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(33, 211);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(54, 13);
            this.label26.TabIndex = 14;
            this.label26.Text = "Tile Brush";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(157, 32);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(56, 13);
            this.label23.TabIndex = 13;
            this.label23.Text = "Fill Bucket";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(96, 32);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(56, 13);
            this.label24.TabIndex = 12;
            this.label24.Text = "Grid Show";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(94, 123);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(68, 13);
            this.label22.TabIndex = 11;
            this.label22.Text = "Event Eraser";
            // 
            // EventEraser
            // 
            this.EventEraser.BackColor = System.Drawing.Color.Yellow;
            this.EventEraser.Image = ((System.Drawing.Image)(resources.GetObject("EventEraser.Image")));
            this.EventEraser.Location = new System.Drawing.Point(103, 139);
            this.EventEraser.Name = "EventEraser";
            this.EventEraser.Size = new System.Drawing.Size(40, 40);
            this.EventEraser.TabIndex = 10;
            this.EventEraser.UseVisualStyleBackColor = false;
            this.EventEraser.Click += new System.EventHandler(this.EventEraser_Click);
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(33, 123);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(57, 13);
            this.label21.TabIndex = 9;
            this.label21.Text = "Tile Eraser";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(39, 32);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(51, 13);
            this.label20.TabIndex = 8;
            this.label20.Text = "Selection";
            // 
            // EventsInfo
            // 
            this.EventsInfo.Controls.Add(this.EventsGroupBox);
            this.EventsInfo.Location = new System.Drawing.Point(4, 22);
            this.EventsInfo.Name = "EventsInfo";
            this.EventsInfo.Size = new System.Drawing.Size(498, 347);
            this.EventsInfo.TabIndex = 5;
            this.EventsInfo.Text = "Events Info";
            this.EventsInfo.UseVisualStyleBackColor = true;
            // 
            // EventsGroupBox
            // 
            this.EventsGroupBox.Controls.Add(this.groupBox9);
            this.EventsGroupBox.Controls.Add(this.label17);
            this.EventsGroupBox.Controls.Add(this.groupBox8);
            this.EventsGroupBox.Controls.Add(this.EventID);
            this.EventsGroupBox.Location = new System.Drawing.Point(17, 62);
            this.EventsGroupBox.Name = "EventsGroupBox";
            this.EventsGroupBox.Size = new System.Drawing.Size(439, 167);
            this.EventsGroupBox.TabIndex = 9;
            this.EventsGroupBox.TabStop = false;
            this.EventsGroupBox.Text = "Event Info";
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.SpeedRampSpawn);
            this.groupBox9.Controls.Add(this.PlayerSpawn);
            this.groupBox9.Controls.Add(this.PowerUpSpawn);
            this.groupBox9.Controls.Add(this.ObstacleSpawn);
            this.groupBox9.Location = new System.Drawing.Point(194, 31);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(201, 65);
            this.groupBox9.TabIndex = 12;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Spawns";
            // 
            // SpeedRampSpawn
            // 
            this.SpeedRampSpawn.AutoSize = true;
            this.SpeedRampSpawn.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.SpeedRampSpawn.Location = new System.Drawing.Point(107, 42);
            this.SpeedRampSpawn.Name = "SpeedRampSpawn";
            this.SpeedRampSpawn.Size = new System.Drawing.Size(87, 17);
            this.SpeedRampSpawn.TabIndex = 11;
            this.SpeedRampSpawn.TabStop = true;
            this.SpeedRampSpawn.Text = "Speed Ramp";
            this.SpeedRampSpawn.UseVisualStyleBackColor = false;
            this.SpeedRampSpawn.CheckedChanged += new System.EventHandler(this.SpeedRampSpawn_CheckedChanged_1);
            // 
            // PlayerSpawn
            // 
            this.PlayerSpawn.AutoSize = true;
            this.PlayerSpawn.BackColor = System.Drawing.Color.Blue;
            this.PlayerSpawn.Location = new System.Drawing.Point(6, 19);
            this.PlayerSpawn.Name = "PlayerSpawn";
            this.PlayerSpawn.Size = new System.Drawing.Size(54, 17);
            this.PlayerSpawn.TabIndex = 7;
            this.PlayerSpawn.TabStop = true;
            this.PlayerSpawn.Text = "Player";
            this.PlayerSpawn.UseVisualStyleBackColor = false;
            this.PlayerSpawn.CheckedChanged += new System.EventHandler(this.PlayerSpawn_CheckedChanged);
            // 
            // PowerUpSpawn
            // 
            this.PowerUpSpawn.AutoSize = true;
            this.PowerUpSpawn.BackColor = System.Drawing.Color.Red;
            this.PowerUpSpawn.Location = new System.Drawing.Point(107, 19);
            this.PowerUpSpawn.Name = "PowerUpSpawn";
            this.PowerUpSpawn.Size = new System.Drawing.Size(72, 17);
            this.PowerUpSpawn.TabIndex = 9;
            this.PowerUpSpawn.TabStop = true;
            this.PowerUpSpawn.Text = "Power Up";
            this.PowerUpSpawn.UseVisualStyleBackColor = false;
            this.PowerUpSpawn.CheckedChanged += new System.EventHandler(this.PowerUpSpawn_CheckedChanged_1);
            // 
            // ObstacleSpawn
            // 
            this.ObstacleSpawn.AutoSize = true;
            this.ObstacleSpawn.BackColor = System.Drawing.Color.Cyan;
            this.ObstacleSpawn.Location = new System.Drawing.Point(6, 42);
            this.ObstacleSpawn.Name = "ObstacleSpawn";
            this.ObstacleSpawn.Size = new System.Drawing.Size(67, 17);
            this.ObstacleSpawn.TabIndex = 8;
            this.ObstacleSpawn.TabStop = true;
            this.ObstacleSpawn.Text = "Obstacle";
            this.ObstacleSpawn.UseVisualStyleBackColor = false;
            this.ObstacleSpawn.CheckedChanged += new System.EventHandler(this.ObstacleSpawn_CheckedChanged_1);
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(107, 116);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(49, 13);
            this.label17.TabIndex = 5;
            this.label17.Text = "Event ID";
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.CCollision);
            this.groupBox8.Controls.Add(this.WCollision);
            this.groupBox8.Location = new System.Drawing.Point(9, 31);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(174, 48);
            this.groupBox8.TabIndex = 11;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Collision";
            // 
            // CCollision
            // 
            this.CCollision.AutoSize = true;
            this.CCollision.BackColor = System.Drawing.Color.Green;
            this.CCollision.Location = new System.Drawing.Point(11, 19);
            this.CCollision.Name = "CCollision";
            this.CCollision.Size = new System.Drawing.Size(61, 17);
            this.CCollision.TabIndex = 10;
            this.CCollision.TabStop = true;
            this.CCollision.Text = "Camera";
            this.CCollision.UseVisualStyleBackColor = false;
            this.CCollision.CheckedChanged += new System.EventHandler(this.CCollision_CheckedChanged);
            // 
            // WCollision
            // 
            this.WCollision.AutoSize = true;
            this.WCollision.BackColor = System.Drawing.Color.Lime;
            this.WCollision.Location = new System.Drawing.Point(88, 19);
            this.WCollision.Name = "WCollision";
            this.WCollision.Size = new System.Drawing.Size(46, 17);
            this.WCollision.TabIndex = 6;
            this.WCollision.TabStop = true;
            this.WCollision.Text = "Wall";
            this.WCollision.UseVisualStyleBackColor = false;
            this.WCollision.CheckedChanged += new System.EventHandler(this.WCollision_CheckedChanged);
            // 
            // EventID
            // 
            this.EventID.Location = new System.Drawing.Point(162, 113);
            this.EventID.Name = "EventID";
            this.EventID.Size = new System.Drawing.Size(100, 20);
            this.EventID.TabIndex = 2;
            this.EventID.Text = "NULL";
            // 
            // SettingsPage
            // 
            this.SettingsPage.Controls.Add(this.groupBox3);
            this.SettingsPage.Controls.Add(this.groupBox1);
            this.SettingsPage.Controls.Add(this.groupBox2);
            this.SettingsPage.Location = new System.Drawing.Point(4, 22);
            this.SettingsPage.Name = "SettingsPage";
            this.SettingsPage.Padding = new System.Windows.Forms.Padding(3);
            this.SettingsPage.Size = new System.Drawing.Size(498, 347);
            this.SettingsPage.TabIndex = 1;
            this.SettingsPage.Text = "Settings Page";
            this.SettingsPage.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.PixelWidth);
            this.groupBox3.Controls.Add(this.PixelHeight);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label7);
            this.groupBox3.Location = new System.Drawing.Point(128, 205);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(222, 91);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Pixels";
            // 
            // PixelWidth
            // 
            this.PixelWidth.Location = new System.Drawing.Point(64, 30);
            this.PixelWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.PixelWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PixelWidth.Name = "PixelWidth";
            this.PixelWidth.Size = new System.Drawing.Size(120, 20);
            this.PixelWidth.TabIndex = 1;
            this.PixelWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PixelWidth.ValueChanged += new System.EventHandler(this.PixelWidth_ValueChanged);
            // 
            // PixelHeight
            // 
            this.PixelHeight.Location = new System.Drawing.Point(64, 56);
            this.PixelHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.PixelHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PixelHeight.Name = "PixelHeight";
            this.PixelHeight.Size = new System.Drawing.Size(120, 20);
            this.PixelHeight.TabIndex = 3;
            this.PixelHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.PixelHeight.ValueChanged += new System.EventHandler(this.PixelHeight_ValueChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 37);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(35, 13);
            this.label6.TabIndex = 0;
            this.label6.Text = "Width";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 63);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(38, 13);
            this.label7.TabIndex = 2;
            this.label7.Text = "Height";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.MapNameTextBox);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.MapWidth);
            this.groupBox1.Controls.Add(this.MapHeight);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Location = new System.Drawing.Point(16, 26);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(222, 143);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Map";
            // 
            // MapNameTextBox
            // 
            this.MapNameTextBox.Location = new System.Drawing.Point(64, 27);
            this.MapNameTextBox.Name = "MapNameTextBox";
            this.MapNameTextBox.Size = new System.Drawing.Size(120, 20);
            this.MapNameTextBox.TabIndex = 5;
            this.MapNameTextBox.Text = "Untitled";
            this.MapNameTextBox.TextChanged += new System.EventHandler(this.MapName_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 34);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Name";
            // 
            // MapWidth
            // 
            this.MapWidth.Location = new System.Drawing.Point(64, 72);
            this.MapWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.MapWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MapWidth.Name = "MapWidth";
            this.MapWidth.Size = new System.Drawing.Size(120, 20);
            this.MapWidth.TabIndex = 1;
            this.MapWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MapWidth.ValueChanged += new System.EventHandler(this.MapWidth_ValueChanged);
            // 
            // MapHeight
            // 
            this.MapHeight.Location = new System.Drawing.Point(64, 98);
            this.MapHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.MapHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MapHeight.Name = "MapHeight";
            this.MapHeight.Size = new System.Drawing.Size(120, 20);
            this.MapHeight.TabIndex = 3;
            this.MapHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.MapHeight.ValueChanged += new System.EventHandler(this.MapHeight_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 79);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Width";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 105);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Height";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.TilesWidth);
            this.groupBox2.Controls.Add(this.TilesHeight);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(253, 27);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(222, 91);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tiles";
            // 
            // TilesWidth
            // 
            this.TilesWidth.Location = new System.Drawing.Point(64, 30);
            this.TilesWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.TilesWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.TilesWidth.Name = "TilesWidth";
            this.TilesWidth.Size = new System.Drawing.Size(120, 20);
            this.TilesWidth.TabIndex = 1;
            this.TilesWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.TilesWidth.ValueChanged += new System.EventHandler(this.TilesWidth_ValueChanged);
            // 
            // TilesHeight
            // 
            this.TilesHeight.Location = new System.Drawing.Point(64, 56);
            this.TilesHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.TilesHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.TilesHeight.Name = "TilesHeight";
            this.TilesHeight.Size = new System.Drawing.Size(120, 20);
            this.TilesHeight.TabIndex = 3;
            this.TilesHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.TilesHeight.ValueChanged += new System.EventHandler(this.TilesHeight_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 37);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Width";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 63);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(38, 13);
            this.label5.TabIndex = 2;
            this.label5.Text = "Height";
            // 
            // ToolsWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(530, 410);
            this.Controls.Add(this.ToolsTab);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "ToolsWindow";
            this.Text = "Tools Window";
            this.ToolsTab.ResumeLayout(false);
            this.ToolsPage.ResumeLayout(false);
            this.ToolsPage.PerformLayout();
            this.EventsInfo.ResumeLayout(false);
            this.EventsGroupBox.ResumeLayout(false);
            this.EventsGroupBox.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.SettingsPage.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PixelWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PixelHeight)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MapWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.MapHeight)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TilesWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TilesHeight)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button SelectionTool;
        private System.Windows.Forms.Button Grid;
        private System.Windows.Forms.Button Brush;
        private System.Windows.Forms.Button Eraser;
        private System.Windows.Forms.Button Fill;
        private System.Windows.Forms.Button EventBrush;
        private System.Windows.Forms.TabControl ToolsTab;
        private System.Windows.Forms.TabPage ToolsPage;
        private System.Windows.Forms.TabPage SettingsPage;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.NumericUpDown PixelWidth;
        private System.Windows.Forms.NumericUpDown PixelHeight;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox MapNameTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown MapWidth;
        private System.Windows.Forms.NumericUpDown MapHeight;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown TilesWidth;
        private System.Windows.Forms.NumericUpDown TilesHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TabPage EventsInfo;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.RadioButton SpeedRampSpawn;
        private System.Windows.Forms.RadioButton PlayerSpawn;
        private System.Windows.Forms.RadioButton PowerUpSpawn;
        private System.Windows.Forms.RadioButton ObstacleSpawn;
        private System.Windows.Forms.GroupBox EventsGroupBox;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox EventID;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Button EventEraser;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.RadioButton CCollision;
        private System.Windows.Forms.RadioButton WCollision;
    }
}