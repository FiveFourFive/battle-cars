﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Tile_Editor
{
    public partial class ToolsWindow : Form
    {
        public event EventHandler ToolClicked;
        public event EventHandler MapWidthChange;
        public event EventHandler MapHeightChange;
        public event EventHandler TileWidthChange;
        public event EventHandler TileHeightChange;
        public event EventHandler PixelWidthChange;
        public event EventHandler PixelHeightChange;
        public event EventHandler MapNameChange;

        Tools selectedTool;

        public Tools SelectedTool
        {
            get { return selectedTool; }
            set { selectedTool = value; }
        }

        CTile[,] eventList;

        public CTile[,] EventList
        {
            get { return eventList; }
            set { eventList = value; }
        }

        //Size pixelSize;

        public Size PixelSize
        {
            get { return new Size((int)PixelWidth.Value, (int)PixelHeight.Value); }
            set
            {
                PixelWidth.Value = value.Width;
                PixelHeight.Value = value.Height;
            }
        }

        Size imageSize;

        public Size ImageSize
        {
            get { return imageSize; }
            set { imageSize = value; }
        }

        //Size mapSize;

        public Size MapSize
        {
            get { return new Size ((int)MapWidth.Value, (int)MapHeight.Value); }
            set { 
                MapWidth.Value = value.Width;
                MapHeight.Value = value.Height;
               }
        }

        //Size tileSize;

        public Size TileSize
        {
            get { return new Size((int)TilesWidth.Value, (int)TilesHeight.Value); }
            set
            {
                TilesWidth.Value = value.Width;
                TilesHeight.Value = value.Height;
            }
        }

        string mapName;

        public string MapName
        {
            get { return mapName; }
            set { mapName = value; }
        }

        TileType eventType;

        public TileType EventType
        {
            get { return eventType; }
            set { eventType = value; }
        }

        bool gridSelected = true;

        public bool GridSelected
        {
            get { return gridSelected; }
            set { gridSelected = value; }
        }

        public string EventIDName
        {
            get { return EventID.Text; }
            set { EventID.Text = value; }
        }

        string id;

        public string ID
        {
            get { return id; }
            set { id = value; }
        }

        public ToolsWindow()
        {
            InitializeComponent();

            // Create the ToolTip and associate with the Form container.
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 1000;
            toolTip1.InitialDelay = 1000;
            toolTip1.AutoPopDelay = 3000;
            toolTip1.ReshowDelay = 1000;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = false;

            // Set up the ToolTip text for the Button and Checkbox.
            toolTip1.SetToolTip(this.SelectionTool, "Selection Tool: used to select a tile from the tile window");
            toolTip1.SetToolTip(this.Eraser, "Selection Tool: used to select a tile from the tile window");
            toolTip1.SetToolTip(this.Grid, "Grid Toggle: turns the grid for the main window and tile window on and off");
            toolTip1.SetToolTip(this.Brush, "Tile Brush: used to paint the selected tile on the main window");
            toolTip1.SetToolTip(this.EventBrush, "Event Brush: used to place an event tile on the main window");
            toolTip1.SetToolTip(this.Fill, "Fill Tool: used to select a tile from the tile window");

            CCollision.Checked = true;
            ID = "NULL";

        }

        private void Selection_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_SELECTION;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void Eraser_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_ERASER;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }


        private void Grid_Click(object sender, EventArgs e)
        {
            if (gridSelected == true)
            {
                gridSelected = false;
            }
            else
            {
                gridSelected = true;
            }

            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void Brush_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_BRUSH;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void Fill_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_FILL;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void EventBrush_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_EVENT;
            ToolsTab.SelectedIndex = 1;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void MapName_TextChanged(object sender, EventArgs e)
        {
            mapName = MapNameTextBox.Text;

            if (MapNameChange != null)
            {
                MapNameChange(this, EventArgs.Empty);
            }
        }

        private void MapWidth_ValueChanged(object sender, EventArgs e)
        {
            if (MapWidthChange != null)
            {
                MapWidthChange(this, EventArgs.Empty);
            }
        }

        private void MapHeight_ValueChanged(object sender, EventArgs e)
        {
            if (MapHeightChange != null)
            {
                MapHeightChange(this, EventArgs.Empty);
            }
        }

        private void TilesWidth_ValueChanged(object sender, EventArgs e)
        {
            if (TileWidthChange != null)
            {
                TileWidthChange(this, EventArgs.Empty);
            }
        }

        private void TilesHeight_ValueChanged(object sender, EventArgs e)
        {
            if (TileHeightChange != null)
            {
                TileHeightChange(this, EventArgs.Empty);
            }
        }

        private void PixelWidth_ValueChanged(object sender, EventArgs e)
        {
            Size size = new Size();

            size.Width = imageSize.Width / (int)PixelWidth.Value;
            size.Height = TileSize.Height;

            if (size.Width > 0 && size.Height > 0)
                TileSize = size;

            if (PixelWidthChange != null)
            {
                PixelWidthChange(this, EventArgs.Empty);
            }
        }

        private void PixelHeight_ValueChanged(object sender, EventArgs e)
        {
            Size size = new Size();
            size.Width = TileSize.Width;
            size.Height = imageSize.Height / (int)PixelHeight.Value;

            if (size.Width > 0 && size.Height > 0)
                TileSize = size;

            if (PixelHeightChange != null)
            {
                PixelHeightChange(this, EventArgs.Empty);
            }
        }

        private void CCollision_CheckedChanged(object sender, EventArgs e)
        {
            if (CCollision.Checked == true)
            {
                WCollision.Checked = false;
                PlayerSpawn.Checked = false;
                PowerUpSpawn.Checked = false;
                SpeedRampSpawn.Checked = false;
                ObstacleSpawn.Checked = false;
                PowerUps.Visible = false;
                SpeedRampDirection.Visible = false;
                Obstacles.Visible = false;
                EventID.Text = "CameraCollision";
                PowerUps.SelectedIndex = -1;
                SpeedRampDirection.SelectedIndex = -1;
                Obstacles.SelectedIndex = -1;
            }
            else
                CCollision.Checked = false;
        }

        private void WCollision_CheckedChanged(object sender, EventArgs e)
        {
            if (WCollision.Checked == true)
            {
                CCollision.Checked = false;
                PlayerSpawn.Checked = false;
                PowerUpSpawn.Checked = false;
                SpeedRampSpawn.Checked = false;
                ObstacleSpawn.Checked = false;
                PowerUps.Visible = false;
                SpeedRampDirection.Visible = false;
                Obstacles.Visible = false;
                EventID.Text = "WallCollision";
                PowerUps.SelectedIndex = -1;
                SpeedRampDirection.SelectedIndex = -1;
                Obstacles.SelectedIndex = -1;
            }
            else
                WCollision.Checked = false;
        }

        private void PlayerSpawn_CheckedChanged(object sender, EventArgs e)
        {
            if (PlayerSpawn.Checked == true)
            {
                CCollision.Checked = false;
                WCollision.Checked = false;
                PowerUpSpawn.Checked = false;
                SpeedRampSpawn.Checked = false;
                ObstacleSpawn.Checked = false;
                PowerUps.Visible = false;
                SpeedRampDirection.Visible = false;
                Obstacles.Visible = false;
                EventID.Text = "PlayerSpawn";
                PowerUps.SelectedIndex = -1;
                SpeedRampDirection.SelectedIndex = -1;
                Obstacles.SelectedIndex = -1;
            }
            else
                PlayerSpawn.Checked = false;
        }

        private void PowerUpSpawn_CheckedChanged_1(object sender, EventArgs e)
        {
            if (PowerUpSpawn.Checked == true)
            {
                CCollision.Checked = false;
                WCollision.Checked = false;
                PlayerSpawn.Checked = false;
                SpeedRampSpawn.Checked = false;
                ObstacleSpawn.Checked = false;
                PowerUps.Visible = true;
                SpeedRampDirection.Visible = false;
                Obstacles.Visible = false;
                EventID.Text = "PowerUpSpawn";
                SpeedRampDirection.SelectedIndex = -1;
                Obstacles.SelectedIndex = -1;
            }
            else
            {
                PowerUps.Visible = false;
                PowerUpSpawn.Checked = false;
                PowerUps.SelectedIndex = -1;
            }
        }

        private void ObstacleSpawn_CheckedChanged_1(object sender, EventArgs e)
        {
            if (ObstacleSpawn.Checked == true)
            {
                CCollision.Checked = false;
                WCollision.Checked = false;
                PowerUpSpawn.Checked = false;
                SpeedRampSpawn.Checked = false;
                PlayerSpawn.Checked = false;
                PowerUps.Visible = false;
                SpeedRampDirection.Visible = false;
                Obstacles.Visible = true;
                EventID.Text = "ObstacleSpawn";
                PowerUps.SelectedIndex = -1;
                SpeedRampDirection.SelectedIndex = -1;
            }
            else
            {
                Obstacles.Visible = false;
                ObstacleSpawn.Checked = false;
                Obstacles.SelectedIndex = -1;
            }
        }

        private void SpeedRampSpawn_CheckedChanged_1(object sender, EventArgs e)
        {
            if (SpeedRampSpawn.Checked == true)
            {
                CCollision.Checked = false;
                WCollision.Checked = false;
                PowerUpSpawn.Checked = false;
                PlayerSpawn.Checked = false;
                ObstacleSpawn.Checked = false;
                PowerUps.Visible = false;
                SpeedRampDirection.Visible = true;
                Obstacles.Visible = false;
                EventID.Text = "SpeedRampSpawn";
                PowerUps.SelectedIndex = -1;
                Obstacles.SelectedIndex = -1;
            }
            else
            {
                SpeedRampDirection.Visible = false;
                SpeedRampSpawn.Checked = false;
                SpeedRampDirection.SelectedIndex = -1;
            }
        }

        private void EventEraser_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_EVENTERASER;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void SpeedRampDirection_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (SpeedRampDirection.SelectedIndex != -1)
            {

                id = SpeedRampDirection.Items[SpeedRampDirection.SelectedIndex].ToString();
            }
            else
                id = "NULL";
        }

        private void PowerUps_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (PowerUps.SelectedIndex != -1)
            {
                id = PowerUps.Items[PowerUps.SelectedIndex].ToString();
            }
            else
                id = "NULL";
        }

        private void Obstacles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Obstacles.SelectedIndex != -1)
            {
                id = Obstacles.Items[Obstacles.SelectedIndex].ToString();
            }
            else
                id = "NULL";
        }

       
    }
}
