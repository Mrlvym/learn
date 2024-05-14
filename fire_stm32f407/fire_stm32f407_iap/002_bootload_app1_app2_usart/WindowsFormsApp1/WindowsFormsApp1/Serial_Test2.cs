using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    //【3.1】声明委托（测试串口2）
    public delegate void dealSeriaTest2_data(byte[] data);

    internal class Serial_Test2
    {
        //实例化一个端口
        public SerialPort Serial_test2 = new SerialPort();

        //【3.2】定义委托（测试串口2）
        public dealSeriaTest2_data dealSerialTest2_Data;

        //测试串口2，连接函数接口
        public void Serial_Test2_Connect(string portName, int baud)
        {
            //打开测试串口2
            try
            {
                Serial_test2.PortName = portName;    //端口号 
                Serial_test2.BaudRate = baud;    //波特率
                Serial_test2.Parity = Parity.None;        //校验位
                Serial_test2.DataBits = 8;    //数据位
                Serial_test2.StopBits = StopBits.One;    //停止位

                //设置测试串口2，接收事件
                Serial_test2.DataReceived += Serial_Test2_DataReceived;

                Serial_test2.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口2" + ex.Message);
            }
        }

        public void Serial_Test2_Connect_1(string portName, int baud)
        {
            //打开测试串口2
            try
            {
                Serial_test2.PortName = portName;    //端口号 
                Serial_test2.BaudRate = baud;    //波特率
                Serial_test2.Parity = Parity.None;        //校验位
                Serial_test2.DataBits = 8;    //数据位
                Serial_test2.StopBits = StopBits.One;    //停止位

                //设置测试串口2，接收事件
                Serial_test2.DataReceived += Serial_Test2_DataReceived;

                Serial_test2.Open();
            }
            catch (Exception ex)
            {
            }
        }

        //测试串口2，断开连接函数接口
        public void Serial_Test2_disConnect()
        {
            //关闭测试串口2
            try
            {
                Serial_test2.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口2" + ex.Message);
            }
        }

        //检测测试串口2，是否打开的函数
        public bool Serial_Test2_Isopen()
        {
            if (Serial_test2.IsOpen == true)
                return true;
            else
                return false;
        }

        //测试串口2，发送函数接口
        public void Serial_Test2_Send(byte[] data)
        {
            try
            {
                Serial_test2.Write(data, 0, data.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口2" + ex.Message);
            }
        }


        //测试串口2，接收数据事件
        private void Serial_Test2_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = Serial_test2.BytesToRead;
            byte[] data = new byte[bytesToRead];

            ////接收串口数据
            Serial_test2.Read(data, 0, bytesToRead);

            //System.Threading.Thread.Sleep(1);
            if (data == null || data.Length == 0)
            {
                return;
            }

            //【3.5】调用委托（测试串口2）
            dealSerialTest2_Data(data);
        }
    }
}
