using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.Samples.DirectX.UtilityToolkit;
using Microsoft.DirectX.Direct3D;
using System.Xml.Linq;
using SGD;


namespace ParticleTool
{

    public partial class Form1 : Form
    {
        ParticleEmitter emittor = new ParticleEmitter();
        ManagedDirect3D d3d = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;
        public string[] directory;

        Color BG = Color.Black;

        Blend Source;
        Blend Destination;

        ToolWindow toolStartColor = null;
        ToolWindow toolEndColor = null;
        RandomizeWindow RandomWindow = null;

        public bool Looping;
        public decimal deltaTime = 0;
        public double timeslice = 0.0;
        public decimal elapsedEmissionTimer = 0;

        Random rand = new Random();

        public Form1()
        {
            InitializeComponent();
            Looping = true;

            d3d.InitManagedDirect3D(pictureBox1, true);
            TM.InitManagedTextureManager(d3d.Device, d3d.Sprite);

            FrameworkTimer.Start();

            string temp_imagename = "../../resources/particles/raindrop.tga";

            emittor.Imagename = temp_imagename[26].ToString();

            for (int i = 27; i < temp_imagename.Length; i++)
                emittor.Imagename += temp_imagename[i].ToString();

            emittor.TextureID1 = TM.LoadTexture(temp_imagename, Color.FromArgb(255, 0, 0, 0).ToArgb());

            numericParticleCount.Value = emittor.Max_Number1;
            numericParticleEndLife.Value = emittor.EndLife1;
            numericParticleMinLife.Value = emittor.MinLife1;
            numericScaleBegX.Value = emittor.Start_scaleX;
            numericScaleEndX.Value = emittor.End_scaleX;
            numericScaleBegY.Value = emittor.Start_scaleY;
            numericScaleEndY.Value = emittor.End_scaleY;
            numericVelMinX.Value = emittor.VelminX;
            numericVelMinY.Value = emittor.VelminY;
            numericVelMaxX.Value = emittor.VelmaxX;
            numericVelMaxY.Value = emittor.VelmaxY;

            numericAccelX.Value = emittor.AccelX;
            numericAccelY.Value = emittor.AccelY;

            SourceBlendComboBox.SelectedIndex = 4;
            DestinationBlendComboBox.SelectedIndex = 5;

            numericRotation.Value = emittor.Rotation;
        }

        private void StartColorButton_Click(object sender, EventArgs e)
        {
            if (toolStartColor == null)
            {
                toolStartColor = new ToolWindow();
          

                toolStartColor.AlphaNumericValue = emittor.Start_Color1.A;
                toolStartColor.RedNumericValue = emittor.Start_Color1.R;
                toolStartColor.BlueNumericValue = emittor.Start_Color1.B;
                toolStartColor.GreenNumericValue = emittor.Start_Color1.G;

                toolStartColor.FormClosed += new FormClosedEventHandler(toolStartColor_FormClosed);
                toolStartColor.UpdateColors += new EventHandler(toolStartColor_UpdateColors);

                ColorDialog dlg = new ColorDialog();

                dlg.Color = emittor.Start_Color1;

                


                toolStartColor.Show(this);
                if (DialogResult.OK == dlg.ShowDialog())
                {
                    toolStartColor.AlphaNumericValue = dlg.Color.A;
                    toolStartColor.RedNumericValue = dlg.Color.R;
                    toolStartColor.BlueNumericValue = dlg.Color.B;
                    toolStartColor.GreenNumericValue = dlg.Color.G;

                    emittor.Start_Color1 = dlg.Color;
                }
            }
   
        }

        void toolStartColor_UpdateColors(object sender, EventArgs e)
        {
            byte alpha = (byte)toolStartColor.AlphaNumericValue;
            byte red = (byte)toolStartColor.RedNumericValue;
            byte green = (byte)toolStartColor.GreenNumericValue;
            byte blue = (byte)toolStartColor.BlueNumericValue;

            emittor.Start_Color1 = Color.FromArgb(alpha, red, green, blue);


        }

        void toolEndColor_UpdateColors(object sender, EventArgs e)
        {
            byte alpha = (byte)toolEndColor.AlphaNumericValue;
            byte red = (byte)toolEndColor.RedNumericValue;
            byte green = (byte)toolEndColor.GreenNumericValue;
            byte blue = (byte)toolEndColor.BlueNumericValue;

            emittor.End_Color1 = Color.FromArgb(alpha, red, green, blue);
        }


