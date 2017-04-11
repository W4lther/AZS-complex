using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
//using System.Linq;
using System.Text;
//using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading; //Именно это пространство имен поддерживает многопоточность
using MySql.Data.MySqlClient;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        bool timertime;
        public Form1()
        {
            timertime = false;
            InitializeComponent();
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
             
        }
        Thread myThread;
        string comport = "";
        bool established = false;
        void check_connect()
        {
            string aa = "";
            serialPort1.Write("ping");
            while (true) { 
           
            aa = aa + serialPort1.ReadExisting();
                //button1.Text = aa;
                if (aa == "pong")
                {
                    established = true;

                    //  break;
                }
            }
            
           
        }
        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;
            comport = comboBox1.Items[comboBox1.SelectedIndex].ToString();
            serialPort1.PortName = comport; //Указываем наш порт - в данном случае COM1.
            serialPort1.BaudRate = 9600; //указываем скорость.
            serialPort1.ReadTimeout = 500;
            serialPort1.WriteTimeout = 500;
            serialPort1.DataBits = 8;
            serialPort1.Open(); //Открываем порт.
                                //  serialPort1.WriteLine("ping");
            timer1.Enabled = true;
            timer1.Start();
            
             myThread = new Thread(check_connect); //Создаем новый объект потока (Thread)
            myThread.Start();
       
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
            string[] ports = SerialPort.GetPortNames();

          
            // Display each port name to the console.
            foreach (string port in ports)
            {
                comboBox1.Items.Add(port);
            }

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            button1.Enabled = true;
        }
        string CommandText = "Наш SQL скрипт";
        string Connect = "Database=azs;Data Source=127.0.0.1;User Id=root;Password=7720362";
        string rfid = "";
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (myThread.IsAlive)
            { 
            myThread.Abort();
            }
            timer1.Enabled = false;
            serialPort1.Close(); //Закрываем порт.
            if (established)
            {
                MessageBox.Show("Соединение установлено");
                CommandText = "select * from registration;";

                MySqlConnection myConnection = new MySqlConnection(Connect);
                MySqlCommand myCommand = new MySqlCommand(CommandText, myConnection);
                MySqlDataReader reader;
                
                    myConnection.Open();
                    reader = myCommand.ExecuteReader();
                comboBox2.Items.Clear();
                while (reader.Read())
                    {

                        comboBox2.Items.Add(reader["organization"]);
                    /*
                        var items = comboBox2.Items.to
                        .Cast<string>().Distinct().ToArray();
                      
                        comboBox2.Items.AddRange(items);*/
                      /*  comboBox2.Items.Add(reader["organization"]);
                        var items1 = comboBox2.Items.Cast<string>().Distinct().ToArray();
                        comboBox2.Items.Clear();
                        comboBox2.Items.AddRange(items1);*/

                    }
                    reader.Close();
                comboBox2.Items.Add("Добавить новую организацию");
                
                comboBox1.Enabled = false;
                groupBox1.Enabled = true;
            
                
            }
            else

            {
                button1.Enabled = true;
                MessageBox.Show("Соединение не установлено. Попробуйте сменить COM-порт");
            }
        }
        void check_id()
        {
            serialPort1.ReadTimeout = 50000;
            serialPort1.Open(); //Открываем порт.
            string a = "";
            while (a == "") { 
            a = serialPort1.ReadLine();
                for(int i=0;i<a.Length;i++)
                {
                    if (rfid[i] != '/')
                    {
                        rfid = rfid + a[i];
                    }
                    else
                    { break; }
                }
                
                }
            serialPort1.Close();

        }
        private void button3_Click(object sender, EventArgs e)
        {
            textBox3.Text = "";
            serialPort1.ReadTimeout = 50000;
            serialPort1.Open(); //Открываем порт.
            string a = "";
            while (a == "")
            {
                a = serialPort1.ReadLine();
              
                for (int i = 0; i < a.Length; i++)
                {
                 
                    if (a[i] != '\r')
                    {
                        textBox3.Text = textBox3.Text + a[i];
                    }
                    else
                    { break; }
                }


            }
            serialPort1.Close();
            /*if (!myThread.IsAlive)
            {
                //  serialPort1.WriteLine("ping");
                timer1.Enabled = true;
                timer1.Start();

                myThread = new Thread(check_id); //Создаем новый объект потока (Thread)
                myThread.Start();
                button3.Text = "Приложите карту";
                timer2.Enabled = true ;
            }
            else
            {
                timer2.Enabled = false;
                button3.Text = "Прочитать карту";
                myThread.Abort();
                serialPort1.Close();

            }*/
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            if(!myThread.IsAlive)
            {
                timer2.Enabled = false;
                button3.Text = "Прочитать карту";
            }
            if(rfid!="")
            {
                textBox3.Text = rfid;
                timer2.Enabled = false;
            }
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            button4.Enabled = true;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            MySqlConnection myConnection = new MySqlConnection(Connect);
            MySqlCommand myCommand = new MySqlCommand(CommandText, myConnection);





            CommandText = "SELECT COUNT(*) FROM registration WHERE ID='"+ textBox3.Text.ToString() + "';";

             myConnection = new MySqlConnection(Connect);
             myCommand = new MySqlCommand(CommandText, myConnection);
            MySqlDataReader reader;

            myConnection.Open();
         
            int id = Convert.ToInt16(myCommand.ExecuteScalar().ToString());


            if (id == 0)

            {


                myCommand = new MySqlCommand("insert into registration(`ID`,`organization`,`number`,`key`,`balance`,`price`) values(" + textBox3.Text.ToString() + ",'" + comboBox2.Items[comboBox2.SelectedIndex].ToString() + "','" + textBox2.Text + "','"+textBox4.Text+"','0','0');", myConnection);




                myCommand.ExecuteScalar();
                myConnection.Close();
                MessageBox.Show("Пользователь успешно добавлен.");
            }
            else
            {
                MessageBox.Show("Данная карта была зарегестрирована ранее.");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            
           // comboBox2.Items.Add(textBox1.Text);
            comboBox2.Items[comboBox2.SelectedIndex] = textBox1.Text;
            comboBox2.Items.Add("Добавить новую организацию");
            button2.Enabled = false;
            textBox1.Enabled = false;
            //    comboBox2.SelectedIndex = comboBox2.SelectedIndex ;
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(comboBox2.SelectedIndex+1==comboBox2.Items.Count)
            {

                textBox1.Enabled = true;
                button2.Enabled = true;
            }
            else
            {
                textBox1.Enabled = false;
                button2.Enabled = false;
            }
        }
    }
}
