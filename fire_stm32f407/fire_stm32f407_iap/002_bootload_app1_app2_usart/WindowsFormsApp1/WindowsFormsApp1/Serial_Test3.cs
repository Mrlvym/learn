using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    //【4.1】声明委托（测试串口3）
    public delegate void dealSeriaTest3_data(byte[] data);

    internal class Serial_Test3
    {
        //实例化一个端口
        public SerialPort Serial_test3 = new SerialPort();

        //【4.2】定义委托（测试串口3）
        public dealSeriaTest3_data dealSerialTest3_Data;

        //测试串口3，连接函数接口
        public void Serial_Test3_Connect(string portName, int baud)
        {
            //打开测试串口3
            try
            {
                Serial_test3.PortName = portName;    //端口号 
                Serial_test3.BaudRate = baud;    //波特率
                Serial_test3.Parity = Parity.None;        //校验位
                Serial_test3.DataBits = 8;    //数据位
                Serial_test3.StopBits = StopBits.One;    //停止位

                //设置测试串口3，接收事件
                Serial_test3.DataReceived += Serial_Test3_DataReceived;

                Serial_test3.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口3" + ex.Message);
            }
        }

        public void Serial_Test3_Connect_1(string portName, int baud)
        {
            //打开测试串口3
            try
            {
                Serial_test3.PortName = portName;    //端口号 
                Serial_test3.BaudRate = baud;    //波特率
                Serial_test3.Parity = Parity.None;        //校验位
                Serial_test3.DataBits = 8;    //数据位
                Serial_test3.StopBits = StopBits.One;    //停止位

                //设置测试串口3，接收事件
                Serial_test3.DataReceived += Serial_Test3_DataReceived;

                Serial_test3.Open();
            }
            catch (Exception ex)
            {
            }
        }

        //测试串口3，断开连接函数接口
        public void Serial_Test3_disConnect()
        {
            //关闭测试串口2
            try
            {
                Serial_test3.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口3" + ex.Message);
            }
        }

        //检测测试串口3，是否打开的函数
        public bool Serial_Test3_Isopen()
        {
            if (Serial_test3.IsOpen == true)
                return true;
            else
                return false;
        }

        //测试串口3，发送函数接口
        public void Serial_Test3_Send(byte[] data)
        {
            try
            {
                Serial_test3.Write(data, 0, data.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口3" + ex.Message);
            }
        }


        //测试串口3，接收数据事件
        private void Serial_Test3_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = Serial_test3.BytesToRead;
            byte[] data = new byte[bytesToRead];

            ////接收串口数据
            Serial_test3.Read(data, 0, bytesToRead);

            //System.Threading.Thread.Sleep(1);
            if (data == null || data.Length == 0)
            {
                return;
            }

            //【4.5】调用委托（测试串口3）
            dealSerialTest3_Data(data);
        }
    }
}
