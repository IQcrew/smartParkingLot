using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using FireSharp.Config;
using FireSharp.Interfaces;
using FireSharp.Response;
using Newtonsoft.Json;
using Parkovisko;

namespace openLabProjekt
{
    internal static class databaseManager
    {
        public static List<Zamestnanec> zamestnanci = new List<Zamestnanec>() { };
        public static Dictionary<string, string > parkoviskoKomunikacia = new Dictionary<string, string>() { };
        public static string karta = "";
        public static string miesta = "yyyyyy";
        static IFirebaseClient client;


        static IFirebaseConfig databaza = new FirebaseConfig()
        {
            AuthSecret = "w8hjMwOb3yaWnaCd7IxD4iyYfdXXbfiwyiBbXIw2",
            BasePath = "https://parkovisko-14fcb-default-rtdb.europe-west1.firebasedatabase.app"

        };
        public static void SetUp(List<string> MZ)
        {
            try
            {
                client = new FireSharp.FirebaseClient(databaza);
            }

            catch
            {
                MessageBox.Show("Nemožno zaregistrovť, skúste neskôr");
            }
            RefreshDataZamestnanci(MZ);
            databaseManager.refreshParkovisko();
        }
        public static void refreshParkovisko()
        {
            FirebaseResponse res = client.Get("parkovisko");
            parkoviskoKomunikacia = JsonConvert.DeserializeObject<Dictionary<string, string>>(res.Body.ToString());
            karta = parkoviskoKomunikacia["data"].Split(',')[0];
            miesta = parkoviskoKomunikacia["data"].Split(',')[1];

            
        }
        public static void changeZavora(string input)
        {
            client.Set("parkovisko/zavora", input);
        }

        public static void RefreshDataZamestnanci(List<string> MZ)
        {
            try
            {
                FirebaseResponse res = client.Get("Zamestnanci");
                Dictionary<string, Zamestnanec> data = JsonConvert.DeserializeObject<Dictionary<string, Zamestnanec>>(res.Body.ToString());
                zamestnanci.Clear();

                if ( data == null || data.Values.Count < 6)
                {
                    foreach (string item in MZ)
                    {
                        client.Set("Zamestnanci/" + item, new Zamestnanec(item));
                        zamestnanci.Add(new Zamestnanec(item));
                    }
                }
                else
                {
                    zamestnanci = data.Values.ToList();
                }
            }
            catch
            {
            }
           



        }
        public static void updateZamestnanca(Zamestnanec z)
        {
            client.Update("Zamestnanci/" + z.Meno, z);
            for (int i = 0; i < zamestnanci.Count; i++)
            {
                if (zamestnanci[i].Meno == z.Meno) { zamestnanci[i] = z; return; }
            }
        }
        public static Zamestnanec findZamestnanec(string meno)
        {
            foreach (var item in zamestnanci)
            {
                if(item.Meno == meno) { return item; }
            }
            return null;
        }
        public static Zamestnanec findZamestnanecByID(string id)
        {
            foreach (var item in zamestnanci)
            {
                if (item.cardID == id) { return item; }
            }
            return null;
        }
        public static bool validID(string id)
        {
            foreach (var item in zamestnanci)
            {
                if(item.cardID == id) { return true; }
            }
            return false;
        }
    }
}
