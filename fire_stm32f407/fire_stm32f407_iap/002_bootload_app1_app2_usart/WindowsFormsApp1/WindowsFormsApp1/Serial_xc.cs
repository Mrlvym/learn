using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    //【1.1】声明委托（电机串口）
    public delegate void dealSeriaMotor_data(byte[] data);

    internal class Serial_MOTOR
    {
        //实例化一个端口
        public SerialPort Serial_Motor = new SerialPort();

        //【1.2】定义委托（电机串口）
        public dealSeriaMotor_data dealSerialMotor_Data;

        //电机串口，连接函数接口
        public void Serial_Motor_Connect(string portName)
        {
            //打开电机串口
            try
            {
                Serial_Motor.PortName = portName;    //端口号
                Serial_Motor.BaudRate = 115200;    //波特率
                Serial_Motor.Parity = Parity.None;        //校验位
                Serial_Motor.DataBits = 8;    //数据位
                Serial_Motor.StopBits = StopBits.One;    //停止位

                //设置电机串口，接收事件
                Serial_Motor.DataReceived += Serial_Motor_DataReceived;

                Serial_Motor.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("电机"+ex.Message);
            }
        }

        public void Serial_xc_Connect_1(string portName)
        {
            //打开电机串口
            try
            {
                Serial_Motor.PortName = portName;    //端口号
                Serial_Motor.BaudRate = 115200;    //波特率
                Serial_Motor.Parity = Parity.None;        //校验位
                Serial_Motor.DataBits = 8;    //数据位
                Serial_Motor.StopBits = StopBits.One;    //停止位

                //设置电机串口，接收事件
                Serial_Motor.DataReceived += Serial_Motor_DataReceived;

                Serial_Motor.Open();
            }
            catch (Exception ex)
            {
            }
        }
        //电机串口，断开连接函数接口
        public void Serial_Motor_disConnect()
        {
            //关闭电机串口
            try
            {
                Serial_Motor.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("电机"+ex.Message);
            }
        }

        //检测电机串口，是否打开的函数
        public bool Serial_Motor_Isopen()
        {
            if (Serial_Motor.IsOpen == true)
                return true;
            else
                return false;
        }

        //电机串口，发送函数接口
        public void Serial_Motor_Send(byte[] data)
        {
            try
            {
                Serial_Motor.Write(data, 0, data.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show("电机"+ex.Message);
            }
        }


        //电机串口，接收数据事件
        private void Serial_Motor_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = Serial_Motor.BytesToRead;
            byte[] data = new byte[bytesToRead];

            ////接收串口数据
            Serial_Motor.Read(data, 0, bytesToRead);

            //System.Threading.Thread.Sleep(1);
            if (data == null || data.Length == 0)
            {
                return;
            }

            //【1.5】调用委托（电机串口）
            dealSerialMotor_Data(data);
        }
    }

}
