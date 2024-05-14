using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    //【2.1】声明委托（测试串口1）
    public delegate void dealSeriaTest1_data(byte[] data);

    internal class Serial_Test1
    {
        //实例化一个端口
        public SerialPort Serial_test1 = new SerialPort();

        //【2.2】定义委托（测试串口1）
        public dealSeriaTest1_data dealSerialTest1_Data;

        //测试串口1，连接函数接口
        public void Serial_Test1_Connect(string portName, int baud)
        {
            //打开测试串口1
            try
            {
                Serial_test1.PortName = portName;    //端口号 
                Serial_test1.BaudRate = baud;    //波特率
                Serial_test1.Parity = Parity.None;        //校验位
                Serial_test1.DataBits = 8;    //数据位
                Serial_test1.StopBits = StopBits.One;    //停止位

                //设置测试串口1，接收事件
                Serial_test1.DataReceived += Serial_Test1_DataReceived;

                Serial_test1.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口1" + ex.Message);
            }
        }

        public void Serial_Test1_Connect_1(string portName, int baud)
        {
            //打开测试串口1
            try
            {
                Serial_test1.PortName = portName;    //端口号 
                Serial_test1.BaudRate = baud;    //波特率
                Serial_test1.Parity = Parity.None;        //校验位
                Serial_test1.DataBits = 8;    //数据位
                Serial_test1.StopBits = StopBits.One;    //停止位

                //设置测试串口1，接收事件
                Serial_test1.DataReceived += Serial_Test1_DataReceived;

                Serial_test1.Open();
            }
            catch (Exception ex)
            {
            }
        }

        //测试串口1，断开连接函数接口
        public void Serial_Test1_disConnect()
        {
            //关闭测试串口1
            try
            {
                Serial_test1.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口1" + ex.Message);
            }
        }

        //检测测试串口1，是否打开的函数
        public bool Serial_Test1_Isopen()
        {
            if (Serial_test1.IsOpen == true)
                return true;
            else
                return false;
        }

        //测试串口1，发送函数接口
        public void Serial_Test1_Send(byte[] data)
        {
            try
            {
                Serial_test1.Write(data, 0, data.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口1" + ex.Message);
            }
        }


        //测试串口1，接收数据事件
        private void Serial_Test1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = Serial_test1.BytesToRead;
            byte[] data = new byte[bytesToRead];

            ////接收串口数据
            Serial_test1.Read(data, 0, bytesToRead);

            //System.Threading.Thread.Sleep(1);
            if (data == null || data.Length == 0)
            {
                return;
            }

            //【2.5】调用委托（测试串口1）
            dealSerialTest1_Data(data);
        }
    }
}
