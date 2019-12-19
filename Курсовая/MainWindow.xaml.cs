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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.IO;
using System.Media;
using WMPLib;


namespace Курсовая
{
    public struct Hegsagon
    {
        public int status;//статус ячейки
        public int[] PosMovSmall;//возможные ходы длинной 1
        public int[] PosMovBig;//возможные ходы длинной 2

    };

    unsafe public partial class MainWindow : Window
    {
        public int Volume = 30;
        int PlayerScore = 0;
        int BotScore = 0;
        bool ClickStatus = false;//проверка на чётность нажатия
        bool HegChek = false;//проверка цвета нажатой кнопки
        static Hegsagon[] FieldStatus = new Hegsagon[37];//Статус игрового поля  
        public int HardMod = 1;
        int NumBut;
        bool Vis = false;
        bool Game = true;

        const string pathWin = "Win.mp3";
        const string pathField = "../Field.txt";
        const string TrackBot = "../Sorce/Bot.dll";

        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern void BotFieldInit();
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int CheckMove(int peak, int move);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern void Move(int i, int b);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int PeacBot();
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int EasyBot();
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int NormalBot();
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int HardBot();
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int NightmarelBot();
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int CheckPosMov();

        public MainWindow()
        {
            InitializeComponent();
            WindowState = WindowState.Maximized;
            BotFieldInit();
            FieldInit();
        }

