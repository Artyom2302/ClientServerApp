using SharpClient;
using Message = SharpClient.Message;

namespace SharpSocketClient
{
    public partial class ClientForm : Form
    {
        MessageRecipients id = 0;
        bool exit = false;


        public ClientForm()
        {
            InitializeComponent();
            Initialize();
            Thread t = new Thread(ProcessMessages);
            t.Start();
            this.Text = "User #" + (id);
        }

        Message request(MessageTypes type)
        {
            return Message.request(MessageRecipients.MR_BROKER, id, type, "");
        }
        void send(MessageRecipients to, MessageRecipients from, MessageTypes type = MessageTypes.MT_DATA, string data = "")
        {
            Message.send(to, from, type, data);
        }
        void Initialize()
        {
            id = request(MessageTypes.MT_INIT).header.to;
            GetUserList();
        }
        void GetUserList()
        {
            UsersBox.Items.Clear();
            UsersBox.Items.Add("All users");
            Message m = request(MessageTypes.MT_GET_USERS);
            String[] ids = m.data.Split(' ');
            foreach (var idClients in ids)
            {
                if (idClients.Length > 0)
                {
                    UsersBox.Items.Add(idClients);
                }
            }
        }
        void ProcessMessages()
        {
            while (!exit)
            {
                var m = request(MessageTypes.MT_GETDATA);
                switch (m.header.type)
                {
                    case MessageTypes.MT_DATA:
                        ChatBox.AppendText((m.header.from).ToString() + ":");
                        ChatBox.AppendText(m.data + "\n");
                        break;
                    case MessageTypes.MT_NOT_FOUND:
                        {
                            MessageBox.Show("User already do not exist, update list of users");
                            break;
                        }
                    case MessageTypes.MT_ADD_USER:
                        {
                            UsersBox.Items.Add(m.data);
                            break;
                        }
                    case MessageTypes.MT_DELETE_USER:
                        {
                            UsersBox.Items.Remove(m.data);
                            break;
                        }
                    default:
                        Thread.Sleep(500);
                        break;
                }
            }
        }


        private void button1_Click(object sender, EventArgs e)
        {
            var s = EnterBox.Text;
            if (s == "")
            {
                MessageBox.Show("Enter message");
                return;
            }


            if (s is not null && UsersBox.SelectedIndex != -1)
            {
                if ((string)UsersBox.SelectedItem == "All users")
                {
                    send(MessageRecipients.MR_ALL, id, MessageTypes.MT_DATA, s);
                }
                else
                    send((MessageRecipients)(int.Parse((string)UsersBox.SelectedItem)), id, MessageTypes.MT_DATA, s);
            }
        }



        private void ClientForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            exit = true;
        }
    }
}