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
    public enum Tools { TOOL_SELECTION, TOOL_ERASER, TOOL_GRID, TOOL_BRUSH, TOOL_FILL, TOOL_EVENT, TOOL_COLLISION, TOOL_SPAWN, TOOL_MAX }
    public enum TileType { TYPE_TILE, TYPE_PLAYER, TYPE_POWER, TYPE_SPEED, TYPE_COLLISION, TYPE_EVENT, TYPE_MAX };

    public partial class MapEditor : Form
    {
        ManagedDirect3D D3D = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;

        public bool Looping = true;
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
        TileType ToolSpawnType = TileType.TYPE_PLAYER;

        Point MousePoint;

        Point ScrollOffset = Point.Empty;

        public MapEditor()
        {
            InitializeComponent();

            D3D.InitManagedDirect3D(MainMapPanel, false);
            TM.InitManagedTextureManager(D3D.Device, D3D.Sprite);

            MainPath = Environment.CurrentDirectory;

            string path = MainPath;
            path += "/Resource/Images/Simple Tile Set.png";
            //path += "Resource/Images/tilesetpok.bmp";

            BitMapPath = MainPath + "/Resource/Images/";
            TileBitmapFile = "tilesetpok.bmp";

            levelMap.TileBitmap = TM.LoadTexture(path, 0);

            path = MainPath;
            path += "/Resource/Images/FT_CursorEraser.png";

            EraserId = TM.LoadTexture(path, 0);


            levelMap.TileList = new CTile[5, 15];
            levelMap.SpawnList = new CTile[5, 15];
            levelMap.CollisionList = new CTile[5, 15];
            levelMap.EventList = new CTile[5, 15];

            Size LevelSize = new Size(15, 5);
            levelMap.MapSize = LevelSize;
            OldMapSize = LevelSize;

            levelMap.SpawnList = new CTile[LevelSize.Height, LevelSize.Width];
            levelMap.TileList = new CTile[LevelSize.Height, LevelSize.Width];
            levelMap.EventList = new CTile[LevelSize.Height, LevelSize.Width];
            levelMap.CollisionList = new CTile[LevelSize.Height, LevelSize.Width];

            levelMap.TileSelected = new Point(0, 0);
            levelMap.EndSelected = new Point(0, 0);
            levelMap.TileSize = new Size(4, 4);

            levelMap.PixelSize = new Size(32, 32);

            //MainMapPanel.AutoScrollMinSize = new Size(levelMap.MapSize.Width * levelMap.PixelSize.Width, levelMap.MapSize.Height * levelMap.PixelSize.Height);

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
                    levelMap.CollisionList[Ypos, Xpos] = new CTile();
                    levelMap.EventList[Ypos, Xpos] = new CTile();
                    levelMap.SpawnList[Ypos, Xpos] = new CTile();

                    levelMap.TileList[Ypos, Xpos].Type = (TileType)(-1);
                    levelMap.CollisionList[Ypos, Xpos].Type = (TileType)(-1);
                    levelMap.EventList[Ypos, Xpos].Type = (TileType)(-1);
                    levelMap.SpawnList[Ypos, Xpos].Type = (TileType)(-1);

                    levelMap.TileList[Ypos, Xpos].XYIndex = new Point(-1, -1);
                    levelMap.CollisionList[Ypos, Xpos].XYIndex = new Point(-1, -1);
                    levelMap.EventList[Ypos, Xpos].XYIndex = new Point(-1, -1);
                    levelMap.SpawnList[Ypos, Xpos].XYIndex = new Point(-1, -1);
                }
            }

            CreateToolWindow();
            CreateTileWindow();
            if (help == null)
            {
                help = new HelpWindow();

                help.TopLevel = true;
                help.TopMost = true;
                help.Show();
            }
        }

        private void CreateToolWindow()
        {
            if (toolsWindow == null)
            {
                toolsWindow = new ToolsWindow();
                toolsWindow.ToolClicked += new EventHandler(Tool_Clicked);
                toolsWindow.FormClosed += new FormClosedEventHandler(Tool_FormClosed);
                toolsWindow.CollisionListChange += new EventHandler(CollisionListChange);
                toolsWindow.EventListChange += new EventHandler(EventListChange);
                toolsWindow.SpawnListChange += new EventHandler(SpawnListChange);
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
                toolsWindow.CollisionList = levelMap.CollisionList;
                toolsWindow.EventList = levelMap.EventList;
                toolsWindow.SpawnList = levelMap.SpawnList;

                toolsWindow.SpawnType = ToolSpawnType;
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

            //MainMapPanel.AutoScrollMinSize = new Size(levelMap.MapSize.Width * levelMap.PixelSize.Width, levelMap.MapSize.Height * levelMap.PixelSize.Height);
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

            //MainMapPanel.AutoScrollMinSize = new Size(levelMap.MapSize.Width * levelMap.PixelSize.Width, levelMap.MapSize.Height * levelMap.PixelSize.Height);
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

            //MainMapPanel.AutoScrollMinSize = new Size(levelMap.MapSize.Width * levelMap.PixelSize.Width, levelMap.MapSize.Height * levelMap.PixelSize.Height);
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

           // MainMapPanel.AutoScrollMinSize = new Size(levelMap.MapSize.Width * levelMap.PixelSize.Width, levelMap.MapSize.Height * levelMap.PixelSize.Height);
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

        void SpawnListChange(object sender, EventArgs e)
        {
            levelMap.SpawnList = ((ToolsWindow)sender).SpawnList;
        }

        void EventListChange(object sender, EventArgs e)
        {
            levelMap.EventList = ((ToolsWindow)sender).EventList;           
        }

        void CollisionListChange(object sender, EventArgs e)
        {
            levelMap.CollisionList = ((ToolsWindow)sender).CollisionList;
        }

        private void CreateTileWindow()
        {
            if (tileWindow == null)
            {
                tileWindow = new TileWindow();
                tileWindow.FormClosed += new FormClosedEventHandler(Tile_FormClosed);
                tileWindow.TileClicked += new EventHandler(Tile_Clicked);

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
            
            if (gridShow)
            {
                for (int Xindex = 0; Xindex <= levelMap.MapSize.Width; Xindex++)
                {
                    D3D.DrawLine(Xindex * levelMap.PixelSize.Width + offset.X, offset.Y, Xindex * levelMap.PixelSize.Width + offset.X, levelMap.MapSize.Height * levelMap.PixelSize.Height + offset.Y, 255, 0, 0);
                }
                for (int Yindex = 0; Yindex <= levelMap.MapSize.Height; Yindex++)
                {
                    D3D.DrawLine(offset.X, Yindex * levelMap.PixelSize.Height + offset.Y, levelMap.MapSize.Width * levelMap.PixelSize.Width + offset.X, Yindex * levelMap.PixelSize.Height + offset.Y, 255, 0, 0);
                }
            }

            D3D.SpriteBegin();

            for (int Ypos = 0; Ypos < levelMap.MapSize.Height; Ypos++)
            {
                for (int Xpos = 0; Xpos < levelMap.MapSize.Width; Xpos++)
                {
                    if (levelMap.TileList[Ypos, Xpos].Type != (TileType)(-1))
                    {
                        Rectangle tileRect = new Rectangle();
                        tileRect.Location = new Point(levelMap.TileList[Ypos, Xpos].PickedTile.X * levelMap.PixelSize.Width,
                                                      levelMap.TileList[Ypos, Xpos].PickedTile.Y * levelMap.PixelSize.Height);

                        tileRect.Size = new Size(levelMap.PixelSize.Width, levelMap.PixelSize.Height);

                        //Size size = new Size();
                        //if (levelMap.EndSelected.X == levelMap.TileSelected.X)
                        //{
                        //    size.Width = levelMap.PixelSize.Width;
                        //}
                        //else
                        //{
                        //    int difference = levelMap.EndSelected.X - levelMap.TileSelected.X;
                        //    size.Width = difference * levelMap.PixelSize.Width + levelMap.PixelSize.Width;
                        //}

                        //if (levelMap.EndSelected.Y == levelMap.TileSelected.Y)
                        //{
                        //    size.Height = levelMap.PixelSize.Height;
                        //}
                        //else
                        //{
                        //    int difference = levelMap.EndSelected.Y - levelMap.TileSelected.Y;
                        //    size.Height = difference * levelMap.PixelSize.Height + levelMap.PixelSize.Height;
                        //}

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

            if (ToolSelection == Tools.TOOL_ERASER)
            {
                Rectangle tileRect = new Rectangle(0, 0, 0, 0);

                Point TilePos = new Point(GhostPoint.X * levelMap.PixelSize.Width, GhostPoint.Y * levelMap.PixelSize.Height);

                Color col = Color.FromArgb(170, 255, 255, 255);

                TM.Draw(EraserId, TilePos.X + offset.X, TilePos.Y + offset.Y, 1.0f, 1.0f, tileRect, 0, 0, 0, col.ToArgb());
            }


            D3D.DrawText(MousePoint.ToString (), 100, 200, 255, 0, 0);

            D3D.SpriteEnd();

            DrawBox(levelMap.MapSize, levelMap.CollisionList, levelMap.PixelSize, offset, "C", new Point(3, 2), levelMap.CollisionColor);
            DrawBox(levelMap.MapSize, levelMap.SpawnList, levelMap.PixelSize, offset, "S", new Point(6, 2), levelMap.SpawnColor);
            DrawBox(levelMap.MapSize, levelMap.EventList, levelMap.PixelSize, offset, "E", new Point(3, 6), levelMap.EventColor);

            D3D.LineEnd();
            D3D.DeviceEnd();

            D3D.Present(MainMapPanel);
        }

        private void DrawBox(Size LevelSize, CTile[,] List, Size PixelSize, Point PositionOffset, string Text, Point TextLocation, Color color)
        {
            for (int Ypos = 0; Ypos < LevelSize.Height; Ypos++)
            {
                for (int Xpos = 0; Xpos < LevelSize.Width; Xpos++)
                {
                    if (List[Ypos, Xpos].XYIndex.X != -1 || List[Ypos, Xpos].XYIndex.Y != -1)
                    {
                        D3D.DrawLine(List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y,
                                    List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y * PixelSize.Height) + PixelSize.Height + PositionOffset.Y, color.R, color.G, color.B); // left side

                        D3D.DrawLine(List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y,
                                    (List[Ypos, Xpos].XYIndex.X * PixelSize.Width) + PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y, color.R, color.G, color.B); // top side

                        D3D.DrawLine((List[Ypos, Xpos].XYIndex.X + 1) * PixelSize.Width + PositionOffset.X, List[Ypos, Xpos].XYIndex.Y * PixelSize.Height + PositionOffset.Y,
                                    (List[Ypos, Xpos].XYIndex.X + 1) * PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y + 1) * PixelSize.Height + PositionOffset.Y, color.R, color.G, color.B); //right side

                        D3D.DrawLine(List[Ypos, Xpos].XYIndex.X * PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y + 1) * PixelSize.Height + PositionOffset.Y,
                                    ((List[Ypos, Xpos].XYIndex.X) * PixelSize.Width) + PixelSize.Width + PositionOffset.X, (List[Ypos, Xpos].XYIndex.Y + 1) * PixelSize.Height + PositionOffset.Y, color.R, color.G, color.B); // bottom side
                    }
                }
            }
        }

        private void MainMapPanel_MouseClick(object sender, MouseEventArgs e)
        {
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
                levelMap.TileList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);

                levelMap.SpawnList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                levelMap.SpawnList[Pos.Y, Pos.X].Type = (TileType)(-1);
                levelMap.SpawnList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);

                levelMap.CollisionList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                levelMap.CollisionList[Pos.Y, Pos.X].Type = (TileType)(-1);
                levelMap.CollisionList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);

                levelMap.EventList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                levelMap.EventList[Pos.Y, Pos.X].Type = (TileType)(-1);
                levelMap.EventList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);
            }

            int XDifference = levelMap.EndSelected.X - levelMap.TileSelected.X;
            int YDifference = levelMap.EndSelected.Y - levelMap.TileSelected.Y;

            for (int yPos = 0; yPos <= YDifference; yPos++)
            {
                for (int xPos = 0; xPos <= XDifference; xPos++)
                {
                    if (Pos.X + xPos >= levelMap.MapSize.Width || Pos.Y + yPos >= levelMap.MapSize.Height)
                    {
                        return;
                    }

                    if (ToolSelection == Tools.TOOL_BRUSH)
                    {
                        levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                        levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].PickedTile = new Point (levelMap.TileSelected.X + xPos, levelMap.TileSelected.Y + yPos );
                        levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].Type = TileType.TYPE_TILE;
                    }

                    if (ToolSelection == Tools.TOOL_SPAWN)
                    {
                        levelMap.SpawnList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                        levelMap.SpawnList[Pos.Y + yPos, Pos.X + xPos].Name = "NoName";
                        levelMap.SpawnList[Pos.Y + yPos, Pos.X + xPos].Type = ToolSpawnType;
                    }

                    if (ToolSelection == Tools.TOOL_COLLISION)
                    {
                        levelMap.CollisionList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                        levelMap.CollisionList[Pos.Y + yPos, Pos.X + xPos].Name = "NoName";
                        levelMap.CollisionList[Pos.Y + yPos, Pos.X + xPos].Type = TileType.TYPE_COLLISION;
                    }

                    if (ToolSelection == Tools.TOOL_EVENT)
                    {
                        levelMap.EventList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                        levelMap.EventList[Pos.Y + yPos, Pos.X + xPos].Name = "NoName";
                        levelMap.EventList[Pos.Y + yPos, Pos.X + xPos].Type = TileType.TYPE_EVENT;
                    }
                }
            }

            if (XDifference == 0 && YDifference == 0)
            {
                if (ToolSelection == Tools.TOOL_FILL)
                {
                    FillArea(new Point(Pos.X, Pos.Y), levelMap.TileSelected);
                }
            }

            if (toolsWindow != null)
            {
                toolsWindow.LoadLists();
            }
        }

        private void FillArea(Point ClickedPosition, Point SelectedTile)
        {
            List<CTile> fillList = new List<CTile>();

            CTile first = new CTile();
            first.XYIndex = new Point(ClickedPosition.X, ClickedPosition.Y);
            first.PickedTile = SelectedTile;
            first.Type = TileType.TYPE_TILE;

            fillList.Add(first);

            while (fillList.Count != 0)
            {
                CTile tile = fillList[0];

                levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X] = tile;

                fillList.Remove(tile);

                fillList = CheckFourSurroundingTiles(fillList, tile);
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

        private List<CTile> CheckFourSurroundingTiles(List<CTile> list, CTile tile)
        {
                if (tile.XYIndex.X + 1 < levelMap.MapSize.Width)
                {
                    if (levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X + 1].Type == (TileType)(-1))
                    {
                        CTile temp = new CTile();
                        temp.XYIndex = new Point(tile.XYIndex.X + 1, tile.XYIndex.Y);
                        temp.PickedTile = levelMap.TileSelected;
                        temp.Type = TileType.TYPE_TILE;

                        if (CheckList(list, temp) == false)
                        {
                            list.Add(temp);
                        }
                    }
                }

                if (tile.XYIndex.X - 1 > -1)
                {
                    if (levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X - 1].Type == (TileType)(-1))
                    {
                        CTile temp = new CTile();
                        temp.XYIndex = new Point(tile.XYIndex.X - 1, tile.XYIndex.Y);
                        temp.PickedTile = levelMap.TileSelected;
                        temp.Type = TileType.TYPE_TILE;

                        if (CheckList(list, temp) == false)
                        {
                            list.Add(temp);
                        }
                    }
                }

                if (tile.XYIndex.Y + 1 < levelMap.MapSize.Height)
                {
                    if (levelMap.TileList[tile.XYIndex.Y + 1, tile.XYIndex.X].Type == (TileType)(-1))
                    {
                        CTile temp = new CTile();
                        temp.XYIndex = new Point(tile.XYIndex.X, tile.XYIndex.Y + 1);
                        temp.PickedTile = levelMap.TileSelected;
                        temp.Type = TileType.TYPE_TILE;

                        if (CheckList(list, temp) == false)
                        {
                            list.Add(temp);
                        }
                    }
                }

                if (tile.XYIndex.Y - 1 > -1)
                {
                    if (levelMap.TileList[tile.XYIndex.Y - 1, tile.XYIndex.X].Type == (TileType)(-1))
                    {
                        CTile temp = new CTile();
                        temp.XYIndex = new Point(tile.XYIndex.X, tile.XYIndex.Y - 1);
                        temp.PickedTile = levelMap.TileSelected;
                        temp.Type = TileType.TYPE_TILE;

                        if (CheckList(list, temp) == false)
                        {
                            list.Add(temp);
                        }
                    }
                }

            return list;
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

            

            // Creates a ghost for the users to see which tile they are using or which tile they are erasing
            if (ToolSelection == Tools.TOOL_BRUSH || ToolSelection == Tools.TOOL_ERASER)
            {
                GhostPoint = Pos;
            }

            if (Pos.X >= levelMap.MapSize.Width || Pos.Y >= levelMap.MapSize.Height || Pos.X < 0 || Pos.Y < 0)
            {
                return;
            }

            //OnMouseDown(e);

            if (e.Button == MouseButtons.Left)
            {
                int XDifference = levelMap.EndSelected.X - levelMap.TileSelected.X;
                int YDifference = levelMap.EndSelected.Y - levelMap.TileSelected.Y;

                for (int yPos = 0; yPos <= YDifference; yPos++)
                {
                    for (int xPos = 0; xPos <= XDifference; xPos++)
                    {
                        if (Pos.X + xPos >= levelMap.MapSize.Width || Pos.Y + yPos >= levelMap.MapSize.Height)
                        {
                            return;
                        }

                        if (ToolSelection == Tools.TOOL_BRUSH)
                        {
                            levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                            levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].PickedTile = new Point(levelMap.TileSelected.X + xPos, levelMap.TileSelected.Y + yPos);
                            levelMap.TileList[Pos.Y + yPos, Pos.X + xPos].Type = TileType.TYPE_TILE;
                        }

                        if (ToolSelection == Tools.TOOL_SPAWN)
                        {
                            levelMap.SpawnList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                            levelMap.SpawnList[Pos.Y + yPos, Pos.X + xPos].Name = "NoName";
                            levelMap.SpawnList[Pos.Y + yPos, Pos.X + xPos].Type = ToolSpawnType;
                        }

                        if (ToolSelection == Tools.TOOL_COLLISION)
                        {
                            levelMap.CollisionList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                            levelMap.CollisionList[Pos.Y + yPos, Pos.X + xPos].Name = "NoName";
                            levelMap.CollisionList[Pos.Y + yPos, Pos.X + xPos].Type = TileType.TYPE_COLLISION;
                        }

                        if (ToolSelection == Tools.TOOL_EVENT)
                        {
                            levelMap.EventList[Pos.Y + yPos, Pos.X + xPos].XYIndex = new Point(Pos.X + xPos, Pos.Y + yPos);
                            levelMap.EventList[Pos.Y + yPos, Pos.X + xPos].Name = "NoName";
                            levelMap.EventList[Pos.Y + yPos, Pos.X + xPos].Type = TileType.TYPE_EVENT;
                        }
                    }
                }

                if (ToolSelection == Tools.TOOL_ERASER)
                {
                    levelMap.TileList[Pos.Y, Pos.X].Position = Rectangle.Empty;
                    levelMap.TileList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                    levelMap.TileList[Pos.Y, Pos.X].Type = (TileType)(-1);
                    levelMap.TileList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);

                    levelMap.SpawnList[Pos.Y, Pos.X].Position = Rectangle.Empty;
                    levelMap.SpawnList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                    levelMap.SpawnList[Pos.Y, Pos.X].Type = (TileType)(-1);
                    levelMap.SpawnList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);

                    levelMap.CollisionList[Pos.Y, Pos.X].Position = Rectangle.Empty;
                    levelMap.CollisionList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                    levelMap.CollisionList[Pos.Y, Pos.X].Type = (TileType)(-1);
                    levelMap.CollisionList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);

                    levelMap.EventList[Pos.Y, Pos.X].Position = Rectangle.Empty;
                    levelMap.EventList[Pos.Y, Pos.X].PickedTile = Point.Empty;
                    levelMap.EventList[Pos.Y, Pos.X].Type = (TileType)(-1);
                    levelMap.EventList[Pos.Y, Pos.X].XYIndex = new Point(-1, -1);
                }

                if (toolsWindow != null)
                {
                    toolsWindow.LoadLists();
                }
            }
        }

        private void toolsWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateToolWindow();
        }

        private void tileWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTileWindow();
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

        private void MapRest()
        {
            if (levelMap.MapSize.Height > 0 && levelMap.MapSize.Width > 0)
            {
                CTile[,] OldTileList = levelMap.TileList;
                CTile[,] OldCollisionList = levelMap.CollisionList;
                CTile[,] OldEventList = levelMap.EventList;
                CTile[,] OldSpawnList = levelMap.SpawnList;

                levelMap.TileList = new CTile[levelMap.MapSize.Height, levelMap.MapSize.Width];
                levelMap.CollisionList = new CTile[levelMap.MapSize.Height, levelMap.MapSize.Width];
                levelMap.EventList = new CTile[levelMap.MapSize.Height, levelMap.MapSize.Width];
                levelMap.SpawnList = new CTile[levelMap.MapSize.Height, levelMap.MapSize.Width];


                for (int Ypos = 0; Ypos < levelMap.MapSize.Height; Ypos++)
                {
                    for (int Xpos = 0; Xpos < levelMap.MapSize.Width; Xpos++)
                    {
                        CTile tile = new CTile();

                        tile.Type = (TileType)(-1);
                        tile.XYIndex = new Point(-1, -1);
                        levelMap.TileList[Ypos, Xpos] = tile;

                        tile = new CTile();

                        tile.Type = (TileType)(-1);
                        tile.XYIndex = new Point(-1, -1);
                        levelMap.CollisionList[Ypos, Xpos] = tile;

                        tile = new CTile();

                        tile.Type = (TileType)(-1);
                        tile.XYIndex = new Point(-1, -1);
                        levelMap.EventList[Ypos, Xpos] = tile;

                        tile = new CTile();

                        tile.Type = (TileType)(-1);
                        tile.XYIndex = new Point(-1, -1);
                        levelMap.SpawnList[Ypos, Xpos] = tile;
                    }
                }

                if (levelMap.MapSize.Width >= OldMapSize.Width && levelMap.MapSize.Height >= OldMapSize.Height)
                {
                    for (int Ypos = 0; Ypos < OldMapSize.Height; Ypos++)
                    {
                        for (int Xpos = 0; Xpos < OldMapSize.Width; Xpos++)
                        {
                            levelMap.TileList[Ypos, Xpos] = OldTileList[Ypos, Xpos];
                            levelMap.CollisionList[Ypos, Xpos] = OldCollisionList[Ypos, Xpos];
                            levelMap.EventList[Ypos, Xpos] = OldEventList[Ypos, Xpos];
                            levelMap.SpawnList[Ypos, Xpos] = OldSpawnList[Ypos, Xpos];
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
                            levelMap.CollisionList[Ypos, Xpos] = OldCollisionList[Ypos, Xpos];
                            levelMap.EventList[Ypos, Xpos] = OldEventList[Ypos, Xpos];
                            levelMap.SpawnList[Ypos, Xpos] = OldSpawnList[Ypos, Xpos];
                        }
                    }
                }

                if (toolsWindow != null)
                {
                    toolsWindow.SpawnList = levelMap.SpawnList;
                    toolsWindow.CollisionList = levelMap.CollisionList;
                    toolsWindow.EventList = levelMap.EventList;
                    toolsWindow.LoadLists();
                }
            }
        }

        private bool CheckForSave()
        {
            for (int YPos = 0; YPos < levelMap.MapSize.Height; YPos++)
            {
                for (int XPos = 0; XPos < levelMap.MapSize.Width; XPos++)
                {
                    if (levelMap.TileList[YPos, XPos].Type != (TileType)(-1))
                    {
                        return true;
                    }

                    if (levelMap.SpawnList[YPos, XPos].Type != (TileType)(-1))
                    {
                        return true;
                    }

                    if (levelMap.EventList[YPos, XPos].Type != (TileType)(-1))
                    {
                        return true;
                    }

                    if (levelMap.CollisionList[YPos, XPos].Type != (TileType)(-1))
                    {
                        return true;
                    }

                }
            }

            return false;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //ask to save;
            if (CheckForSave())
            {
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
                                levelMap.CollisionList[YPos, XPos] = new CTile();
                                levelMap.EventList[YPos, XPos] = new CTile();
                                levelMap.SpawnList[YPos, XPos] = new CTile();

                                levelMap.TileList[YPos, XPos].Type = (TileType)(-1);
                                levelMap.CollisionList[YPos, XPos].Type = (TileType)(-1);
                                levelMap.EventList[YPos, XPos].Type = (TileType)(-1);
                                levelMap.SpawnList[YPos, XPos].Type = (TileType)(-1);

                                levelMap.TileList[YPos, XPos].XYIndex = new Point(-1, -1);
                                levelMap.CollisionList[YPos, XPos].XYIndex = new Point(-1, -1);
                                levelMap.EventList[YPos, XPos].XYIndex = new Point(-1, -1);
                                levelMap.SpawnList[YPos, XPos].XYIndex = new Point(-1, -1);
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

                XElement XCollisionLocation = xLevel.Element("CollisionLocation");
                LoadCollisionInfo(path, (string)XCollisionLocation.Value);

                XElement XEventsLocation = xLevel.Element("EventLocation");
                LoadEventInfo(path, (string)XEventsLocation.Value);

                XElement XSpawnsLocation = xLevel.Element("SpawnLocation");
                LoadSpawnInfo(path, (string)XSpawnsLocation.Value);

                if (toolsWindow != null)
                {
                    toolsWindow.MapSize = levelMap.MapSize;
                    toolsWindow.PixelSize = levelMap.PixelSize;
                    toolsWindow.TileSize = levelMap.TileSize;

                    toolsWindow.CollisionList = levelMap.CollisionList;
                    toolsWindow.EventList = levelMap.EventList;
                    toolsWindow.SpawnList = levelMap.SpawnList;

                    toolsWindow.LoadLists();

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

                                     if (tile.Type != (TileType)(-1))
                                        levelMap.TileList[tile.XYIndex.Y, tile.XYIndex.X] = tile;
                                 }
                            }
                        }
                    }
                }
            }
        }

        private void LoadCollisionInfo(string path, string filename)
        {
            string Filepath = path + filename;

            XElement xCollisions = XElement.Load(Filepath); // root

            IEnumerable<XElement> xCollision = xCollisions.Elements();

            foreach (XElement c in xCollision)
            {
                CTile tile = new CTile();

                tile.Name = c.Value;

                XAttribute xXpos = c.Attribute("XPos");
                if (xXpos != null)
                {
                    XAttribute xYpos = c.Attribute("YPos");
                    if (xYpos != null)
                    {
                        tile.XYIndex = new Point(int.Parse(xXpos.Value), int.Parse(xYpos.Value));

                         XAttribute xType = c.Attribute("Type");
                         if (xType != null)
                         {
                             tile.Type = (TileType)(int.Parse(xType.Value));
                             if (tile.Type != (TileType)(-1))
                                levelMap.CollisionList[tile.XYIndex.Y, tile.XYIndex.X] = tile;
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
                             if (tile.Type != (TileType)(-1))
                                levelMap.EventList[tile.XYIndex.Y, tile.XYIndex.X] = tile;
                         }
                    }
                }
            }
        }

        private void LoadSpawnInfo(string path, string filename)
        {
            string Filepath = path + filename;

            XElement xSpawns = XElement.Load(Filepath); // root

            IEnumerable<XElement> xSpawn = xSpawns.Elements();

            foreach (XElement s in xSpawn)
            {
                CTile tile = new CTile();

                tile.Name = s.Value;

                XAttribute xXpos = s.Attribute("XPos");
                if (xXpos != null)
                {
                    XAttribute xYpos = s.Attribute("YPos");
                    if (xYpos != null)
                    {
                        tile.XYIndex = new Point(int.Parse(xXpos.Value), int.Parse(xYpos.Value));

                        XAttribute xType = s.Attribute("Type");
                        if (xType != null)
                        {
                            tile.Type = (TileType)(int.Parse (xType.Value));
                            if (tile.Type != (TileType)(-1))
                                levelMap.SpawnList[tile.XYIndex.Y, tile.XYIndex.X] = tile;
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

                XElement xCollisionLocation = new XElement("CollisionLocation", "CollisionInfo.xml");
                SaveCollisionInfo(path, "CollisionInfo.xml");

                XElement xEventLocation = new XElement("EventLocation", "EventInfo.xml");
                SaveEventInfo(path, "EventInfo.xml");

                XElement xSpawnLocation = new XElement("SpawnLocation", "SpawnInfo.xml");
                SaveSpawnInfo(path, "SpawnInfo.xml");

                xLevel.Add(xTileLocation);
                xLevel.Add(xCollisionLocation);
                xLevel.Add(xEventLocation);
                xLevel.Add(xSpawnLocation);

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

        private void SaveCollisionInfo(string path, string filename)
        {
            path += filename;

                XElement xCollisions = new XElement("Collisions"); // root

                foreach (CTile c in levelMap.CollisionList)
                {
                    if (c.XYIndex.X != -1 && c.XYIndex.Y != -1)
                    {
                        XElement xTile = new XElement("Collision", c.Name);
                        xCollisions.Add(xTile);

                        XAttribute xIndexX = new XAttribute("XPos", c.XYIndex.X);
                        xTile.Add(xIndexX);

                        XAttribute xIndexY = new XAttribute("YPos", c.XYIndex.Y);
                        xTile.Add(xIndexY);

                        XAttribute xType = new XAttribute("Type", (int)c.Type);
                        xTile.Add(xType);
                    }
                }

                xCollisions.Save(path);
        }

        private void SaveEventInfo(string path, string filename)
        {
            path += filename;

                XElement xEvents = new XElement("Events"); // root

                foreach (CTile e in levelMap.EventList)
                {
                    if (e.XYIndex.X != -1 && e.XYIndex.Y != -1)
                    {
                        XElement xTile = new XElement("Event", e.Name);
                        xEvents.Add(xTile);

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

        private void SaveSpawnInfo(string path, string filename)
        {
            path += filename;

                XElement xSpawns = new XElement("Spawns"); // root

                foreach (CTile s in levelMap.SpawnList)
                {
                    if (s.XYIndex.X != -1 && s.XYIndex.Y != -1)
                    {
                        XElement xTile = new XElement("Spawn", s.Name);
                        xSpawns.Add(xTile);

                        XAttribute xIndexX = new XAttribute("XPos", s.XYIndex.X);
                        xTile.Add(xIndexX);

                        XAttribute xIndexY = new XAttribute("YPos", s.XYIndex.Y);
                        xTile.Add(xIndexY);

                        XAttribute xType = new XAttribute("Type", (int)s.Type);
                        xTile.Add(xType);
                    }
                }

                xSpawns.Save(path);
        }


        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Looping = false;
            Close();
        }

        private void MapEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (CheckForSave())
            {
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