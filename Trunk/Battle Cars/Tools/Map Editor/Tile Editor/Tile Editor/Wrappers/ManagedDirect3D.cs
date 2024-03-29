////////////////////////////////////////////////////////////////////////////////////////
//  File			:	"ManagedDirect3D.cs"
//
//  Author			:	David Brown (DB)
//
//  Date Created	:	6/25/2007
//
//  Purpose			:	Wrapper class for Managed Direct3D.
//
//  NOTE:   In order to use these wrapper classes you will need
//          to add References to the following assemblies to your Solution:
//
//          -Microsoft.DirectX              version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3D     version: 1.0.2902.0 runtime: v1.1.4322
//          -Microsoft.DirectX.Direct3DX    version: 1.0.2902.0 runtime: v1.1.4322
////////////////////////////////////////////////////////////////////////////////////////

using System;               // for Int32
using System.Windows.Forms; // for MessageBox
using System.Collections.Generic; // for List

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace SGD
{
    public sealed class ManagedDirect3D
    {
        Microsoft.DirectX.Direct3D.Device device = null;
        Microsoft.DirectX.Direct3D.Sprite sprite = null;
        Microsoft.DirectX.Direct3D.Line line = null;
        Microsoft.DirectX.Direct3D.PresentParameters presentParams = null;

        Microsoft.DirectX.Direct3D.Font font = null;
        Microsoft.DirectX.Direct3D.FontDescription fontDescription;

        List<ManagedSwapChain> lSwapChains = new List<ManagedSwapChain>();

        // thread-safe singleton
        static readonly ManagedDirect3D instance = new ManagedDirect3D();

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static ManagedDirect3D()
        {
        }

        ManagedDirect3D()
        {
        }

        /// <summary>
        /// Gets the instance of the singleton.
        /// </summary>
        public static ManagedDirect3D Instance
        {
            get
            {
                return instance;
            }
        }

        /// <summary>
        /// Gets the Direct3D device.
        /// </summary>
        public Microsoft.DirectX.Direct3D.Device Device
        {
            get
            {
                return device;
            }
        }

        /// <summary>
        /// Gets the Sprite object.
        /// </summary>
        public Microsoft.DirectX.Direct3D.Sprite Sprite
        {
            get
            {
                return sprite;
            }
        }

        /// <summary>
        /// Initializes the Managed Direct3D Wrapper.
        /// 
        /// This overload is designed for displaying a diferent resolution then the controls size.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="screenWidth">Width of the screen in pixels.</param>
        /// <param name="screenHeight">Height of the screen in pixels.</param>
        /// <param name="isWindowed">Is the application windowed or not (!!ALWAYS PASS true FOR NOW!!).</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool InitManagedDirect3D(System.Windows.Forms.Control renderWindow, int screenWidth, int screenHeight, bool isWindowed, bool vsync)
        {
            try
            {
                // Now  setup our D3D present parameters
                presentParams = new PresentParameters();
                presentParams.BackBufferWidth = screenWidth;
                presentParams.BackBufferHeight = screenHeight;
                presentParams.BackBufferFormat = (isWindowed) ? Format.Unknown : Format.R5G6B5;
                presentParams.BackBufferCount = 1;
                presentParams.MultiSample = MultiSampleType.None;
                presentParams.MultiSampleQuality = 0;
                presentParams.SwapEffect = SwapEffect.Copy;// Discard;
                presentParams.DeviceWindow = renderWindow;
                presentParams.Windowed = isWindowed;
                presentParams.EnableAutoDepthStencil = false;
                presentParams.FullScreenRefreshRateInHz = 0;
                presentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

                // only Create if we haven't done so already
                if(device == null)
                device = new Device(0, DeviceType.Hardware, renderWindow,
                        CreateFlags.HardwareVertexProcessing, presentParams);

                //Create the SwapChain
            lSwapChains.Add(new ManagedSwapChain(device, presentParams));
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create D3D Device", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                sprite = new Microsoft.DirectX.Direct3D.Sprite(device);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the Sprite object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                line = new Microsoft.DirectX.Direct3D.Line(device);

                line.Antialias = true;
                line.Width = 3.0f;
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the Line Object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                fontDescription.FaceName = "arial";
                fontDescription.Quality = FontQuality.Default;
                fontDescription.Weight = FontWeight.Bold;

                font = new Microsoft.DirectX.Direct3D.Font(device, fontDescription);

                line.Antialias = true;
                line.Width = 3.0f;
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the font Object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            return true;
        }

        /// <summary>
        /// Initializes the Managed Direct3D Wrapper.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool InitManagedDirect3D(System.Windows.Forms.Control renderWindow, bool vsync)
        {
            try
            {
                // Now  setup our D3D present parameters
                presentParams = new PresentParameters();
                presentParams.BackBufferWidth = renderWindow.Width;
                presentParams.BackBufferHeight = renderWindow.Height;
                presentParams.BackBufferFormat = Format.Unknown;
                presentParams.BackBufferCount = 1;
                presentParams.MultiSample = MultiSampleType.None;
                presentParams.MultiSampleQuality = 0;
                presentParams.SwapEffect = SwapEffect.Copy;// Discard;
                presentParams.DeviceWindow = renderWindow;
                presentParams.Windowed = true;
                presentParams.EnableAutoDepthStencil = false;
                presentParams.FullScreenRefreshRateInHz = 0;
                presentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

                // only Create if we haven't done so already
                if (device == null)
                    device = new Device(0, DeviceType.Hardware, renderWindow,
                            CreateFlags.HardwareVertexProcessing, presentParams);

                //Create the SwapChain
                lSwapChains.Add(new ManagedSwapChain(device, presentParams));
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create D3D Device", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                sprite = new Microsoft.DirectX.Direct3D.Sprite(device);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the Sprite object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                line = new Microsoft.DirectX.Direct3D.Line(device);

                line.Antialias = true;
                line.Width = 3.0f;
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the Line Object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            try
            {
                fontDescription.FaceName = "arial";
                fontDescription.Quality = FontQuality.Default;
                fontDescription.Weight = FontWeight.Bold;

                font = new Microsoft.DirectX.Direct3D.Font(device, fontDescription);

                line.Antialias = true;
                line.Width = 3.0f;
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to Create the font Object", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            return true;
        }

        /// <summary>
        /// Changes the display parameters of the Managed Direct3D Wrapper.
        /// 
        /// Use this to change what window you render to.
        /// This overload is designed for displaying a diferent resolution size then the controls size
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="screenWidth">Width of the screen in pixels.</param>
        /// <param name="screenHeight">Height of the screen in pixels.</param>
        /// <param name="isWindowed">Is the application windowed or not (!!ALWAYS PASS true FOR NOW!!).</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool ChangeDisplayParam(System.Windows.Forms.Control renderWindow, int screenWidth, int screenHeight, bool isWindowed, bool vsync)
        {
            try
            {
                // Now  setup our D3D present parameters
                presentParams = new PresentParameters();
                presentParams.BackBufferWidth = screenWidth;
                presentParams.BackBufferHeight = screenHeight;
                presentParams.BackBufferFormat = (isWindowed) ? Format.Unknown : Format.R5G6B5;
                presentParams.BackBufferCount = 1;
                presentParams.MultiSample = MultiSampleType.None;
                presentParams.MultiSampleQuality = 0;
                presentParams.SwapEffect = SwapEffect.Copy;// Discard;
                presentParams.DeviceWindow = renderWindow;
                presentParams.Windowed = isWindowed;
                presentParams.EnableAutoDepthStencil = false;
                presentParams.FullScreenRefreshRateInHz = 0;
                presentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

               

                // Find the swap chain
                int nIndex = -1;
                for (int i = 0; i < lSwapChains.Count; i++)
                    if (lSwapChains[i].Chain.PresentParameters.DeviceWindow == renderWindow)
                    {
                        nIndex = i;
                        break;
                    }
                if (nIndex < 0)
                {
                    DialogResult r = MessageBox.Show("Not a valid render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return false;
                } 
                
                font.OnLostDevice();
                line.OnLostDevice();
                sprite.OnLostDevice();
                device.Reset(presentParams);
                sprite.OnResetDevice();
                line.OnResetDevice();
                font.OnResetDevice();

                lSwapChains[nIndex].Reset(device, presentParams);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to change the render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                return false;
            }

            return true;
        }

        /// <summary>
        /// Changes the display parameters of the Managed Direct3D Wrapper.
        /// 
        /// Use this to change what window you render to.
        /// </summary>
        /// <param name="renderWindow">Window to render to.</param>
        /// <param name="vsync">Should the renderer wait for a vsync before drawing?</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool ChangeDisplayParam(System.Windows.Forms.Control renderWindow, bool vsync)
        {
            try
            {
                // Now  setup our D3D present parameters
                presentParams = new PresentParameters();
                presentParams.BackBufferWidth = renderWindow.Width;
                presentParams.BackBufferHeight = renderWindow.Height;
                presentParams.BackBufferFormat = Format.Unknown;
                presentParams.BackBufferCount = 1;
                presentParams.MultiSample = MultiSampleType.None;
                presentParams.MultiSampleQuality = 0;
                presentParams.SwapEffect = SwapEffect.Copy;// Discard;
                presentParams.DeviceWindow = renderWindow;
                presentParams.Windowed = true;
                presentParams.EnableAutoDepthStencil = false;
                presentParams.FullScreenRefreshRateInHz = 0;
                presentParams.PresentationInterval = (vsync) ? PresentInterval.Default : PresentInterval.Immediate;

              

                // Find the swap chain
                int nIndex = 0;
                for (int i = 0; i < lSwapChains.Count; i++)
                    if (lSwapChains[i].renderWindow == renderWindow)
                    {
                        nIndex = i;
                        break;
                    }

                if (nIndex < 0)
                {
                    DialogResult r = MessageBox.Show("Not a valid render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return false;
                } 

                font.OnLostDevice();
                line.OnLostDevice();
                sprite.OnLostDevice();
                device.Reset(presentParams);
                sprite.OnResetDevice();
                line.OnResetDevice();
                font.OnResetDevice();
                lSwapChains[nIndex].Reset(device, presentParams);
 
               
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("failed to change the render window", "manageddirect3d::error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);


                return false;
            }

            return true;
        }

        /// <summary>
        /// Handles resetting the device (Private utility function).
        /// </summary>
        private void Reset()
        {
            font.OnLostDevice();
            line.OnLostDevice();
            sprite.OnLostDevice();

            device.Reset(presentParams);

            font.OnResetDevice();
            sprite.OnResetDevice();
            line.OnResetDevice();

            for (int i = 0; i < lSwapChains.Count; i++ )
            {
                lSwapChains[i].Reset(device);
            }
        }

        /// <summary>
        /// Clears the screen to a specific color.
        /// </summary>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Clear(System.Windows.Forms.Control renderWindow, Int32 red, Int32 green, Int32 blue)
        {
            if (device == null)
                return false;

            // Check for alt+tab
            try
            {
                device.TestCooperativeLevel();
            }
            catch (DeviceLostException)
            {
            }
            catch (DeviceNotResetException)
            {
                Reset();
            }

            // Find the swap chain
            int nIndex = -1;
            for (int i = 0; i < lSwapChains.Count; i++ )
                if (lSwapChains[i].Chain.Disposed)
                {
                    lSwapChains[i].Reset(device);
                    return true;
                }
                else if (lSwapChains[i].Chain.PresentParameters.DeviceWindow == renderWindow)
                {
                    nIndex = i;
                    break;
                }
            if (nIndex < 0)
            {
                DialogResult r = MessageBox.Show("Not a valid render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }


            Surface pBackBuffer = lSwapChains[nIndex].Chain.GetBackBuffer(0, BackBufferType.Mono);

            // set it as our current back buffer
            device.SetRenderTarget(0, pBackBuffer);

            //Clear the backbuffer to a color 
            device.Clear(ClearFlags.Target, System.Drawing.Color.FromArgb(red, green, blue), 1.0f, 0);

           
            pBackBuffer.Dispose();

            return true;
        }

        /// <summary>
        /// Begins the Device.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DeviceBegin()
        {
            if (device == null)
                return false;

            try
            {
                device.BeginScene();
            }
            catch (InvalidCallException)
            {
                DialogResult r = MessageBox.Show("Failed to begin device scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Begins the Sprite (MUST be called between DeviceBegin() and DeviceEnd()!).
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool SpriteBegin()
        {
            if (sprite == null)
                return false;

            try
            {
                sprite.Begin(SpriteFlags.AlphaBlend);
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to begin sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Begins the Line.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool LineBegin()
        {
            if (line == null)
                return false;

            try
            {
                line.Begin();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to begin sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Ends the Device.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DeviceEnd()
        {
            if (device == null)
                return false;

            try
            {
                device.EndScene();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to end device scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Ends the Sprite (MUST be called between DeviceBegin() and DeviceEnd()!).
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool SpriteEnd()
        {
            if (sprite == null)
                return false;

            try
            {
                sprite.End();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to end sprite scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Ends the Line.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool LineEnd()
        {
            if (line == null)
                return false;

            try
            {
                line.End();
            }
            catch (Exception)
            {
                DialogResult r = MessageBox.Show("Failed to end line scene.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Present to the screen.
        /// </summary>
        /// <returns>true if successful, false otherwise.</returns>
        public bool Present(System.Windows.Forms.Control renderWindow)
        {
            if (device == null)
                return false;

            // Find the swap chain
            int nIndex = -1;
            for (int i = 0; i < lSwapChains.Count; i++)
                if (lSwapChains[i].Chain.PresentParameters.DeviceWindow == renderWindow)
                {
                    nIndex = i;
                    break;
                }
            if (nIndex < 0)
            {
                DialogResult r = MessageBox.Show("Not a valid render window", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }


            try
            {
                //device.Present();
                lSwapChains[nIndex].Chain.Present();
            }
            catch (Exception)
            {
                DialogResult r;
                r = MessageBox.Show("Failed to present.", "ManagedDirect3D::Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }

            return true;
        }



        /// <summary>
        /// Draws a colored rectangle to the screen.
        /// </summary>
        /// <param name="rect">The rectangle holding the area to fill.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawRect(System.Drawing.Rectangle rect, Int32 red, Int32 green, Int32 blue)
        {
            if (device == null)
                return false;

            System.Drawing.Rectangle[] regions = new System.Drawing.Rectangle[1];
            regions[0] = rect;

            device.Clear(ClearFlags.Target, System.Drawing.Color.FromArgb(red, green, blue), 1.0f, 0, regions);

            return true;
        }

        /// <summary>
        /// Draws a colored line to the screen.
        /// </summary>
        /// <param name="x1">Starting X position.</param>
        /// <param name="y1">Starting Y position.</param>
        /// <param name="x2">Ending X position.</param>
        /// <param name="y2">Ending Y position.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawLine(int x1, int y1, int x2, int y2, Int32 red, Int32 green, Int32 blue)
        {
            if (line == null)
                return false;

            Vector2[] verts = new Vector2[2];

            verts[0].X = x1;
            verts[0].Y = y1;

            verts[1].X = x2;
            verts[1].Y = y2;

            line.Draw(verts, System.Drawing.Color.FromArgb(red, green, blue));

            return true;
        }

        /// <summary>
        /// Draws colored text to the screen.
        /// </summary>
        /// <param name="text">Text to print to the screen.</param>
        /// <param name="x">X position of text.</param>
        /// <param name="y">Y position of text.</param>
        /// <param name="red">Red component of the color (0-255).</param>
        /// <param name="green">Green component of the color (0-255).</param>
        /// <param name="blue">Blue component of the color (0-255).</param>
        /// <returns>true if successful, false otherwise.</returns>
        public bool DrawText(string text, int x, int y, Int32 red, Int32 green, Int32 blue)
        {
            if (font == null)
                return false;

            font.DrawText(sprite, text, new System.Drawing.Point(x, y), System.Drawing.Color.FromArgb(red, green, blue));

            return true;
        }

        public bool Shutdown()
        {
            for(int i = 0; i < lSwapChains.Count; i++)
            {
                lSwapChains[i].Release();
            }
            return true;
        }

    } // end ManagedDirect3D
    public class ManagedSwapChain
    {
        PresentParameters presentParams = null;
        Control RenderWindow = null;

        public Control renderWindow
        {
            get { return RenderWindow; }
            set { RenderWindow = value; }
        }
        SwapChain chain = null;

        public SwapChain Chain
        {
            get { return chain; }
        }

        public ManagedSwapChain(Device device, PresentParameters param)
        {
            presentParams = param;
            renderWindow = presentParams.DeviceWindow;
            chain = new SwapChain(device, presentParams);
        }
        public void Reset(Device device)
        {
            chain.Dispose();
            chain = new SwapChain(device, presentParams);
        }
        public void Reset(Device device, PresentParameters param)
        {
            presentParams = param;
            renderWindow = presentParams.DeviceWindow;
            chain.Dispose();
            chain = new SwapChain(device, param);
        }
        public void Release()
        {
            chain.Dispose();
            chain = null;
            presentParams = null;
        }
    }
}