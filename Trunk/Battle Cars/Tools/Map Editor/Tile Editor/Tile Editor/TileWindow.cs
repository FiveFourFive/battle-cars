using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SGD;

namespace Tile_Editor
{
    public partial class TileWindow : Form
    {
        ManagedTextureManager TM = ManagedTextureManager.Instance;
        ManagedDirect3D D3D = ManagedDirect3D.Instance;

        public event EventHandler TileClicked;

        public void Render()
        {
            Point offset = Point.Empty;

            offset.X += TilePanel.AutoScrollPosition.X;
            offset.Y += TilePanel.AutoScrollPosition.Y;

            Point Position = Point.Empty;

            Position.X += TilePanel.AutoScrollPosition.X;
            Position.Y += TilePanel.AutoScrollPosition.Y;

            D3D.ChangeDisplayParam(TilePanel, false);

            D3D.Clear(TilePanel, 255, 255, 255);
            D3D.DeviceBegin();
            D3D.LineBegin();

            D3D.SpriteBegin();
            Rectangle tile = new Rectangle(0, 0, tileSize.Width * pixelSize.Width, tileSize.Height * pixelSize.Height);
            TM.Draw(tileBitmap, offset.X, offset.Y, 1.0f, 1.0f, tile, 0, 0, 0, 0);
            D3D.SpriteEnd();

            //D3D.Sprite.Flush();

            if (gridShow)
            {
                for (int Xindex = 0; Xindex <= tileSize.Width; Xindex++)
                {
                    D3D.DrawLine(Xindex * pixelSize.Width + Position.X, Position.Y, Xindex * pixelSize.Width + Position.X, tileSize.Height * pixelSize.Height + Position.Y, 255, 0, 0);
                }
                for (int Yindex = 0; Yindex <= tileSize.Height; Yindex++)
                {
                    D3D.DrawLine(Position.X, Yindex * pixelSize.Height + Position.Y, tileSize.Width * pixelSize.Width + Position.X, Yindex * pixelSize.Height + Position.Y, 255, 0, 0);
                }
            }

            D3D.DrawLine(tileSelected.X * pixelSize.Width + Position.X, tileSelected.Y * pixelSize.Height + Position.Y,
                        tileSelected.X * pixelSize.Width + Position.X, (tileSelected.Y * pixelSize.Height) + pixelSize.Height + Position.Y, 0, 255, 0); // left side

            D3D.DrawLine(tileSelected.X * pixelSize.Width + Position.X, tileSelected.Y * pixelSize.Height + Position.Y,
                        (tileSelected.X * pixelSize.Width) + pixelSize.Width + Position.X, tileSelected.Y * pixelSize.Height + Position.Y, 0, 255, 0); // top side

            D3D.DrawLine((tileSelected.X + 1) * pixelSize.Width + Position.X, tileSelected.Y * pixelSize.Height + Position.Y,
                        (tileSelected.X + 1) * pixelSize.Width + Position.X, (tileSelected.Y + 1) * pixelSize.Height + Position.Y, 0, 255, 0); //right side

            D3D.DrawLine(tileSelected.X * pixelSize.Width + Position.X, (tileSelected.Y + 1) * pixelSize.Height + Position.Y,
                        ((tileSelected.X) * pixelSize.Width) + pixelSize.Width + Position.X, (tileSelected.Y + 1) * pixelSize.Height + Position.Y, 0, 255, 0);

            D3D.LineEnd();
            D3D.DeviceEnd();

            D3D.Present(TilePanel);
        }

        public TileWindow()
        {
            InitializeComponent();
        }

        bool gridShow;

        public bool ShowGrid
        {
            get { return gridShow; }
            set { gridShow = value; }
        }

        Point tileSelected;

        public Point TileSelected
        {
            get { return tileSelected; }
            set { tileSelected = value; }
        }

        Size pixelSize;

        public Size PixelSize
        {
            get { return pixelSize; }
            set { pixelSize = value; }
        }

        Size tileSize;

        public Size TileSize
        {
            get { return tileSize; }
            set { tileSize = value; }
        }

        int tileBitmap;

        public int TileBitmap
        {
            get { return tileBitmap; }
            set { tileBitmap = value; }
        }

        //Panel screenSize;

        //public Panel ScreenSize
        //{
        //    set { screenSize = value; }
        //}

        public void SetAutorScroll()
        {
            TilePanel.AutoScrollMinSize = new Size (tileSize.Width * pixelSize.Width, tileSize.Height * pixelSize.Height);
        }

        private void PicBox_MouseClick(object sender, MouseEventArgs e)
        {
            Size position = Size.Empty;
            Point offset = e.Location;

            offset.X -= TilePanel.AutoScrollPosition.X;
            offset.Y -= TilePanel.AutoScrollPosition.Y;


            for (int ypos = 0; ypos < tileSize.Height; ypos++)
            {
                for (int xpos = 0; xpos < tileSize.Width; xpos++)
                {
                    if (offset.X > pixelSize.Width * xpos && offset.Y > pixelSize.Height * ypos && offset.X < pixelSize.Width * (xpos + 1) && offset.Y < pixelSize.Height * (ypos + 1))
                    {
                        Point selectedPos = Point.Empty;
                        selectedPos.X = xpos;//(int)pixelSize.Width * xpos;
                        selectedPos.Y = ypos;//(int)pixelSize.Height * ypos;

                        tileSelected = selectedPos;
                    }
                }
            }

            if (TileClicked != null)
            {
                TileClicked(this, EventArgs.Empty);
            }
        }
    }
}
