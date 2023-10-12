namespace SharpSocketClient
{
    partial class ClientForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            ChatBox = new TextBox();
            EnterBox = new TextBox();
            button1 = new Button();
            UsersBox = new ComboBox();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            button2 = new Button();
            SuspendLayout();
            // 
            // ChatBox
            // 
            ChatBox.Location = new Point(36, 41);
            ChatBox.Multiline = true;
            ChatBox.Name = "ChatBox";
            ChatBox.ReadOnly = true;
            ChatBox.Size = new Size(184, 120);
            ChatBox.TabIndex = 0;
            ChatBox.TextChanged += ChatBox_TextChanged;
            // 
            // EnterBox
            // 
            EnterBox.Location = new Point(320, 77);
            EnterBox.Multiline = true;
            EnterBox.Name = "EnterBox";
            EnterBox.Size = new Size(164, 48);
            EnterBox.TabIndex = 1;
            // 
            // button1
            // 
            button1.Location = new Point(320, 131);
            button1.Name = "button1";
            button1.Size = new Size(75, 30);
            button1.TabIndex = 2;
            button1.Text = "Send";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // UsersBox
            // 
            UsersBox.FormattingEnabled = true;
            UsersBox.Location = new Point(320, 38);
            UsersBox.Name = "UsersBox";
            UsersBox.Size = new Size(164, 23);
            UsersBox.TabIndex = 3;
            UsersBox.UseWaitCursor = true;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(84, 23);
            label1.Name = "label1";
            label1.Size = new Size(32, 15);
            label1.TabIndex = 4;
            label1.Text = "Chat";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(231, 41);
            label2.Name = "label2";
            label2.Size = new Size(83, 15);
            label2.TabIndex = 5;
            label2.Text = "Client Address";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(231, 91);
            label3.Name = "label3";
            label3.Size = new Size(53, 15);
            label3.TabIndex = 6;
            label3.Text = "Message";
            // 
            // button2
            // 
            button2.Location = new Point(408, 132);
            button2.Name = "button2";
            button2.Size = new Size(69, 29);
            button2.TabIndex = 7;
            button2.Text = "Update";
            button2.UseVisualStyleBackColor = true;
            button2.Click += button2_Click;
            // 
            // ClientForm
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(496, 176);
            Controls.Add(button2);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(UsersBox);
            Controls.Add(button1);
            Controls.Add(EnterBox);
            Controls.Add(ChatBox);
            Name = "ClientForm";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox ChatBox;
        private TextBox EnterBox;
        private Button button1;
        private ComboBox UsersBox;
        private Label label1;
        private Label label2;
        private Label label3;
        private Button button2;
    }
}