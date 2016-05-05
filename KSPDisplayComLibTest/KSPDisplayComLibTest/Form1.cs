using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;

namespace KSPDisplayComLibTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void connectBtn_Click(object sender, EventArgs e)
        {
            if (NativeMethods.connectToDevice())
            {
                connectLbl.Enabled = true;
                connectLbl.Text = "Connected";
            }
          
        }

        private void integerBtn_Click(object sender, EventArgs e)
        {
            NativeMethods.sendInteger(Convert.ToInt32(integerTxt.Text));
        }
    }
}

internal static class NativeMethods
{
    [DllImport("KSPDisplayComLib.dll", CallingConvention = CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.I1)]
    public static extern bool connectToDevice();

    [DllImport("KSPDisplayComLib.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void sendInteger(Int32 iRecieved);
}
