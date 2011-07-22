using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Linq;
using SGD;

namespace Tile_Editor
{
    public enum Tools { TOOL_SELECTION, TOOL_ERASER, TOOL_EVENTERASER, TOOL_GRID, TOOL_BRUSH, TOOL_FILL, TOOL_EVENT, TOOL_COLLISION, TOOL_SPAWN, TOOL_MAX }
    public enum TileType {TYPE_PLAYER, TYPE_POWER, TYPE_SPEED, TYPE_COLLISION, TYPE_EVENT, TYPE_MAX };

    public partial class MapEditor : Form
    {
        ManagedDirect3D D3D = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;

        public bool Looping = true;
        bool SaveBool = false;
        bool gridShow = true;

        ToolsWindow toolsWindow = null;
        TileWindow tileWindow = null;
        HelpWindow help = null;

        string MainPath;
        string BitMapPath;
        string TileBitmapFile;

        Point GhostPoint = new Point();
        int EraserId = -1;

        CMap levelMap = new CMap();
        Size OldMapSize;

        Tools ToolSelection = Tools.TOOL_SELECTION;
        TileType ToolEventType = TileType.TYPE_PLAYER;

        Point ScrollOffset = Point.Empty;

        public MapEditor()
        {
            InitializeComponent();

            D3D.InitManagedDirect3D(MainMapPanel, false);
            TM.InitManagedTextureManager(D3D.Device, D3D.Sprite);

            MainPath = Environment.CurrentDirectory;

            string path = MainPath;
            path += "/Resource/Images/SimpleTileSet.png";

            BitMapPath = MainPath + "/Resource/Images/";
            TileBitmapFile = "SimpleTileSet.png";

            levelMap.TileBitmap = TM.LoadTexture(path, 0);

            path = MainPath;
            path += "/Resource/Images/FT_CursorEraser.png";

            EraserId = TM.LoadTexture(path, 0);


            levelMap.TileList = new CTile[5, 15];
            levelMap.EventList = new CTile[5, 15];

            Size LevelSize = new Size(15, 5);
            levelMap.MapSize = LevelSize;
            OldMapSize = LevelSize;

            levelMap.TileList = new CTile[LevelSize.Height, LevelSize.Width];
            levelMap.EventList = new CTile[LevelSize.Height, LevelSize.Width];

            levelMap.TileSelected = new Point(0, 0);
            levelMap.EndSelected = new Point(0, 0);
            levelMap.TileSize = new Size(4, 4);
            levelMap.PixelSize = new Size(32, 32);

            vScrollBar1.Minimum = 0;
            hScrollBar1.Minimum = 0;
            hScrollBar1.Maximum = levelMap.MapSize.Width * levelMap.PixelSize.Width;
            vScrollBar1.Maximum = levelMap.MapSize.Height * levelMap.PixelSize.Height;

            ScrollOffset.X = 0;
            ScrollOffset.Y = 0;

            levelMap.MapName = "Untitled";

            levelMap.CollisionColor = Color.FromArgb(255, 0, 255, 0);
            levelMap.SpawnColor = Color.FromArgb(255, 255, 0, 255);
            levelMap.EventColor = Color.FromArgb(255, 255, 255, 0);

            for (int Ypos = 0; Ypos < levelMap.MapSize.Height; Ypos++)
            {
                for (int Xpos = 0; Xpos < levelMap.MapSize.Width; Xpos++)
                {
                    levelMap.TileList[Ypos, Xpos] = new CTile();
                    levelMap.EventList[Ypos, Xpos] = new CTile();

                    levelMap.TileList[Ypos, Xpos].Type = (TileType)(-1);
                    levelMap.EventList[Ypos, Xpos].Type = (TileType)(-1);

                    levelMap.TileList[Ypos, Xpos].XYIndex = new Point(Xpos, Ypos);
                    levelMap.EventList[Ypos, Xpos].XYIndex = new Point(Xpos, Ypos);

                    levelMap.EventList[Ypos, Xpos].Name = "NULL";
                    levelMap.EventList[Ypos, Xpos].ID = "NULL";
                }
            }

            CreateToolWindow();
            CreateTileWindow();
        }

        private void CreateToolWindow()
        {
            if (toolsWindow == null)
            {
                toolsWindow = new ToolsWindow();
                toolsWindow.ToolClicked += new EventHandler(Tool_Clicked);
                toolsWindow.FormClosed += new FormClosedEventHandler(Tool_FormClosed);
                toolsWindow.MapWidthChange += new EventHandler(MapWidthChange);
                toolsWindow.MapHeightChange += new EventHandler(MapHeightChange);
                toolsWindow.TileWidthChange += new EventHandler(TileWidthChange);
                toolsWindow.TileHeightChange += new EventHandler(TileHeightChange);
                toolsWindow.PixelWidthChange += new EventHandler(PixelWidthChange);
                toolsWindow.PixelHeightChange += new EventHandler(PixelHeightChange);
                toolsWindow.MapNameChange += new EventHandler(MapNameChange);

                toolsWindow.ImageSize = new Size(TM.GetTextureWidth(levelMap.TileBitmap), TM.GetTextureHeight(levelMap.TileBitmap));
                toolsWindow.MapName = levelMap.MapName;
                toolsWindow.MapSize = levelMap.MapSize;
                toolsWindow.TileSize = levelMap.TileSize;
                toolsWindow.PixelSize = levelMap.PixelSize;
                toolsWindow.EventList = levelMap.EventList;

                toolsWindow.EventType = ToolEventType;
                toolsWindow.SelectedTool = ToolSelection;

                toolsWindow.Show(this);
            }
        }
        void Tool_FormClosed(object sender, FormClosedEventArgs e)
        {
            toolsWindow = null;
        }

