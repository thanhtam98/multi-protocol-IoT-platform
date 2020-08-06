namespace AppNodeConfiguration
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.prgBarLoad = new System.Windows.Forms.ProgressBar();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btnClose = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btnOpen = new System.Windows.Forms.Button();
            this.cBoxParity = new System.Windows.Forms.ComboBox();
            this.cBoxStopbits = new System.Windows.Forms.ComboBox();
            this.cBoxDatabits = new System.Windows.Forms.ComboBox();
            this.cBoxBaudrate = new System.Windows.Forms.ComboBox();
            this.cBoxComPort = new System.Windows.Forms.ComboBox();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.Manual = new System.Windows.Forms.GroupBox();
            this.tBoxSend = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.tBoxRecv = new System.Windows.Forms.TextBox();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.cBoxMbMode = new System.Windows.Forms.ComboBox();
            this.tBoxSid = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.cBoxPari = new System.Windows.Forms.ComboBox();
            this.cBoxSB = new System.Windows.Forms.ComboBox();
            this.cBoxDb = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.cBoxMbBaud = new System.Windows.Forms.ComboBox();
            this.btnSetMB = new System.Windows.Forms.Button();
            this.btnGetMB = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label16 = new System.Windows.Forms.Label();
            this.cBoxAutoJoin = new System.Windows.Forms.ComboBox();
            this.btnJoin = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.cBoxPower = new System.Windows.Forms.ComboBox();
            this.cBoxCfm = new System.Windows.Forms.ComboBox();
            this.label29 = new System.Windows.Forms.Label();
            this.cBoxClass = new System.Windows.Forms.ComboBox();
            this.label28 = new System.Windows.Forms.Label();
            this.chBoxJoin = new System.Windows.Forms.CheckBox();
            this.cBoxAdr = new System.Windows.Forms.ComboBox();
            this.label27 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.cBoxFreq = new System.Windows.Forms.ComboBox();
            this.tBoxAppKey = new System.Windows.Forms.TextBox();
            this.tBoxAppEUI = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.tBoxDevEUI = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.cBoxLrMode = new System.Windows.Forms.ComboBox();
            this.label14 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label34 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.label32 = new System.Windows.Forms.Label();
            this.label31 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.textBox9 = new System.Windows.Forms.TextBox();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.cBoxPul = new System.Windows.Forms.ComboBox();
            this.cBoxOneW = new System.Windows.Forms.ComboBox();
            this.label24 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.cBoxADC2 = new System.Windows.Forms.ComboBox();
            this.label21 = new System.Windows.Forms.Label();
            this.cBoxADC1 = new System.Windows.Forms.ComboBox();
            this.label22 = new System.Windows.Forms.Label();
            this.cBoxSW2 = new System.Windows.Forms.ComboBox();
            this.label20 = new System.Windows.Forms.Label();
            this.cBoxSW1 = new System.Windows.Forms.ComboBox();
            this.label19 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.cBoxLoRaMode = new System.Windows.Forms.ComboBox();
            this.label36 = new System.Windows.Forms.Label();
            this.cBoxModbusMode = new System.Windows.Forms.ComboBox();
            this.label35 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.Manual.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.prgBarLoad);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.btnClose);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.btnOpen);
            this.groupBox1.Controls.Add(this.cBoxParity);
            this.groupBox1.Controls.Add(this.cBoxStopbits);
            this.groupBox1.Controls.Add(this.cBoxDatabits);
            this.groupBox1.Controls.Add(this.cBoxBaudrate);
            this.groupBox1.Controls.Add(this.cBoxComPort);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(27, 22);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Size = new System.Drawing.Size(279, 287);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Com Port Control";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(12, 171);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(52, 20);
            this.label5.TabIndex = 10;
            this.label5.Text = "Parity";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // prgBarLoad
            // 
            this.prgBarLoad.Location = new System.Drawing.Point(13, 247);
            this.prgBarLoad.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.prgBarLoad.Name = "prgBarLoad";
            this.prgBarLoad.Size = new System.Drawing.Size(239, 31);
            this.prgBarLoad.TabIndex = 15;
            this.prgBarLoad.Click += new System.EventHandler(this.prgBarLoad_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 137);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(75, 20);
            this.label4.TabIndex = 9;
            this.label4.Text = "Stop bits";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 103);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 20);
            this.label3.TabIndex = 8;
            this.label3.Text = "Data bits";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 68);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 20);
            this.label2.TabIndex = 7;
            this.label2.Text = "Baudrate";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(139, 209);
            this.btnClose.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(113, 34);
            this.btnClose.TabIndex = 13;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 34);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 20);
            this.label1.TabIndex = 6;
            this.label1.Text = "Com Port";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(13, 209);
            this.btnOpen.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(120, 34);
            this.btnOpen.TabIndex = 12;
            this.btnOpen.Text = "Open";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.button1_Click);
            // 
            // cBoxParity
            // 
            this.cBoxParity.FormattingEnabled = true;
            this.cBoxParity.Items.AddRange(new object[] {
            "Odd",
            "Even",
            "None"});
            this.cBoxParity.Location = new System.Drawing.Point(131, 162);
            this.cBoxParity.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxParity.Name = "cBoxParity";
            this.cBoxParity.Size = new System.Drawing.Size(121, 28);
            this.cBoxParity.TabIndex = 5;
            this.cBoxParity.Text = "None";
            // 
            // cBoxStopbits
            // 
            this.cBoxStopbits.FormattingEnabled = true;
            this.cBoxStopbits.Items.AddRange(new object[] {
            "1",
            "2"});
            this.cBoxStopbits.Location = new System.Drawing.Point(131, 129);
            this.cBoxStopbits.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxStopbits.Name = "cBoxStopbits";
            this.cBoxStopbits.Size = new System.Drawing.Size(121, 28);
            this.cBoxStopbits.TabIndex = 4;
            this.cBoxStopbits.Text = "1";
            // 
            // cBoxDatabits
            // 
            this.cBoxDatabits.FormattingEnabled = true;
            this.cBoxDatabits.Items.AddRange(new object[] {
            "7",
            "8"});
            this.cBoxDatabits.Location = new System.Drawing.Point(131, 95);
            this.cBoxDatabits.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxDatabits.Name = "cBoxDatabits";
            this.cBoxDatabits.Size = new System.Drawing.Size(121, 28);
            this.cBoxDatabits.TabIndex = 3;
            this.cBoxDatabits.Text = "8";
            this.cBoxDatabits.SelectedIndexChanged += new System.EventHandler(this.comboBox3_SelectedIndexChanged);
            // 
            // cBoxBaudrate
            // 
            this.cBoxBaudrate.FormattingEnabled = true;
            this.cBoxBaudrate.Items.AddRange(new object[] {
            "4800",
            "57600",
            "9600",
            "115200"});
            this.cBoxBaudrate.Location = new System.Drawing.Point(131, 60);
            this.cBoxBaudrate.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxBaudrate.Name = "cBoxBaudrate";
            this.cBoxBaudrate.Size = new System.Drawing.Size(121, 28);
            this.cBoxBaudrate.TabIndex = 2;
            this.cBoxBaudrate.Text = "115200";
            // 
            // cBoxComPort
            // 
            this.cBoxComPort.FormattingEnabled = true;
            this.cBoxComPort.Location = new System.Drawing.Point(131, 26);
            this.cBoxComPort.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxComPort.Name = "cBoxComPort";
            this.cBoxComPort.Size = new System.Drawing.Size(121, 28);
            this.cBoxComPort.TabIndex = 1;
            // 
            // Manual
            // 
            this.Manual.Controls.Add(this.tBoxSend);
            this.Manual.Controls.Add(this.btnSend);
            this.Manual.Controls.Add(this.tBoxRecv);
            this.Manual.Location = new System.Drawing.Point(27, 313);
            this.Manual.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Manual.Name = "Manual";
            this.Manual.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Manual.Size = new System.Drawing.Size(279, 341);
            this.Manual.TabIndex = 11;
            this.Manual.TabStop = false;
            this.Manual.Text = "Manual";
            // 
            // tBoxSend
            // 
            this.tBoxSend.Location = new System.Drawing.Point(16, 81);
            this.tBoxSend.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tBoxSend.Multiline = true;
            this.tBoxSend.Name = "tBoxSend";
            this.tBoxSend.Size = new System.Drawing.Size(257, 57);
            this.tBoxSend.TabIndex = 14;
            this.tBoxSend.TextChanged += new System.EventHandler(this.tBoxSend_TextChanged);
            this.tBoxSend.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tBoxSend_KeyDown);
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(16, 34);
            this.btnSend.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(257, 34);
            this.btnSend.TabIndex = 14;
            this.btnSend.Text = "Send Data";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // tBoxRecv
            // 
            this.tBoxRecv.Location = new System.Drawing.Point(16, 151);
            this.tBoxRecv.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tBoxRecv.Multiline = true;
            this.tBoxRecv.Name = "tBoxRecv";
            this.tBoxRecv.Size = new System.Drawing.Size(257, 159);
            this.tBoxRecv.TabIndex = 12;
            this.tBoxRecv.TextChanged += new System.EventHandler(this.tBoxRecv_TextChanged);
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label11);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.cBoxMbMode);
            this.groupBox2.Controls.Add(this.tBoxSid);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.label9);
            this.groupBox2.Controls.Add(this.cBoxPari);
            this.groupBox2.Controls.Add(this.cBoxSB);
            this.groupBox2.Controls.Add(this.cBoxDb);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.cBoxMbBaud);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(329, 22);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox2.Size = new System.Drawing.Size(339, 258);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Modbus";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(20, 199);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(72, 20);
            this.label11.TabIndex = 25;
            this.label11.Text = "Slave ID";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(20, 34);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(50, 20);
            this.label10.TabIndex = 24;
            this.label10.Text = "Mode";
            this.label10.Click += new System.EventHandler(this.label10_Click);
            // 
            // cBoxMbMode
            // 
            this.cBoxMbMode.FormattingEnabled = true;
            this.cBoxMbMode.Items.AddRange(new object[] {
            "RTU",
            "ASCII"});
            this.cBoxMbMode.Location = new System.Drawing.Point(163, 26);
            this.cBoxMbMode.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxMbMode.Name = "cBoxMbMode";
            this.cBoxMbMode.Size = new System.Drawing.Size(121, 28);
            this.cBoxMbMode.TabIndex = 23;
            this.cBoxMbMode.Text = "RTU";
            this.cBoxMbMode.SelectedIndexChanged += new System.EventHandler(this.comboBox5_SelectedIndexChanged);
            // 
            // tBoxSid
            // 
            this.tBoxSid.Location = new System.Drawing.Point(163, 197);
            this.tBoxSid.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tBoxSid.Name = "tBoxSid";
            this.tBoxSid.Size = new System.Drawing.Size(121, 27);
            this.tBoxSid.TabIndex = 22;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(20, 171);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(52, 20);
            this.label7.TabIndex = 21;
            this.label7.Text = "Parity";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(20, 137);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(75, 20);
            this.label8.TabIndex = 20;
            this.label8.Text = "Stop bits";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(20, 103);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(77, 20);
            this.label9.TabIndex = 19;
            this.label9.Text = "Data bits";
            // 
            // cBoxPari
            // 
            this.cBoxPari.FormattingEnabled = true;
            this.cBoxPari.Items.AddRange(new object[] {
            "Odd",
            "Even",
            "None"});
            this.cBoxPari.Location = new System.Drawing.Point(163, 162);
            this.cBoxPari.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxPari.Name = "cBoxPari";
            this.cBoxPari.Size = new System.Drawing.Size(121, 28);
            this.cBoxPari.TabIndex = 18;
            this.cBoxPari.Text = "None";
            this.cBoxPari.SelectedIndexChanged += new System.EventHandler(this.cBoxPari_SelectedIndexChanged);
            // 
            // cBoxSB
            // 
            this.cBoxSB.FormattingEnabled = true;
            this.cBoxSB.Items.AddRange(new object[] {
            "1",
            "2"});
            this.cBoxSB.Location = new System.Drawing.Point(163, 129);
            this.cBoxSB.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxSB.Name = "cBoxSB";
            this.cBoxSB.Size = new System.Drawing.Size(121, 28);
            this.cBoxSB.TabIndex = 17;
            this.cBoxSB.Text = "1";
            // 
            // cBoxDb
            // 
            this.cBoxDb.FormattingEnabled = true;
            this.cBoxDb.Items.AddRange(new object[] {
            "7",
            "8"});
            this.cBoxDb.Location = new System.Drawing.Point(163, 94);
            this.cBoxDb.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxDb.Name = "cBoxDb";
            this.cBoxDb.Size = new System.Drawing.Size(121, 28);
            this.cBoxDb.TabIndex = 16;
            this.cBoxDb.Text = "8";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(20, 68);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 20);
            this.label6.TabIndex = 15;
            this.label6.Text = "Baudrate";
            // 
            // cBoxMbBaud
            // 
            this.cBoxMbBaud.FormattingEnabled = true;
            this.cBoxMbBaud.Items.AddRange(new object[] {
            "4800",
            "57600",
            "9600",
            "115200"});
            this.cBoxMbBaud.Location = new System.Drawing.Point(163, 60);
            this.cBoxMbBaud.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxMbBaud.Name = "cBoxMbBaud";
            this.cBoxMbBaud.Size = new System.Drawing.Size(121, 28);
            this.cBoxMbBaud.TabIndex = 14;
            this.cBoxMbBaud.Text = "115200";
            // 
            // btnSetMB
            // 
            this.btnSetMB.Location = new System.Drawing.Point(35, 242);
            this.btnSetMB.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSetMB.Name = "btnSetMB";
            this.btnSetMB.Size = new System.Drawing.Size(100, 66);
            this.btnSetMB.TabIndex = 38;
            this.btnSetMB.Text = "Set";
            this.btnSetMB.UseVisualStyleBackColor = true;
            this.btnSetMB.Click += new System.EventHandler(this.button5_Click);
            // 
            // btnGetMB
            // 
            this.btnGetMB.Location = new System.Drawing.Point(185, 241);
            this.btnGetMB.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnGetMB.Name = "btnGetMB";
            this.btnGetMB.Size = new System.Drawing.Size(100, 66);
            this.btnGetMB.TabIndex = 27;
            this.btnGetMB.Text = "Get";
            this.btnGetMB.UseVisualStyleBackColor = true;
            this.btnGetMB.Click += new System.EventHandler(this.button4_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label16);
            this.groupBox3.Controls.Add(this.cBoxAutoJoin);
            this.groupBox3.Controls.Add(this.btnJoin);
            this.groupBox3.Controls.Add(this.label12);
            this.groupBox3.Controls.Add(this.label30);
            this.groupBox3.Controls.Add(this.cBoxPower);
            this.groupBox3.Controls.Add(this.cBoxCfm);
            this.groupBox3.Controls.Add(this.label29);
            this.groupBox3.Controls.Add(this.cBoxClass);
            this.groupBox3.Controls.Add(this.label28);
            this.groupBox3.Controls.Add(this.chBoxJoin);
            this.groupBox3.Controls.Add(this.cBoxAdr);
            this.groupBox3.Controls.Add(this.label27);
            this.groupBox3.Controls.Add(this.label25);
            this.groupBox3.Controls.Add(this.cBoxFreq);
            this.groupBox3.Controls.Add(this.tBoxAppKey);
            this.groupBox3.Controls.Add(this.tBoxAppEUI);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Controls.Add(this.label18);
            this.groupBox3.Controls.Add(this.tBoxDevEUI);
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Controls.Add(this.cBoxLrMode);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox3.Location = new System.Drawing.Point(329, 287);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox3.Size = new System.Drawing.Size(632, 367);
            this.groupBox3.TabIndex = 13;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "LoraWAN";
            this.groupBox3.Enter += new System.EventHandler(this.groupBox3_Enter);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(303, 164);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(93, 20);
            this.label16.TabIndex = 52;
            this.label16.Text = "Join Status";
            // 
            // cBoxAutoJoin
            // 
            this.cBoxAutoJoin.FormattingEnabled = true;
            this.cBoxAutoJoin.Items.AddRange(new object[] {
            "NO",
            "YES"});
            this.cBoxAutoJoin.Location = new System.Drawing.Point(163, 160);
            this.cBoxAutoJoin.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxAutoJoin.Name = "cBoxAutoJoin";
            this.cBoxAutoJoin.Size = new System.Drawing.Size(121, 28);
            this.cBoxAutoJoin.TabIndex = 51;
            this.cBoxAutoJoin.Text = "NO";
            // 
            // btnJoin
            // 
            this.btnJoin.Location = new System.Drawing.Point(497, 215);
            this.btnJoin.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnJoin.Name = "btnJoin";
            this.btnJoin.Size = new System.Drawing.Size(112, 94);
            this.btnJoin.TabIndex = 37;
            this.btnJoin.Text = "Join";
            this.btnJoin.UseVisualStyleBackColor = true;
            this.btnJoin.Click += new System.EventHandler(this.button3_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(23, 164);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(79, 20);
            this.label12.TabIndex = 50;
            this.label12.Text = "Auto Join";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(303, 117);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(74, 20);
            this.label30.TabIndex = 49;
            this.label30.Text = "TxPower";
            // 
            // cBoxPower
            // 
            this.cBoxPower.FormattingEnabled = true;
            this.cBoxPower.Items.AddRange(new object[] {
            "MAX",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7"});
            this.cBoxPower.Location = new System.Drawing.Point(486, 110);
            this.cBoxPower.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxPower.Name = "cBoxPower";
            this.cBoxPower.Size = new System.Drawing.Size(121, 28);
            this.cBoxPower.TabIndex = 48;
            this.cBoxPower.Text = "MAX";
            // 
            // cBoxCfm
            // 
            this.cBoxCfm.FormattingEnabled = true;
            this.cBoxCfm.Items.AddRange(new object[] {
            "YES",
            "NO"});
            this.cBoxCfm.Location = new System.Drawing.Point(486, 69);
            this.cBoxCfm.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxCfm.Name = "cBoxCfm";
            this.cBoxCfm.Size = new System.Drawing.Size(121, 28);
            this.cBoxCfm.TabIndex = 47;
            this.cBoxCfm.Text = "YES";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(303, 74);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(68, 20);
            this.label29.TabIndex = 46;
            this.label29.Text = "Confirm";
            // 
            // cBoxClass
            // 
            this.cBoxClass.FormattingEnabled = true;
            this.cBoxClass.Items.AddRange(new object[] {
            "A",
            "B",
            "C"});
            this.cBoxClass.Location = new System.Drawing.Point(163, 107);
            this.cBoxClass.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxClass.Name = "cBoxClass";
            this.cBoxClass.Size = new System.Drawing.Size(121, 28);
            this.cBoxClass.TabIndex = 45;
            this.cBoxClass.Text = "C";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(23, 117);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(52, 20);
            this.label28.TabIndex = 44;
            this.label28.Text = "Class";
            // 
            // chBoxJoin
            // 
            this.chBoxJoin.AutoSize = true;
            this.chBoxJoin.Location = new System.Drawing.Point(463, 166);
            this.chBoxJoin.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chBoxJoin.Name = "chBoxJoin";
            this.chBoxJoin.Size = new System.Drawing.Size(18, 17);
            this.chBoxJoin.TabIndex = 43;
            this.chBoxJoin.UseVisualStyleBackColor = true;
            this.chBoxJoin.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // cBoxAdr
            // 
            this.cBoxAdr.BackColor = System.Drawing.SystemColors.Window;
            this.cBoxAdr.FormattingEnabled = true;
            this.cBoxAdr.Items.AddRange(new object[] {
            "Adapted",
            "SF7",
            "SF8",
            "SF9",
            "SF10",
            "SF11",
            "SF12"});
            this.cBoxAdr.Location = new System.Drawing.Point(163, 63);
            this.cBoxAdr.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxAdr.Name = "cBoxAdr";
            this.cBoxAdr.Size = new System.Drawing.Size(121, 28);
            this.cBoxAdr.TabIndex = 42;
            this.cBoxAdr.Text = "Adapted";
            this.cBoxAdr.SelectedIndexChanged += new System.EventHandler(this.cBoxAdr_SelectedIndexChanged);
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(21, 74);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(85, 20);
            this.label27.TabIndex = 41;
            this.label27.Text = "Data Rate";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(303, 30);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(144, 20);
            this.label25.TabIndex = 39;
            this.label25.Text = "Frequency Region";
            this.label25.Click += new System.EventHandler(this.label25_Click);
            // 
            // cBoxFreq
            // 
            this.cBoxFreq.FormattingEnabled = true;
            this.cBoxFreq.Items.AddRange(new object[] {
            "AS923",
            "AU915",
            "CN470",
            "CN779",
            "EU433",
            "EU868",
            "KR920",
            "IN865",
            "AU915",
            "AU915"});
            this.cBoxFreq.Location = new System.Drawing.Point(486, 26);
            this.cBoxFreq.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxFreq.Name = "cBoxFreq";
            this.cBoxFreq.Size = new System.Drawing.Size(121, 28);
            this.cBoxFreq.TabIndex = 38;
            this.cBoxFreq.Text = "AS923";
            // 
            // tBoxAppKey
            // 
            this.tBoxAppKey.Location = new System.Drawing.Point(163, 247);
            this.tBoxAppKey.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tBoxAppKey.Name = "tBoxAppKey";
            this.tBoxAppKey.Size = new System.Drawing.Size(328, 27);
            this.tBoxAppKey.TabIndex = 28;
            this.tBoxAppKey.TextChanged += new System.EventHandler(this.textBox4_TextChanged);
            // 
            // tBoxAppEUI
            // 
            this.tBoxAppEUI.Location = new System.Drawing.Point(163, 282);
            this.tBoxAppEUI.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tBoxAppEUI.Name = "tBoxAppEUI";
            this.tBoxAppEUI.Size = new System.Drawing.Size(328, 27);
            this.tBoxAppEUI.TabIndex = 32;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(20, 254);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(71, 20);
            this.label15.TabIndex = 27;
            this.label15.Text = "App Key";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(20, 288);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(70, 20);
            this.label18.TabIndex = 31;
            this.label18.Text = "App EUI";
            // 
            // tBoxDevEUI
            // 
            this.tBoxDevEUI.Location = new System.Drawing.Point(163, 214);
            this.tBoxDevEUI.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tBoxDevEUI.Name = "tBoxDevEUI";
            this.tBoxDevEUI.Size = new System.Drawing.Size(328, 27);
            this.tBoxDevEUI.TabIndex = 26;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(20, 34);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(50, 20);
            this.label13.TabIndex = 24;
            this.label13.Text = "Mode";
            // 
            // cBoxLrMode
            // 
            this.cBoxLrMode.FormattingEnabled = true;
            this.cBoxLrMode.Items.AddRange(new object[] {
            "OTAA",
            "ADP"});
            this.cBoxLrMode.Location = new System.Drawing.Point(163, 26);
            this.cBoxLrMode.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxLrMode.Name = "cBoxLrMode";
            this.cBoxLrMode.Size = new System.Drawing.Size(121, 28);
            this.cBoxLrMode.TabIndex = 23;
            this.cBoxLrMode.Text = "OTAA";
            this.cBoxLrMode.SelectedIndexChanged += new System.EventHandler(this.comboBox6_SelectedIndexChanged);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(20, 222);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(71, 20);
            this.label14.TabIndex = 21;
            this.label14.Text = "Dev EUI";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label34);
            this.groupBox4.Controls.Add(this.label33);
            this.groupBox4.Controls.Add(this.label32);
            this.groupBox4.Controls.Add(this.label31);
            this.groupBox4.Controls.Add(this.label26);
            this.groupBox4.Controls.Add(this.label17);
            this.groupBox4.Controls.Add(this.textBox9);
            this.groupBox4.Controls.Add(this.textBox8);
            this.groupBox4.Controls.Add(this.textBox7);
            this.groupBox4.Controls.Add(this.textBox4);
            this.groupBox4.Controls.Add(this.textBox3);
            this.groupBox4.Controls.Add(this.textBox2);
            this.groupBox4.Controls.Add(this.cBoxPul);
            this.groupBox4.Controls.Add(this.cBoxOneW);
            this.groupBox4.Controls.Add(this.label24);
            this.groupBox4.Controls.Add(this.label23);
            this.groupBox4.Controls.Add(this.cBoxADC2);
            this.groupBox4.Controls.Add(this.label21);
            this.groupBox4.Controls.Add(this.cBoxADC1);
            this.groupBox4.Controls.Add(this.label22);
            this.groupBox4.Controls.Add(this.cBoxSW2);
            this.groupBox4.Controls.Add(this.label20);
            this.groupBox4.Controls.Add(this.cBoxSW1);
            this.groupBox4.Controls.Add(this.label19);
            this.groupBox4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox4.Location = new System.Drawing.Point(675, 25);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox4.Size = new System.Drawing.Size(634, 258);
            this.groupBox4.TabIndex = 39;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "IO Configuration";
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.Location = new System.Drawing.Point(344, 215);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(77, 20);
            this.label34.TabIndex = 62;
            this.label34.Text = "Interval 6";
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(344, 180);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(77, 20);
            this.label33.TabIndex = 61;
            this.label33.Text = "Interval 5";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(344, 145);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(77, 20);
            this.label32.TabIndex = 60;
            this.label32.Text = "Interval 4";
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.Location = new System.Drawing.Point(344, 108);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(77, 20);
            this.label31.TabIndex = 59;
            this.label31.Text = "Interval 3";
            this.label31.Click += new System.EventHandler(this.label31_Click);
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(345, 73);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(77, 20);
            this.label26.TabIndex = 58;
            this.label26.Text = "Interval 2";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(345, 42);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(77, 20);
            this.label17.TabIndex = 57;
            this.label17.Text = "Interval 1";
            this.label17.Click += new System.EventHandler(this.label17_Click);
            // 
            // textBox9
            // 
            this.textBox9.Location = new System.Drawing.Point(445, 213);
            this.textBox9.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox9.Name = "textBox9";
            this.textBox9.Size = new System.Drawing.Size(141, 27);
            this.textBox9.TabIndex = 56;
            // 
            // textBox8
            // 
            this.textBox8.Location = new System.Drawing.Point(445, 177);
            this.textBox8.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox8.Name = "textBox8";
            this.textBox8.Size = new System.Drawing.Size(141, 27);
            this.textBox8.TabIndex = 55;
            // 
            // textBox7
            // 
            this.textBox7.Location = new System.Drawing.Point(445, 142);
            this.textBox7.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(141, 27);
            this.textBox7.TabIndex = 54;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(445, 105);
            this.textBox4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(141, 27);
            this.textBox4.TabIndex = 53;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(445, 70);
            this.textBox3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(141, 27);
            this.textBox3.TabIndex = 52;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(445, 34);
            this.textBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(141, 27);
            this.textBox2.TabIndex = 51;
            // 
            // cBoxPul
            // 
            this.cBoxPul.FormattingEnabled = true;
            this.cBoxPul.Items.AddRange(new object[] {
            "Per Second",
            "Per Minimute",
            "Per Hour"});
            this.cBoxPul.Location = new System.Drawing.Point(117, 177);
            this.cBoxPul.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxPul.Name = "cBoxPul";
            this.cBoxPul.Size = new System.Drawing.Size(193, 28);
            this.cBoxPul.TabIndex = 50;
            this.cBoxPul.Text = "Per Second";
            // 
            // cBoxOneW
            // 
            this.cBoxOneW.FormattingEnabled = true;
            this.cBoxOneW.Items.AddRange(new object[] {
            "DHT11",
            "DHT22",
            "DS18B20"});
            this.cBoxOneW.Location = new System.Drawing.Point(117, 215);
            this.cBoxOneW.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxOneW.Name = "cBoxOneW";
            this.cBoxOneW.Size = new System.Drawing.Size(193, 28);
            this.cBoxOneW.TabIndex = 49;
            this.cBoxOneW.Text = "DHT11";
            this.cBoxOneW.SelectedIndexChanged += new System.EventHandler(this.cBoxOneW_SelectedIndexChanged);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(13, 223);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(80, 20);
            this.label24.TabIndex = 48;
            this.label24.Text = "One Wire";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(13, 185);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(51, 20);
            this.label23.TabIndex = 47;
            this.label23.Text = "Pulse";
            // 
            // cBoxADC2
            // 
            this.cBoxADC2.FormattingEnabled = true;
            this.cBoxADC2.Items.AddRange(new object[] {
            "Temp Sensor ",
            "Light Sensor",
            "Humidy Sensor",
            "Gas Sensor",
            "Motion Sensor"});
            this.cBoxADC2.Location = new System.Drawing.Point(117, 142);
            this.cBoxADC2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxADC2.Name = "cBoxADC2";
            this.cBoxADC2.Size = new System.Drawing.Size(193, 28);
            this.cBoxADC2.TabIndex = 46;
            this.cBoxADC2.Text = "Temp Sensor";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(13, 150);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(74, 20);
            this.label21.TabIndex = 45;
            this.label21.Text = "Analog 2";
            // 
            // cBoxADC1
            // 
            this.cBoxADC1.FormattingEnabled = true;
            this.cBoxADC1.Items.AddRange(new object[] {
            "Temp Sensor ",
            "Light Sensor",
            "Humidy Sensor",
            "Gas Sensor",
            "Motion Sensor"});
            this.cBoxADC1.Location = new System.Drawing.Point(117, 103);
            this.cBoxADC1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxADC1.Name = "cBoxADC1";
            this.cBoxADC1.Size = new System.Drawing.Size(193, 28);
            this.cBoxADC1.TabIndex = 44;
            this.cBoxADC1.Text = "Light Sensor";
            this.cBoxADC1.SelectedIndexChanged += new System.EventHandler(this.cBoxADC1_SelectedIndexChanged);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(13, 112);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(74, 20);
            this.label22.TabIndex = 43;
            this.label22.Text = "Analog 1";
            this.label22.Click += new System.EventHandler(this.label22_Click);
            // 
            // cBoxSW2
            // 
            this.cBoxSW2.FormattingEnabled = true;
            this.cBoxSW2.Items.AddRange(new object[] {
            "NO",
            "NC"});
            this.cBoxSW2.Location = new System.Drawing.Point(117, 69);
            this.cBoxSW2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxSW2.Name = "cBoxSW2";
            this.cBoxSW2.Size = new System.Drawing.Size(193, 28);
            this.cBoxSW2.TabIndex = 42;
            this.cBoxSW2.Text = "NC";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(13, 78);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(73, 20);
            this.label20.TabIndex = 41;
            this.label20.Text = "Switch 2";
            // 
            // cBoxSW1
            // 
            this.cBoxSW1.FormattingEnabled = true;
            this.cBoxSW1.Items.AddRange(new object[] {
            "NO",
            "NC"});
            this.cBoxSW1.Location = new System.Drawing.Point(117, 34);
            this.cBoxSW1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxSW1.Name = "cBoxSW1";
            this.cBoxSW1.Size = new System.Drawing.Size(193, 28);
            this.cBoxSW1.TabIndex = 40;
            this.cBoxSW1.Text = "NO";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(13, 42);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(73, 20);
            this.label19.TabIndex = 39;
            this.label19.Text = "Switch 1";
            this.label19.Click += new System.EventHandler(this.label19_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.cBoxLoRaMode);
            this.groupBox5.Controls.Add(this.label36);
            this.groupBox5.Controls.Add(this.cBoxModbusMode);
            this.groupBox5.Controls.Add(this.label35);
            this.groupBox5.Controls.Add(this.btnGetMB);
            this.groupBox5.Controls.Add(this.btnSetMB);
            this.groupBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox5.Location = new System.Drawing.Point(976, 288);
            this.groupBox5.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox5.Size = new System.Drawing.Size(333, 367);
            this.groupBox5.TabIndex = 40;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Control";
            // 
            // cBoxLoRaMode
            // 
            this.cBoxLoRaMode.FormattingEnabled = true;
            this.cBoxLoRaMode.Items.AddRange(new object[] {
            "LoRaWAN",
            "LoRa",
            "Disable"});
            this.cBoxLoRaMode.Location = new System.Drawing.Point(141, 77);
            this.cBoxLoRaMode.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxLoRaMode.Name = "cBoxLoRaMode";
            this.cBoxLoRaMode.Size = new System.Drawing.Size(171, 28);
            this.cBoxLoRaMode.TabIndex = 53;
            this.cBoxLoRaMode.Text = "LoRaWAN";
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Location = new System.Drawing.Point(6, 77);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(95, 20);
            this.label36.TabIndex = 52;
            this.label36.Text = "LoRa Mode";
            // 
            // cBoxModbusMode
            // 
            this.cBoxModbusMode.FormattingEnabled = true;
            this.cBoxModbusMode.Items.AddRange(new object[] {
            "Master",
            "Slave",
            "Disable"});
            this.cBoxModbusMode.Location = new System.Drawing.Point(141, 35);
            this.cBoxModbusMode.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.cBoxModbusMode.Name = "cBoxModbusMode";
            this.cBoxModbusMode.Size = new System.Drawing.Size(171, 28);
            this.cBoxModbusMode.TabIndex = 51;
            this.cBoxModbusMode.Text = "Slave";
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Location = new System.Drawing.Point(6, 43);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(114, 20);
            this.label35.TabIndex = 39;
            this.label35.Text = "Modbus Mode";
            this.label35.Click += new System.EventHandler(this.label35_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1373, 666);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.Manual);
            this.Controls.Add(this.groupBox1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "C# App Node Configuration";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.Manual.ResumeLayout(false);
            this.Manual.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox cBoxParity;
        private System.Windows.Forms.ComboBox cBoxStopbits;
        private System.Windows.Forms.ComboBox cBoxDatabits;
        private System.Windows.Forms.ComboBox cBoxBaudrate;
        private System.Windows.Forms.ComboBox cBoxComPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.GroupBox Manual;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.ProgressBar prgBarLoad;
        private System.Windows.Forms.TextBox tBoxRecv;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox tBoxSid;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox cBoxPari;
        private System.Windows.Forms.ComboBox cBoxSB;
        private System.Windows.Forms.ComboBox cBoxDb;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox cBoxMbBaud;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox cBoxMbMode;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox cBoxLrMode;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox tBoxAppKey;
        private System.Windows.Forms.TextBox tBoxAppEUI;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox tBoxDevEUI;
        private System.Windows.Forms.Button btnJoin;
        private System.Windows.Forms.TextBox tBoxSend;
        private System.Windows.Forms.Button btnSetMB;
        private System.Windows.Forms.Button btnGetMB;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.ComboBox cBoxADC2;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.ComboBox cBoxADC1;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.ComboBox cBoxSW2;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.ComboBox cBoxSW1;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.ComboBox cBoxPul;
        private System.Windows.Forms.ComboBox cBoxOneW;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.ComboBox cBoxFreq;
        private System.Windows.Forms.ComboBox cBoxAdr;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.TextBox textBox9;
        private System.Windows.Forms.TextBox textBox8;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.CheckBox chBoxJoin;
        private System.Windows.Forms.ComboBox cBoxClass;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.ComboBox cBoxCfm;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.ComboBox cBoxPower;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.ComboBox cBoxAutoJoin;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.ComboBox cBoxModbusMode;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.ComboBox cBoxLoRaMode;
    }
}

