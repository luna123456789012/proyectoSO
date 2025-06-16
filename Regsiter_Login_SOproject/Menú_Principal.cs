using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.IO;

namespace Regsiter_Login_SOproject
{
    public partial class Menú_Principal : Form
    {
        private Socket server;
        private string usuarioActual;

        private volatile bool escuchando = true;
        private bool cerradoManualmente = true;

        private bool conectado;
        private int ID_partida;

        private Thread hiloEscucha;
        public Menú_Principal(Socket serverSocket, string usuario, bool conectado)
        {
            InitializeComponent();
            this.BackgroundImage = Properties.Resources.Portada;
            this.BackgroundImageLayout = ImageLayout.Stretch;

            CheckForIllegalCrossThreadCalls = false;
            this.server = serverSocket;
            this.usuarioActual = usuario;
            this.conectado = conectado;

            this.Load += Menú_Principal_Load;
            this.FormClosing += Menú_Principal_FormClosing;
        }

        private void Menú_Principal_Load(object sender, EventArgs e)
        {
            escuchando = true;
            ThreadStart ts = delegate { EscucharServidor(); };
            hiloEscucha = new Thread(ts);
            hiloEscucha.IsBackground = true;
            hiloEscucha.Start();
        }

        private void EscucharServidor()
        {            
            while (escuchando)
            {
                try
                {
                    byte[] buffer = new byte[2048];
                    int bytesRecibidos = server.Receive(buffer);
                    string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim();

                    // Mensaje de depuración para ver la respuesta exacta del servidor
                    MessageBox.Show($"Depuración - Respuesta recibida: '{respuesta}'", "DEBUG", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    this.Invoke((MethodInvoker)delegate
                    {
                        ProcesarRespuesta(respuesta);
                    });
                }
                catch
                {
                    if (conectado)
                    {
                        this.Invoke((MethodInvoker)(() =>
                        {
                            MessageBox.Show("Error de conexión o el servidor se cerró.");
                        }));
                    }
                    break;
                }
            }
        }

        private void ProcesarRespuesta(string respuesta)
        {
            try
            {
                if (respuesta.StartsWith("INVITE/"))
                {
                    string[] partes = respuesta.Split('/');
                    string invitador = partes[1];
                    string lista = partes[2];

                    DialogResult result = MessageBox.Show($"{invitador} te ha invitado a una partida.\n¿Deseas aceptar?", "Invitación", MessageBoxButtons.YesNo);
                    string decision = result == DialogResult.Yes ? "ACEPTAR" : "RECHAZAR";
                    string mensaje = $"12/{usuarioActual}/{invitador}/{decision}";
                    byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
                else if (respuesta.StartsWith("1/"))
                {
                    string[] partes = respuesta.Split('/');
                    if (partes.Length >= 3)
                    {
                        try
                        {
                            int partida = int.Parse(partes[1]);
                            ID_partida = partida;
                            MessageBox.Show(partes[2], "Información", MessageBoxButtons.OK, MessageBoxIcon.Information);

                            // Abre el siguiente formulario después de encontrar partida
                            Querys query = new Querys(server, usuarioActual, conectado, ID_partida);
                            query.Show();

                            cerradoManualmente = false;
                            this.Close(); // Cerrar el formulario de menú principal
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("Error al procesar la respuesta: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                    else
                    {
                        MessageBox.Show(partes[1], "Error", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else if (respuesta.StartsWith("5/"))
                {
                    try
                    {
                        string[] partes = respuesta.Split('/');
                        string mensajeDesconexion = partes[1];
                        // Confirmación de cierre de sesión
                        MessageBox.Show(mensajeDesconexion);
                        conectado = false;

                        server.Shutdown(SocketShutdown.Both);
                        server.Close();

                        this.Close();
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error al cerrar sesión: " + ex.Message);
                    }
                }
                else if (respuesta.StartsWith("20/"))
                {
                    try
                    {
                        string[] partes = respuesta.Split('/');
                        string[] usuarios = partes[1].Split(',');
                        foreach (string usuario in usuarios)
                            if (!string.IsNullOrWhiteSpace(usuario))
                                JugadoresAnteriores.Items.Add(usuario);


                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error: " + ex.Message);
                    }
                }
                else if (respuesta.StartsWith("21/"))
                {
                    try
                    {
                        string[] partes = respuesta.Split('\n');
                        string numero = partes[0];
                        foreach (string partida in partes)
                            if (!string.IsNullOrWhiteSpace(partida))
                                PartidasAnteriores.Items.Add(partida);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error: " + ex.Message);
                    }
                }
                else if (respuesta.StartsWith("22/"))
                {
                    try
                    {

                        string[] partes = respuesta.Split('/');
                        string[] partidas = partes[1].Split(',');
                        foreach (string partida in partidas)
                            if (!string.IsNullOrWhiteSpace(partida))
                                PartidasRecientes.Items.Add(partida);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error: " + ex.Message);
                    }
                }
                else if (respuesta.StartsWith("23/"))
                {
                    try
                    {
                        string[] partes = respuesta.Split('/');
                        string res = partes[1];
                        if (res == "OK")
                        {
                            MessageBox.Show("Cuenta eliminada con éxito");
                        }
                        else
                        {
                            MessageBox.Show("Error al eliminar la cuenta");
                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error: " + ex.Message);
                    }
                }
            }
            catch
            {
                MessageBox.Show($"Mensaje no reconocido: {respuesta}");
            }

        }
        private void button1_Click(object sender, EventArgs e)
        {
            string mensaje = $"1/{usuarioActual}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);                                             
        }

        private void Menú_Principal_FormClosing(object sender, FormClosingEventArgs e)
        {
            escuchando = false;

            if (hiloEscucha != null && hiloEscucha.IsAlive)
            {
                hiloEscucha.Join(1000); // Espera a que el hilo termine
            }

            if (cerradoManualmente && server != null && server.Connected)
            {
                try
                {
                    string mensaje = $"5/{usuarioActual}";
                    byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("No se pudo notificar cierre de sesión al servidor: " + ex.Message);
                }
            }
        }


        private void btnCerrasSesion_Click(object sender, EventArgs e)
        {
            if (server == null || !server.Connected)
            {
                MessageBox.Show("No estás conectado al servidor.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string mensaje = $"5/{usuarioActual}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void Menú_Principal_Load_1(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            string mensaje = $"20/{usuarioActual}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (usuarioBox.Text == "")
            {
                MessageBox.Show("Por favor, rellene todos los campos requeridos");
            }
            else
            {
                string mensaje = $"21/{usuarioActual}/{usuarioBox.Text}";
                MessageBox.Show(mensaje);
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if ((diaBox.Text == "") || (mesBox.Text == "") || (añoBox.Text == ""))
            {
                MessageBox.Show("Por favor, rellene todos los campos requeridos");
            }
            else
            {
                string mensaje = $"22/{usuarioActual}/{diaBox.Text}/{mesBox.Text}/{añoBox.Text}";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show($"¿Estás seguro de que quieres borrar la cuenta?", "Eliminar", MessageBoxButtons.YesNo);
            string decision = result == DialogResult.Yes ? "SI" : "NO";
            if (decision == "SI")
            {
                string mensaje = $"23/{usuarioActual}";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }
    }
}
