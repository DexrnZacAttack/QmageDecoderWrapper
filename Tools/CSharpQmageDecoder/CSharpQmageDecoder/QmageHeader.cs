using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpQmageDecoderNS
{
    public class QmageHeader
    {
        public byte VerMajor;
        public byte VerMinor;
        public byte VerRevision;
        public byte Flags;
        public byte Quality;
        public short Width;
        public short Height;

        public QmageHeader(byte[] headerRaw)
        {
            if (headerRaw.Length < 12 || headerRaw.Length > 12)
                throw new ArgumentException("Invalid header size!");

            byte[] headerName = headerRaw.Take(2).ToArray();
            if (headerName[0] != 'Q' || headerName[1] != 'G')
                throw new ArgumentException("Bad header name! Only QG is supported!");

            byte[] versionRaw = headerRaw.Skip(2).Take(3).ToArray();
            VerMajor = versionRaw[0];
            VerMinor = versionRaw[1];
            VerRevision = versionRaw[2];
            Flags = headerRaw.Skip(5).Take(1).ToArray()[0];
            Quality = headerRaw.Skip(6).Take(1).ToArray()[0];
            Width = BitConverter.ToInt16(headerRaw.Skip(7).Take(2).ToArray(), 0);
            Height = BitConverter.ToInt16(headerRaw.Skip(9).Take(2).ToArray(), 0);
        }
    }
}
