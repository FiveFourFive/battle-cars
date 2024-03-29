﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Tile_Editor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new MapEditor());

            MapEditor theform = new MapEditor();

            theform.Show();

            while (theform.Looping)
            {
                theform.Update();
                theform.Render();

                Application.DoEvents();
            }
        }
    }
}
