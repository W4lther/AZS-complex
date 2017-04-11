using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using System.Threading;
namespace WindowsFormsApp2
{
    public partial class Form1 : Form
    {
        Thread myThread;
        MySqlCommand myCommand;
        string temp = ""
            ;
        string temp1 = "";
        string Connect = "Database=azs;Data Source=127.0.0.1;User Id=root;Password=7720362";
        MySqlConnection myConnection;//= new MySqlConnection(Connect);
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            myConnection = new MySqlConnection(Connect);
          
            myThread = new Thread(check_connect);
            myConnection.Open();
            serialPort1.PortName = "COM30";
            serialPort1.BaudRate = 9600; //указываем скорость.
           // serialPort1.ReadTimeout = 500;
            //serialPort1.WriteTimeout = 500;
            serialPort1.DataBits = 8;
            serialPort1.Open(); //Открываем порт.
            try
            {
                myThread.Start();
            }
            catch(Exception ex)
            {
                myThread.Start();
            }
        }
        int rfid = 0;
        float balance = 0;
        float price = 0;
        float litres = 0;
        MySqlDataReader reader;
        void check_connect()
        {

          // serialPort1.ReadTimeout = 2000000000;
           
           // serialPort1.Open(); //Открываем порт.
            string a = "";
            string b = "";
            while (true)
            {
                int last = 0;
                a = serialPort1.ReadLine();
                string id = "";
                id=id + a[0]+ a[1] + a[2] + a[3];
                for (int i = 5; i < a.Length; i++)
                {
                    if(a[i]=='!')
                    {
                        last = i;
                        break;
                    }
                }
                b = "";

                if ((id) == "divi")
                {
                    for (int i = 5; i < last; i++)
                    {

                        if (a[i] != '!')
                        {

                            if (a[i] != '.')
                            {
                                b = b + a[i];
                            }
                            else
                            {
                                b = b + ',';

                            }

                        }

                    }


                    myCommand = new MySqlCommand("SELECT price FROM registration WHERE ID='" + rfid.ToString() + "';", myConnection);
                    reader = myCommand.ExecuteReader();
                    while (reader.Read())
                    {
                         temp = reader[0].ToString();
                         temp1 = "";
                        for (int i = 0; i < temp.Length; i++)
                        {
                            if (temp[i] == '.')
                            {
                                temp1 = temp1 + ',';
                            }
                            else
                            {
                                temp1 = temp1 + temp[i];
                            }
                        }
                        price = (float)Convert.ToDouble(temp1);
                    }
                    reader.Close(); 
                    float result = (float)Math.Floor((Convert.ToDouble(b) * price)*100)/100;
                    //------------------
                     temp = result.ToString();
                     temp1 = "";
                    for(int i=0;i<temp.Length;i++)
                    {
                        if(temp[i]==',')
                        {
                            temp1 = temp1+'.';
                        }
                        else {
                            temp1 = temp1 + temp[i];
                        }
                    }
                    myCommand = new MySqlCommand("UPDATE registration SET balance=balance-'"+temp1+"'   WHERE ID='" + rfid.ToString() + "';", myConnection);
                    myCommand.ExecuteNonQuery();
                    
                }
                if ((id) == "rfid")
                {
                    for (int i = 5; i < last; i++)
                    {

                        if (a[i] != '!')
                        {
                            b = b + a[i];
                        }

                    }
                        
                            rfid = Convert.ToInt32(b);

                            //------------------
                            myCommand = new MySqlCommand("SELECT balance FROM registration WHERE ID='" + rfid.ToString() + "';", myConnection);
                            reader = myCommand.ExecuteReader();
                            while (reader.Read())
                            {
                        string temp = reader[0].ToString();
                        string temp1 = "";
                        for (int i = 0; i < temp.Length; i++)
                        {
                            if (temp[i] == '.')
                            {
                                temp1 = temp1 + ',';
                            }
                            else
                            {
                                temp1 = temp1 + temp[i];
                            }
                        }
                        balance = (float)Convert.ToDouble(temp1);
                            }
                            reader.Close();
                            myCommand = new MySqlCommand("SELECT price FROM registration WHERE ID='" + rfid.ToString() + "';", myConnection);
                            reader = myCommand.ExecuteReader();
                            while (reader.Read())
                            {
                        string temp = reader[0].ToString();
                        string temp1 = "";
                        for (int i = 0; i < temp.Length; i++)
                        {
                            if (temp[i] == '.')
                            {
                                temp1 = temp1 + ',';
                            }
                            else
                            {
                                temp1 = temp1 + temp[i];
                            }
                        }
                        price = (float)Convert.ToDouble(temp1);
                            }
                            reader.Close();


                            //---------------
                            if (price == 0)
                            {
                                price = 1;
                            }
                            litres = balance / price;
                            string pincode = "!";
                            serialPort1.Write((Math.Round(litres, 2) * 100).ToString());
                            myCommand = new MySqlCommand("SELECT `key` FROM registration WHERE ID='" + rfid.ToString() + "';", myConnection);
                            reader = myCommand.ExecuteReader();
                            while (reader.Read())
                            {

                                pincode = "!" + reader[0].ToString();
                            }
                            reader.Close();
                            serialPort1.Write(pincode);
                            //break;
                        

                    

                }
                b = "";
            }


        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (myThread.IsAlive)
            {
                myThread.Abort();
                serialPort1.Close();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (!myThread.IsAlive)
            {
                myThread.Start();
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            myConnection.Close();
            
        }
    }
}
