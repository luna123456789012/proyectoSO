using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Regsiter_Login_SOproject
{
    public partial class Form1 : Form
    {
        private Socket server;
        private string usuarioActual;
        private int Id_partida;

        public Form1(Socket serverSocket, string usuario, int partida)
        {
            InitializeComponent();
            this.server = serverSocket;
            this.usuarioActual = usuario;
            this.Id_partida = partida;          
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string mensaje = "13/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] buffer = new byte[2048];
            int bytesRecibidos = server.Receive(buffer);
            string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim();

            // Mensaje de depuración para ver la respuesta exacta del servidor
            MessageBox.Show($"Depuración - Respuesta recibida: '{respuesta}'", "DEBUG", MessageBoxButtons.OK, MessageBoxIcon.Information);

            if (respuesta.StartsWith("13/"))
            {
                try
                {
                    string[] partes = respuesta.Split('/');
                    string jugadores = partes[1];

                    // Lista de usuarios conectados
                    listBoxUsuarios.Items.Clear();
                    string[] usuarios = jugadores.Split('\n');

                    foreach (string usuario_connectado in usuarios)
                        if (!string.IsNullOrWhiteSpace(usuario_connectado))
                            listBoxUsuarios.Items.Add(usuario_connectado);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error al obtener usuarios: " + ex.Message);
                }
            }
        }

        private void Form1_close(object sender, EventArgs e)
        {
            this.Close();
        }

        

        private void btn_Actualizar_Click(object sender, EventArgs e)
        {
            string mensaje = "13/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] buffer = new byte[2048];
            int bytesRecibidos = server.Receive(buffer);
            string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim();

            // Mensaje de depuración para ver la respuesta exacta del servidor
            MessageBox.Show($"Depuración - Respuesta recibida: '{respuesta}'", "DEBUG", MessageBoxButtons.OK, MessageBoxIcon.Information);
            if (respuesta.StartsWith("13/"))
            {
                try
                {
                    string[] partes = respuesta.Split('/');
                    string jugadores = partes[1];

                    // Lista de usuarios conectados
                    listBoxUsuarios.Items.Clear();
                    string[] usuarios = jugadores.Split('\n');

                    foreach (string usuario_connectado in usuarios)
                        if (!string.IsNullOrWhiteSpace(usuario_connectado))
                            listBoxUsuarios.Items.Add(usuario_connectado);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error al obtener usuarios: " + ex.Message);
                }
            }
        }
    }
}

