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

        Point ScrollOffset;

        Tools currentTool;

        public Tools CurrentTool
        {
            get { return currentTool; }
            set { currentTool = value; }
        }


        public TileWindow()
        {
            InitializeComponent();

            ScrollOffset.X = 0;
            ScrollOffset.Y = 0;
            vScrollBar1.Value = 0;
            hScrollBar1.Value = 0;
            vScrollBar1.Maximum = tileSize.Height * pixelSize.Height;
            hScrollBar1.Maximum = tileSize.Width * pixelSize.Width;
        }

        public void Render()
        {
            Point offset = Point.Empty;

            offset.X -= ScrollOffset.X;
            offset.Y -= ScrollOffset.Y;

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
                    D3D.DrawLine(Xindex * pixelSize.Width + offset.X, offset.Y, Xindex * pixelSize.Width + offset.X, tileSize.Height * pixelSize.Height + offset.Y, 255, 0, 0);
                }
                for (int Yindex = 0; Yindex <= tileSize.Height; Yindex++)
                {
                    D3D.DrawLine(offset.X, Yindex * pixelSize.Height + offset.Y, tileSize.Width * pixelSize.Width + offset.X, Yindex * pixelSize.Height + offset.Y, 255, 0, 0);
                }
            }

            D3D.DrawLine((tileSelected.X * pixelSize.Width) + offset.X, (tileSelected.Y * pixelSize.Height) + offset.Y,
                        (tileSelected.X * pixelSize.Width) + offset.X, (endSelection.Y * pixelSize.Height) + pixelSize.Height + offset.Y, 0, 255, 0); // left side

            D3D.DrawLine((tileSelected.X * pixelSize.Width) + offset.X, (tileSelected.Y * pixelSize.Height) + offset.Y,
                        (endSelection.X * pixelSize.Width) + pixelSize.Width + offset.X, (tileSelected.Y * pixelSize.Height) + offset.Y, 0, 255, 0); // top side

            D3D.DrawLine((endSelection.X * pixelSize.Width) + pixelSize.Width + offset.X, tileSelected.Y * pixelSize.Height + offset.Y,
                        (endSelection.X * pixelSize.Width) + pixelSize.Width + offset.X, (endSelection.Y * pixelSize.Height) + pixelSize.Height + offset.Y, 0, 255, 0); //right side

            D3D.DrawLine((tileSelected.X * pixelSize.Width) + offset.X, (endSelection.Y * pixelSize.Height) + pixelSize.Height + offset.Y,
                        (endSelection.X * pixelSize.Width) + pixelSize.Width + offset.X, (endSelection.Y * pixelSize.Height) + pixelSize.Height + offset.Y, 0, 255, 0);//bottom side

            D3D.LineEnd();
            D3D.DeviceEnd();

            D3D.Present(TilePanel);
        }

        Tools selectedTool;

        public Tools SelectedTool
        {
            get { return selectedTool; }
            set { selectedTool = value; }
        }

        bool FirstSelected = false;
        Point endSelection;

        public Point EndSelection
        {
            get { return endSelection; }
            set { endSelection = value; }
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

        public void SetAutorScroll()
        {
            vScrollBar1.Value = 0;
            hScrollBar1.Value = 0;
            vScrollBar1.Maximum = tileSize.Height * pixelSize.Height;
            hScrollBar1.Maximum = tileSize.Width * pixelSize.Width;
        }

        private void PicBox_MouseClick(object sender, MouseEventArgs e)
        {
            if (FirstSelected == false)
            {
                Point position = Point.Empty;
                Point offset = e.Location;

                offset.X += ScrollOffset.X;
                offset.Y += ScrollOffset.Y;

                position.X = offset.X / pixelSize.Width;
                position.Y = offset.Y / pixelSize.Height;

                if (position.X >= tileSize.Width || position.Y >= tileSize.Height || position.X < 0 || position.Y < 0)
                    return;

                tileSelected = position;
                endSelection = tileSelected;

                if (TileClicked != null)
                {
                    TileClicked(this, EventArgs.Empty);
                }
            }
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            ScrollOffset.Y = e.NewValue;
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            ScrollOffset.X = e.NewValue;
        }

        private void TilePanel_MouseMove(object sender, MouseEventArgs e)
        {
            Point position = Point.Empty;
            Point offset = e.Location;

            offset.X += ScrollOffset.X;
            offset.Y += ScrollOffset.Y;

            if (e.Button == MouseButtons.Left)
            {
                if (FirstSelected == false)
                {
                    FirstSelected = true;
                    position.X = offset.X / pixelSize.Width;
                    position.Y = offset.Y / pixelSize.Height;

                    if (position.X >= tileSize.Width || position.Y >= tileSize.Height || position.X < 0 || position.Y < 0)
                        return;


                    tileSelected = position;
                    endSelection = tileSelected;

                    if (TileClicked != null)
                    {
                        TileClicked(this, EventArgs.Empty);
                    }

                    return;
                }

                if (currentTool == Tools.TOOL_SELECTION)
                {
                    position.X = offset.X / pixelSize.Width;
                    position.Y = offset.Y / pixelSize.Height;

                    if (position.X >= tileSize.Width || position.Y >= tileSize.Height || position.X < 0 || position.Y < 0)
                        return;

                    endSelection = position;
                }

                if (TileClicked != null)
                {
                    TileClicked(this, EventArgs.Empty);
                }
            }
        }

        private void TilePanel_MouseUp(object sender, MouseEventArgs e)
        {
            FirstSelected = false;
        }
    }
}
