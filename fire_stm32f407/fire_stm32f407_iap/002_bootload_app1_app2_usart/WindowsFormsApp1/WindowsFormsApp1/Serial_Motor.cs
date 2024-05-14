using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    //【1.1】声明委托（电机串口）
    public delegate void dealSeriaMotor_data(byte[] data);

    internal class Serial_Motor
    {
        //实例化一个端口
        public SerialPort Serial_motor = new SerialPort();

        //【1.2】定义委托（电机串口）
        public dealSeriaMotor_data dealSerialMotor_Data;

        //电机串口，连接函数接口
        public void Serial_Motor_Connect(string portName, int baud)
        {
            //打开电机串口
            try
            {
                Serial_motor.PortName = portName;    //端口号 
                Serial_motor.BaudRate = baud;    //波特率
                Serial_motor.Parity = Parity.None;        //校验位
                Serial_motor.DataBits = 8;    //数据位
                Serial_motor.StopBits = StopBits.One;    //停止位

                //设置电机串口，接收事件
                Serial_motor.DataReceived += Serial_Motor_DataReceived;

                Serial_motor.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("电机" + ex.Message);
            }
        }

        public void Serial_Motor_Connect_1(string portName, int baud)
        {
            //打开电机串口
            try
            {
                Serial_motor.PortName = portName;    //端口号 
                Serial_motor.BaudRate = baud;    //波特率
                Serial_motor.Parity = Parity.None;        //校验位
                Serial_motor.DataBits = 8;    //数据位
                Serial_motor.StopBits = StopBits.One;    //停止位

                //设置电机串口，接收事件
                Serial_motor.DataReceived += Serial_Motor_DataReceived;

                Serial_motor.Open();
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
                Serial_motor.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("电机" + ex.Message);
            }
        }

        //检测电机串口，是否打开的函数
        public bool Serial_Motor_Isopen()
        {
            if (Serial_motor.IsOpen == true)
                return true;
            else
                return false;
        }

        //电机串口，发送函数接口
        public void Serial_Motor_Send(byte[] data)
        {
            try
            {
                Serial_motor.Write(data, 0, data.Length);
            }
            catch (Exception ex)
            {
                MessageBox.Show("电机" + ex.Message);
            }
        }


        //电机串口，接收数据事件
        private void Serial_Motor_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = Serial_motor.BytesToRead;
            byte[] data = new byte[bytesToRead];

            ////接收串口数据
            Serial_motor.Read(data, 0, bytesToRead);

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
