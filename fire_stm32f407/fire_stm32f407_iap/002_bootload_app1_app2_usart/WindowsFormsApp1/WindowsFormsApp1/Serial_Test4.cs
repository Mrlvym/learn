using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    //【5.1】声明委托（测试串口4）
    public delegate void dealSeriaTest4_data(byte[] data);

    internal class Serial_Test4
    {
        //实例化一个端口
        public SerialPort Serial_test4 = new SerialPort();

        //【5.2】定义委托（测试串口4）
        public dealSeriaTest4_data dealSerialTest4_Data;

        //测试串口4，连接函数接口
        public void Serial_Test4_Connect(string portName, int baud)
        {
            //打开测试串口4
            try
            {
                Serial_test4.PortName = portName;    //端口号 
                Serial_test4.BaudRate = baud;    //波特率
                Serial_test4.Parity = Parity.None;        //校验位
                Serial_test4.DataBits = 8;    //数据位
                Serial_test4.StopBits = StopBits.One;    //停止位

                //设置测试串口4，接收事件
                Serial_test4.DataReceived += Serial_Test3_DataReceived;

                Serial_test4.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口4" + ex.Message);
            }
        }

        public void Serial_Test4_Connect_1(string portName, int baud)
        {
            //打开测试串口4
            try
            {
                Serial_test4.PortName = portName;    //端口号 
                Serial_test4.BaudRate = baud;    //波特率
                Serial_test4.Parity = Parity.None;        //校验位
                Serial_test4.DataBits = 8;    //数据位
                Serial_test4.StopBits = StopBits.One;    //停止位

                //设置测试串口4，接收事件
                Serial_test4.DataReceived += Serial_Test3_DataReceived;

                Serial_test4.Open();
            }
            catch (Exception ex)
            {
            }
        }

        //测试串口4，断开连接函数接口
        public void Serial_Test4_disConnect()
        {
            //关闭测试串口4
            try
            {
                Serial_test4.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口4" + ex.Message);
            }
        }

        //检测测试串口4，是否打开的函数
        public bool Serial_Test4_Isopen()
        {
            if (Serial_test4.IsOpen == true)
                return true;
            else
                return false;
        }

        //测试串口4，发送函数接口
        public void Serial_Test4_Send(byte[] data)
        {
            try
            {
                Serial_test4.Write(data, 0, data.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show("测试串口4" + ex.Message);
            }
        }


        //测试串口4，接收数据事件
        private void Serial_Test3_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = Serial_test4.BytesToRead;
            byte[] data = new byte[bytesToRead];

            ////接收串口数据
            Serial_test4.Read(data, 0, bytesToRead);

            //System.Threading.Thread.Sleep(1);
            if (data == null || data.Length == 0)
            {
                return;
            }

            //【5.5】调用委托（测试串口3）
            dealSerialTest4_Data(data);
        }
    }
}