        void Tool_Clicked(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;

            ToolSelection = tool.SelectedTool;

            gridShow = tool.GridSelected;

            if (tileWindow != null)
            {
                tileWindow.CurrentTool = ToolSelection;
                tileWindow.ShowGrid = gridShow;
            }
        }

        void MapWidthChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;

            OldMapSize = levelMap.MapSize;
            levelMap.MapSize = new Size(tool.MapSize.Width, levelMap.MapSize.Height);

            vScrollBar1.Minimum = 0;
            hScrollBar1.Minimum = 0;
            hScrollBar1.Maximum = levelMap.MapSize.Width * levelMap.PixelSize.Width;
            vScrollBar1.Maximum = levelMap.MapSize.Height * levelMap.PixelSize.Height;

            MapRest();
        }

        void MapHeightChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;

            OldMapSize = levelMap.MapSize;
            levelMap.MapSize = new Size(levelMap.MapSize.Width, tool.MapSize.Height);
            vScrollBar1.Minimum = 0;
            hScrollBar1.Minimum = 0;
            hScrollBar1.Maximum = levelMap.MapSize.Width * levelMap.PixelSize.Width;
            vScrollBar1.Maximum = levelMap.MapSize.Height * levelMap.PixelSize.Height;

            MapRest();
        }

        void TileWidthChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;
            levelMap.TileSize = new Size(tool.TileSize.Width, levelMap.TileSize.Height);

            if (tileWindow != null)
            {
                tileWindow.TileSize = new Size(tool.TileSize.Width, levelMap.TileSize.Height);
            }
        }

        void TileHeightChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;
            levelMap.TileSize = new Size(levelMap.TileSize.Width, tool.TileSize.Height);

            if (tileWindow != null)
            {
                tileWindow.TileSize = new Size(levelMap.TileSize.Width, tool.TileSize.Height);
            }
        }

        void PixelWidthChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;
            levelMap.PixelSize = new Size(tool.PixelSize.Width, levelMap.PixelSize.Height);

            vScrollBar1.Minimum = 0;
            hScrollBar1.Minimum = 0;
            hScrollBar1.Maximum = levelMap.MapSize.Width * levelMap.PixelSize.Width;
            vScrollBar1.Maximum = levelMap.MapSize.Height * levelMap.PixelSize.Height;

            if (tileWindow != null)
            {
                tileWindow.PixelSize = new Size(tool.PixelSize.Width, levelMap.PixelSize.Height);
            }
            MapRest();
        }

        void PixelHeightChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;
            levelMap.PixelSize = new Size(levelMap.PixelSize.Width, tool.PixelSize.Height);

            vScrollBar1.Minimum = 0;
            hScrollBar1.Minimum = 0;
            vScrollBar1.Maximum = levelMap.MapSize.Height * levelMap.PixelSize.Height;
            hScrollBar1.Maximum = levelMap.MapSize.Width * levelMap.PixelSize.Width;

            if (tileWindow != null)
            {
                tileWindow.PixelSize = new Size(levelMap.PixelSize.Width, tool.PixelSize.Height);
            }
            MapRest();
        }

        void MapNameChange(object sender, EventArgs e)
        {
            ToolsWindow tool = (ToolsWindow)sender;

            levelMap.MapName = tool.MapName;
        }

        void EventListChange(object sender, EventArgs e)
        {
            levelMap.EventList = ((ToolsWindow)sender).EventList;           
        }

        private void CreateTileWindow()
        {
            if (tileWindow == null)
            {
                tileWindow = new TileWindow();
                tileWindow.FormClosed += new FormClosedEventHandler(Tile_FormClosed);
                tileWindow.TileClicked += new EventHandler(Tile_Clicked);

                tileWindow.CurrentTool = ToolSelection;
                tileWindow.TileBitmap = levelMap.TileBitmap;
                tileWindow.PixelSize = levelMap.PixelSize;
                tileWindow.EndSelection = levelMap.EndSelected;
                tileWindow.TileSelected = levelMap.TileSelected;
                tileWindow.TileSize = levelMap.TileSize;
                tileWindow.ShowGrid = gridShow;
                tileWindow.SetAutorScroll();


                tileWindow.Show(this);
            }
        }

        void Tile_FormClosed(object sender, FormClosedEventArgs e)
        {
            tileWindow = null;
        }

        void Tile_Clicked(object sender, EventArgs e)
        {
            TileWindow tile = (TileWindow)sender;

            levelMap.TileSelected = tile.TileSelected;
            levelMap.EndSelected = tile.EndSelection;
        }

        private void toolsWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateToolWindow();
        }

        private void tileWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTileWindow();
        }

        public new void Update()
        {
        }

        public void Render()
        {
            Point offset = Point.Empty;

            offset.X -= ScrollOffset.X;
            offset.Y -= ScrollOffset.Y;

            if (tileWindow != null)
            {
                tileWindow.Render();
            }

            D3D.ChangeDisplayParam(MainMapPanel, false);

            D3D.Clear(MainMapPanel, 255, 255, 255);
            D3D.DeviceBegin();
            D3D.LineBegin();

            int XBegin = ((offset.X*-1) / levelMap.PixelSize.Width);
            int YBegin = ((offset.Y*-1) / levelMap.PixelSize.Height);
            int XEnd = ((offset.X * -1) + MainMapPanel.Width) / levelMap.PixelSize.Width;
            int YEnd = ((offset.Y * -1) + MainMapPanel.Height) / levelMap.PixelSize.Height;

            if (XEnd >= levelMap.MapSize.Width)
                XEnd = levelMap.MapSize.Width;

            if (YEnd >= levelMap.MapSize.Height)
                YEnd = levelMap.MapSize.Height;

            if (gridShow)
            {
                for (int Xindex = XBegin; Xindex <= XEnd; Xindex++)
                {
                    D3D.DrawLine(Xindex * levelMap.PixelSize.Width + offset.X, offset.Y, Xindex * levelMap.PixelSize.Width + offset.X, levelMap.MapSize.Height * levelMap.PixelSize.Height + offset.Y, 255, 0, 0);
                }
                for (int Yindex = YBegin; Yindex <= YEnd; Yindex++)
                {
                    D3D.DrawLine(offset.X, Yindex * levelMap.PixelSize.Height + offset.Y, levelMap.MapSize.Width * levelMap.PixelSize.Width + offset.X, Yindex * levelMap.PixelSize.Height + offset.Y, 255, 0, 0);
                }
            }

            D3D.SpriteBegin();

            for (int Ypos = YBegin; Ypos < YEnd; Ypos++)
            {
                for (int Xpos = XBegin; Xpos < XEnd; Xpos++)
                {
                    if (levelMap.TileList[Ypos, Xpos].Type != (TileType)(-1))
                    {
                        Rectangle tileRect = new Rectangle();
                        tileRect.Location = new Point(levelMap.TileList[Ypos, Xpos].PickedTile.X * levelMap.PixelSize.Width,
                                                      levelMap.TileList[Ypos, Xpos].PickedTile.Y * levelMap.PixelSize.Height);

                        tileRect.Size = new Size(levelMap.PixelSize.Width, levelMap.PixelSize.Height);

                        Point TilePos = new Point(levelMap.TileList[Ypos, Xpos].XYIndex.X * levelMap.PixelSize.Width, levelMap.TileList[Ypos, Xpos].XYIndex.Y * levelMap.PixelSize.Height);

                        TM.Draw(levelMap.TileBitmap, TilePos.X + offset.X, TilePos.Y + offset.Y, 1.0f, 1.0f, tileRect, 0, 0, 0, 0);
                    }
                }
            }


            if (ToolSelection == Tools.TOOL_BRUSH)
            {
                Rectangle tileRect = new Rectangle();
                tileRect.Location = new Point(levelMap.TileSelected.X * levelMap.PixelSize.Width,
                                              levelMap.TileSelected.Y * levelMap.PixelSize.Height);
                tileRect.Size = new Size();

                Size size = new Size();
                if (levelMap.EndSelected.X == levelMap.TileSelected.X)
                {
                    size.Width = levelMap.PixelSize.Width;
                }
                else
                {
                    int difference = levelMap.EndSelected.X - levelMap.TileSelected.X;
                    size.Width = difference * levelMap.PixelSize.Width + levelMap.PixelSize.Width;
                }

                if (levelMap.EndSelected.Y == levelMap.TileSelected.Y)
                {
                    size.Height = levelMap.PixelSize.Height;
                }
                else
                {
                    int difference = levelMap.EndSelected.Y - levelMap.TileSelected.Y;
                    size.Height = difference * levelMap.PixelSize.Height + levelMap.PixelSize.Height;
                }

                tileRect.Size = size;
                Point TilePos = new Point(GhostPoint.X * levelMap.PixelSize.Width, GhostPoint.Y * levelMap.PixelSize.Height);

                Color col = Color.FromArgb(135, 255, 255, 255);

                TM.Draw(levelMap.TileBitmap, TilePos.X + offset.X, TilePos.Y + offset.Y, 1.0f, 1.0f, tileRect, 0, 0, 0, col.ToArgb());
            }

            if (ToolSelection == Tools.TOOL_ERASER || ToolSelection == Tools.TOOL_EVENTERASER)
            {
                Rectangle tileRect = new Rectangle(0, 0, 0, 0);

                Point TilePos = new Point(GhostPoint.X * levelMap.PixelSize.Width, GhostPoint.Y * levelMap.PixelSize.Height);

                Color col = Color.FromArgb(135, 255, 255, 255);

                TM.Draw(EraserId, TilePos.X + offset.X, TilePos.Y + offset.Y, 1.0f, 1.0f, tileRect, 0, 0, 0, col.ToArgb());
            }

            //D3D.DrawText(MousePoint.ToString (), 100, 200, 255, 0, 0);

            D3D.SpriteEnd();

            DrawBox(levelMap.MapSize, levelMap.EventList, levelMap.PixelSize, offset);

            D3D.LineEnd();
            D3D.DeviceEnd();

            D3D.Present(MainMapPanel);
        }

        private void DrawBox(Size LevelSize, CTile[,] List, Size PixelSize, Point PositionOffset)
        {
            int XBegin = ((PositionOffset.X * -1) / levelMap.PixelSize.Width);
            int YBegin = ((PositionOffset.Y * -1) / levelMap.PixelSize.Height);
            int XEnd = ((PositionOffset.X * -1) + MainMapPanel.Width) / levelMap.PixelSize.Width;
            int YEnd = ((PositionOffset.Y * -1) + MainMapPanel.Height) / levelMap.PixelSize.Height;

            if (XEnd >= levelMap.MapSize.Width)
                XEnd = levelMap.MapSize.Width;

            if (YEnd >= levelMap.MapSize.Height)
                YEnd = levelMap.MapSize.Height;

            for (int Ypos = 0; Ypos < 200; Ypos++)
            {
                for (int Xpos = 0; Xpos < 200; Xpos++)
                {
                    if (List[Ypos, Xpos].Type != (TileType)(-1))
                    {
                        if (List[Ypos, Xpos].Name == "CameraCollision")
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 0, 128, 0);
                        }
                        else if (List[Ypos, Xpos].Name == "WallCollision")
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 0, 255, 0);
                        }
                        else if (List[Ypos, Xpos].Name == "PlayerSpawn")
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 0, 0, 255);
                        }
                        else if (List[Ypos, Xpos].Name == "PowerUpSpawn")
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 255, 0, 0);
                        }
                        else if (List[Ypos, Xpos].Name == "SpeedRampSpawn")
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 255, 128, 0);
                        }
                        else if (List[Ypos, Xpos].Name == "ObstacleSpawn")
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 0, 255, 255);
                        }
                        else
                        {
                            levelMap.CollisionColor = Color.FromArgb(255, 255, 255, 0);
                        }
                        D3D.DrawLine(List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y,
                                    List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y * PixelSize.Height) + PixelSize.Height + PositionOffset.Y, levelMap.CollisionColor.R, levelMap.CollisionColor.G, levelMap.CollisionColor.B); // left side

                        D3D.DrawLine(List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y,
                                    (List[Ypos, Xpos].XYIndex.X * PixelSize.Width) + PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y, levelMap.CollisionColor.R, levelMap.CollisionColor.G, levelMap.CollisionColor.B); // top side

                        D3D.DrawLine((List[Ypos, Xpos].XYIndex.X + 1) * PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y,
                                    (List[Ypos, Xpos].XYIndex.X + 1) * PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y + 1) * PixelSize.Height + PositionOffset.Y, levelMap.CollisionColor.R, levelMap.CollisionColor.G, levelMap.CollisionColor.B); //right side

                        D3D.DrawLine(List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y + 1) * PixelSize.Height + PositionOffset.Y,
                                    ((List[Ypos, Xpos].XYIndex.X) * PixelSize.Width) + PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y + 1) * PixelSize.Height + PositionOffset.Y, levelMap.CollisionColor.R, levelMap.CollisionColor.G, levelMap.CollisionColor.B); // bottom side
                    }
                }
            }
        }

        private void MainMapPanel_MouseClick(object sender, MouseEventArgs e)
        {
            SaveBool = true;

            Point offset = e.Location;

            offset.X += ScrollOffset.X;
            offset.Y += ScrollOffset.Y;

            Point Pos = Point.Empty;
            Pos.X = (int)offset.X / levelMap.PixelSize.Width;
            Pos.Y = (int)offset.Y / levelMap.PixelSize.Height;

            if (Pos.X >= levelMap.MapSize.Width || Pos.Y >= levelMap.MapSize.Height || Pos.X < 0 || Pos.Y < 0)
            {
                return;
            }

            if (ToolSelection == Tools.TOOL_ERASER)
            {
                levelMap.TileList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                levelMap.TileList[Pos.Y, Pos.X].Type = (TileType)(-1);
            }

            if (ToolSelection == Tools.TOOL_EVENTERASER)
            {
                levelMap.EventList[Pos.Y, Pos.X].Type = (TileType)(-1);
                levelMap.EventList[Pos.Y, Pos.X].Name = "NULL";
            }

            int XDifference = levelMap.EndSelected.X - levelMap.TileSelected.X;
            int YDifference = levelMap.EndSelected.Y - levelMap.TileSelected.Y;

            for (int yPos = 0; yPos <= YDifference; yPos++)
            {
                for (int xPos = 0; xPos <= XDifference; xPos++)
                {
                    if (Pos.X + xPos >= levelMap.MapSize.Width || Pos.Y + yPos >= levelMap.MapSize.Height)
                    {
                        continue;
                    }

                    if (ToolSelection == Tools.TOOL_BRUSH)
                    {
                        levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                        levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].PickedTile = new Point (levelMap.TileSelected.X + xPos, levelMap.TileSelected.Y + yPos );
                        levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].Type = (TileType)((levelMap.TileSelected.Y + yPos) * levelMap.TileSize.Width + levelMap.TileSelected.X + xPos);
                    }
                }
            }

            if (ToolSelection == Tools.TOOL_EVENT)
            {
                levelMap.EventList[Pos.Y, Pos.X].XYIndex = new Point(Pos.X, Pos.Y);
                if (toolsWindow != null)
                {
                    levelMap.EventList[Pos.Y, Pos.X].Name = toolsWindow.EventIDName;
                    levelMap.EventList[Pos.Y, Pos.X].ID = toolsWindow.ID;
                }
                else
                {
                    levelMap.EventList[Pos.Y, Pos.X].Name = "NULL";
                    levelMap.EventList[Pos.Y, Pos.X].ID = "NULL";
                }

                levelMap.EventList[Pos.Y, Pos.X].Type = (TileType)(((levelMap.TileSize.Height) * levelMap.TileSize.Width + levelMap.TileSize.Width) + TileType.TYPE_EVENT);
            }

            if (XDifference == 0 && YDifference == 0)
            {
                if (ToolSelection == Tools.TOOL_FILL)
                {
                    FillArea(new Point(Pos.X, Pos.Y), levelMap.TileSelected);
                }
            }

            //if (toolsWindow != null)
            //{
            //    toolsWindow.LoadLists();
            //}
        }

        private void MainMapPanel_MouseMove(object sender, MouseEventArgs e)
        {
            Size position = Size.Empty;
            Point offset = e.Location;

            offset.X += ScrollOffset.X;
            offset.Y += ScrollOffset.Y;

            Point Pos = Point.Empty;
            Pos.X = (int)offset.X / levelMap.PixelSize.Width;
            Pos.Y = (int)offset.Y / levelMap.PixelSize.Height;

            
            if (ToolSelection == Tools.TOOL_BRUSH || ToolSelection == Tools.TOOL_ERASER || ToolSelection == Tools.TOOL_EVENTERASER)
            {
                GhostPoint = Pos;
            }

            if (Pos.X >= levelMap.MapSize.Width || Pos.Y >= levelMap.MapSize.Height || Pos.X < 0 || Pos.Y < 0)
            {
                return;
            }

            if (e.Button == MouseButtons.Left)
            {
                SaveBool = true;

                int XDifference = levelMap.EndSelected.X - levelMap.TileSelected.X;
                int YDifference = levelMap.EndSelected.Y - levelMap.TileSelected.Y;

                for (int yPos = 0; yPos <= YDifference; yPos++)
                {
                    for (int xPos = 0; xPos <= XDifference; xPos++)
                    {
                        if (Pos.X + xPos >= levelMap.MapSize.Width || Pos.Y + yPos >= levelMap.MapSize.Height)
                        {
                            continue;
                        }

                        if (ToolSelection == Tools.TOOL_BRUSH)
                        {
                            levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                            levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].PickedTile = new Point(levelMap.TileSelected.X + xPos, levelMap.TileSelected.Y + yPos);
                            levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].Type = (TileType)((levelMap.TileSelected.Y + yPos) * levelMap.TileSize.Width + levelMap.TileSelected.X + xPos);
                        }
                    }
                }

                if (ToolSelection == Tools.TOOL_EVENT)
                {
                    levelMap.EventList[Pos.Y, Pos.X].XYIndex = new Point(Pos.X, Pos.Y);
                    if (toolsWindow != null)
                    {
                        levelMap.EventList[Pos.Y, Pos.X].Name = toolsWindow.EventIDName;
                        levelMap.EventList[Pos.Y, Pos.X].ID = toolsWindow.ID;
                    }
                    else
                    {
                        levelMap.EventList[Pos.Y, Pos.X].Name = "NULL";
                        levelMap.EventList[Pos.Y, Pos.X].ID = "NULL";
                    }
                    levelMap.EventList[Pos.Y, Pos.X].Type = (TileType)(((levelMap.TileSize.Height) * levelMap.TileSize.Width + levelMap.TileSize.Width) + TileType.TYPE_EVENT);
                }

                if (ToolSelection == Tools.TOOL_ERASER)
                {
                    levelMap.TileList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                    levelMap.TileList[Pos.Y, Pos.X].Type = (TileType)(-1);
                }

                if (ToolSelection == Tools.TOOL_EVENTERASER)
                {
                    levelMap.EventList[Pos.Y, Pos.X].Type = (TileType)(-1);
                    levelMap.EventList[Pos.Y, Pos.X].Name = "NULL";
                }

                //if (toolsWindow != null)
                //{
                //    toolsWindow.LoadLists();
                //}
            }
        }

        private void FillArea(Point ClickedPosition, Point SelectedTile)
        {
            List<CTile> fillList = new List<CTile>();
            TileType controlTileType = levelMap.TileList[ClickedPosition.Y, ClickedPosition.X].Type;

            if (controlTileType == (TileType)((SelectedTile.Y) * levelMap.TileSize.Width + SelectedTile.X))
                return;

            CTile first = new CTile();
            first.XYIndex = new Point(ClickedPosition.X, ClickedPosition.Y);
            first.PickedTile = SelectedTile;
            first.Type = (TileType)((SelectedTile.Y) * levelMap.TileSize.Width + SelectedTile.X);

            fillList.Add(first);

            SaveBool = true;

            while (fillList.Count != 0)
            {
                CTile tile = fillList[0];

                levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X] = tile;

                fillList.Remove(tile);

                fillList = CheckSurroundingTiles(fillList, tile, controlTileType);
            }
        }

        public bool CheckList(List<CTile> list, CTile tile)
        {
            for (int i = 0; i < list.Count; i++)
            {
                if (list[i].XYIndex.X == tile.XYIndex.X && list[i].XYIndex.Y == tile.XYIndex.Y)
                {
                    return true;
                }
            }

            return false;
        }

        private List<CTile> CheckSurroundingTiles(List<CTile> list, CTile tile, TileType control)
        {
            if (tile.XYIndex.X + 1 < levelMap.MapSize.Width)
            {
                if (levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X + 1].Type == control)
                {
                    CTile temp = new CTile();
                    temp.XYIndex = new Point(tile.XYIndex.X + 1, tile.XYIndex.Y);
                    temp.PickedTile = levelMap.TileSelected;
                    temp.Type = (TileType)((levelMap.TileSelected.Y) * levelMap.TileSize.Width + levelMap.TileSelected.X);

                    if (CheckList(list, temp) == false)
                    {
                        list.Add(temp);
                    }
                }
            }

            if (tile.XYIndex.X - 1 > -1)
            {
                if (levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X - 1].Type == control)
                {
                    CTile temp = new CTile();
                    temp.XYIndex = new Point(tile.XYIndex.X - 1, tile.XYIndex.Y);
                    temp.PickedTile = levelMap.TileSelected;
                    temp.Type = (TileType)((levelMap.TileSelected.Y) * levelMap.TileSize.Width + levelMap.TileSelected.X);

                    if (CheckList(list, temp) == false)
                    {
                        list.Add(temp);
                    }
                }
            }

            if (tile.XYIndex.Y + 1 < levelMap.MapSize.Height)
            {
                if (levelMap.TileList[tile.XYIndex.Y + 1, tile.XYIndex.X].Type == control)
                {
                    CTile temp = new CTile();
                    temp.XYIndex = new Point(tile.XYIndex.X, tile.XYIndex.Y + 1);
                    temp.PickedTile = levelMap.TileSelected;
                    temp.Type = (TileType)((levelMap.TileSelected.Y) * levelMap.TileSize.Width + levelMap.TileSelected.X);

                    if (CheckList(list, temp) == false)
                    {
                        list.Add(temp);
                    }
                }
            }

            if (tile.XYIndex.Y - 1 > -1)
            {
                if (levelMap.TileList[tile.XYIndex.Y - 1, tile.XYIndex.X].Type == control)
                {
                    CTile temp = new CTile();
                    temp.XYIndex = new Point(tile.XYIndex.X, tile.XYIndex.Y - 1);
                    temp.PickedTile = levelMap.TileSelected;
                    temp.Type = (TileType)((levelMap.TileSelected.Y) * levelMap.TileSize.Width + levelMap.TileSelected.X);

                    if (CheckList(list, temp) == false)
                    {
                        list.Add(temp);
                    }
                }
            }

            return list;
        }

        private void MapRest()
        {
            if (levelMap.MapSize.Height > 0 && levelMap.MapSize.Width > 0)
            {
                CTile[,] OldTileList = levelMap.TileList;
                CTile[,] OldEventList = levelMap.EventList;

                levelMap.TileList = new CTile[levelMap.MapSize.Height, levelMap.MapSize.Width];
                levelMap.EventList = new CTile[levelMap.MapSize.Height, levelMap.MapSize.Width];


                for (int Ypos = 0; Ypos < levelMap.MapSize.Height; Ypos++)
                {
                    for (int Xpos = 0; Xpos < levelMap.MapSize.Width; Xpos++)
                    {
                        CTile tile = new CTile();

                        tile.Type = (TileType)(-1);
                        tile.XYIndex = new Point(Xpos, Ypos);
                        levelMap.TileList[Ypos, Xpos] = tile;

                        tile = new CTile();

                        tile.Type = (TileType)(-1);
                        tile.XYIndex = new Point(Xpos, Ypos);
                        tile.Name = "NULL";
                        tile.ID = "NULL";
                        levelMap.EventList[Ypos, Xpos] = tile;
                    }
                }

                if (levelMap.MapSize.Width >= OldMapSize.Width && levelMap.MapSize.Height >= OldMapSize.Height)
                {
                    for (int Ypos = 0; Ypos < OldMapSize.Height; Ypos++)
                    {
                        for (int Xpos = 0; Xpos < OldMapSize.Width; Xpos++)
                        {
                            levelMap.TileList[Ypos, Xpos] = OldTileList[Ypos, Xpos];
                            levelMap.EventList[Ypos, Xpos] = OldEventList[Ypos, Xpos];
                        }
                    }
                }
                else if (levelMap.MapSize.Width <= OldMapSize.Width && levelMap.MapSize.Height <= OldMapSize.Height)
                {
                    for (int Ypos = 0; Ypos < levelMap.MapSize.Height; Ypos++)
                    {
                        for (int Xpos = 0; Xpos < levelMap.MapSize.Width; Xpos++)
                        {
                            levelMap.TileList[Ypos, Xpos] = OldTileList[Ypos, Xpos];
                            levelMap.EventList[Ypos, Xpos] = OldEventList[Ypos, Xpos];
                        }
                    }
                }

                if (toolsWindow != null)
                {
                    toolsWindow.EventList = levelMap.EventList;
                }
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //ask to save;
            if (SaveBool)
            {
                SaveBool = false;
                var result = MessageBox.Show("Save Changes?", "Are you sure??", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
                if (result == DialogResult.Yes)
                {
                    saveToolStripMenuItem_Click(sender, e);
                }
                else if (result == DialogResult.No)
                {

                    // clear lists;
                    if (result != DialogResult.Cancel)
                    {
                        for (int YPos = 0; YPos < levelMap.MapSize.Height; YPos++)
                        {
                            for (int XPos = 0; XPos < levelMap.MapSize.Width; XPos++)
                            {
                                levelMap.TileList[YPos, XPos] = new CTile();
                                levelMap.EventList[YPos, XPos] = new CTile();

                                levelMap.TileList[YPos, XPos].Type = (TileType)(-1);
                                levelMap.EventList[YPos, XPos].Type = (TileType)(-1);

                                levelMap.TileList[YPos, XPos].XYIndex = new Point(XPos, YPos);
                                levelMap.EventList[YPos, XPos].XYIndex = new Point(XPos, YPos);
                            }
                        }
                    }
                }
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.InitialDirectory = Environment.CurrentDirectory;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                SaveBool = false;

                XElement xLevel = XElement.Load(dlg.FileName);

                string[] Splitpath;
                string path = "";

                Splitpath = dlg.FileName.Split('\\');

                for (int index = 0; index < Splitpath.Length - 1; index++)
                {
                    path += Splitpath[index];
                    path += "/";
                }

                XElement xLevelInfo = xLevel.Element("LevelInfo");

                levelMap.MapName = xLevelInfo.Value;

                levelMap.MapSize = new Size (int.Parse (xLevelInfo.Attribute("Width").Value), int.Parse (xLevelInfo.Attribute ("Height").Value));

                levelMap.PixelSize = new Size(int.Parse(xLevelInfo.Attribute("PixelWidth").Value), int.Parse(xLevelInfo.Attribute("PixelHeight").Value));

                MainMapPanel.AutoScrollMinSize = new Size(levelMap.MapSize.Width * levelMap.PixelSize.Width, levelMap.MapSize.Height * levelMap.PixelSize.Height);

                MapRest();

                XElement XTileLocation = xLevel.Element("TileLocation");
                LoadTileInfo(path, (string)XTileLocation.Value);

                XElement XEventsLocation = xLevel.Element("EventLocation");
                LoadEventInfo(path, (string)XEventsLocation.Value);

                if (toolsWindow != null)
                {
                    toolsWindow.MapSize = levelMap.MapSize;
                    toolsWindow.PixelSize = levelMap.PixelSize;
                    toolsWindow.TileSize = levelMap.TileSize;
                    toolsWindow.EventList = levelMap.EventList;
                }
            }
        }

        private void LoadTileInfo(string path, string filename)
        {
            string Filepath = path + filename;

            XElement xTilesInfo = XElement.Load(Filepath); // root

            levelMap.TileSize = new Size(int.Parse(xTilesInfo.Attribute("Width").Value), int.Parse(xTilesInfo.Attribute("Height").Value));

            MessageBox.Show("Could not find tile set please find the tile set or find a new tile set.", "Load Fail.", MessageBoxButtons.OK);
            loadTileSet();
            if (tileWindow != null)
            {
                tileWindow.Render();
            }

            XElement xTiles = xTilesInfo.Element("Tiles");

            IEnumerable<XElement> xTile = xTiles.Elements();

            foreach (XElement t in xTile)
            {
                CTile tile = new CTile();

                XAttribute xXpos = t.Attribute("XPos");
                if (xXpos != null)
                {
                    XAttribute xYpos = t.Attribute("YPos");
                    if (xYpos != null)
                    {
                        tile.XYIndex = new Point(int.Parse(xXpos.Value), int.Parse(xYpos.Value));

                        XAttribute xPickedX = t.Attribute("PickedX");
                        if (xPickedX != null)
                        {
                            XAttribute xPickedY = t.Attribute("PickedY");
                            if (xPickedY != null)
                            {
                                tile.PickedTile = new Point(int.Parse(xPickedX.Value), int.Parse(xPickedY.Value));

                                 XAttribute xType = t.Attribute("Type");
                                 if (xType != null)
                                 {
                                     tile.Type = (TileType)(int.Parse(xType.Value));
                                     levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X] = tile;
                                 }
                            }
                        }
                    }
                }
            }
        }

        private void LoadEventInfo(string path, string filename)
        {
            string Filepath = path + filename;

            XElement xEvents = XElement.Load(Filepath); // root

            IEnumerable<XElement> xEvent = xEvents.Elements();

            foreach (XElement e in xEvent)
            {
                CTile tile = new CTile();

                tile.Name = e.Value;

                XAttribute xXpos = e.Attribute("XPos");
                if (xXpos != null)
                {
                    XAttribute xYpos = e.Attribute("YPos");
                    if (xYpos != null)
                    {
                        tile.XYIndex = new Point(int.Parse(xXpos.Value), int.Parse(xYpos.Value));

                        XAttribute xType = e.Attribute("Type");
                         if (xType != null)
                         {
                             tile.Type = (TileType)(int.Parse(xType.Value));
                             XAttribute xID = e.Attribute("ID");
                             if (xID != null)
                                 tile.ID = xID.Value;
                             else
                                 tile.ID = "NULL";

                             levelMap.EventList[tile.XYIndex.Y, tile.XYIndex.X] = tile;
                         }
                    }
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.FileName = levelMap.MapName;
            dlg.DefaultExt = "xml";
            dlg.InitialDirectory = Environment.CurrentDirectory;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                SaveBool = false;

                //save each list first

                string[] Splitpath;
                string path = "";

                Splitpath = dlg.FileName.Split('\\');

                for (int index = 0; index < Splitpath.Length - 1; index++)
                {
                    path += Splitpath[index];
                    path += "/";
                }


                XElement xLevel = new XElement("Level"); // root

                XElement xTileLocation = new XElement("TileLocation", "TileInfo.xml");
                SaveTileInfo(path, "TileInfo.xml");

                XElement xEventLocation = new XElement("EventLocation", "EventInfo.xml");
                SaveEventInfo(path, "EventInfo.xml");

                xLevel.Add(xTileLocation);
                xLevel.Add(xEventLocation);

                XElement xLevelInfo = new XElement("LevelInfo", levelMap.MapName);
                xLevel.Add(xLevelInfo);

                XAttribute xMapWidth = new XAttribute("Width", levelMap.MapSize.Width);
                XAttribute xMapHeight = new XAttribute("Height", levelMap.MapSize.Height);
                XAttribute xPixelWidth = new XAttribute("PixelWidth", levelMap.PixelSize.Width);
                XAttribute xPixelHeight = new XAttribute("PixelHeight", levelMap.PixelSize.Height);

                xLevelInfo.Add(xMapWidth);
                xLevelInfo.Add(xMapHeight);
                xLevelInfo.Add(xPixelWidth);
                xLevelInfo.Add(xPixelHeight);

                xLevel.Save(dlg.FileName);
            }
        }

        private void SaveTileInfo(string path, string filename)
        {
            path += filename;

            XElement xTilesInfo = new XElement("TilesInfo"); // root

            XAttribute xWidth = new XAttribute("Width", levelMap.TileSize.Width);
            XAttribute xHeight = new XAttribute("Height", levelMap.TileSize.Height);

            xTilesInfo.Add(xWidth);
            xTilesInfo.Add(xHeight);

            XElement xBitmapName = new XElement("BitmapName", TileBitmapFile);

            xTilesInfo.Add(xBitmapName);

            XElement xTiles = new XElement("Tiles");
            xTilesInfo.Add(xTiles);

            foreach (CTile t in levelMap.TileList)
            {
                XElement xTile = new XElement("Tile");
                xTiles.Add(xTile);

                XAttribute xIndexX = new XAttribute("XPos", t.XYIndex.X);
                xTile.Add(xIndexX);

                XAttribute xIndexY = new XAttribute("YPos", t.XYIndex.Y);
                xTile.Add(xIndexY);

                XAttribute xPickedX = new XAttribute("PickedX", t.PickedTile.X);
                xTile.Add(xPickedX);

                XAttribute xPickedY = new XAttribute("PickedY", t.PickedTile.Y);
                xTile.Add(xPickedY);

                
                XAttribute xType = new XAttribute("Type", (int)t.Type);
                xTile.Add(xType);
            }

            xTilesInfo.Save(path);
        }

        private void SaveEventInfo(string path, string filename)
        {
            path += filename;

                XElement xEvents = new XElement("Events"); // root

                foreach (CTile e in levelMap.EventList)
                {
                    if (e.Type != (TileType)(-1))
                    {
                        XElement xTile = new XElement("Event", e.Name);
                        xEvents.Add(xTile);

                        XAttribute xID = new XAttribute("ID", e.ID);
                        xTile.Add(xID);

                        XAttribute xIndexX = new XAttribute("XPos", e.XYIndex.X);
                        xTile.Add(xIndexX);

                        XAttribute xIndexY = new XAttribute("YPos", e.XYIndex.Y);
                        xTile.Add(xIndexY);

                        XAttribute xType = new XAttribute("Type", (int)e.Type);
                        xTile.Add(xType);
                    }
                }

                xEvents.Save(path);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Looping = false;
            Close();
        }

        private void MapEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (SaveBool)
            {
                SaveBool = false;
                var result = MessageBox.Show("Save Changes?", "Are you sure??", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1);
                if (result == DialogResult.Yes)
                {
                    saveToolStripMenuItem_Click(sender, e);
                }else if (result == DialogResult.No)
                {
                    Looping = false;
                }
            }
            else
            {
                Looping = false;
            }
        }

        private void loadTileSetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            loadTileSet();
        }

        private void loadTileSet()
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files|*.*|PNG Files|*.png";
            dlg.FilterIndex = 2;
            dlg.InitialDirectory = Environment.CurrentDirectory;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                string[] Splitpath;
                Splitpath = dlg.FileName.Split('\\');

                TileBitmapFile = Splitpath[Splitpath.Length - 1];

                levelMap.TileBitmap = TM.LoadTexture(dlg.FileName, 0);

                int textureHeight = TM.GetTextureHeight(levelMap.TileBitmap);
                int textureWidth = TM.GetTextureWidth(levelMap.TileBitmap);

                Size tilesize = new Size();
                tilesize.Width = textureWidth / levelMap.PixelSize.Width;
                tilesize.Height = textureHeight / levelMap.PixelSize.Height;

                levelMap.TileSize = tilesize;

                if (tileWindow != null)
                {
                    tileWindow.TileSize = levelMap.TileSize;
                    tileWindow.TileBitmap = levelMap.TileBitmap;
                    tileWindow.SetAutorScroll();
                }

                if (toolsWindow != null)
                {
                    toolsWindow.ImageSize = new Size(textureWidth, textureHeight);
                    toolsWindow.TileSize = levelMap.TileSize;
                }
            }
        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (help == null)
            {
                help = new HelpWindow();

                help.Show();
            }
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            ScrollOffset.X = e.NewValue;
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            ScrollOffset.Y = e.NewValue;
        }
    }
}