using System;

// need this to make API calls
using System.Runtime.InteropServices;

namespace TestFramework.Utils
{
    /// <summary>
    /// Static class for console color manipulation.
    /// </summary>
    public class ConsoleColor
    {
        // constants for console streams
        const int STD_INPUT_HANDLE = -10;
        const int STD_OUTPUT_HANDLE = -11;
        const int STD_ERROR_HANDLE = -12;

        [DllImportAttribute("Kernel32.dll")]
        private static extern IntPtr GetStdHandle
            (
            int nStdHandle // input, output, or error device
            );

        [DllImportAttribute("Kernel32.dll")]
        private static extern bool SetConsoleTextAttribute
            (
            IntPtr hConsoleOutput, // handle to screen buffer
            int wAttributes    // text and background colors
            );

        // colors that can be set
        [Flags]
            public enum ForeGroundColor
        {
            Black = 0x0000,
            Blue = 0x0001,
            Green = 0x0002, 
            Cyan = 0x0003,
            Red = 0x0004,
            Magenta = 0x0005,
            Yellow = 0x0006,
            Grey = 0x0007,
            White = 0x0008
        }

        // class can not be created, so we can set colors 
        // without a variable
        private ConsoleColor()
        {
        }

        public static bool SetForeGroundColor()
        {
            // default to a white-grey
            return SetForeGroundColor(ForeGroundColor.Grey);
        }

        public static bool SetForeGroundColor(
            ForeGroundColor foreGroundColor)
        {
            // default to a bright white-grey
            return SetForeGroundColor(foreGroundColor, true);
        }

        public static bool SetForeGroundColor(
            ForeGroundColor foreGroundColor, 
            bool brightColors)
        {
            // get the current console handle
            IntPtr nConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            int colorMap;
            
            // if we want bright colors OR it with white
            if (brightColors)
                colorMap = (int) foreGroundColor | 
                    (int) ForeGroundColor.White;
            else
                colorMap = (int) foreGroundColor;

            // call the api and return the result
            return SetConsoleTextAttribute(nConsole, colorMap);
        }
    }
}