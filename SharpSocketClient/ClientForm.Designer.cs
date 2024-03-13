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
            SuspendLayout();
            // 
            // ChatBox
            // 
            ChatBox.Location = new Point(41, 55);
            ChatBox.Margin = new Padding(3, 4, 3, 4);
            ChatBox.Multiline = true;
            ChatBox.Name = "ChatBox";
            ChatBox.ReadOnly = true;
            ChatBox.Size = new Size(210, 159);
            ChatBox.TabIndex = 0;
            // 
            // EnterBox
            // 
            EnterBox.Location = new Point(366, 103);
            EnterBox.Margin = new Padding(3, 4, 3, 4);
            EnterBox.Multiline = true;
            EnterBox.Name = "EnterBox";
            EnterBox.Size = new Size(187, 63);
            EnterBox.TabIndex = 1;
            // 
            // button1
            // 
            button1.Location = new Point(417, 175);
            button1.Margin = new Padding(3, 4, 3, 4);
            button1.Name = "button1";
            button1.Size = new Size(86, 40);
            button1.TabIndex = 2;
            button1.Text = "Send";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // UsersBox
            // 
            UsersBox.FormattingEnabled = true;
            UsersBox.Location = new Point(366, 51);
            UsersBox.Margin = new Padding(3, 4, 3, 4);
            UsersBox.Name = "UsersBox";
            UsersBox.Size = new Size(187, 28);
            UsersBox.TabIndex = 3;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(96, 31);
            label1.Name = "label1";
            label1.Size = new Size(39, 20);
            label1.TabIndex = 4;
            label1.Text = "Chat";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(257, 55);
            label2.Name = "label2";
            label2.Size = new Size(104, 20);
            label2.TabIndex = 5;
            label2.Text = "Client Address";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(264, 121);
            label3.Name = "label3";
            label3.Size = new Size(67, 20);
            label3.TabIndex = 6;
            label3.Text = "Message";
            // 
            // ClientForm
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(568, 231);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(UsersBox);
            Controls.Add(button1);
            Controls.Add(EnterBox);
            Controls.Add(ChatBox);
            Margin = new Padding(3, 4, 3, 4);
            Name = "ClientForm";
            Text = "Form1";
            FormClosing += ClientForm_FormClosing;
            FormClosed += ClientForm_FormClosed;
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
    }
}