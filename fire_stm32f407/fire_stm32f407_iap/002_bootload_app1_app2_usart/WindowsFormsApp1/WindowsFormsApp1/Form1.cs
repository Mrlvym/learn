using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        /* 配置文件接口开始 */
        [DllImport("kernel32")]// 读配置文件方法的6个参数：所在的分区（section）、 键值、     初始缺省值、   StringBuilder、  参数长度上限 、配置文件路径
        public static extern long GetPrivateProfileString(string section, string key, string defaultValue, StringBuilder retVal, int size, string filePath);
        [DllImport("kernel32")]//写入配置文件方法的4个参数：  所在的分区（section）、  键值、     参数值、       配置文件路径
        private static extern long WritePrivateProfileString(string section, string key, string value, string filePath);
        //获取配置文件内容
        public static string GetValue(string section, string key)
        {
            // ▼ 获取当前程序启动目录
            // string strPath = Application.StartupPath + @"/config.ini"; 这里是绝对路径
            string strPath = "./config.ini";  //这里是相对路径
            if (File.Exists(strPath))  //检查是否有配置文件，并且配置文件内是否有相关数据。
            {
                StringBuilder sb = new StringBuilder(255);
                GetPrivateProfileString(section, key, "配置文件不存在，读取未成功!", sb, 255, strPath);

                return sb.ToString();
            }
            else
            {
                return string.Empty;
            }
        }
        //写入配置文件内容
        public static void SetValue(string section, string key, string value)
        {
            // ▼ 获取当前程序启动目录
            // string strPath = Application.StartupPath + @"/config.ini";  这里是绝对路径
            string strPath = "./config.ini";      //这里是相对路径，
            WritePrivateProfileString(section, key, value, strPath);
        }

        //写入当前配置
        private void write_config()
        {
            //静电三轴平台
            //测试串口1
            SetValue("测试串口1", "comboBox_test1_com", comboBox_test1_com.Text);
            SetValue("测试串口1", "comboBox_test1_baud", comboBox_test1_baud.Text);

            SetValue("测试串口1", "textBox_gj_addr", textBox_gj_addr.Text);

            SetValue("测试串口1", "radioButton_luoji", (radioButton_luoji.Checked).ToString());
            SetValue("测试串口1", "radioButton_freertos", (radioButton_freertos.Checked).ToString());
        }

        private void read_config()
        {
            //测试串口1
            comboBox_test1_com.Text = GetValue("测试串口1", "comboBox_test1_com");
            comboBox_test1_baud.Text = GetValue("测试串口1", "comboBox_test1_baud");

            textBox_gj_addr.Text = GetValue("测试串口1", "textBox_gj_addr");

            radioButton_luoji.Checked = Convert.ToBoolean(GetValue("测试串口1", "radioButton_luoji"));
            radioButton_freertos.Checked = Convert.ToBoolean(GetValue("测试串口1", "radioButton_freertos"));
        }

        public Form1()
        {
            InitializeComponent();
        }

        //窗口加载
        private void Form1_Load(object sender, EventArgs e)
        {
            read_config();

            //测试串口1,自动获取当前电脑串口列表
            comboBox_test1_com.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
            //if (comboBox_test1_com.Text == "") button_Motor.Enabled = false;

            AddLogInfo("上位机打开成功！");
        }

        //窗口关闭中
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            write_config();

            if (thread_serial_test1_rec == null || thread_serial_test1_rec.IsAlive == true)
            {
                if (thread_serial_test1_rec != null)
                {
                    thread_serial_test1_rec.Abort();
                }
            }

            if (thread_download_test == null || thread_download_test.IsAlive == true)
            {
                if (thread_download_test != null)
                {
                    thread_download_test.Abort();
                }
            }
        } 

        //添加log信息接口
        private void AddLogInfo(string info)
        {
            string log_info = DateTime.Now.ToString("HH:mm:ss.fff") + ":" + info;
            if (listBox_log.InvokeRequired)
            {
                listBox_log.Invoke(new Action(() =>
                {
                    listBox_log.Items.Insert(0, log_info);
                }
                ));
            }
            else
            {
                listBox_log.Items.Insert(0, log_info);
            }
        }

        //清除log信息
        private void button_log_clear_Click(object sender, EventArgs e)
        {
            listBox_log.Items.Clear();
        }

        //刷新所有串口com号
        private void button_Com_Flash_Click(object sender, EventArgs e)
        {
            //测试串口1,自动获取当前电脑串口列表
            comboBox_test1_com.Items.Clear();
            comboBox_test1_com.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
        }


        /*********************测试串口1(静电xyz轴平台)相关函数**********************/

        //测试串口1（静电三轴平台）
        Serial_Test1 serial_test1 = new Serial_Test1();
        RingBuff serial_test1_rb = new RingBuff();
        Thread thread_serial_test1_rec = null;

        Thread thread_download_test = null;

        //测试串口1连接按键按下
        //private void button_test1_Click_1(object sender, EventArgs e)
        private void button_test1_Click(object sender, EventArgs e)
        {
            string portname;
            int buad = 0;

            try
            {
                //测试串口1已打开
                if (serial_test1.Serial_Test1_Isopen())
                {
                    button_test1.Text = "Connect";
                    //button_test1.BackColor = Color.White;

                    textBox_test1_led.BackColor = Color.White;

                    comboBox_test1_com.Enabled = true;
                    comboBox_test1_baud.Enabled = true;

                    serial_test1.Serial_Test1_disConnect();

                    //终止线程
                    thread_serial_test1_rec.Abort();

                    //log提示信息
                    AddLogInfo("测试串口1 关闭成功");
                }
                else //已关闭
                {
                    //清空环形缓冲区
                    serial_test1_rb.clear_ringbuff();

                    portname = comboBox_test1_com.Text;

                    if (!int.TryParse(comboBox_test1_baud.Text, out buad))
                    {
                        AddLogInfo("comboBox_test1_baud 转换 int 失败");
                    }

                    serial_test1.Serial_Test1_Connect(portname, buad);

                    if (serial_test1.Serial_Test1_Isopen() == true)
                    {
                        button_test1.Text = "DisConnect";
                        //button_test1.BackColor = Color.Green;

                        textBox_test1_led.BackColor = Color.Green;

                        comboBox_test1_com.Enabled = false;
                        comboBox_test1_baud.Enabled = false;

                        //【2.4】绑定委托（测试串口1）
                        serial_test1.dealSerialTest1_Data = dealSerialTest1_Data;


                        //创建线程
                        thread_serial_test1_rec = new Thread(new ThreadStart(do_serial_test1_rec_thread));
                        //开始线程
                        thread_serial_test1_rec.Start();

                        AddLogInfo("测试串口1 打开成功");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        byte[] tmp_buf = new byte[256];
        byte[] mh_imu_buf = new byte[100];
        byte[] curVal = new byte[1];
        byte[] lastVal = new byte[1];
        byte count = 0;
        byte data_len1 = 0;
        bool hand_flag = false;
        //private void get_serial_test1_msg()
        //{
        //    byte[] t_imu_buf = new byte[100];
        //    string info = string.Empty;
        //    if (serial_test1_rb.rb_can_read() > 0)
        //    {
        //        serial_test1_rb.rb_read_buf(serial_test1_rb.buf, curVal, 1);
        //        if ((lastVal[0] == 0xef) && (curVal[0] == 0xaa))
        //        {
        //            tmp_buf[0] = 0xef;
        //            tmp_buf[1] = 0xaa;
        //            count = 2;
        //            return;
        //        }
        //        tmp_buf[count] = curVal[0];
        //        count++;
        //        lastVal[0] = curVal[0];

        //        if (count == 5)
        //        {
        //            data_len1 = (byte)((tmp_buf[3] << 8) | tmp_buf[4]);
        //            if (data_len1 + 6 >= 100)
        //            {
        //                lastVal[0] = 0;
        //                curVal[0] = 0;
        //                count = 0;

        //                return;
        //            }
        //            //return;
        //        }

        //        if (count == 5 && tmp_buf[0] == 0xef && tmp_buf[1] == 0xaa)
        //        {
        //            while ((serial_test1_rb.rb_can_read() < data_len1 + 1)) ;

        //            serial_test1_rb.rb_read_buf(serial_test1_rb.buf, t_imu_buf, (UInt16)(data_len1 + 1));

        //            Buffer.BlockCopy(t_imu_buf, 0, tmp_buf, 5, data_len1 + 1);

        //            Buffer.BlockCopy(tmp_buf, 0, mh_imu_buf, 0, data_len1 + 6);

        //            Deal_XYZ_Buf(mh_imu_buf);

        //            lastVal[0] = 0;
        //            curVal[0] = 0;
        //            count = 0;

        //            return;
        //        }
        //    }
        //}
        private void get_serial_test1_msg()
        {
            byte[] t_imu_buf = new byte[256];
            string info = string.Empty;
            if (serial_test1_rb.rb_can_read() > 0)
            {
                serial_test1_rb.rb_read_buf(serial_test1_rb.buf, curVal, 1);
                //if (((lastVal[0] == 'B') && (curVal[0] == 'o')) && hand_flag == false)
                if (((lastVal[0] == 0x42) && (curVal[0] == 0x6F)) && hand_flag == false)
                //if (((lastVal[0] == 0x44) && (curVal[0] == 0x65)) && hand_flag == false)
                {
                    tmp_buf[0] = (byte)0x42;
                    tmp_buf[1] = (byte)0x6F;
                    count = 2;
                    hand_flag = true;
                    return;
                }
                else if (((lastVal[0] == 'A') && (curVal[0] == 'p')) && hand_flag == false)
                {
                    tmp_buf[0] = (byte)'A';
                    tmp_buf[1] = (byte)'p';
                    count = 2;
                    hand_flag = true;
                    return;
                }

                tmp_buf[count] = curVal[0];
                count++;
                
                //if (((lastVal[0] == '\r') && (curVal[0] == '\n')) && hand_flag == true)
                if (((lastVal[0] == 0x0d) && (curVal[0] == 0x0a)) && hand_flag == true)
                {
                    //Buffer.BlockCopy(tmp_buf, 0, mh_imu_buf, 0, count);

                    info = System.Text.Encoding.UTF8.GetString(tmp_buf);
                    Array.Clear(tmp_buf, 0, tmp_buf.Length);

                    if (info == "App0 Check App Addr error ... \r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" || info == "App1 Check App Addr error ... \r\n\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0")
                    {
                        AddLogInfo("固件下载失败。。。\r\n");

                        if (button_download_gj.InvokeRequired)
                        {
                            button_download_gj.Invoke(new Action(() =>
                            {
                                button_download_gj.Text = "下载";
                            }
                            ));
                        }
                        else
                        {
                            button_download_gj.Text = "下载";
                        }

                        thread_download_test.Abort();
                    }

                    AddLogInfo(info);

                    lastVal[0] = 0;
                    curVal[0] = 0;
                    count = 0;
                    hand_flag = false;
                }
                lastVal[0] = curVal[0];
            }
        }

        //【2.3】实现委托（测试串口1）测试串口1接收函数
        private void dealSerialTest1_Data(byte[] data)
        {
            if (serial_test1_rb.rb_can_write() > data.Length)
            {
                UInt16 ret = serial_test1_rb.rb_write_buf(serial_test1_rb.buf, data, (UInt16)data.Length);
                if (ret == 0)
                {
                    AddLogInfo("test1 ringbuf write error !");
                }
            }
            else
            {
                AddLogInfo("test1 ringbuf is full !");
            }
        }

        //处理测试串口1接收数据线程
        private void do_serial_test1_rec_thread()
        {
            string portName = string.Empty;
            int buad = 0;
            if (comboBox_test1_com.InvokeRequired)
            {
                comboBox_test1_com.Invoke(new Action(() =>
                {
                    portName = comboBox_test1_com.Text;
                }
                ));
            }
            else
            {
                portName = comboBox_test1_com.Text;
            }

            if (comboBox_test1_baud.InvokeRequired)
            {
                comboBox_test1_baud.Invoke(new Action(() =>
                {
                    if (!int.TryParse(comboBox_test1_baud.Text, out buad))
                    {
                        AddLogInfo("comboBox_test1_baud 转换 int 失败");
                    }
                }
                ));
            }
            else
            {
                if (!int.TryParse(comboBox_test1_baud.Text, out buad))
                {
                    AddLogInfo("comboBox_test1_baud 转换 int 失败");
                }
            }

            while (true)
            {
                if (serial_test1.Serial_Test1_Isopen() == false)
                {
                    AddLogInfo("测试串口1断开，请重新连接！");
                    //尝试打开串口
                    serial_test1.Serial_Test1_Connect_1(portName, buad);
                    if (serial_test1.Serial_Test1_Isopen() == true)
                    {
                        AddLogInfo("测试串口1重连成功 ！");
                    }
                }

                get_serial_test1_msg();
            }
        }


        public static bool CheckSum(byte[] data, byte len)
        {
            bool ret = false;
            byte cs = 0;
            for (int i = 2; i < len - 1; i++)
            {
                cs = (byte)(cs + data[i]);
                //cs = (byte)((cs + data[i]) % 256);
            }
            //cs = (byte)(~cs + 1);
            if (cs == data[len - 1])
            {
                ret = true;
            }
            else
            {
                ret = false;
            }
            return ret;
        }

        public static byte CheckSum_1(byte[] data, byte len)
        {
            byte cs = 0;
            for (int i = 2; i < len - 1; i++)
            {
                cs = (byte)(cs + data[i]);
                //cs = (byte)((cs + data[i]) % 256);
            }
            //cs = (byte)(~cs + 1);
            return cs;
        }

        public static byte CheckSum_2(byte[] data, UInt32 len)
        {
            byte cs = 0;
            for (int i = 0; i < len; i++)
            {
                cs = (byte)(cs + data[i]);
                //cs = (byte)((cs + data[i]) % 256);
            }
            //cs = (byte)(~cs + 1);
            return cs;
        }

        //打开固件按键按下
        private void button_open_gj_addr_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog(); 
            fileDialog.Multiselect = true;
            fileDialog.Title = "请选择文件";
            //fileDialog.Filter = "所有文件(*.*)|*.*";
            fileDialog.Filter = "bin文件(*.bin*)|*.bin*";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                string file = fileDialog.FileName;
                //MessageBox.Show("已选择文件:" + file, "选择文件提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                textBox_gj_addr.Text = file;
                AddLogInfo("文件选择：" + file);
            }
        }

        //开始下载按键按下
        private void button_download_gj_Click(object sender, EventArgs e)
        {

            if (serial_test1.Serial_Test1_Isopen())
            {
                if (button_download_gj.Text == "下载")
                {
                    button_download_gj.Text = "停止";

                    //创建线程
                    thread_download_test = new Thread(new ThreadStart(do_download_test_thread));
                    //开始线程
                    thread_download_test.Start();
                }
                else if (button_download_gj.Text == "停止")
                {
                    button_download_gj.Text = "下载";

                    thread_download_test.Abort();

                    set_progressBar_Value((int)0);
                }
            }
            else
            {
                AddLogInfo("请先打开串口。。。");
            }
            
        }

        private void do_download_test_thread()
        {
            byte[] bin_data;
            string file = textBox_gj_addr.Text;
            UInt32 data_len = 0;
            byte check_sum = 0;

            bin_data = ReadBinaryFile(file);

            //发送iap start cmd
            Send_Iap_Start_Cmd();
            Thread.Sleep(1000);

            //发送data_len
            data_len = (UInt32)bin_data.Length;
            Send_Iap_DataLen(data_len);
            Thread.Sleep(1000);

            //发送check sum
            check_sum = CheckSum_2(bin_data, data_len);
            Send_Iap_CheckSum(check_sum);
            Thread.Sleep(1000);

            //发送固件数据
            Send_Iap_binData(bin_data);
            Thread.Sleep(1000);

            //发送iap stop cmd
            Send_Iap_Stop_Cmd();


            Thread.Sleep(1000);
            if (button_download_gj.InvokeRequired)
            {
                button_download_gj.Invoke(new Action(() =>
                {
                    button_download_gj.Text = "下载";
                }
                ));
            }
            else
            {
                button_download_gj.Text = "下载";
            }

            set_progressBar_Value((int)0);

            AddLogInfo("固件下载成功。。。");
        }

        private void Send_Iap_Start_Cmd()
        { 
            byte[] iap_start_cmd = new byte[6]{ 0xEF, 0xAA, 0x01, 0x00, 0x00, 0x00};

            iap_start_cmd[5] = CheckSum_1(iap_start_cmd, 6);

            serial_test1.Serial_Test1_Send(iap_start_cmd);
        }

        private void Send_Iap_Stop_Cmd()
        {
            byte[] iap_stop_cmd = new byte[6] { 0xEF, 0xAA, 0x02, 0x00, 0x00, 0x00 };

            iap_stop_cmd[5] = CheckSum_1(iap_stop_cmd, 6);

            serial_test1.Serial_Test1_Send(iap_stop_cmd);
        }

        private void Send_Iap_DataLen(UInt32 data_len)
        {
            byte[] iap_datalen_cmd = new byte[10] { 0xEF, 0xAA, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,0x00 };

            iap_datalen_cmd[5] = (byte)((data_len >> 24) & 0xff);
            iap_datalen_cmd[6] = (byte)((data_len >> 16) & 0xff);
            iap_datalen_cmd[7] = (byte)((data_len >> 8)  & 0xff);
            iap_datalen_cmd[8] = (byte)((data_len >> 0)  & 0xff);

            iap_datalen_cmd[9] = CheckSum_1(iap_datalen_cmd, 10);

            serial_test1.Serial_Test1_Send(iap_datalen_cmd);
        }

        private void Send_Iap_CheckSum(byte checksum)
        {
            byte[] iap_checksum_cmd = new byte[7] { 0xEF, 0xAA, 0x04, 0x00, 0x01, 0x00, 0x00 };

            iap_checksum_cmd[5] = checksum;

            iap_checksum_cmd[6] = CheckSum_1(iap_checksum_cmd, 7);

            serial_test1.Serial_Test1_Send(iap_checksum_cmd);
        }

        private void Send_Iap_binData(byte[] binData)
        {
            UInt32 datalen = 0, i = 0;
            UInt16 len = 0;

            byte[] iap_bin_data = new byte[206];

            datalen = (UInt32)binData.Length;

            iap_bin_data[0] = 0xEF;
            iap_bin_data[1] = 0xAA;
            iap_bin_data[2] = 0x05;

            len = 200;
            set_progressBar_Maximum((int)(datalen / len));
            for (i = 0; i < datalen / len; i++) 
            {
                iap_bin_data[3] = (byte)((len >> 8) & 0xff);
                iap_bin_data[4] = (byte)((len >> 0) & 0xff);

                Buffer.BlockCopy(binData, (int)(i*len), iap_bin_data, 5, len);

                iap_bin_data[205] = CheckSum_1(iap_bin_data, 206);

                serial_test1.Serial_Test1_Send(iap_bin_data);

                set_progressBar_Value((int)i);

                if (radioButton_freertos.Checked == true) {
                    Thread.Sleep(800);
                }
                
            }
            len = (UInt16)(datalen % len);
            if (len != 0)
            {
                byte[] iap_bin_data1 = new byte[len+6];

                iap_bin_data1[0] = 0xEF;
                iap_bin_data1[1] = 0xAA;
                iap_bin_data1[2] = 0x05;

                iap_bin_data1[3] = (byte)((len >> 8) & 0xff);
                iap_bin_data1[4] = (byte)((len >> 0) & 0xff);

                Buffer.BlockCopy(binData, (int)(datalen - len), iap_bin_data1, 5, len);

                iap_bin_data1[len + 5] = CheckSum_1(iap_bin_data1, (byte)(len + 6));

                serial_test1.Serial_Test1_Send(iap_bin_data1);

                set_progressBar_Value((int)i);
            }
        }

        private void set_progressBar_Maximum(int maximun)
        {       
            if (progressBar_dowmload_jd.InvokeRequired)
            {
                progressBar_dowmload_jd.Invoke(new Action(() =>
                {
                    progressBar_dowmload_jd.Maximum = maximun;
                }
                ));
            }
            else
            {
                progressBar_dowmload_jd.Maximum = maximun;
            }
        }

        private void set_progressBar_Value(int val)
        {   
            if (progressBar_dowmload_jd.InvokeRequired)
            {
                progressBar_dowmload_jd.Invoke(new Action(() =>
                {
                    progressBar_dowmload_jd.Value = val;
                }
                ));
            }
            else
            {
                progressBar_dowmload_jd.Value = val;
            }
        }

        private byte[] ReadBinaryFile(string filePath)
        {
            byte[] fileContent;
            // 确保文件存在
            if (!File.Exists(filePath))
            {
                AddLogInfo("文件不存在");
                return BitConverter.GetBytes(0);
            }

            // 使用FileStream和BinaryReader读取文件
            using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read))
            {
                using (BinaryReader br = new BinaryReader(fs))
                {
                    // 读取文件全部内容
                    fileContent = br.ReadBytes((int)fs.Length);

                    return fileContent;

                    //// 输出读取到的内容（可选）
                    //Console.WriteLine(BitConverter.ToString(fileContent));
                }
            }
        }
    }
}
