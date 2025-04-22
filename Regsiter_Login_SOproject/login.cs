using System;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;

namespace Regsiter_Login_SOproject
{
    public partial class login : Form
    {
        private Socket server;
        bool conectado = true;

        public login(Socket serverSocket)  // Constructor recibe el socket
        {
            InitializeComponent();
            this.server = serverSocket;
        }

        private void loginbutton_Click(object sender, EventArgs e)
        {
            string usuario = txtUsuarioLogin.Text.Trim();
            string contrasena = txtContrasenaLogin.Text.Trim();

            if (string.IsNullOrEmpty(usuario) || string.IsNullOrEmpty(contrasena))
            {
                MessageBox.Show("Por favor, ingrese tanto usuario como contraseña.", "Datos incompletos", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (server == null || !server.Connected)
            {
                MessageBox.Show("No estás conectado al servidor.", "Error de conexión", MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
                return;
            }

            // Enviar datos al servidor con el formato: "3/usuario/contrasena"
            string mensaje = $"3/{usuario}/{contrasena}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Recibir respuesta del servidor
            byte[] buffer = new byte[512];
            int bytesRecibidos = server.Receive(buffer);
            string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim(); // 🔹 Limpiar espacios invisibles

            // Mensaje de depuración para ver la respuesta exacta del servidor
            MessageBox.Show($"Depuración - Respuesta recibida: '{respuesta}'", "DEBUG", MessageBoxButtons.OK, MessageBoxIcon.Information);

            // Comparar exactamente con "Login exitoso"
            if (respuesta.Equals("Login exitoso", StringComparison.OrdinalIgnoreCase)) // 🔹 Comparación segura
            {
                MessageBox.Show("Login exitoso.", "Éxito", MessageBoxButtons.OK, MessageBoxIcon.Information);

                // Abre el siguiente formulario después del login
                Querys query = new Querys(server, usuario,conectado);
                query.Show();

                this.Hide(); // Ocultar el formulario de login
            }
            else
            {
                MessageBox.Show($"Usuario o contraseña incorrectos. Respuesta recibida: '{respuesta}'", "Error de login", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