        private void EndColorButton_Click(object sender, EventArgs e)
        {
            if (toolEndColor == null)
            {
                toolEndColor = new ToolWindow();

                toolEndColor.AlphaNumericValue = emittor.End_Color1.A;
                toolEndColor.RedNumericValue = emittor.End_Color1.R;
                toolEndColor.BlueNumericValue = emittor.End_Color1.B;
                toolEndColor.GreenNumericValue = emittor.End_Color1.G;

                toolEndColor.FormClosed += new FormClosedEventHandler(toolEndColor_FormClosed);
                toolEndColor.UpdateColors += new EventHandler(toolEndColor_UpdateColors);

                ColorDialog dlg = new ColorDialog();
                
                dlg.Color = emittor.End_Color1;

                toolEndColor.Show(this);

                if (DialogResult.OK == dlg.ShowDialog())
                {
                    toolEndColor.AlphaNumericValue = dlg.Color.A;
                    toolEndColor.RedNumericValue = dlg.Color.R;
                    toolEndColor.BlueNumericValue = dlg.Color.B;
                    toolEndColor.GreenNumericValue = dlg.Color.G;

                    emittor.End_Color1 = dlg.Color;
                }
            }
        }

        void toolStartColor_FormClosed(object sender, FormClosedEventArgs e)
        {
            toolStartColor = null;
        }

        void toolEndColor_FormClosed(object sender, FormClosedEventArgs e)
        {
            toolEndColor = null;
        }

        public void UpdateParticles(decimal elapsedTime)
        {
            deltaTime = elapsedTime;
            timeslice += (double)deltaTime;

            for (int i = 0; i < emittor.Max_Number1; i++)
            {
                emittor.Particles[i].SpawnTime += (double)deltaTime;

                if (emittor.Particles[i].SpawnTime <= emittor.Particles[i].SpawnDelay1)
                    continue;
                else
                    emittor.Particles[i].IsDead = false;

                emittor.Particles[i].PosX += (emittor.Particles[i].VelX + (double)(emittor.AccelX * deltaTime));
                emittor.Particles[i].PosY += (emittor.Particles[i].VelY + (double)(emittor.AccelY * deltaTime));
                

                emittor.Particles[i].Currlife += deltaTime;
                emittor.Particles[i].Colorfade_timer += (double)deltaTime;
                emittor.Particles[i].Scale_timerX += (double)deltaTime;
                emittor.Particles[i].Scale_timerY += (double)deltaTime;

                emittor.Particles[i].Rotation += emittor.Rotation;
                if (emittor.Particles[i].Rotation >= 6.28M)
                    emittor.Particles[i].Rotation = 0;

                    UpdateColor(i);
                


                UpdateScale(i);

                if (emittor.Particles[i].Colorfade_timer >= (double)emittor.Particles[i].Endlife)
                    emittor.Particles[i].Colorfade_timer = 0;
                if (emittor.Particles[i].Scale_timerX >= (double)emittor.Particles[i].Endlife)
                    emittor.Particles[i].Scale_timerX = 0;
                if (emittor.Particles[i].Scale_timerY >= (double)emittor.Particles[i].Endlife)
                    emittor.Particles[i].Scale_timerY = 0;


                if (RepeatcheckBox.Checked)
                {
                    if (emittor.Particles[i].Currlife > emittor.Particles[i].Endlife)
                    {

                        emittor.Particles[i].Currlife = 0;

                        emittor.Particles[i].PosX = emittor.Position.X;
                        emittor.Particles[i].PosY = emittor.Position.Y;
                        emittor.Particles[i].VelX = rand.Next((int)emittor.VelminX, (int)emittor.VelmaxX) * .01;
                        emittor.Particles[i].VelY = rand.Next((int)emittor.VelminY, (int)emittor.VelmaxY) * .01;
                        emittor.Particles[i].Color = emittor.Start_Color1;
                        emittor.Particles[i].ScaleX = emittor.Start_scaleX;
                        emittor.Particles[i].ScaleY = emittor.Start_scaleY;

                        emittor.Particles[i].SpawnTime = 0;
                        emittor.Particles[i].IsDead = true;
                    }
                }
                else
                {
                    if (emittor.Particles[i].Currlife > emittor.Particles[i].Endlife)
                    {
                        emittor.Particles[i].IsDead = true;
                    }
                }
            }


            FrameworkTimer.Start();

        }

