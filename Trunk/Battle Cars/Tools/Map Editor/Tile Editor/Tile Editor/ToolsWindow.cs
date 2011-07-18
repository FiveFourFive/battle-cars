using System;
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
        public event EventHandler CollisionListChange;
        public event EventHandler EventListChange;
        public event EventHandler SpawnListChange;
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

        CTile[,] collisionList;

        public CTile[,] CollisionList
        {
            get { return collisionList; }
            set { collisionList = value; }
        }

        CTile[,] spawnList;

        public CTile[,] SpawnList
        {
            get { return spawnList; }
            set { spawnList = value; }
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

        TileType SelectedSpawn;

        public TileType SpawnType
        {
            get { return SelectedSpawn; }
            set { SelectedSpawn = value; }
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
            toolTip1.SetToolTip(this.CollisionBrush , "Collision Brush: used to put a collision tile on the main window");
            toolTip1.SetToolTip(this.SpawnBrush, "Spawn Brush: used to place a spawn point tile on the main window");
            toolTip1.SetToolTip(this.EventBrush, "Event Brush: used to place an event tile on the main window");
            toolTip1.SetToolTip(this.Fill, "Fill Tool: used to select a tile from the tile window");

            ToolsPlayerSpwan.Checked = true;
        }

        public new void Update()
        {

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

        bool gridSelected = true;

        public bool GridSelected
        {
            get { return gridSelected; }
            set { gridSelected = value; }
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

        private void SpawnBrush_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_SPAWN;

            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void CollisionBrush_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_COLLISION;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void EventBrush_Click(object sender, EventArgs e)
        {
            SelectedTool = Tools.TOOL_EVENT;
            if (ToolClicked != null)
            {
                ToolClicked(this, EventArgs.Empty);
            }
        }

        private void PlayerSpwan_CheckedChanged(object sender, EventArgs e)
        {
            if (ToolsPlayerSpwan.Checked)
            {
                SelectedSpawn = TileType.TYPE_PLAYER;
            }
        }

        private void PowerUpSpawn_CheckedChanged(object sender, EventArgs e)
        {
            if (ToolsPowerUpSpawn.Checked)
            {
                SelectedSpawn = TileType.TYPE_POWER;
            }
        }

        private void SpeedRampSpawn_CheckedChanged(object sender, EventArgs e)
        {
            if (ToolsSpeedRampSpawn.Checked)
            {
                SelectedSpawn = TileType.TYPE_SPEED;
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

        private void CollisionAdd_Click(object sender, EventArgs e)
        {
            if (CollisionXPos.Value < MapWidth.Value && CollisionYPos.Value < MapHeight.Value)
            {
                CTile collision = new CTile();

                collision.Name = CollisionName.Text;

                collision.XYIndex = new Point((int)(CollisionXPos.Value), (int)(CollisionYPos.Value));

                collision.Type = TileType.TYPE_COLLISION;

                if (collisionList[(int)CollisionYPos.Value, (int)CollisionXPos.Value].Type != (TileType)(-1))
                {
                    CollisionListBox.Items.Add(collision);
                    collisionList[(int)CollisionYPos.Value, (int)CollisionXPos.Value] = collision;
                }

                ClearBoxs();

                if (CollisionListChange != null)
                {
                    CollisionListChange(this, EventArgs.Empty);
                }
            }
        }

        private void CollisionUpdate_Click(object sender, EventArgs e)
        {

            if (CollisionXPos.Value < MapWidth.Value && CollisionYPos.Value < MapHeight.Value)
            {
                if (CollisionListBox.SelectedIndex != -1)
                {
                    CTile collision = (CTile)CollisionListBox.Items[CollisionListBox.SelectedIndex];

                    if (collisionList[(int)CollisionYPos.Value, (int)CollisionXPos.Value].Type != (TileType)(-1))
                    {
                        collisionList[collision.XYIndex.Y, collision.XYIndex.X].Type = (TileType)(-1);
                        collisionList[collision.XYIndex.Y, collision.XYIndex.X].XYIndex = new Point(-1, -1);

                        collision.Name = CollisionName.Text;
                        collision.XYIndex = new Point((int)(CollisionXPos.Value), (int)(CollisionYPos.Value));
                        collision.Type = TileType.TYPE_COLLISION;

                        CollisionListBox.Items.Insert(CollisionListBox.SelectedIndex, collision);
                        CollisionListBox.Items.RemoveAt(CollisionListBox.SelectedIndex);

                        collisionList[(int)CollisionYPos.Value, (int)CollisionXPos.Value] = collision;
                    }
                    ClearBoxs();

                    if (CollisionListChange != null)
                    {
                        CollisionListChange(this, EventArgs.Empty);
                    }
                }
            }
        }

        private void CollisionRemove_Click(object sender, EventArgs e)
        {
            if (CollisionListBox.SelectedIndex != -1)
            {
                CTile collision = (CTile)CollisionListBox.Items[CollisionListBox.SelectedIndex];

                collisionList[collision.XYIndex.Y, collision.XYIndex.X].Name = "NoName";
                collisionList[collision.XYIndex.Y, collision.XYIndex.X].Type = (TileType)(-1);
                collisionList[collision.XYIndex.Y, collision.XYIndex.X].XYIndex = new Point(-1, -1);

                CollisionListBox.Items.RemoveAt(CollisionListBox.SelectedIndex);

                CollisionListBox.SelectedIndex = -1;

                ClearBoxs();
                if (CollisionListChange != null)
                {
                    CollisionListChange(this, EventArgs.Empty);
                }
            }
        }

        private void EventAdd_Click(object sender, EventArgs e)
        {
            if (EventXPos.Value < MapWidth.Value && EventYPos.Value < MapHeight.Value)
            {
                CTile Event = new CTile();

                Event.Name = EventName.Text;

                // Rectangle pos = new Rectangle((int)XPosition.Value * pixelSize.Width, (int)YPosition.Value * pixelSize.Height, pixelSize.Width, pixelSize.Height);

                Event.XYIndex = new Point((int)(EventXPos.Value), (int)(EventYPos.Value));

                Event.Type = TileType.TYPE_COLLISION;

                if (eventList[(int)EventYPos.Value, (int)EventXPos.Value].Type != (TileType)(-1))
                {
                    EventListBox.Items.Add(Event);
                    eventList[(int)EventYPos.Value, (int)EventXPos.Value] = Event;
                }
                ClearBoxs();
                if (EventListChange != null)
                {
                    EventListChange(this, EventArgs.Empty);
                }
            }
        }

        private void EventUpdate_Click(object sender, EventArgs e)
        {
            if (EventXPos.Value < MapWidth.Value && EventYPos.Value < MapHeight.Value)
            {
                if (EventListBox.SelectedIndex != -1)
                {
                    CTile Event = (CTile)EventListBox.Items[EventListBox.SelectedIndex];

                    if (eventList[(int)EventYPos.Value, (int)EventXPos.Value].Type != (TileType)(-1))
                    {
                        eventList[Event.XYIndex.Y, Event.XYIndex.X].Type = (TileType)(-1);
                        eventList[Event.XYIndex.Y, Event.XYIndex.X].XYIndex = new Point(-1, -1);


                        Event.Name = EventName.Text;
                        Event.XYIndex = new Point((int)(EventXPos.Value), (int)(EventYPos.Value));
                        Event.Type = TileType.TYPE_EVENT;

                        EventListBox.Items.Insert(EventListBox.SelectedIndex, Event);
                        EventListBox.Items.RemoveAt(EventListBox.SelectedIndex);

                        eventList[(int)EventYPos.Value, (int)EventXPos.Value] = Event;
                    }
                    ClearBoxs();

                    if (EventListChange != null)
                    {
                        EventListChange(this, EventArgs.Empty);
                    }
                }
            }
        }

        private void EventRemove_Click(object sender, EventArgs e)
        {
            if (EventListBox.SelectedIndex != -1)
            {
                CTile Event = (CTile)EventListBox.Items[EventListBox.SelectedIndex];

                eventList[Event.XYIndex.Y, Event.XYIndex.X].Name = "NoName";
                eventList[Event.XYIndex.Y, Event.XYIndex.X].Type = (TileType)(-1);
                eventList[Event.XYIndex.Y, Event.XYIndex.X].XYIndex = new Point(-1, -1);

                EventListBox.Items.RemoveAt(EventListBox.SelectedIndex);

                EventListBox.SelectedIndex = -1;

                ClearBoxs();

                if (EventListChange != null)
                {
                    EventListChange(this, EventArgs.Empty);
                }
            }
        }

        private void SpawnAdd_Click(object sender, EventArgs e)
        {
            if (SpawnXPos.Value < MapWidth.Value && SpawnYPos.Value < MapWidth.Value)
            {
                CTile spawn = new CTile();

                spawn.Name = SpawnName.Text;
                spawn.XYIndex = new Point((int)(SpawnXPos.Value), (int)(SpawnYPos.Value));
                spawn.Type = SelectedSpawn;

                if (spawnList[(int)SpawnYPos.Value, (int)SpawnXPos.Value].Type != (TileType)(-1))
                {
                    SpawnListBox.Items.Add(spawn);
                    spawnList[(int)SpawnYPos.Value, (int)SpawnXPos.Value] = spawn;
                }

                ClearBoxs();
                if (SpawnListChange != null)
                {
                    SpawnListChange(this, EventArgs.Empty);
                }
            }
        }

        private void SpawnUpdate_Click(object sender, EventArgs e)
        {
            if (SpawnXPos.Value < MapWidth.Value && SpawnYPos.Value < MapHeight.Value)
            {
                if (SpawnListBox.SelectedIndex != -1)
                {
                    CTile spawn = (CTile)SpawnListBox.Items[SpawnListBox.SelectedIndex];

                    if (spawnList[(int)SpawnYPos.Value, (int)SpawnXPos.Value].Type != (TileType)(-1))
                    {
                        spawnList[spawn.XYIndex.Y, spawn.XYIndex.X].Type = (TileType)(-1);
                        spawnList[spawn.XYIndex.Y, spawn.XYIndex.X].XYIndex = new Point(-1, -1);


                        spawn.Name = SpawnName.Text;

                        spawn.XYIndex = new Point((int)(SpawnXPos.Value), (int)(SpawnYPos.Value));
                        spawn.Type = SelectedSpawn;

                        SpawnListBox.Items.Insert(SpawnListBox.SelectedIndex, spawn);
                        SpawnListBox.Items.RemoveAt(SpawnListBox.SelectedIndex);

                        spawnList[(int)SpawnYPos.Value, (int)SpawnXPos.Value] = spawn;
                    }
                    ClearBoxs();
                    if (SpawnListChange != null)
                    {
                        SpawnListChange(this, EventArgs.Empty);
                    }
                }
            }
        }

        private void SpawnRemove_Click(object sender, EventArgs e)
        {
            if (SpawnListBox.SelectedIndex != -1)
            {
                CTile spawn = (CTile)SpawnListBox.Items[SpawnListBox.SelectedIndex];

                spawnList[spawn.XYIndex.Y, spawn.XYIndex.X].Name = "NoName";
                spawnList[spawn.XYIndex.Y, spawn.XYIndex.X].Type = (TileType)(-1);
                spawnList[spawn.XYIndex.Y, spawn.XYIndex.X].XYIndex = new Point(-1, -1);

                SpawnListBox.Items.RemoveAt(SpawnListBox.SelectedIndex);

                SpawnListBox.SelectedIndex = -1;

                ClearBoxs();
                if (SpawnListChange != null)
                {
                    SpawnListChange(this, EventArgs.Empty);
                }
            }
        }

        private void SpawnPlayer_CheckedChanged(object sender, EventArgs e)
        {
            if (SpawnPlayer.Checked)
            {
                SelectedSpawn = TileType.TYPE_PLAYER;
            }
        }

        private void SpawnPower_CheckedChanged(object sender, EventArgs e)
        {
            if (SpawnPower.Checked)
            {
                SelectedSpawn = TileType.TYPE_PLAYER;
            }
        }

        private void SpawnSpeed_CheckedChanged(object sender, EventArgs e)
        {
            if (SpawnSpeed.Checked)
            {
                SelectedSpawn = TileType.TYPE_PLAYER;
            }
        }

        private void SpawnListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (SpawnListBox.SelectedIndex != -1)
            {
                CTile spawn = (CTile)SpawnListBox.Items[SpawnListBox.SelectedIndex];

                SpawnName.Text = spawn.Name;


                switch (spawn.Type)
                {
                    case TileType.TYPE_PLAYER:
                        SpawnPlayer.Checked = true;
                        break;
                    case TileType.TYPE_POWER:
                        SpawnPower.Checked = true;
                        break;
                    case TileType.TYPE_SPEED:
                        SpawnSpeed.Checked = true;
                        break;
                }

                SpawnXPos.Value = spawn.XYIndex.X;
                SpawnYPos.Value = spawn.XYIndex.Y;


            }
        }

        private void EventListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (EventListBox.SelectedIndex != -1)
            {
                CTile Event = (CTile)EventListBox.Items[EventListBox.SelectedIndex];

                EventName.Text = Event.Name;

                EventXPos.Value = Event.XYIndex.X;
                EventYPos.Value = Event.XYIndex.Y;
            }
        }

        private void CollisionListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CollisionListBox.SelectedIndex != -1)
            {
                CTile collision = (CTile)CollisionListBox.Items[CollisionListBox.SelectedIndex];

                CollisionName.Text = collision.Name;

                CollisionXPos.Value = collision.XYIndex.X;
                CollisionYPos.Value = collision.XYIndex.Y;
            }
        }

        private void ClearBoxs()
        {
            CollisionName.Text = "NoName";
            SpawnName.Text = "NoName";
            EventName.Text = "NoName";

            CollisionXPos.Value = 0;
            CollisionYPos.Value = 0;
            SpawnXPos.Value = 0;
            SpawnYPos.Value = 0;
            EventXPos.Value = 0;
            EventYPos.Value = 0;

        }

        public void LoadLists()
        {

            CollisionListBox.Items.Clear();
            for (int Ypos = 0; Ypos < MapSize.Height; Ypos++)
            {
                for (int Xpos = 0; Xpos < MapSize.Width; Xpos++)
                {
                    if (collisionList[Ypos, Xpos].Type != (TileType)(-1))
                        CollisionListBox.Items.Add(collisionList[Ypos, Xpos]);
                }
            }

            EventListBox.Items.Clear();
            for (int Ypos = 0; Ypos < MapSize.Height; Ypos++)
            {
                for (int Xpos = 0; Xpos < MapSize.Width; Xpos++)
                {
                    if (eventList[Ypos, Xpos].Type != (TileType)(-1))
                        EventListBox.Items.Add(eventList[Ypos, Xpos]);
                }
            }

            SpawnListBox.Items.Clear();
            for (int Ypos = 0; Ypos < MapSize.Height; Ypos++)
            {
                for (int Xpos = 0; Xpos < MapSize.Width; Xpos++)
                {
                    if (spawnList[Ypos, Xpos].Type != (TileType)(-1))
                        SpawnListBox.Items.Add(spawnList[Ypos, Xpos]);
                }
            }
        }
    }
}
