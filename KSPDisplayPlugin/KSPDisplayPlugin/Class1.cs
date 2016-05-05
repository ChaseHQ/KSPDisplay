using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace KSPDisplayPlugin
{
    public class KSPDisplayPlugin : PartModule
    {
        public override void OnStart(PartModule.StartState state)
        {
            if (NativeMethods.connectToDevice())
            {
                print("Connected to KSP Display");
            }
            else
            {
                print("Failed to connect to KSP Display");
            }
        }

        public override void OnUpdate()
        {
            base.OnUpdate();
            switch (this.vessel.situation)
            {
                case Vessel.Situations.ORBITING:
                case Vessel.Situations.ESCAPING:
                    NativeMethods.sendInteger(Convert.ToInt32(this.vessel.GetObtVelocity().magnitude));
                    break;
                default:
                    NativeMethods.sendInteger(Convert.ToInt32(this.vessel.GetSrfVelocity().magnitude));
                    break;
            }
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