        public void Render(int displayfps)
        {
            d3d.Clear(BG.R, BG.G, BG.B);
            d3d.DeviceBegin();
            d3d.SpriteBegin();

            
            d3d.Device.SetRenderState(RenderStates.AlphaBlendEnable, true);
            d3d.Device.SetRenderState(RenderStates.SourceBlend, (int)Source);
            d3d.Device.SetRenderState(RenderStates.DestinationBlend, (int)Destination);

            for (int i = 0; i < emittor.Particles.Count(); i++)
            {
                if( emittor.Particles[i].IsDead != true)
                    TM.Draw(emittor.TextureID1, (int)emittor.Particles[i].PosX - (int)((TM.GetTextureWidth(emittor.TextureID1) * 0.5M) * emittor.Particles[i].ScaleX), (int)emittor.Particles[i].PosY - (int)((TM.GetTextureHeight(emittor.TextureID1)*0.5M) * emittor.Particles[i].ScaleY),
                            (float)emittor.Particles[i].ScaleX, (float)emittor.Particles[i].ScaleY, Rectangle.Empty, (int)(TM.GetTextureWidth(emittor.TextureID1) * 0.5), (int)(TM.GetTextureHeight(emittor.TextureID1) * 0.5), (float)emittor.Particles[i].Rotation, emittor.Particles[i].Color.ToArgb());
            }

            d3d.Sprite.Flush();

            d3d.Device.SetRenderState(RenderStates.AlphaBlendEnable, false);
            d3d.DrawText(displayfps.ToString(), 10, 10, 255, 255, 255);

            d3d.SpriteEnd();
            d3d.DeviceEnd();
            d3d.Present();

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Looping = false;
        }

        private void SourceBlendComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (SourceBlendComboBox.SelectedIndex)
            {
                case 0:
                    Source = Blend.Zero;
                    break;
                case 1:
                    Source = Blend.One;
                    break;
                case 2:
                    Source = Blend.SourceColor;
                    break;
                case 3:
                    Source = Blend.InvSourceColor;
                    break;
                case 4:
                    Source = Blend.SourceAlpha;
                    break;
                case 5:
                    Source = Blend.InvSourceAlpha;
                    break;
                case 6:
                    Source = Blend.DestinationAlpha;
                    break;
                case 7:
                    Source = Blend.InvDestinationAlpha;
                    break;
                case 8:
                    Source = Blend.DestinationColor;
                    break;
                case 9:
                    Source = Blend.InvDestinationColor;
                    break;
                case 10:
                    Source = Blend.SourceAlphaSat;
                    break;
                case 11:
                    Source = Blend.BothInvSourceAlpha;
                    break;
                case 12:
                    Source = Blend.BlendFactor;
                    break;
                case 13:
                    Source = Blend.InvBlendFactor;
                    break;
            };

        }

