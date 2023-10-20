using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace CSharpQmageDecoderNS
{
    class CSharpQmageDecoder
    {
        static void Main(string[] args)
        {
            try
            {
                FileStream file = new FileStream(args[0], FileMode.Open);
                byte[] headerRaw = new byte[12];
                file.Read(headerRaw, 0, 12);

                QmageHeader header = new QmageHeader(headerRaw);
                Console.WriteLine($"Qmage File v{header.VerMajor}.{header.VerMinor}.{header.VerRevision}");
                Console.WriteLine($"- Flags: {Convert.ToString(header.Flags, 2).PadLeft(8, '0')}");
                Console.WriteLine($"- Quality: {header.Quality}%");
                Console.WriteLine($"- Resolution: {header.Width}x{header.Height}");
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"ERROR: {ex.Message}");
            }
        }
    }
}
