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
    public partial class RandomizeWindow : Form
    {
        public event EventHandler Randomize;

        public RandomizeWindow()
        {
            InitializeComponent();

            NumParticlesCheckBox.Checked = false;
        }

        public bool NumParticles
        {
            get { return NumParticlesCheckBox.Checked; }
        }

        public bool MinLife
        {
            get { return MinLifeCheckBox.Checked; }
        }

        public bool MaxLife
        {
            get { return MaxLifeCheckBox.Checked; }
        }

        public bool StartXScale
        {
            get { return StartXScaleCheckBox.Checked; }
        }

        public bool EndXScale
        {
            get { return EndXScaleCheckBox.Checked; }
        }

        public bool StartYScale
        {
            get { return StartYScalecheckBox.Checked; }
        }

        public bool EndYScale
        {
            get { return EndYScalecheckBox.Checked; }
        }

        public bool MinXVel
        {
            get { return MinXVelcheckBox.Checked; }
        }

        public bool MaxXVel
        {
            get { return MaxXVelcheckBox.Checked; }
        }

        public bool MinYVel
        {
            get { return MinYVelcheckBox.Checked; }
        }

        public bool MaxYVel
        {
            get { return MaxYVelcheckBox.Checked; }
        }

        public bool AccelX
        {
            get { return AccelXcheckBox.Checked; }
        }

        public bool AccelY
        {
            get { return AccelYcheckBox.Checked; }
        }

        public bool Rotation
        {
            get { return RotationcheckBox.Checked; }
        }

        public bool SourceBlend
        {
            get { return SourcecheckBox.Checked; }
        }
        public bool DestinationBlend
        {
            get { return DestinationcheckBox.Checked; }
        }
        public bool StartColor
        {
            get { return StartColorcheckBox.Checked; }
        }
        public bool EndColor
        {
            get { return EndColorcheckBox.Checked; }
        }

        public bool CheckAll
        {
            get { return CheckAllcheckBox.Checked; }
        }

        private void RandomButton_Click(object sender, EventArgs e)
        {
            if (Randomize != null)
            {
                Randomize(this, EventArgs.Empty);
            }
        }

        private void CheckAllcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (CheckAllcheckBox.Checked)
            {
                NumParticlesCheckBox.Checked =  true;
                MinLifeCheckBox.Checked =       true;
                MaxLifeCheckBox.Checked =       true;
                StartXScaleCheckBox.Checked =   true;
                EndXScaleCheckBox.Checked =     true;
                StartYScalecheckBox.Checked =   true;
                EndYScalecheckBox.Checked =     true;
                StartColorcheckBox.Checked =    true;
                EndColorcheckBox.Checked =      true;
                MinXVelcheckBox.Checked =       true;
                MaxXVelcheckBox.Checked =       true;
                MinYVelcheckBox.Checked =       true;
                MaxYVelcheckBox.Checked =       true;
                AccelXcheckBox.Checked =        true;
                AccelYcheckBox.Checked =        true;
                RotationcheckBox.Checked =      true;
                SourcecheckBox.Checked =        true;
                DestinationcheckBox.Checked =   true;
            }
            else
            {
                NumParticlesCheckBox.Checked =  false;
                MinLifeCheckBox.Checked =       false;
                MaxLifeCheckBox.Checked =       false;
                StartXScaleCheckBox.Checked =   false;
                EndXScaleCheckBox.Checked =     false;
                StartYScalecheckBox.Checked =   false;
                EndYScalecheckBox.Checked =     false;
                StartColorcheckBox.Checked =    false;
                EndColorcheckBox.Checked =      false;
                MinXVelcheckBox.Checked =       false;
                MaxXVelcheckBox.Checked =       false;
                MinYVelcheckBox.Checked =       false;
                MaxYVelcheckBox.Checked =       false;
                AccelXcheckBox.Checked =        false;
                AccelYcheckBox.Checked =        false;
                RotationcheckBox.Checked =      false;
                SourcecheckBox.Checked =        false;
                DestinationcheckBox.Checked =   false;
            }
        }

        
    }
}
