using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;



namespace ParticleTool
{
    public class Particle
    {
        double posX;
        double posY;
        double velX;
        double velY;
        decimal currlife;
        decimal endlife;
        Color color;
        decimal scaleX;
        decimal scaleY;
        decimal rotation;
        double colorfade_timer;
        double scale_timerX;
        double scale_timerY;
        bool isDead;
        double spawnTime;
        double SpawnDelay;

        public Particle()
        {
            rotation = 0;
            currlife = 0;
            isDead = true;
        }

        public double PosX
        {
            get { return posX; }
            set { posX = value; }
        }
        public double PosY
        {
            get { return posY; }
            set { posY = value; }
        }
        public double VelX
        {
            get { return velX; }
            set { velX = value; }
        }
        public double VelY
        {
            get { return velY; }
            set { velY = value; }
        }
        public decimal Currlife
        {
            get { return currlife; }
            set { currlife = value; }
        }
        public decimal Endlife
        {
            get { return endlife; }
            set { endlife = value; }
        }
        public Color Color
        {
            get { return color; }
            set { color = value; }
        }
        public decimal ScaleX
        {
            get { return scaleX; }
            set { scaleX = value; }
        }
        public decimal ScaleY
        {
            get { return scaleY; }
            set { scaleY = value; }
        }
        public decimal Rotation
        {
            get { return rotation; }
            set { rotation = value; }
        }
        public double Colorfade_timer
        {
            get { return colorfade_timer; }
            set { colorfade_timer = value; }
        }
        public double Scale_timerX
        {
            get { return scale_timerX; }
            set { scale_timerX = value; }
        }
        public double Scale_timerY
        {
            get { return scale_timerY; }
            set { scale_timerY = value; }
        }
        public bool IsDead
        {
            get { return isDead; }
            set { isDead = value; }
        }
        public double SpawnTime
        {
            get { return spawnTime; }
            set { spawnTime = value; }
        }
        public double SpawnDelay1
        {
            get { return SpawnDelay; }
            set { SpawnDelay = value; }
        }
    };

    public class ParticleEmitter
    {
        int TextureID;
        decimal EndLife;
        decimal MinLife;
        Color Start_Color;
        Color End_Color;
        List<Particle> particles;
        decimal Max_Number;
        decimal start_scaleX;
        decimal end_scaleX;
        decimal start_scaleY;
        decimal end_scaleY;

        decimal velminX;
        decimal velminY;
        decimal velmaxX;
        decimal velmaxY;

        decimal accelX;
        decimal accelY;

        decimal rotation;
              
        Point position;

        string imagename;

        public ParticleEmitter()
        {
            EndLife = 1;
            MinLife = 1;
            Start_Color = Color.FromArgb(255, 0, 255, 255);
            End_Color = Color.FromArgb(0,255,255,0);
            Max_Number = 200;
            position = new Point(300, 300);
            particles = new List<Particle>();

            start_scaleX = 0.3M;
            end_scaleX = 2.0M;
            start_scaleY = 0.3M;
            end_scaleY = 2.0M;

            velminX = -100;
            velminY = -100;

            velmaxX = 100;
            velmaxY = 100;

            accelX = 0;
            accelY = 0;
        }

        public Point Position
        {
            get { return position; }
            set { position = value; }
        }

        public int TextureID1
        {
            get { return TextureID; }
            set { TextureID = value; }
        }

        public decimal EndLife1
        {
            get { return EndLife; }
            set { EndLife = value; }
        }
        public decimal MinLife1
        {
            get { return MinLife; }
            set { MinLife = value; }
        }

        public Color Start_Color1
        {
            get { return Start_Color; }
            set { Start_Color = value; }
        }
        
        public Color End_Color1
        {
            get { return End_Color; }
            set { End_Color = value; }
        }
        
        internal List<Particle> Particles
        {
            get { return particles; }
            set { particles = value; }
        }
       
        public decimal Max_Number1
        {
            get { return Max_Number; }
            set { Max_Number = value; }
        }
        public decimal Start_scaleX
        {
            get { return start_scaleX; }
            set { start_scaleX = value; }
        }
        public decimal End_scaleX
        {
            get { return end_scaleX; }
            set { end_scaleX = value; }
        }
        public decimal Start_scaleY
        {
            get { return start_scaleY; }
            set { start_scaleY = value; }
        }
        public decimal End_scaleY
        {
            get { return end_scaleY; }
            set { end_scaleY = value; }
        }
        public decimal VelminX
        {
            get { return velminX; }
            set { velminX = value; }
        }
        public decimal VelminY
        {
            get { return velminY; }
            set { velminY = value; }
        }
        public decimal VelmaxX
        {
            get { return velmaxX; }
            set { velmaxX = value; }
        }
        public decimal VelmaxY
        {
            get { return velmaxY; }
            set { velmaxY = value; }
        }
        public decimal AccelX
        {
            get { return accelX; }
            set { accelX = value; }
        }

        public decimal AccelY
        {
            get { return accelY; }
            set { accelY = value; }
        }
        public decimal Rotation
        {
            get { return rotation; }
            set { rotation = value; }
        }
        public string Imagename
        {
            get { return imagename; }
            set { imagename = value; }
        }
    };
}
