using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Tile_Editor
{
    public class CMap
    {
        CTile[,] tileList; // the list of tiles for the map

        public CTile[,] TileList
        {
            get { return tileList; }
            set { tileList = value; }
        }

        CTile[,] eventList; // the list of events for the map

        public CTile[,] EventList
        {
            get { return eventList; }
            set { eventList = value; }
        }

        Size mapSize; // How big the map array is

        public Size MapSize
        {
            get { return mapSize; }
            set { mapSize = value; }
        }

        Size tileSize; // How big the tile window array is

        public Size TileSize
        {
            get { return tileSize; }
            set { tileSize = value; }
        }

        Point tileSelected; // which tile is selected

        public Point TileSelected
        {
            get { return tileSelected; }
            set { tileSelected = value; }
        }

        Point endSelected;

        public Point EndSelected
        {
            get { return endSelected; }
            set { endSelected = value; }
        }

        Size pixelSize; // How many pixels is each cell 

        public Size PixelSize
        {
            get { return pixelSize; }
            set { pixelSize = value; }
        }

        Size maxPixelSize; // Max Pixels for the map 

        public Size MaxPixelSize
        {
            get { return maxPixelSize; }
            set { maxPixelSize = value; }
        }

        int tileBitmap; // the bitmap in use

        public int TileBitmap
        {
            get { return tileBitmap; }
            set { tileBitmap = value; }
        }

        String mapName; // the name of the map

        public String MapName
        {
            get { return mapName; }
            set { mapName = value; }
        }

        Color collisionColor;

        public Color CollisionColor
        {
            get { return collisionColor; }
            set { collisionColor = value; }
        }

        Color eventColor;

        public Color EventColor
        {
            get { return eventColor; }
            set { eventColor = value; }
        }

        Color spawnColor;

        public Color SpawnColor
        {
            get { return spawnColor; }
            set { spawnColor = value; }
        }

        // file names for each list location.
    }
}
