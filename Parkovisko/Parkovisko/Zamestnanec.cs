using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Parkovisko
{
    internal class Zamestnanec

    {
        public Zamestnanec(string Meno)
        {
            this.Meno = Meno;
        }
        public string Meno { get; set; }
        public string cardID = "           ";
        public string Prichod = "";
        public List<String> historia = new List<string> {"                                                             < Zaciatok >            "};

    }
}
