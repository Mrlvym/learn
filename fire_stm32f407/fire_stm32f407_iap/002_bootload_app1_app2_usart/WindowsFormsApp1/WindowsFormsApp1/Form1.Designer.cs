namespace WindowsFormsApp1
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.listBox_log = new System.Windows.Forms.ListBox();
            this.button_log_clear = new System.Windows.Forms.Button();
            this.button_Com_Flash = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button_download_gj = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.progressBar_dowmload_jd = new System.Windows.Forms.ProgressBar();
            this.button_open_gj_addr = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_gj_addr = new System.Windows.Forms.TextBox();
            this.textBox_test1_led = new System.Windows.Forms.TextBox();
            this.button_test1 = new System.Windows.Forms.Button();
            this.comboBox_test1_baud = new System.Windows.Forms.ComboBox();
            this.comboBox_test1_com = new System.Windows.Forms.ComboBox();
            this.label26 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.radioButton_luoji = new System.Windows.Forms.RadioButton();
            this.radioButton_freertos = new System.Windows.Forms.RadioButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listBox_log
            // 
            this.listBox_log.FormattingEnabled = true;
            this.listBox_log.ItemHeight = 15;
            this.listBox_log.Location = new System.Drawing.Point(11, 173);
            this.listBox_log.Name = "listBox_log";
            this.listBox_log.Size = new System.Drawing.Size(795, 364);
            this.listBox_log.TabIndex = 4;
            // 
            // button_log_clear
            // 
            this.button_log_clear.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_log_clear.Location = new System.Drawing.Point(812, 266);
            this.button_log_clear.Name = "button_log_clear";
            this.button_log_clear.Size = new System.Drawing.Size(52, 62);
            this.button_log_clear.TabIndex = 42;
            this.button_log_clear.Text = "清除";
            this.button_log_clear.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_log_clear.UseVisualStyleBackColor = true;
            this.button_log_clear.Click += new System.EventHandler(this.button_log_clear_Click);
            // 
            // button_Com_Flash
            // 
            this.button_Com_Flash.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_Com_Flash.Location = new System.Drawing.Point(812, 180);
            this.button_Com_Flash.Name = "button_Com_Flash";
            this.button_Com_Flash.Size = new System.Drawing.Size(52, 62);
            this.button_Com_Flash.TabIndex = 43;
            this.button_Com_Flash.Text = "刷新";
            this.button_Com_Flash.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_Com_Flash.UseVisualStyleBackColor = true;
            this.button_Com_Flash.Click += new System.EventHandler(this.button_Com_Flash_Click);
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.radioButton_freertos);
            this.panel1.Controls.Add(this.radioButton_luoji);
            this.panel1.Controls.Add(this.button_download_gj);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.progressBar_dowmload_jd);
            this.panel1.Controls.Add(this.button_open_gj_addr);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.textBox_gj_addr);
            this.panel1.Controls.Add(this.textBox_test1_led);
            this.panel1.Controls.Add(this.button_test1);
            this.panel1.Controls.Add(this.comboBox_test1_baud);
            this.panel1.Controls.Add(this.comboBox_test1_com);
            this.panel1.Controls.Add(this.label26);
            this.panel1.Controls.Add(this.label27);
            this.panel1.Location = new System.Drawing.Point(11, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(853, 155);
            this.panel1.TabIndex = 44;
            // 
            // button_download_gj
            // 
            this.button_download_gj.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_download_gj.Location = new System.Drawing.Point(719, 63);
            this.button_download_gj.Name = "button_download_gj";
            this.button_download_gj.Size = new System.Drawing.Size(103, 28);
            this.button_download_gj.TabIndex = 59;
            this.button_download_gj.Text = "下载";
            this.button_download_gj.UseVisualStyleBackColor = true;
            this.button_download_gj.Click += new System.EventHandler(this.button_download_gj_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(221, 72);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(87, 15);
            this.label2.TabIndex = 58;
            this.label2.Text = "下载进度：";
            // 
            // progressBar_dowmload_jd
            // 
            this.progressBar_dowmload_jd.Location = new System.Drawing.Point(314, 65);
            this.progressBar_dowmload_jd.Name = "progressBar_dowmload_jd";
            this.progressBar_dowmload_jd.Size = new System.Drawing.Size(377, 26);
            this.progressBar_dowmload_jd.TabIndex = 57;
            // 
            // button_open_gj_addr
            // 
            this.button_open_gj_addr.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_open_gj_addr.Location = new System.Drawing.Point(719, 15);
            this.button_open_gj_addr.Name = "button_open_gj_addr";
            this.button_open_gj_addr.Size = new System.Drawing.Size(103, 28);
            this.button_open_gj_addr.TabIndex = 56;
            this.button_open_gj_addr.Text = "打开";
            this.button_open_gj_addr.UseVisualStyleBackColor = true;
            this.button_open_gj_addr.Click += new System.EventHandler(this.button_open_gj_addr_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(221, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 15);
            this.label1.TabIndex = 55;
            this.label1.Text = "选择固件：";
            // 
            // textBox_gj_addr
            // 
            this.textBox_gj_addr.Location = new System.Drawing.Point(314, 18);
            this.textBox_gj_addr.Name = "textBox_gj_addr";
            this.textBox_gj_addr.Size = new System.Drawing.Size(377, 25);
            this.textBox_gj_addr.TabIndex = 54;
            this.textBox_gj_addr.WordWrap = false;
            // 
            // textBox_test1_led
            // 
            this.textBox_test1_led.Location = new System.Drawing.Point(38, 99);
            this.textBox_test1_led.Name = "textBox_test1_led";
            this.textBox_test1_led.Size = new System.Drawing.Size(26, 25);
            this.textBox_test1_led.TabIndex = 48;
            this.textBox_test1_led.WordWrap = false;
            // 
            // button_test1
            // 
            this.button_test1.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_test1.Location = new System.Drawing.Point(98, 98);
            this.button_test1.Name = "button_test1";
            this.button_test1.Size = new System.Drawing.Size(103, 28);
            this.button_test1.TabIndex = 49;
            this.button_test1.Text = "Connect";
            this.button_test1.UseVisualStyleBackColor = true;
            this.button_test1.Click += new System.EventHandler(this.button_test1_Click);
            // 
            // comboBox_test1_baud
            // 
            this.comboBox_test1_baud.FormattingEnabled = true;
            this.comboBox_test1_baud.Items.AddRange(new object[] {
            "9600",
            "115200",
            "921600"});
            this.comboBox_test1_baud.Location = new System.Drawing.Point(123, 63);
            this.comboBox_test1_baud.Name = "comboBox_test1_baud";
            this.comboBox_test1_baud.Size = new System.Drawing.Size(78, 23);
            this.comboBox_test1_baud.TabIndex = 53;
            // 
            // comboBox_test1_com
            // 
            this.comboBox_test1_com.FormattingEnabled = true;
            this.comboBox_test1_com.Location = new System.Drawing.Point(123, 26);
            this.comboBox_test1_com.Name = "comboBox_test1_com";
            this.comboBox_test1_com.Size = new System.Drawing.Size(78, 23);
            this.comboBox_test1_com.TabIndex = 51;
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label26.Location = new System.Drawing.Point(21, 66);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(71, 15);
            this.label26.TabIndex = 52;
            this.label26.Text = "波特率：";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label27.Location = new System.Drawing.Point(21, 29);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(71, 15);
            this.label27.TabIndex = 50;
            this.label27.Text = "串口号：";
            // 
            // radioButton_luoji
            // 
            this.radioButton_luoji.AutoSize = true;
            this.radioButton_luoji.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButton_luoji.Location = new System.Drawing.Point(314, 114);
            this.radioButton_luoji.Name = "radioButton_luoji";
            this.radioButton_luoji.Size = new System.Drawing.Size(92, 19);
            this.radioButton_luoji.TabIndex = 60;
            this.radioButton_luoji.TabStop = true;
            this.radioButton_luoji.Text = "裸机下载";
            this.radioButton_luoji.UseVisualStyleBackColor = true;
            // 
            // radioButton_freertos
            // 
            this.radioButton_freertos.AutoSize = true;
            this.radioButton_freertos.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.radioButton_freertos.Location = new System.Drawing.Point(569, 114);
            this.radioButton_freertos.Name = "radioButton_freertos";
            this.radioButton_freertos.Size = new System.Drawing.Size(132, 19);
            this.radioButton_freertos.TabIndex = 61;
            this.radioButton_freertos.TabStop = true;
            this.radioButton_freertos.Text = "FreeRTOS下载";
            this.radioButton_freertos.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(876, 544);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.button_Com_Flash);
            this.Controls.Add(this.button_log_clear);
            this.Controls.Add(this.listBox_log);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "STM32F407_IAP_TEST_1.0";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ListBox listBox_log;
        private System.Windows.Forms.Button button_log_clear;
        private System.Windows.Forms.Button button_Com_Flash;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox textBox_test1_led;
        private System.Windows.Forms.Button button_test1;
        private System.Windows.Forms.ComboBox comboBox_test1_baud;
        private System.Windows.Forms.ComboBox comboBox_test1_com;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Button button_download_gj;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ProgressBar progressBar_dowmload_jd;
        private System.Windows.Forms.Button button_open_gj_addr;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_gj_addr;
        private System.Windows.Forms.RadioButton radioButton_freertos;
        private System.Windows.Forms.RadioButton radioButton_luoji;
    }
}

