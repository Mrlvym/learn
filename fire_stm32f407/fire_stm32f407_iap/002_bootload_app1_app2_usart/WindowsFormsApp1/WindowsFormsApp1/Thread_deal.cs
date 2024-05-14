using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Mechanical_arm
{
    //[1.1]声明委托（机械臂串口接收线程委托）
    public delegate void jxb_serial_rec();

    //[3.1]声明委托（tcp客户端接收线程委托）
    public delegate void tcp_client_rec_pic();

    //[4.1]声明委托（开始测试线程委托）
    public delegate void start_test();

    //[6.1]声明委托（行程控制串口接收线程委托）
    public delegate void xc_serial_rec();

    //[8.1]声明委托（模组测试串口接收线程委托）
    public delegate void mozu_test_serial_rec();

    //[10.1]声明委托（模组串口接收线程委托）
    public delegate void mozu_serial_rec();

    public class Thread_deal
    {
        //[1.2]定义委托（机械臂串口接收线程委托）
        public jxb_serial_rec do_jxb_serial_rec;

        //定义机械臂串口接收线程
        public Thread thread_jxb_serial_rec;

        //创建机械臂串口接收线程接口
        public void creat_jxb_serial_rec()
        {
            thread_jxb_serial_rec = new Thread(new ThreadStart(do_jxb_serial_rec_thread));
        }

        //机械臂串口接收线程执行函数
        private void do_jxb_serial_rec_thread()
        {
            //[1.5]调用委托（机械臂串口接收线程委托）
            do_jxb_serial_rec();
        }

        //[3.2]定义委托（tcp客户端接收线程委托）
        public tcp_client_rec_pic do_tcp_client_rec_pic;

        //定义tcp客户端pic接收线程
        public Thread thread_tcp_client_rec_pic;

        //创建tcp客户端pic接收线程
        public void creat_tcp_client_rec_pic()
        {
            thread_tcp_client_rec_pic = new Thread(new ThreadStart(do_tcp_client_rec_pic_thread));
        }

        //tcp客户端pic接收线程执行函数
        private void do_tcp_client_rec_pic_thread()
        {
            //[3.5]调用委托（tcp客户端接收线程委托）
            do_tcp_client_rec_pic();
        }

        //[4.2]定义委托（开始测试线程委托）
        public start_test do_start_test;

        //定义开始测试线程
        public Thread thread_start_test;

        //创建开始测试线程
        public void creat_start_test()
        {
            thread_start_test = new Thread(new ThreadStart(do_start_test_thread));
        }

        //开始测试线程执行函数
        private void do_start_test_thread()
        {
            //[4.5]调用委托（开始测试线程委托）
            do_start_test();
        }

        //[6.2]定义委托（行程控制串口接收线程委托）
        public xc_serial_rec do_xc_serial_rec;

        //定义开始测试线程
        public Thread thread_xc_serial_rec;

        //创建行程控制串口接收线程
        public void creat_xc_serial_rec()
        {
            thread_xc_serial_rec = new Thread(new ThreadStart(do_xc_serial_rec_thread));
        }

        //行程控制串口接收线程执行函数
        private void do_xc_serial_rec_thread()
        {
            //[6.5]调用委托（行程控制串口接收线程委托）
            do_xc_serial_rec();
        }


        //[8.2]定义委托（模组测试串口接收线程委托）
        public mozu_test_serial_rec do_mozu_test_serial_rec;

        //定义开始测试线程
        public Thread thread_mozu_test_serial_rec;

        //创建模组测试串口接收线程
        public void creat_mozu_test_serial_rec()
        {
            thread_mozu_test_serial_rec = new Thread(new ThreadStart(do_mozu_test_serial_rec_thread));
        }

        //模组测试串口接收线程执行函数
        private void do_mozu_test_serial_rec_thread()
        {
            //[8.5]调用委托（模组测试串口接收线程委托）
            do_mozu_test_serial_rec();
        }

        //[10.2]定义委托（模组串口接收线程委托）
        public mozu_test_serial_rec do_mozu_serial_rec;

        //定义开始测试线程
        public Thread thread_mozu_serial_rec;

        //创建模组串口接收线程
        public void creat_mozu_serial_rec()
        {
            thread_mozu_serial_rec = new Thread(new ThreadStart(do_mozu_serial_rec_thread));
        }

        //模组串口接收线程执行函数
        private void do_mozu_serial_rec_thread()
        {
            //[10.5]调用委托（模组串口接收线程委托）
            do_mozu_serial_rec();
        }
    }
}
