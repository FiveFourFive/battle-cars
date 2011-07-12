using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ParticleTool
{
    public partial class ToolWindow : Form
    {
        public event EventHandler UpdateColors;

        public ToolWindow()
        {
            InitializeComponent();

            AlphaNumeric.Value = 0;
            RedNumeric.Value = 0;
            GreenNumeric.Value = 0;
            BlueNumeric.Value = 0;
        }

        public decimal AlphaNumericValue
        { 
            get { return AlphaNumeric.Value; }
            set { AlphaNumeric.Value = value; }
        }

        public decimal RedNumericValue
        {
            get { return RedNumeric.Value; }
            set { RedNumeric.Value = value; }
        }

        public decimal GreenNumericValue
        {
            get { return GreenNumeric.Value; }
            set { GreenNumeric.Value = value; }
        }

        public decimal BlueNumericValue
        {
            get { return BlueNumeric.Value; }
            set { BlueNumeric.Value = value; }
        }

        private void UpdateColorsButton_Click(object sender, EventArgs e)
        {
            if (UpdateColors != null)
            {
                UpdateColors(this, EventArgs.Empty);
            }
        }
    }
}
