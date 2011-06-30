namespace ParticleTool
{
    partial class ToolWindow
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.AlphaNumeric = new System.Windows.Forms.NumericUpDown();
            this.RedNumeric = new System.Windows.Forms.NumericUpDown();
            this.GreenNumeric = new System.Windows.Forms.NumericUpDown();
            this.BlueNumeric = new System.Windows.Forms.NumericUpDown();
            this.UpdateColorsButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.AlphaNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RedNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.GreenNumeric)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.BlueNumeric)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(32, 56);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(34, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Alpha";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(32, 95);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Red";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(32, 134);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Green";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(32, 176);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Blue";
            // 
            // AlphaNumeric
            // 
            this.AlphaNumeric.Location = new System.Drawing.Point(101, 48);
            this.AlphaNumeric.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.AlphaNumeric.Name = "AlphaNumeric";
            this.AlphaNumeric.Size = new System.Drawing.Size(66, 20);
            this.AlphaNumeric.TabIndex = 4;
            this.AlphaNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // RedNumeric
            // 
            this.RedNumeric.Location = new System.Drawing.Point(101, 88);
            this.RedNumeric.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.RedNumeric.Name = "RedNumeric";
            this.RedNumeric.Size = new System.Drawing.Size(66, 20);
            this.RedNumeric.TabIndex = 5;
            this.RedNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // GreenNumeric
            // 
            this.GreenNumeric.Location = new System.Drawing.Point(101, 132);
            this.GreenNumeric.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.GreenNumeric.Name = "GreenNumeric";
            this.GreenNumeric.Size = new System.Drawing.Size(66, 20);
            this.GreenNumeric.TabIndex = 6;
            this.GreenNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // BlueNumeric
            // 
            this.BlueNumeric.Location = new System.Drawing.Point(101, 174);
            this.BlueNumeric.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.BlueNumeric.Name = "BlueNumeric";
            this.BlueNumeric.Size = new System.Drawing.Size(66, 20);
            this.BlueNumeric.TabIndex = 7;
            this.BlueNumeric.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // UpdateColorsButton
            // 
            this.UpdateColorsButton.AutoSize = true;
            this.UpdateColorsButton.Location = new System.Drawing.Point(52, 218);
            this.UpdateColorsButton.Name = "UpdateColorsButton";
            this.UpdateColorsButton.Size = new System.Drawing.Size(84, 23);
            this.UpdateColorsButton.TabIndex = 8;
            this.UpdateColorsButton.Text = "Update Colors";
            this.UpdateColorsButton.UseVisualStyleBackColor = true;
            this.UpdateColorsButton.Click += new System.EventHandler(this.UpdateColorsButton_Click);
            // 
            // ToolWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(213, 259);
            this.Controls.Add(this.UpdateColorsButton);
            this.Controls.Add(this.BlueNumeric);
            this.Controls.Add(this.GreenNumeric);
            this.Controls.Add(this.RedNumeric);
            this.Controls.Add(this.AlphaNumeric);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "ToolWindow";
            this.Text = "ToolWindow";
            ((System.ComponentModel.ISupportInitialize)(this.AlphaNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RedNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.GreenNumeric)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.BlueNumeric)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown AlphaNumeric;
        private System.Windows.Forms.NumericUpDown RedNumeric;
        private System.Windows.Forms.NumericUpDown GreenNumeric;
        private System.Windows.Forms.NumericUpDown BlueNumeric;
        private System.Windows.Forms.Button UpdateColorsButton;
    }
}