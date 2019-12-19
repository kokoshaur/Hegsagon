using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.IO;


namespace Курсовая
{
    public partial class Settings : Window
    {
        const string pathRU = "../Language/RU.txt";
        const string pathUK = "../Language/UK.txt";

        public Settings()
        {
            InitializeComponent();
        }

        private void SetLanRu(object sender, RoutedEventArgs e)
        {
            StreamReader sr = new StreamReader(pathRU);
            SaveClose.Content = sr.ReadLine();
            Language.Header = sr.ReadLine();
            Hard.Header = sr.ReadLine();
            ModEs.Content = sr.ReadLine();
            ModMed.Content = sr.ReadLine();
            ModHard.Content = sr.ReadLine();
            Vol.Header = sr.ReadLine();
            CheckPlayerName.Header = sr.ReadLine();
            LanRu.ToolTip = sr.ReadLine();
            LanUk.ToolTip = sr.ReadLine(); 
            SaveClose.ToolTip = sr.ReadLine();
            MainWindow main = this.Owner as MainWindow;
            main.ButSettings.ToolTip = sr.ReadLine();
            main.RullesBut.ToolTip = sr.ReadLine();
            main.Rulles1.Text = sr.ReadLine();
            main.Rulles2.Text = sr.ReadLine();
            main.Rulles3.Text = sr.ReadLine();
            main.Rulles4.Text = sr.ReadLine();
        }

        private void SetLanUk(object sender, RoutedEventArgs e)
        {
            StreamReader sr = new StreamReader(pathUK);
            SaveClose.Content = sr.ReadLine();
            Language.Header = sr.ReadLine();
            Hard.Header = sr.ReadLine();
            ModEs.Content = sr.ReadLine();
            ModMed.Content = sr.ReadLine();
            ModHard.Content = sr.ReadLine();
            Vol.Header = sr.ReadLine();
            CheckPlayerName.Header = sr.ReadLine();
            LanRu.ToolTip = sr.ReadLine();
            LanUk.ToolTip = sr.ReadLine();
            SaveClose.ToolTip = sr.ReadLine();
            MainWindow main = this.Owner as MainWindow;
            main.ButSettings.ToolTip = sr.ReadLine();
            main.RullesBut.ToolTip = sr.ReadLine();
            main.Rulles1.Text = sr.ReadLine();
            main.Rulles2.Text = sr.ReadLine();
            main.Rulles3.Text = sr.ReadLine();
            main.Rulles4.Text = sr.ReadLine();
        }

        private void Save(object sender, RoutedEventArgs e)
        {
            MainWindow main = this.Owner as MainWindow;
            main.PlayerName.Content = CheckName.Text;
            main.Volume = Convert.ToInt32(VolumLevel.Value);
            this.Close();
        }

        private void ModEs_Checked(object sender, RoutedEventArgs e)
        {
            MainWindow main = this.Owner as MainWindow;
            main.HardMod = 1;
            main.BotName.Content = "Лёгкий бот";
        }

        private void ModMed_Checked(object sender, RoutedEventArgs e)
        {
            MainWindow main = this.Owner as MainWindow;
            main.HardMod = 2;
            main.BotName.Content = "Средний бот";
        }

        private void ModHard_Checked(object sender, RoutedEventArgs e)
        {
            MainWindow main = this.Owner as MainWindow;
            main.HardMod = 3;
            main.BotName.Content = "Сложный бот";
        }
    }
}
