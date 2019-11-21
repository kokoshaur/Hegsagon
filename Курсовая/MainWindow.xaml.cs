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


namespace Курсовая
{
    public partial class MainWindow : Window
    {
        bool ClickStatus = false;//проверка на чётность нажатия
        bool HegChek = false;//проверка цвета нажатой кнопки
        static int[] FieldStatus = new int[37];//Статус игрового поля
        int[] PMoves = new int[37];//Возможные ходы

        const string pathRU = "../Language/RU.txt";
        const string pathUK = "../Language/UK.txt";

        //const string TrackBot = "C:/Users/USER/Desktop/Лабы/Моя какуля/Курсовая/Release/Bot.dll";
        const string TrackBot = "C:/Users/User/Desktop/Шарага/инфа/Лабы/Моя какуля/Курсовая/Release/Bot.dll";

        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern void PossibleMoves(int NumHegs, int[] FieldStatus, int[] PMoves);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int PeacBot(int[] FieldStatus);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int EasyBot(int[] FieldStatus);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int NormalBot(int[] FieldStatus);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int HardlBot(int[] FieldStatus);
        [DllImport(TrackBot, CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern int NightmarelBot(int[] FieldStatus);
        public MainWindow()
        {
            InitializeComponent();
            WindowState = WindowState.Maximized;
            for (int i = 0; i < 36; i++)
            {
                FieldStatus[i] = 0;
                PMoves[i] = 0;
            }
            FieldStatus[0] = -1;
            FieldStatus[1] = 1;
            FieldStatus[36] = 2;
            PMoves[0] = -1;
        }
        public class Bot
        {
            public
            static int Move(int Mod)
            {
                switch (Mod)
                {
                    case 0:
                        return PeacBot(FieldStatus);
                    case 1:
                        return EasyBot(FieldStatus);
                    case 2:
                        return NormalBot(FieldStatus);
                    case 3:
                        return HardlBot(FieldStatus);
                    case 4:
                        return NightmarelBot(FieldStatus);
                    default:
                        return 0;
                }
            }
        }

        public void Rate(int rate, bool flag)
        {
            if (flag)
            {
                (FindName("B" + rate) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsGren");
                GrenStatus.Content = Convert.ToInt64(GrenStatus.Content) + 1;
            }
            else
            {
                (FindName("B" + rate) as Button).Style = (Style)Application.Current.FindResource("ButtomHegsRed");
                RedStatus.Content = Convert.ToInt64(RedStatus.Content) + 1;
            }
        }
        unsafe private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (ClickStatus)
            {
                ClickStatus = false;
                if ((sender as Button).Style == (Style)Application.Current.FindResource("ButtomHegs") && (HegChek))//
                {
                    int Butnumber = int.Parse((sender as Button).Name.Replace("B", string.Empty));//Находим номер кнопки
                    FieldStatus[Butnumber] = 1;//Запоминаем ход

                    Rate(Butnumber, true);//Ходим мы

                    Rate(Bot.Move(0), false);//Ходит бот

                }

                HegChek = false;
            }
            else
            {
                if ((sender as Button).Style == (Style)Application.Current.FindResource("ButtomHegsGren"))
                {
                    HegChek = true;
                    ClickStatus = true;
                    PossibleMoves(int.Parse((sender as Button).Name.Replace("B", string.Empty)), FieldStatus, PMoves);
                    PMoves[1] = 1;
                }

            }
            
        }

        private void SetLanRu(object sender, RoutedEventArgs e)
        {
            StreamReader sr = new StreamReader(pathRU);
            Exit.Content = sr.ReadLine();
        }

        private void SetLanUk(object sender, RoutedEventArgs e)
        {
            StreamReader sr = new StreamReader(pathUK);
            Exit.Content = sr.ReadLine();
        }

        private void Exit(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
