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
        List<ParticleEmitter> m_ListOfEmittors = new List<ParticleEmitter>();
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
            textBoxEmittorName.Text = emittor.Name;

            emittor.IsContinuous = true;
            emittor.IsBursting = false;

            m_ListOfEmittors.Add(emittor);
            listBoxEmittors.Items.Add(emittor);
            listBoxEmittors.SelectedIndex = 0;
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

            for (int a = 0; a < m_ListOfEmittors.Count; a++)
            {
                ParticleEmitter temp_render_emittor = m_ListOfEmittors[a];

                for (int i = 0; i < temp_render_emittor.Max_Number1; i++)
                {
                    if (temp_render_emittor.IsBursting == false)
                    {
                        temp_render_emittor.Particles[i].SpawnTime += (double)deltaTime;

                        if (temp_render_emittor.Particles[i].SpawnTime <= temp_render_emittor.Particles[i].SpawnDelay1)
                            continue;
                        else
                            temp_render_emittor.Particles[i].IsDead = false;

                    }
                    else
                        temp_render_emittor.Particles[i].IsDead = false;

                    temp_render_emittor.Particles[i].PosX += (temp_render_emittor.Particles[i].VelX + (double)(temp_render_emittor.AccelX * deltaTime));
                    temp_render_emittor.Particles[i].PosY += (temp_render_emittor.Particles[i].VelY + (double)(temp_render_emittor.AccelY * deltaTime));


                    temp_render_emittor.Particles[i].Currlife += deltaTime;
                    temp_render_emittor.Particles[i].Colorfade_timer += (double)deltaTime;
                    temp_render_emittor.Particles[i].Scale_timerX += (double)deltaTime;
                    temp_render_emittor.Particles[i].Scale_timerY += (double)deltaTime;

                    temp_render_emittor.Particles[i].Rotation += temp_render_emittor.Rotation * deltaTime;
                    if (temp_render_emittor.Particles[i].Rotation >= 6.28M)
                        temp_render_emittor.Particles[i].Rotation = 0;

                    UpdateColor(i, temp_render_emittor);



                    UpdateScale(i, temp_render_emittor);

                    if (temp_render_emittor.Particles[i].Colorfade_timer >= (double)temp_render_emittor.Particles[i].Endlife)
                        temp_render_emittor.Particles[i].Colorfade_timer = 0;
                    if (temp_render_emittor.Particles[i].Scale_timerX >= (double)temp_render_emittor.Particles[i].Endlife)
                        temp_render_emittor.Particles[i].Scale_timerX = 0;
                    if (temp_render_emittor.Particles[i].Scale_timerY >= (double)temp_render_emittor.Particles[i].Endlife)
                        temp_render_emittor.Particles[i].Scale_timerY = 0;


                    if (temp_render_emittor.IsContinuous)
                    {
                        if (temp_render_emittor.Particles[i].Currlife > temp_render_emittor.Particles[i].Endlife)
                        {

                            temp_render_emittor.Particles[i].Currlife = 0;

                            temp_render_emittor.Particles[i].PosX = temp_render_emittor.Position.X;
                            temp_render_emittor.Particles[i].PosY = temp_render_emittor.Position.Y;
                            temp_render_emittor.Particles[i].VelX = rand.Next((int)temp_render_emittor.VelminX, (int)temp_render_emittor.VelmaxX) * .01;
                            temp_render_emittor.Particles[i].VelY = rand.Next((int)temp_render_emittor.VelminY, (int)temp_render_emittor.VelmaxY) * .01;
                            temp_render_emittor.Particles[i].Color = temp_render_emittor.Start_Color1;
                            temp_render_emittor.Particles[i].ScaleX = temp_render_emittor.Start_scaleX;
                            temp_render_emittor.Particles[i].ScaleY = temp_render_emittor.Start_scaleY;

                            temp_render_emittor.Particles[i].SpawnTime = 0;
                            temp_render_emittor.Particles[i].IsDead = true;
                        }
                    }
                    else
                    {
                        if (temp_render_emittor.Particles[i].Currlife > temp_render_emittor.Particles[i].Endlife)
                        {
                            temp_render_emittor.Particles[i].IsDead = true;
                        }
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
            

            for (int a = 0; a < m_ListOfEmittors.Count; a++)
            {
                ParticleEmitter temp_render_emittor = m_ListOfEmittors[a];

                d3d.Device.SetRenderState(RenderStates.SourceBlend, (int)temp_render_emittor.Source);
                d3d.Device.SetRenderState(RenderStates.DestinationBlend, (int)temp_render_emittor.Destination);
                

                for (int i = 0; i < temp_render_emittor.Particles.Count(); i++)
                {
                    if (temp_render_emittor.Particles[i].IsDead != true)
                        TM.Draw(temp_render_emittor.TextureID1, (int)temp_render_emittor.Particles[i].PosX - (int)((TM.GetTextureWidth(temp_render_emittor.TextureID1) * 0.5M) * temp_render_emittor.Particles[i].ScaleX), (int)temp_render_emittor.Particles[i].PosY - (int)((TM.GetTextureHeight(temp_render_emittor.TextureID1) * 0.5M) * temp_render_emittor.Particles[i].ScaleY),
                                (float)temp_render_emittor.Particles[i].ScaleX, (float)temp_render_emittor.Particles[i].ScaleY, Rectangle.Empty, (int)(TM.GetTextureWidth(temp_render_emittor.TextureID1)), (int)(TM.GetTextureHeight(temp_render_emittor.TextureID1)), (float)temp_render_emittor.Particles[i].Rotation, temp_render_emittor.Particles[i].Color.ToArgb());
                }
                d3d.Sprite.Flush();
            }

            

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

            emittor.Source = (int)Source;

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

            emittor.Destination = (int)Destination;
        }

        private void UpdateColor(int i, ParticleEmitter emittor_temp)
        {
            int tempA = 0;
            int tempR = 0;
            int tempG = 0;
            int tempB = 0;

                tempA = (emittor_temp.Start_Color1.A - emittor_temp.End_Color1.A);
                tempA = (emittor_temp.Start_Color1.A - (int)(tempA * (emittor_temp.Particles[i].Colorfade_timer / (double)emittor_temp.EndLife1)));

                tempR = (emittor_temp.Start_Color1.R - emittor_temp.End_Color1.R);
                tempR = (emittor_temp.Start_Color1.R - (int)(tempR * (emittor_temp.Particles[i].Colorfade_timer / (double)emittor_temp.EndLife1)));

                tempG = (emittor_temp.Start_Color1.G - emittor_temp.End_Color1.G);
                tempG = (emittor_temp.Start_Color1.G - (int)(tempG * (emittor_temp.Particles[i].Colorfade_timer / (double)emittor_temp.EndLife1)));

                tempB = (emittor_temp.Start_Color1.B - emittor_temp.End_Color1.B);
                tempB = (emittor_temp.Start_Color1.B - (int)(tempB * (emittor_temp.Particles[i].Colorfade_timer / (double)emittor_temp.EndLife1)));

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

            emittor_temp.Particles[i].Color = tempcolor;

        }

        private void UpdateScale(int i, ParticleEmitter emittor_temp)
        {
            decimal scaleX = 0;

            scaleX = (emittor_temp.Start_scaleX - emittor_temp.End_scaleX);
            scaleX = (emittor_temp.Start_scaleX - (scaleX * ((decimal)emittor_temp.Particles[i].Scale_timerX / emittor_temp.EndLife1)));

            emittor_temp.Particles[i].ScaleX = scaleX;

            decimal scaleY = 0;

            scaleY = (emittor_temp.Start_scaleY - emittor_temp.End_scaleY);
            scaleY = (emittor_temp.Start_scaleY - (scaleY * ((decimal)emittor_temp.Particles[i].Scale_timerY / emittor_temp.EndLife1)));

            emittor_temp.Particles[i].ScaleY = scaleY;
        }

        ///<summary> -- All values changed

        private void numericParticleMinLife_ValueChanged(object sender, EventArgs e)
        {
            if (numericParticleMinLife.Value > emittor.EndLife1)
            {
                emittor.MinLife1 = emittor.EndLife1;
                numericParticleMinLife.Value = emittor.MinLife1;
            }
            else
                emittor.MinLife1 = numericParticleMinLife.Value;

            for (int i = 0; i < emittor.Particles.Count; i++)
            {
                emittor.Particles[i].Endlife = rand.Next((int)emittor.MinLife1, (int)emittor.EndLife1);
            }

        }

        private void numericParticleEndLife_ValueChanged(object sender, EventArgs e)
        {
            if (numericParticleEndLife.Value < emittor.MinLife1)
            {
                emittor.EndLife1 = emittor.MinLife1;
                numericParticleEndLife.Value = emittor.EndLife1;
            }
            else
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
                emittor.EndLife1 = rand.Next((int)emittor.MinLife1, 10);
            if (RandomWindow.StartXScale)
                emittor.Start_scaleX = (decimal)(rand.NextDouble() + (double)rand.Next(10));
            if( RandomWindow.EndXScale)
                emittor.End_scaleX = (decimal)(rand.NextDouble() + (double)rand.Next(10));
            if(RandomWindow.StartYScale)
                emittor.Start_scaleY = (decimal)(rand.NextDouble() + (double)rand.Next(10));
            if( RandomWindow.EndYScale)
                emittor.End_scaleY = (decimal)(rand.NextDouble() + (double)rand.Next(10));
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
                Source = (Blend)rand.Next(1, 15);
            if (RandomWindow.DestinationBlend)
                Destination = (Blend)rand.Next(1, 15);
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

            SourceBlendComboBox.SelectedIndex = (int)Source - 1;
            DestinationBlendComboBox.SelectedIndex = (int)Destination - 1;

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
            emittor.IsContinuous = RepeatcheckBox.Checked;
            if( emittor.IsContinuous)
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
                XElement xRoot = new XElement("Emittors");
                XElement xEmittor = new XElement("Emittor");
                xRoot.Add(xEmittor);

                    XElement xName = new XElement("Name", emittor.Name);
                    xEmittor.Add(xName);

                    XElement xNumPart = new XElement("NumberofParticles", emittor.Max_Number1);
                    xEmittor.Add(xNumPart);

                    XElement xMinLife = new XElement("MinLifeSpan", emittor.MinLife1);
                    xEmittor.Add(xMinLife);

                    XElement xMaxLife = new XElement("MaxLifeSpan", emittor.EndLife1);
                    xEmittor.Add(xMaxLife);

                    XElement xStartColor = new XElement("StartColor");
                    xEmittor.Add(xStartColor);
                    XAttribute xStartA = new XAttribute("A", emittor.Start_Color1.A);
                    xStartColor.Add(xStartA);
                    XAttribute xStartR = new XAttribute("R", emittor.Start_Color1.R);
                    xStartColor.Add(xStartR);
                    XAttribute xStartG = new XAttribute("G", emittor.Start_Color1.G);
                    xStartColor.Add(xStartG);
                    XAttribute xStartB = new XAttribute("B", emittor.Start_Color1.B);
                    xStartColor.Add(xStartB);

                    XElement xEndColor = new XElement("EndColor");
                    xEmittor.Add(xEndColor);
                    XAttribute xEndA = new XAttribute("A", emittor.End_Color1.A);
                    xEndColor.Add(xEndA);
                    XAttribute xEndR = new XAttribute("R", emittor.End_Color1.R);
                    xEndColor.Add(xEndR);
                    XAttribute xEndG = new XAttribute("G", emittor.End_Color1.G);
                    xEndColor.Add(xEndG);
                    XAttribute xEndB = new XAttribute("B", emittor.End_Color1.B);
                    xEndColor.Add(xEndB);

                    XElement xStartXScale = new XElement("XStartScale", emittor.Start_scaleX);
                    xEmittor.Add(xStartXScale);

                    XElement xEndXScale = new XElement("XEndScale", emittor.End_scaleX);
                    xEmittor.Add(xEndXScale);

                    XElement xStartYScale = new XElement("YStartScale", emittor.Start_scaleY);
                    xEmittor.Add(xStartYScale);

                    XElement xEndYScale = new XElement("YEndScale", emittor.End_scaleY);
                    xEmittor.Add(xEndYScale);

                    XElement xVelXMin = new XElement("MinXVelocity", emittor.VelminX);
                    xEmittor.Add(xVelXMin);

                    XElement xVelXMax = new XElement("MaxXVelocity", emittor.VelmaxX);
                    xEmittor.Add(xVelXMax);

                    XElement xVelYMin = new XElement("MinYVelocity", emittor.VelminY);
                    xEmittor.Add(xVelYMin);

                    XElement xVelYMax = new XElement("MaxYVelocity", emittor.VelmaxY);
                    xEmittor.Add(xVelYMax);

                    XElement xAccelX = new XElement("XAcceleration", emittor.AccelX);
                    xEmittor.Add(xAccelX);

                    XElement xAccelY = new XElement("YAcceleration", emittor.AccelY);
                    xEmittor.Add(xAccelY);

                    XElement xRotation = new XElement("Rotation", emittor.Rotation);
                    xEmittor.Add(xRotation);

                    XElement xSource = new XElement("SourceBlend", emittor.Source);
                    xEmittor.Add(xSource);

                    XElement xDestination = new XElement("DestinationBlend", emittor.Destination);
                    xEmittor.Add(xDestination);

                    XElement xImageName = new XElement("ImageName", emittor.Imagename);
                    xEmittor.Add(xImageName);

                    XElement xRepeat = new XElement("Continuous", emittor.IsContinuous);
                    xEmittor.Add(xRepeat);

                    XElement xBurst = new XElement("Bursting", emittor.IsBursting);
                    xEmittor.Add(xBurst);


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
                listBoxEmittors.Items.Clear();
                m_ListOfEmittors.Clear();

                emittor = null;
   
                XElement xRoot = XElement.Load(dlg.FileName);

                IEnumerable<XElement> emittor_list = xRoot.Elements();
                foreach( XElement xEmittor in emittor_list)
                {
                    ParticleEmitter temp_emittor = new ParticleEmitter();

                    XElement xName = xEmittor.Element("Name");
                    if (xName != null)
                        temp_emittor.Name = xName.Value;

                    XElement xNumPart = xEmittor.Element("NumberofParticles");
                    if (xNumPart != null)
                        temp_emittor.Max_Number1 = decimal.Parse(xNumPart.Value);

                    XElement xMinLife = xEmittor.Element("MinLifeSpan");
                    if (xMinLife != null)
                        temp_emittor.MinLife1 = decimal.Parse(xMinLife.Value);

                    XElement xMaxLife = xEmittor.Element("MaxLifeSpan");
                    if (xMaxLife != null)
                        temp_emittor.EndLife1 = decimal.Parse(xMaxLife.Value);

                    XElement xStartColor = xEmittor.Element("StartColor");
                    if (xStartColor != null)
                    {
                        XAttribute xStartA = xStartColor.Attribute("A");
                        XAttribute xStartR = xStartColor.Attribute("R");
                        XAttribute xStartG = xStartColor.Attribute("G");
                        XAttribute xStartB = xStartColor.Attribute("B");

                        temp_emittor.Start_Color1 = Color.FromArgb(Int32.Parse(xStartA.Value),
                                                              Int32.Parse(xStartR.Value),
                                                              Int32.Parse(xStartG.Value),
                                                              Int32.Parse(xStartB.Value));
                    }

                    XElement xEndColor = xEmittor.Element("EndColor");
                    if (xEndColor != null)
                    {
                        XAttribute xEndA = xEndColor.Attribute("A");
                        XAttribute xEndR = xEndColor.Attribute("R");
                        XAttribute xEndG = xEndColor.Attribute("G");
                        XAttribute xEndB = xEndColor.Attribute("B");

                        temp_emittor.End_Color1 = Color.FromArgb(Int32.Parse(xEndA.Value),
                                                              Int32.Parse(xEndR.Value),
                                                              Int32.Parse(xEndG.Value),
                                                              Int32.Parse(xEndB.Value));
                    }

                    XElement xStartXScale = xEmittor.Element("XStartScale");
                    if (xStartXScale != null)
                        temp_emittor.Start_scaleX = decimal.Parse(xStartXScale.Value);

                    XElement xEndXScale = xEmittor.Element("XEndScale");
                    if (xEndXScale != null)
                        temp_emittor.End_scaleX = decimal.Parse(xEndXScale.Value);

                    XElement xStartYScale = xEmittor.Element("YStartScale");
                    if (xStartYScale != null)
                        temp_emittor.Start_scaleY = decimal.Parse(xStartYScale.Value);

                    XElement xEndYScale = xEmittor.Element("YEndScale");
                    if (xEndYScale != null)
                        temp_emittor.End_scaleY = decimal.Parse(xEndYScale.Value);

                    XElement xVelXMin = xEmittor.Element("MinXVelocity");
                    if (xVelXMin != null)
                        temp_emittor.VelminX = decimal.Parse(xVelXMin.Value);

                    XElement xVelXMax = xEmittor.Element("MaxXVelocity");
                    if (xVelXMax != null)
                        temp_emittor.VelmaxX = decimal.Parse(xVelXMax.Value);

                    XElement xVelYMin = xEmittor.Element("MinYVelocity");
                    if (xVelYMin != null)
                        temp_emittor.VelminY = decimal.Parse(xVelYMin.Value);

                    XElement xVelYMax = xEmittor.Element("MaxYVelocity");
                    if (xVelYMax != null)
                        temp_emittor.VelmaxY = decimal.Parse(xVelYMax.Value);

                    XElement xAccelX = xEmittor.Element("XAcceleration");
                    if (xAccelX != null)
                        temp_emittor.AccelX = decimal.Parse(xAccelX.Value);

                    XElement xAccelY = xEmittor.Element("YAcceleration");
                    if (xAccelY != null)
                        temp_emittor.AccelY = decimal.Parse(xAccelY.Value);

                    XElement xRotation = xEmittor.Element("Rotation");
                    if (xRotation != null)
                        temp_emittor.Rotation = decimal.Parse(xRotation.Value);

                    XElement xSource = xEmittor.Element("SourceBlend");
                    if (xSource != null)
                        temp_emittor.Source = Int32.Parse(xSource.Value);

                    XElement xDestination = xEmittor.Element("DestinationBlend");
                    if (xDestination != null)
                        temp_emittor.Destination = Int32.Parse(xDestination.Value);

                    XElement xImageName = xEmittor.Element("ImageName");
                    if (xImageName != null)
                        temp_emittor.Imagename = xImageName.Value;

                    string awesome = "../../resources/particles/";

                    awesome += temp_emittor.Imagename;
                    temp_emittor.TextureID1 = TM.LoadTexture(awesome, Color.FromArgb(255, 0, 0, 0).ToArgb());

                    if (temp_emittor.TextureID1 == -1)
                    {
                        OpenFileDialog dlg2 = new OpenFileDialog();

                        dlg2.Filter = "PNG(*.png)|*.png|BMP(*.bmp)|*.bmp";
                        dlg2.FilterIndex = 1;
                        dlg2.Title = "ATTEMPTING TO RECOVER : Load an image";

                        // Display the dialog to the user
                        if (DialogResult.OK == dlg2.ShowDialog())
                        {
                            string temp = dlg2.FileName;

                            temp_emittor.TextureID1 = TM.LoadTexture(temp, Color.FromArgb(255, 0, 0, 0).ToArgb());

                            temp_emittor.Imagename = dlg2.SafeFileName;
                        }
                        else
                            temp_emittor.TextureID1 = 0;
                    }

                    XElement xRepeat = xEmittor.Element("Continuous");
                    if (xRepeat != null)
                        temp_emittor.IsContinuous = bool.Parse(xRepeat.Value);

                    XElement xBurst = xEmittor.Element("Bursting");
                    if (xBurst != null)
                        temp_emittor.IsBursting = bool.Parse(xBurst.Value);


                    for (int i = 0; i < temp_emittor.Max_Number1; i++)
                    {

                        Particle temp = new Particle();

                        temp.PosX = temp_emittor.Position.X;
                        temp.PosY = temp_emittor.Position.Y;
                        temp.Color = temp_emittor.Start_Color1;

                        temp.Endlife = rand.Next((int)temp_emittor.MinLife1, (int)temp_emittor.EndLife1);
                        temp.SpawnDelay1 = rand.NextDouble();

                        double velX;
                        double velY;

                        velX = rand.Next((int)temp_emittor.VelminX, (int)temp_emittor.VelmaxX) * .01;
                        velY = rand.Next((int)temp_emittor.VelminY, (int)temp_emittor.VelmaxY) * .01;


                        temp.VelX = velX;
                        temp.VelY = velY;

                        temp.Rotation = temp_emittor.Rotation;

                        temp.Colorfade_timer = 0;
                        temp.Scale_timerX = 0;
                        temp.Scale_timerY = 0;
                        temp.SpawnTime = 0;

                        temp_emittor.Particles.Add(temp);
                    }
                    m_ListOfEmittors.Add(temp_emittor);
                    listBoxEmittors.Items.Add(temp_emittor);
                }

                listBoxEmittors.SelectedIndex = 0;
                emittor = m_ListOfEmittors[listBoxEmittors.SelectedIndex];

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

                SourceBlendComboBox.SelectedIndex = (int)Source - 1;
                DestinationBlendComboBox.SelectedIndex = (int)Destination - 1;

                numericRotation.Value = emittor.Rotation;

            }
        }

        private void button_Advance_Click(object sender, EventArgs e)
        {
            groupBox5.Visible = !groupBox5.Visible; // Set the group box's visibility to the opposite to show or unshow the list of emittors.
        }

        private void textBoxEmittorName_TextChanged(object sender, EventArgs e)
        {
            emittor.Name = textBoxEmittorName.Text;
            if( listBoxEmittors.SelectedIndex >= 0)
                listBoxEmittors.Items[listBoxEmittors.SelectedIndex] = emittor.Name;
        }

        private void buttonAddEmittor_Click(object sender, EventArgs e)
        {
            ParticleEmitter temp_emittor = new ParticleEmitter();

            temp_emittor.Imagename = "../../resources/particles/raindrop.tga";

            temp_emittor.TextureID1 = TM.LoadTexture(temp_emittor.Imagename, Color.FromArgb(255, 0, 0, 0).ToArgb());

            temp_emittor.Particles.Clear();

            for (int i = 0; i < temp_emittor.Max_Number1; i++)
            {

                Particle temp = new Particle();

                temp.PosX = temp_emittor.Position.X;
                temp.PosY = temp_emittor.Position.Y;
                temp.Color = temp_emittor.Start_Color1;

                temp.Endlife = rand.Next((int)temp_emittor.MinLife1, (int)temp_emittor.EndLife1);
                temp.SpawnDelay1 = rand.NextDouble();

                double velX;
                double velY;

                velX = rand.Next((int)temp_emittor.VelminX, (int)temp_emittor.VelmaxX) * .01;
                velY = rand.Next((int)temp_emittor.VelminY, (int)temp_emittor.VelmaxY) * .01;


                temp.VelX = velX;
                temp.VelY = velY;

                temp.Rotation = temp_emittor.Rotation;

                temp.Colorfade_timer = 0;
                temp.Scale_timerX = 0;
                temp.Scale_timerY = 0;
                temp.SpawnTime = 0;

                temp_emittor.Particles.Add(temp);
            }

            m_ListOfEmittors.Add(temp_emittor);
            listBoxEmittors.Items.Add(temp_emittor);
        }

        private void listBoxEmittors_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBoxEmittors.SelectedIndex >= 0)
                emittor = m_ListOfEmittors[listBoxEmittors.SelectedIndex];

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

            SourceBlendComboBox.SelectedIndex = emittor.Source - 1;
            DestinationBlendComboBox.SelectedIndex = emittor.Destination - 1;

            RepeatcheckBox.Checked = emittor.IsContinuous;
            checkBoxBurst.Checked = emittor.IsBursting;

            numericRotation.Value = emittor.Rotation;
            textBoxEmittorName.Text = emittor.Name;
        }

        private void buttonRemoveEmittor_Click(object sender, EventArgs e)
        {
            if (listBoxEmittors.Items.Count <= 1)
                return;

            m_ListOfEmittors.Remove(m_ListOfEmittors[listBoxEmittors.SelectedIndex]);
            ParticleEmitter temp = (ParticleEmitter) listBoxEmittors.Items[listBoxEmittors.SelectedIndex];
            int temp_index = listBoxEmittors.SelectedIndex;
            listBoxEmittors.Items.Remove(temp);

            if (temp_index < listBoxEmittors.Items.Count )
                listBoxEmittors.SelectedIndex = temp_index;
            else if(temp_index > -1)
                listBoxEmittors.SelectedIndex = temp_index - 1;

            if (listBoxEmittors.SelectedIndex < 0)
                listBoxEmittors.SelectedIndex = 0;
            
        }

        private void checkBoxBurst_CheckedChanged(object sender, EventArgs e)
        {
            emittor.IsBursting = checkBoxBurst.Checked;
            if (emittor.IsBursting)
                ResetButton_Click(sender, e);
        }

        private void saveAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement xRoot = new XElement("Emittors");

                for (int i = 0; i < m_ListOfEmittors.Count; i++)
                {
                    emittor = m_ListOfEmittors[i];

                    XElement xEmittor = new XElement("Emittor");
                    xRoot.Add(xEmittor);

                    XElement xName = new XElement("Name", emittor.Name);
                    xEmittor.Add(xName);

                    XElement xNumPart = new XElement("NumberofParticles", emittor.Max_Number1);
                    xEmittor.Add(xNumPart);

                    XElement xMinLife = new XElement("MinLifeSpan", emittor.MinLife1);
                    xEmittor.Add(xMinLife);

                    XElement xMaxLife = new XElement("MaxLifeSpan", emittor.EndLife1);
                    xEmittor.Add(xMaxLife);

                    XElement xStartColor = new XElement("StartColor");
                    xEmittor.Add(xStartColor);
                    XAttribute xStartA = new XAttribute("A", emittor.Start_Color1.A);
                    xStartColor.Add(xStartA);
                    XAttribute xStartR = new XAttribute("R", emittor.Start_Color1.R);
                    xStartColor.Add(xStartR);
                    XAttribute xStartG = new XAttribute("G", emittor.Start_Color1.G);
                    xStartColor.Add(xStartG);
                    XAttribute xStartB = new XAttribute("B", emittor.Start_Color1.B);
                    xStartColor.Add(xStartB);

                    XElement xEndColor = new XElement("EndColor");
                    xEmittor.Add(xEndColor);
                    XAttribute xEndA = new XAttribute("A", emittor.End_Color1.A);
                    xEndColor.Add(xEndA);
                    XAttribute xEndR = new XAttribute("R", emittor.End_Color1.R);
                    xEndColor.Add(xEndR);
                    XAttribute xEndG = new XAttribute("G", emittor.End_Color1.G);
                    xEndColor.Add(xEndG);
                    XAttribute xEndB = new XAttribute("B", emittor.End_Color1.B);
                    xEndColor.Add(xEndB);

                    XElement xStartXScale = new XElement("XStartScale", emittor.Start_scaleX);
                    xEmittor.Add(xStartXScale);

                    XElement xEndXScale = new XElement("XEndScale", emittor.End_scaleX);
                    xEmittor.Add(xEndXScale);

                    XElement xStartYScale = new XElement("YStartScale", emittor.Start_scaleY);
                    xEmittor.Add(xStartYScale);

                    XElement xEndYScale = new XElement("YEndScale", emittor.End_scaleY);
                    xEmittor.Add(xEndYScale);

                    XElement xVelXMin = new XElement("MinXVelocity", emittor.VelminX);
                    xEmittor.Add(xVelXMin);

                    XElement xVelXMax = new XElement("MaxXVelocity", emittor.VelmaxX);
                    xEmittor.Add(xVelXMax);

                    XElement xVelYMin = new XElement("MinYVelocity", emittor.VelminY);
                    xEmittor.Add(xVelYMin);

                    XElement xVelYMax = new XElement("MaxYVelocity", emittor.VelmaxY);
                    xEmittor.Add(xVelYMax);

                    XElement xAccelX = new XElement("XAcceleration", emittor.AccelX);
                    xEmittor.Add(xAccelX);

                    XElement xAccelY = new XElement("YAcceleration", emittor.AccelY);
                    xEmittor.Add(xAccelY);

                    XElement xRotation = new XElement("Rotation", emittor.Rotation);
                    xEmittor.Add(xRotation);

                    XElement xSource = new XElement("SourceBlend", emittor.Source);
                    xEmittor.Add(xSource);

                    XElement xDestination = new XElement("DestinationBlend", emittor.Destination);
                    xEmittor.Add(xDestination);

                    XElement xImageName = new XElement("ImageName", emittor.Imagename);
                    xEmittor.Add(xImageName);

                    XElement xRepeat = new XElement("Continuous", emittor.IsContinuous);
                    xEmittor.Add(xRepeat);

                    XElement xBurst = new XElement("Bursting", emittor.IsBursting);
                    xEmittor.Add(xBurst);

                }

                xRoot.Save(dlg.FileName);
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            emittor = null;
            m_ListOfEmittors.Clear();
            listBoxEmittors.Items.Clear();

            emittor = new ParticleEmitter();

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
            textBoxEmittorName.Text = emittor.Name;

            emittor.IsContinuous = true;
            emittor.IsBursting = false;

            m_ListOfEmittors.Add(emittor);
            listBoxEmittors.Items.Add(emittor);
            listBoxEmittors.SelectedIndex = 0;
        }

        private void buttonMoveUp_Click(object sender, EventArgs e)
        {
            if (listBoxEmittors.SelectedIndex > 0)
            {
                int temp = listBoxEmittors.SelectedIndex;

                m_ListOfEmittors.Reverse(listBoxEmittors.SelectedIndex - 1, 2);

                listBoxEmittors.Items.Clear();

                for (int i = 0; i < m_ListOfEmittors.Count; i++)
                {
                    listBoxEmittors.Items.Add(m_ListOfEmittors[i]);
                }

                listBoxEmittors.SelectedIndex = temp;
            }
        }

        private void buttonMoveDown_Click(object sender, EventArgs e)
        {
            if (listBoxEmittors.SelectedIndex + 1 < m_ListOfEmittors.Count)
            {
                int temp = listBoxEmittors.SelectedIndex;

                m_ListOfEmittors.Reverse(listBoxEmittors.SelectedIndex, 2);

                listBoxEmittors.Items.Clear();

                for (int i = 0; i < m_ListOfEmittors.Count; i++)
                {
                    listBoxEmittors.Items.Add(m_ListOfEmittors[i]);
                }

                listBoxEmittors.SelectedIndex = temp;

            }
        }



    }
}