        private void DestinationBlendComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (DestinationBlendComboBox.SelectedIndex)
            {
                case 0:
                    Destination = Blend.Zero;
                    break;
                case 1:
                    Destination = Blend.One;
                    break;
                case 2:
                    Destination = Blend.SourceColor;
                    break;
                case 3:
                    Destination = Blend.InvSourceColor;
                    break;
                case 4:
                    Destination = Blend.SourceAlpha;
                    break;
                case 5:
                    Destination = Blend.InvSourceAlpha;
                    break;
                case 6:
                    Destination = Blend.DestinationAlpha;
                    break;
                case 7:
                    Destination = Blend.InvDestinationAlpha;
                    break;
                case 8:
                    Destination = Blend.DestinationColor;
                    break;
                case 9:
                    Destination = Blend.InvDestinationColor;
                    break;
                case 10:
                    Destination = Blend.SourceAlphaSat;
                    break;
                case 11:
                    Destination = Blend.BothInvSourceAlpha;
                    break;
                case 12:
                    Destination = Blend.BlendFactor;
                    break;
                case 13:
                    Destination = Blend.InvBlendFactor;
                    break;
            };
        }

        private void UpdateColor(int i)
        {
            int tempA = 0;
            int tempR = 0;
            int tempG = 0;
            int tempB = 0;

                tempA = (emittor.Start_Color1.A -  emittor.End_Color1.A);
                tempA = (emittor.Start_Color1.A - (int)(tempA * (emittor.Particles[i].Colorfade_timer / (double)emittor.EndLife1)));

                tempR = (emittor.Start_Color1.R - emittor.End_Color1.R);
                tempR = (emittor.Start_Color1.R - (int)(tempR * (emittor.Particles[i].Colorfade_timer / (double)emittor.EndLife1)));

                tempG = (emittor.Start_Color1.G -  emittor.End_Color1.G);
                tempG = (emittor.Start_Color1.G - (int)(tempG * (emittor.Particles[i].Colorfade_timer / (double)emittor.EndLife1)));                       
                                                                                               
                tempB = (emittor.Start_Color1.B -  emittor.End_Color1.B);
                tempB = (emittor.Start_Color1.B - (int)(tempB * (emittor.Particles[i].Colorfade_timer / (double)emittor.EndLife1)));

                if (tempA > 255)
                    tempA = 255;
                if (tempR > 255)
                    tempR = 255;
                if (tempG > 255)
                    tempG = 255;
                if (tempB > 255)
                    tempB = 255;

                if (tempA < 0)
                    tempA = 0;
                if (tempR < 0)
                    tempR = 0;
                if (tempG < 0)
                    tempG = 0;
                if (tempB < 0)
                    tempB = 0;
            

            Color tempcolor = Color.FromArgb( (int)tempA, (int)tempR, (int)tempG, (int)tempB);

            emittor.Particles[i].Color = tempcolor;

        }

        private void UpdateScale(int i)
        {
            decimal scaleX = 0;

            scaleX = (emittor.Start_scaleX - emittor.End_scaleX);
            scaleX = (emittor.Start_scaleX - (scaleX * ((decimal)emittor.Particles[i].Scale_timerX / emittor.EndLife1)));

            emittor.Particles[i].ScaleX = scaleX;

            decimal scaleY = 0;

            scaleY = (emittor.Start_scaleY - emittor.End_scaleY);
            scaleY = (emittor.Start_scaleY - (scaleY * ((decimal)emittor.Particles[i].Scale_timerY / emittor.EndLife1)));

            emittor.Particles[i].ScaleY = scaleY;
        }

        ///<summary> -- All values changed

        private void numericParticleMinLife_ValueChanged(object sender, EventArgs e)
        {
            emittor.MinLife1 = numericParticleMinLife.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].Endlife = rand.Next((int)emittor.MinLife1, (int)emittor.EndLife1);
            }
        }

        private void numericParticleEndLife_ValueChanged(object sender, EventArgs e)
        {
            emittor.EndLife1 = numericParticleEndLife.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].Endlife = rand.Next((int)emittor.MinLife1, (int)emittor.EndLife1);
            }
        }

        private void numericParticleCount_ValueChanged(object sender, EventArgs e)
        {
            emittor.Max_Number1 = numericParticleCount.Value;

            emittor.Particles.Clear();

            for (int i = 0; i < emittor.Max_Number1; i++)
            {

                Particle temp = new Particle();

                temp.PosX = emittor.Position.X;
                temp.PosY = emittor.Position.Y;
                temp.Color = emittor.Start_Color1;

                temp.Endlife = rand.Next((int)emittor.MinLife1, (int)emittor.EndLife1);
                temp.SpawnDelay1 = rand.NextDouble();

                double velX;
                double velY;

                velX = rand.Next((int)emittor.VelminX, (int)emittor.VelmaxX) * .01;
                velY = rand.Next((int)emittor.VelminY, (int)emittor.VelmaxY) * .01;


                temp.VelX = velX;
                temp.VelY = velY;

                temp.Rotation = emittor.Rotation;

                temp.Colorfade_timer = 0;
                temp.Scale_timerX = 0;
                temp.Scale_timerY = 0;
                temp.SpawnTime = 0;

                emittor.Particles.Add(temp);
            }
        }

        private void numericVelX_ValueChanged(object sender, EventArgs e)
        {
            emittor.VelminX = numericVelMinX.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].VelX = rand.Next((int)emittor.VelminX, (int)emittor.VelmaxX) * .01;
            }
        }

        private void numericVelY_ValueChanged(object sender, EventArgs e)
        {
            emittor.VelminY = numericVelMinY.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].VelY = rand.Next((int)emittor.VelminY, (int)emittor.VelmaxY) * .01;
            }

        }

        private void numericVelMaxY_ValueChanged(object sender, EventArgs e)
        {
            emittor.VelmaxY = numericVelMaxY.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].VelY = rand.Next((int)emittor.VelminY, (int)emittor.VelmaxY) * .01;
            }
        }

        private void numericVelMaxX_ValueChanged(object sender, EventArgs e)
        {
            emittor.VelmaxX = numericVelMaxX.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].VelX = rand.Next((int)emittor.VelminX, (int)emittor.VelmaxX) * .01;
            }
        }

        private void numericAccelX_ValueChanged(object sender, EventArgs e)
        {
            emittor.AccelX = numericAccelX.Value;
        }

        private void numericAccelY_ValueChanged(object sender, EventArgs e)
        {
            emittor.AccelY = numericAccelY.Value;
        }

        private void numericScaleBegX_ValueChanged(object sender, EventArgs e)
        {
            emittor.Start_scaleX = numericScaleBegX.Value;
        }

        private void numericScaleEndX_ValueChanged(object sender, EventArgs e)
        {
            emittor.End_scaleX = numericScaleEndX.Value;
        }

        private void numericScaleBegY_ValueChanged(object sender, EventArgs e)
        {
            emittor.Start_scaleY = numericScaleBegY.Value;
        }

        private void numericScaleEndY_ValueChanged(object sender, EventArgs e)
        {
            emittor.End_scaleY = numericScaleEndY.Value;
        }

        private void numericRotation_ValueChanged(object sender, EventArgs e)
        {
            emittor.Rotation = numericRotation.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].Rotation = emittor.Rotation;
            }

        }

        ///</summary>
        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.Filter = "PNG(*.png)|*.png|BMP(*.bmp)|*.bmp";
            dlg.FilterIndex = 1;

            // Display the dialog to the user
            if (DialogResult.OK == dlg.ShowDialog())
            {
                string temp = dlg.FileName;

                
                TM.ReleaseTexture(emittor.TextureID1);
                emittor.TextureID1 = TM.LoadTexture(temp, Color.FromArgb(255, 0, 0, 0).ToArgb());

                emittor.Imagename = dlg.SafeFileName;
            }
        }

        private void randomizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (RandomWindow == null)
            {
                RandomWindow = new RandomizeWindow();


                RandomWindow.FormClosed += new FormClosedEventHandler(RandomWindow_FormClosed);
                RandomWindow.Randomize += new EventHandler(RandomWindow_Randomize);

                RandomWindow.Show();
            }
        }

        void RandomWindow_Randomize(object sender, EventArgs e)
        {
            if (RandomWindow.NumParticles)
                emittor.Max_Number1 = rand.Next(1, 5000);
            if (RandomWindow.MinLife)
                emittor.MinLife1 = rand.Next(1, 5);
            if (RandomWindow.MaxLife)
                emittor.EndLife1 = rand.Next((int)emittor.MinLife1, 20);
            if (RandomWindow.StartXScale)
                emittor.Start_scaleX = (decimal)(rand.NextDouble() + (double)rand.Next(20));
            if( RandomWindow.EndXScale)
                emittor.End_scaleX = (decimal)(rand.NextDouble() + (double)rand.Next(20));
            if(RandomWindow.StartYScale)
                emittor.Start_scaleY = (decimal)(rand.NextDouble() + (double)rand.Next(20));
            if( RandomWindow.EndYScale)
                emittor.End_scaleY = (decimal)(rand.NextDouble() + (double)rand.Next(20));
            if (RandomWindow.MinXVel)
                emittor.VelminX = -rand.Next(1000);
            if (RandomWindow.MaxXVel)
                emittor.VelmaxX = rand.Next(1000);
            if (RandomWindow.MinYVel)
                emittor.VelminY = -rand.Next(1000);
            if (RandomWindow.MaxYVel)
                emittor.VelmaxY = rand.Next(1000);
            if (RandomWindow.AccelX)
                emittor.AccelX = rand.Next(-300,300);
            if (RandomWindow.AccelY)
                emittor.AccelY = rand.Next(-300, 300);
            if (RandomWindow.Rotation)
                emittor.Rotation = (decimal)(rand.NextDouble() + (double)rand.Next(6));
            if (RandomWindow.SourceBlend)
                Source = (Blend)rand.Next(14);
            if (RandomWindow.DestinationBlend)
                Destination = (Blend)rand.Next(14);
            if (RandomWindow.StartColor)
                emittor.Start_Color1 = Color.FromArgb(rand.Next(255), rand.Next(255), rand.Next(255), rand.Next(255));
            if(RandomWindow.EndColor)
                emittor.End_Color1 = Color.FromArgb(rand.Next(255), rand.Next(255), rand.Next(255), rand.Next(255));

            numericParticleCount.Value = emittor.Max_Number1;
            numericParticleEndLife.Value = emittor.EndLife1;
            numericParticleMinLife.Value = emittor.MinLife1;
            numericScaleBegX.Value = emittor.Start_scaleX;
            numericScaleEndX.Value = emittor.End_scaleX;
            numericScaleBegY.Value = emittor.Start_scaleY;
            numericScaleEndY.Value = emittor.End_scaleY;
            numericVelMinX.Value = emittor.VelminX;
            numericVelMinY.Value = emittor.VelminY;
            numericVelMaxX.Value = emittor.VelmaxX;
            numericVelMaxY.Value = emittor.VelmaxY;

            numericAccelX.Value = emittor.AccelX;
            numericAccelY.Value = emittor.AccelY;

            SourceBlendComboBox.SelectedIndex = (int)Source;
            DestinationBlendComboBox.SelectedIndex = (int)Destination;

            numericRotation.Value = emittor.Rotation;
        }

        void RandomWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            RandomWindow = null;
        }

        private void backGroundColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                BG = dlg.Color;
            }
        }

        private void ResetButton_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].Currlife = 0;

                emittor.Particles[i].PosX = emittor.Position.X;
                emittor.Particles[i].PosY = emittor.Position.Y;
                emittor.Particles[i].VelX = rand.Next((int)emittor.VelminX, (int)emittor.VelmaxX) * .01;
                emittor.Particles[i].VelY = rand.Next((int)emittor.VelminY, (int)emittor.VelmaxY) * .01;
                emittor.Particles[i].Color = emittor.Start_Color1;
                emittor.Particles[i].ScaleX = emittor.Start_scaleX;
                emittor.Particles[i].ScaleY = emittor.Start_scaleY;
                emittor.Particles[i].Rotation = 0;

                emittor.Particles[i].Colorfade_timer = 0;
                emittor.Particles[i].Scale_timerX = 0;
                emittor.Particles[i].Scale_timerY = 0;

                emittor.Particles[i].SpawnTime = 0;
                emittor.Particles[i].IsDead = true;
            }
        }

        private void RepeatcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if( RepeatcheckBox.Checked)
                ResetButton_Click(sender, e);
        }

        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            emittor.Position = e.Location;
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                emittor.Position = e.Location;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement xRoot = new XElement("Emittor");

                    XElement xNumPart = new XElement("NumberofParticles", emittor.Max_Number1);
                    xRoot.Add(xNumPart);

                    XElement xMinLife = new XElement("MinLifeSpan", emittor.MinLife1);
                    xRoot.Add(xMinLife);

                    XElement xMaxLife = new XElement("MaxLifeSpan", emittor.EndLife1);
                    xRoot.Add(xMaxLife);

                    XElement xStartColor = new XElement("StartColor");
                    xRoot.Add(xStartColor);
                    XAttribute xStartA = new XAttribute("A", emittor.Start_Color1.A);
                    xStartColor.Add(xStartA);
                    XAttribute xStartR = new XAttribute("R", emittor.Start_Color1.R);
                    xStartColor.Add(xStartR);
                    XAttribute xStartG = new XAttribute("G", emittor.Start_Color1.G);
                    xStartColor.Add(xStartG);
                    XAttribute xStartB = new XAttribute("B", emittor.Start_Color1.B);
                    xStartColor.Add(xStartB);

                    XElement xEndColor = new XElement("EndColor");
                    xRoot.Add(xEndColor);
                    XAttribute xEndA = new XAttribute("A", emittor.End_Color1.A);
                    xEndColor.Add(xEndA);
                    XAttribute xEndR = new XAttribute("R", emittor.End_Color1.R);
                    xEndColor.Add(xEndR);
                    XAttribute xEndG = new XAttribute("G", emittor.End_Color1.G);
                    xEndColor.Add(xEndG);
                    XAttribute xEndB = new XAttribute("B", emittor.End_Color1.B);
                    xEndColor.Add(xEndB);

                    XElement xStartXScale = new XElement("XStartScale", emittor.Start_scaleX);
                    xRoot.Add(xStartXScale);

                    XElement xEndXScale = new XElement("XEndScale", emittor.End_scaleX);
                    xRoot.Add(xEndXScale);

                    XElement xStartYScale = new XElement("YStartScale", emittor.Start_scaleY);
                    xRoot.Add(xStartYScale);

                    XElement xEndYScale = new XElement("YEndScale", emittor.End_scaleY);
                    xRoot.Add(xEndYScale);

                    XElement xVelXMin = new XElement("MinXVelocity", emittor.VelminX);
                    xRoot.Add(xVelXMin);

                    XElement xVelXMax = new XElement("MaxXVelocity", emittor.VelmaxX);
                    xRoot.Add(xVelXMax);

                    XElement xVelYMin = new XElement("MinYVelocity", emittor.VelminY);
                    xRoot.Add(xVelYMin);

                    XElement xVelYMax = new XElement("MaxYVelocity", emittor.VelmaxY);
                    xRoot.Add(xVelYMax);

                    XElement xAccelX = new XElement("XAcceleration", emittor.AccelX);
                    xRoot.Add(xAccelX);

                    XElement xAccelY = new XElement("YAcceleration", emittor.AccelY);
                    xRoot.Add(xAccelY);

                    XElement xRotation = new XElement("Rotation", emittor.Rotation);
                    xRoot.Add(xRotation);

                    XElement xSource = new XElement("SourceBlend", (int)Source);
                    xRoot.Add(xSource);

                    XElement xDestination = new XElement("DestinationBlend", (int)Destination);
                    xRoot.Add(xDestination);

                    XElement xImageName = new XElement("ImageName", emittor.Imagename);
                    xRoot.Add(xImageName);

                    XElement xRepeat = new XElement("Continuous", RepeatcheckBox.Checked);
                    xRoot.Add(xRepeat);


                xRoot.Save(dlg.FileName);
            }
        }

        private void loadParticleEffectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.Filter = "XML(*.xml)|*.xml|SBS(*.sbs)|*.sbs";
            dlg.FilterIndex = 1;

            if (DialogResult.OK == dlg.ShowDialog())
            {

                emittor = null;
                emittor = new ParticleEmitter();


                XElement xRoot = XElement.Load(dlg.FileName);

                XElement xNumPart = xRoot.Element("NumberofParticles");
                if (xNumPart != null)
                    emittor.Max_Number1 = decimal.Parse(xNumPart.Value);

                XElement xMinLife = xRoot.Element("MinLifeSpan");
                if (xMinLife != null)
                    emittor.MinLife1 = decimal.Parse(xMinLife.Value);

                XElement xMaxLife = xRoot.Element("MaxLifeSpan");
                if (xMaxLife != null)
                    emittor.EndLife1 = decimal.Parse(xMaxLife.Value);

                XElement xStartColor = xRoot.Element("StartColor");
                if (xStartColor != null)
                {
                    XAttribute xStartA = xStartColor.Attribute("A");
                    XAttribute xStartR = xStartColor.Attribute("R");
                    XAttribute xStartG = xStartColor.Attribute("G");
                    XAttribute xStartB = xStartColor.Attribute("B");

                    emittor.Start_Color1 = Color.FromArgb(Int32.Parse(xStartA.Value),
                                                          Int32.Parse(xStartR.Value),
                                                          Int32.Parse(xStartG.Value),
                                                          Int32.Parse(xStartB.Value));
                }

                XElement xEndColor = xRoot.Element("EndColor");
                if (xEndColor != null)
                {
                    XAttribute xEndA = xEndColor.Attribute("A");
                    XAttribute xEndR = xEndColor.Attribute("R");
                    XAttribute xEndG = xEndColor.Attribute("G");
                    XAttribute xEndB = xEndColor.Attribute("B");

                    emittor.End_Color1 = Color.FromArgb(Int32.Parse(xEndA.Value),
                                                          Int32.Parse(xEndR.Value),
                                                          Int32.Parse(xEndG.Value),
                                                          Int32.Parse(xEndB.Value));
                }

                XElement xStartXScale = xRoot.Element("XStartScale");
                if (xStartXScale != null)
                    emittor.Start_scaleX = decimal.Parse(xStartXScale.Value);

                XElement xEndXScale = xRoot.Element("XEndScale");
                if (xEndXScale != null)
                    emittor.End_scaleX = decimal.Parse(xEndXScale.Value);
                
                XElement xStartYScale = xRoot.Element("YStartScale");
                if (xStartYScale != null)
                    emittor.Start_scaleY = decimal.Parse(xStartYScale.Value);

                XElement xEndYScale = xRoot.Element("YEndScale");
                if (xEndYScale != null)
                    emittor.End_scaleY = decimal.Parse(xEndYScale.Value);

                XElement xVelXMin = xRoot.Element("MinXVelocity");
                if (xVelXMin != null)
                    emittor.VelminX = decimal.Parse(xVelXMin.Value);

                XElement xVelXMax = xRoot.Element("MaxXVelocity");
                if (xVelXMax != null)
                    emittor.VelmaxX = decimal.Parse(xVelXMax.Value);

                XElement xVelYMin = xRoot.Element("MinYVelocity");
                if (xVelYMin != null)
                    emittor.VelminY = decimal.Parse(xVelYMin.Value);

                XElement xVelYMax = xRoot.Element("MaxYVelocity");
                if (xVelYMax != null)
                    emittor.VelmaxY = decimal.Parse(xVelYMax.Value);

                XElement xAccelX = xRoot.Element("XAcceleration");
                if (xAccelX != null)
                    emittor.AccelX = decimal.Parse(xAccelX.Value);

                XElement xAccelY = xRoot.Element("YAcceleration");
                if (xAccelY != null)
                    emittor.AccelY = decimal.Parse(xAccelY.Value);

                XElement xRotation = xRoot.Element("Rotation");
                if (xRotation != null)
                    emittor.Rotation = decimal.Parse(xRotation.Value);

                XElement xSource = xRoot.Element("SourceBlend");
                if (xSource != null)
                    Source = (Blend)Int32.Parse(xSource.Value);

                XElement xDestination = xRoot.Element("DestinationBlend");
                if (xDestination != null)
                    Destination = (Blend)Int32.Parse(xDestination.Value);

                XElement xImageName = xRoot.Element("ImageName");
                if (xImageName != null)
                    emittor.Imagename = xImageName.Value;

                string awesome = dlg.FileName;
                directory = awesome.Split('\\');

                directory[directory.Length - 1] = null;
                awesome = null;

                for (int i = 0; i < directory.Length; i++)
                    if (directory[i] != null)
                        awesome += directory[i] + "\\";

                awesome += emittor.Imagename;

                TM.ReleaseTexture(emittor.TextureID1);
                emittor.TextureID1 = TM.LoadTexture(awesome, Color.FromArgb(255,0,0,0).ToArgb());

                XElement xRepeat = xRoot.Element("Continuous");
                if (xRepeat != null)
                    RepeatcheckBox.Checked = bool.Parse(xRepeat.Value);


                for (int i = 0; i < emittor.Max_Number1; i++)
                {

                    Particle temp = new Particle();

                    temp.PosX = emittor.Position.X;
                    temp.PosY = emittor.Position.Y;
                    temp.Color = emittor.Start_Color1;

                    temp.Endlife = rand.Next((int)emittor.MinLife1, (int)emittor.EndLife1);
                    temp.SpawnDelay1 = rand.NextDouble();

                    double velX;
                    double velY;

                    velX = rand.Next((int)emittor.VelminX, (int)emittor.VelmaxX) * .01;
                    velY = rand.Next((int)emittor.VelminY, (int)emittor.VelmaxY) * .01;


                    temp.VelX = velX;
                    temp.VelY = velY;

                    temp.Rotation = emittor.Rotation;

                    temp.Colorfade_timer = 0;
                    temp.Scale_timerX = 0;
                    temp.Scale_timerY = 0;
                    temp.SpawnTime = 0;

                    emittor.Particles.Add(temp);
                }

            }
        }



    }
}
