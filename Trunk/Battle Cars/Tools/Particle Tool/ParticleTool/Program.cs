using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

using Microsoft.Samples.DirectX.UtilityToolkit;

namespace ParticleTool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            
            int FrameCount = 0;
            int DisplayFPS = 0;
            decimal timeStamp = 0;
            decimal deltatime = 0;

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());

            Form1 theForm = new Form1();

            theForm.Show();

            while (theForm.Looping)
            {
                deltatime = (decimal)FrameworkTimer.GetElapsedTime();
                theForm.UpdateParticles(deltatime);

                theForm.Render(DisplayFPS);

                FrameCount++;
                timeStamp += deltatime;
                if (timeStamp >= 1.0M)
                {
                    DisplayFPS = FrameCount;
                    FrameCount = 0;
                    timeStamp = 0;
                }


                Application.DoEvents();
            }

        }
    }
}
