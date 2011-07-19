using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Tile_Editor
{
    public class CTile
    {
        Point picked;

        public Point PickedTile
        {
            get { return picked; }
            set { picked = value; }
        }

        Point index;

        public Point XYIndex
        {
            get { return index; }
            set { index = value; }
        }

        String name;

        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        TileType type;

        public TileType Type
        {
            get { return type; }
            set { type = value; }
        }

        public override string ToString()
        {
            return name;
        }
    }
}
