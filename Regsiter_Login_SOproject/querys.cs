using System;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;

namespace Regsiter_Login_SOproject
{
    public partial class Querys : Form
    {
        private Socket server;
        private string usuarioActual;

        public Querys(Socket serverSocket, string usuario)  // Recibe el socket y el usuario
        {
            InitializeComponent();
            this.server = serverSocket;
            this.usuarioActual = usuario;
        }

        private void Querys_Load(object sender, EventArgs e)
        {
            // Opcional: Cargar la lista de usuarios al abrir el formulario
            ObtenerUsuariosConectados();
        }

        private void btnVerUsuarios_Click(object sender, EventArgs e)
        {
            ObtenerUsuariosConectados();
        }

        private void ObtenerUsuariosConectados()
        {
            if (server == null || !server.Connected)
            {
                MessageBox.Show("No estás conectado al servidor.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string mensaje = "4/";  // Código para obtener usuarios conectados
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Recibir la lista de usuarios conectados
            byte[] buffer = new byte[1024];
            int bytesRecibidos = server.Receive(buffer);
            string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim();

            // Mostrar la lista en un ListBox
            listBoxUsuarios.Items.Clear(); // Limpiar antes de actualizar
            string[] usuarios = respuesta.Split('\n');
            foreach (string usuario in usuarios)
            {
                if (!string.IsNullOrEmpty(usuario))
                    listBoxUsuarios.Items.Add(usuario);
            }
        }
        private void btnCerrarSesion_Click_1(object sender, EventArgs e)
        {
            if (server == null || !server.Connected)
            {
                MessageBox.Show("No estás conectado al servidor.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Enviar mensaje con el usuario actual
            string mensaje = $"5/{usuarioActual}";  // Código para cerrar sesión con el usuario
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Recibir la respuesta del servidor
            byte[] buffer = new byte[1024];
            int bytesRecibidos = server.Receive(buffer);
            string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim();

            // Limpiar lista de usuarios conectados y mostrar mensaje de confirmación
            listBoxUsuarios.Items.Clear();
            MessageBox.Show(respuesta, "Información", MessageBoxButtons.OK, MessageBoxIcon.Information);

            // Cerrar conexión y formulario
            server.Shutdown(SocketShutdown.Both);
            server.Close();
            this.Close();
        }

    }
}