        public class Bot
        {
            public
            static int Move(int Mod)
            {
                switch (Mod)
                {
                    case 0:
                        return PeacBot();
                    case 1:
                        return EasyBot();
                    case 2:
                        return NormalBot();
                    case 3:
                        return HardBot();
                    case 4:
                        return NightmarelBot();
                    default:
                        return 0;
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (ClickStatus)
            {
                ClickStatus = false;
                if ((sender as Button).Style == (Style)Application.Current.FindResource("ButtomHegsGrenMove") && (HegChek))//
                {
                    ShowMove(NumBut, false);
                    int Butnumber = int.Parse((sender as Button).Name.Replace("B", string.Empty));//Находим номер кнопки
                    if (CheckMove(NumBut, Butnumber) == 1)
                    {
                        FieldStatus[Butnumber].status = 1;//Запоминаем ход
                        Rate(Butnumber, true);//Ходим мы
                        Move(Butnumber,1);//Рассказываем боту, что мы походили (синхронизация с dllкой)
                        Rate(Bot.Move(HardMod), false);//Ходит бот
                        GrenStatus.Content = PlayerScore;
                        RedStatus.Content = BotScore;
                        if ((CheckPosMov() == 0) & Game)
                            CheckRezult(false);
                    }
                    else if (CheckMove(NumBut, Butnumber) == 2)
                    {
                        PlayerScore--;
                        (FindName("B" + NumBut) as Button).Style = (Style)Application.Current.FindResource("ButtomHegs");
                        FieldStatus[NumBut].status = 0;
                        FieldStatus[Butnumber].status = 1;//Запоминаем ход
                        Rate(Butnumber, true);//Ходим мы
                        Move(Butnumber,1);
                        Move(NumBut, 0);
                        Rate(Bot.Move(HardMod), false);//Ходит бот
                        GrenStatus.Content = PlayerScore;
                        RedStatus.Content = BotScore;
                        if ((CheckPosMov() == 0) & Game)
                            CheckRezult(false);
                    }
                }
                else
                    ShowMove(NumBut, false);
                HegChek = false;
            }
            else
            {
                if ((sender as Button).Style == (Style)Application.Current.FindResource("ButtomHegsGren"))
                {
                    NumBut = int.Parse((sender as Button).Name.Replace("B", string.Empty));
                    ShowMove(NumBut, true);
                    HegChek = true;
                    ClickStatus = true;                  
                }

            }
            
        }

        public void CheckNeig(int z, int peak)//проверяем соседей, чтобы перекрасить
        {
            for (int i = 0; i < 6; i++)
                if (FieldStatus[peak].PosMovSmall[i] == -1)
                    break;
                else
                    if (FieldStatus[FieldStatus[peak].PosMovSmall[i]].status == z)
                    {
                        if(z == 1)
                        {
                            (FindName("B" + FieldStatus[peak].PosMovSmall[i]) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsRed");
                            PlayerScore--;
                            BotScore++;
                        }
                        else
                        {
                            (FindName("B" + FieldStatus[peak].PosMovSmall[i]) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsGren");
                            PlayerScore++;
                            BotScore--;
                        }
                        FieldStatus[FieldStatus[peak].PosMovSmall[i]].status = 3 - z;
                    }
        }

        public void FieldInit()
        {
            StreamReader ReaderField = new StreamReader(pathField);
            string str;
            FieldStatus[0].status = 0;
            FieldStatus[0].PosMovSmall = new int[] { -1, -1, -1, -1, -1, -1 };
            FieldStatus[0].PosMovBig = new int[] { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

            for (int i = 1; i < 37; i++)
            {
                FieldStatus[i].PosMovSmall = new int[] { -1, -1, -1, -1, -1, -1 };
                FieldStatus[i].PosMovBig = new int[] { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
                str = ReaderField.ReadLine();
                string[] words = str.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
                FieldStatus[i].status = Convert.ToInt32(words[0]);
                if (Convert.ToInt32(words[0]) == 1)
                {
                    (FindName("B" + i) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsGren");
                    PlayerScore++;
                }
                else if (Convert.ToInt32(words[0]) == 2)
                {
                    (FindName("B" + i) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsRed");
                    BotScore++;
                }
                string[] wordsSm = words[1].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                for (int b = 0; b < wordsSm.Length; b++)
                    FieldStatus[i].PosMovSmall[b] = Convert.ToInt32(wordsSm[b]);
                string[] wordsBg = words[2].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                for (int b = 0; b < wordsBg.Length; b++)
                    FieldStatus[i].PosMovBig[b] = Convert.ToInt32(wordsBg[b]);
            }
        }

        public void Rate(int rate, bool flag)
        {
            if (flag)
            {
                (FindName("B" + rate) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsGren");
                PlayerScore++;
                Move(rate, 1);
                CheckNeig(2, rate);
            }
            else
            {
                if (rate == 0)
                    CheckRezult(true);
                else
                {
                    if (rate > 37)
                    {
                        FieldStatus[rate/100].status = 2;
                        FieldStatus[rate % 100].status = 0;
                        (FindName("B" + rate/100) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsRed");
                        (FindName("B" + rate % 100) as Button).Style = (Style)Application.Current.FindResource("ButtomHegs");
                        CheckNeig(1, rate/100);
                    }
                    else
                    {
                        FieldStatus[rate].status = 2;
                        (FindName("B" + rate) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsRed");
                        BotScore++;
                        CheckNeig(1, rate);
                    }
                }
            }
        }

        public void CheckRezult(bool q)
        {
            int up = 0;
            for (int i = 1; i < 37; i++)
                if (FieldStatus[i].status == 0)
                    up++;
            if (q)
            {
                MessageBox.Show("Похоже, мне больше некуда ходить! Оставшиеся "+ Convert.ToString(up) + " ячеек достаются тебе. Давай посмотрим, что получилось!");
                PlayerScore += up;
                GrenStatus.Content = PlayerScore;
            }
            else
            {
                MessageBox.Show("Похоже, тебе больше некуда ходить! Оставшиеся " + Convert.ToString(up) + " ячеек достаются мне. Давай посмотрим, что получилось!");
                BotScore += up;
                RedStatus.Content = BotScore;
            }
            if (PlayerScore > BotScore)
            {
                WindowsMediaPlayer wmp = new WindowsMediaPlayer();
                wmp.URL = pathWin;
                wmp.settings.volume = Volume;
                wmp.controls.play();
                MessageBox.Show("АРРРРР НЕВОЗМОЖНО!!!");
                this.Close();
                wmp.controls.stop();
                Game = false;
            }
            else if (PlayerScore < BotScore)
            {
                MessageBox.Show("Ха, тупой мешок мяса, я даже не почувствовал)");
                this.Close();
                Game = false;
            }
            else if (PlayerScore == BotScore)
            {
                MessageBox.Show("Хм, ничья, у тебя почти получилось, давай реванш.");
                this.Close();
                Game = false;
            }

        }

        public void ShowMove(int rate, bool off)
        {
            if (off)
            {
                for (int i = 0; i < 6; i++)
                    if (FieldStatus[rate].PosMovSmall[i] != -1)
                        if (FieldStatus[FieldStatus[rate].PosMovSmall[i]].status == 0)
                            (FindName("B" + FieldStatus[rate].PosMovSmall[i]) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsGrenMove");
                for (int i = 0; i < 12; i++)
                    if (FieldStatus[rate].PosMovBig[i] != -1)
                        if (FieldStatus[FieldStatus[rate].PosMovBig[i]].status == 0)
                            (FindName("B" + FieldStatus[rate].PosMovBig[i]) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsGrenMove");
            }
            else
            {
                for (int i = 0; i < 6; i++)
                    if (FieldStatus[rate].PosMovSmall[i] != -1)
                        if (FieldStatus[FieldStatus[rate].PosMovSmall[i]].status == 0)
                            (FindName("B" + FieldStatus[rate].PosMovSmall[i]) as Button).Style = (Style)Application.Current.FindResource("ButtomHegs");
                for (int i = 0; i < 12; i++)
                    if (FieldStatus[rate].PosMovBig[i] != -1)
                        if (FieldStatus[FieldStatus[rate].PosMovBig[i]].status == 0)
                            (FindName("B" + FieldStatus[rate].PosMovBig[i]) as Button).Style = (Style)Application.Current.FindResource("ButtomHegs");
            }
        }

        private void ExitBut(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void SettingsOpen(object sender, RoutedEventArgs e)
        {
            Settings SetForm = new Settings();
            SetForm.Owner = this;
            SetForm.Show();
        }

        private void MinBut(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void RullesBut_Click(object sender, RoutedEventArgs e)
        {
            if (Vis)
            {
                Rulles1.Visibility = Visibility.Hidden;
                Rulles2.Visibility = Visibility.Hidden;
                Rulles3.Visibility = Visibility.Hidden;
                Rulles4.Visibility = Visibility.Hidden;
                Vis = false;
            }
            else
            {
                Rulles1.Visibility = Visibility.Visible;
                Rulles2.Visibility = Visibility.Visible;
                Rulles3.Visibility = Visibility.Visible;
                Rulles4.Visibility = Visibility.Visible;
                Vis = true;
            }
        }
    }
}
