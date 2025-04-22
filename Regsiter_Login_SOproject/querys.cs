using System;
using System.Drawing;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using System.Linq;

namespace Regsiter_Login_SOproject
{
    public partial class Querys : Form
    {
        private Socket server;
        private string usuarioActual;

        private bool conectado;
        private Thread escuchaThread;


        private PictureBox currentPictureBox;
        private Point mouseOffset;
        private bool isDragging = false;
        private bool isMousePressed = false;

        private System.Windows.Forms.Timer tiempo;

        public Querys(Socket serverSocket, string usuario, bool conectado)
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            this.server = serverSocket;
            this.usuarioActual = usuario;
            this.conectado = conectado;
        }

        private void Querys_Load(object sender, EventArgs e)
        {
            ThreadStart ts = delegate { EscucharServidor (); };
            escuchaThread = new Thread(ts);
            escuchaThread.IsBackground = true;
            escuchaThread.Start();

            tiempo = new System.Windows.Forms.Timer();
            tiempo.Interval = 1000; // 2000 milisegundos = 2 segundos
            tiempo.Tick += Temporizador_Tick;
            tiempo.Start();
        }

        private void Temporizador_Tick(object sender, EventArgs e)
        {
            tiempo.Stop(); // Para que solo se ejecute una vez

            // Acción a realizar después de 2 segundos
            string mensaje = "6/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            Thread.Sleep(100);

            string mensaje1 = "8/";
            byte[] msg1 = Encoding.ASCII.GetBytes(mensaje1);
            server.Send(msg1);
        }
        private void Querys_close(object sender, EventArgs e)
        {
            escuchaThread.Abort();
        }

        //PROCEDIMIENTO DE REPARTO INICIAL DE CARTAS
        private void CargarCartasIniciales(string respuesta)
        {
            try
            {
                string[] partes = respuesta.Split('/');
                if (partes.Length > 1)
                {
                    string[] cartas = partes[1].Split(';');
                    MostrarCartas(cartas);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error al cargar cartas: " + ex.Message);
            }
        }

        //PROCEDIMIENTO PARA MOSTRAR LAS CARTAS EN LA PANTALLA
        private void MostrarCartas(string[] cartas)
        {
            panel.Controls.Clear(); // Limpiar panel antes de mostrar las nuevas cartas

            int x = 10;
            int y = 10;
            int separacion = 110;

            foreach (string carta in cartas)
            {
                if (!string.IsNullOrWhiteSpace(carta))
                {
                    string[] datosCarta = carta.Split(','); // [ID, Color, Número]
                    if (datosCarta.Length == 3)
                    {
                        string nombreArchivo = $"{datosCarta[2]}{datosCarta[1]}.png";
                        string rutaCompleta = Path.Combine(Application.StartupPath, "cartas", nombreArchivo);
                        if (System.IO.File.Exists(rutaCompleta)) // Verificar si la imagen existe
                        {
                            PictureBox pic = new PictureBox
                            {
                                Image = Image.FromFile(rutaCompleta),
                                SizeMode = PictureBoxSizeMode.StretchImage,
                                Width = 100,
                                Height = 150,
                                Location = new Point(x, y),
                                Margin = new Padding(5)
                            };
                            pic.MouseDown += PictureBox_MouseDown;
                            pic.MouseMove += PictureBox_MouseMove;
                            pic.MouseUp += PictureBox_MouseUp;
                            pic.MouseClick += new MouseEventHandler(PictureBox_MouseClick);

                            pic.Tag = nombreArchivo; //guardar nombre del archivo

                            panel.Controls.Add(pic); // Agregar la imagen al panel

                            x += separacion;
                            if (x + 100 > panel.Width)
                            {
                                x = 10;
                                y += 160;
                            }
                        }
                        else
                        {
                            MessageBox.Show($"No se encontró la imagen: {nombreArchivo}");
                        }
                    }
                }
            }
        }

        // PROCEDIMIENTO PARA VER SI EL MOVIMIENTO ES VÁLIDO
        private int Apto(object sender, string color, string numero)
        {

            // Obtener la carta en panel1 (supongamos que solo hay una carta)
            PictureBox cartaEnPanel1 = panel1.Controls.OfType<PictureBox>().FirstOrDefault();
            if (cartaEnPanel1 != null)
            {
                string nombreArchivo1 = cartaEnPanel1.Tag?.ToString();
                string fileName1 = Path.GetFileName(nombreArchivo1); // Obtener el nombre del archivo
                string nombreSinExtension1 = Path.GetFileNameWithoutExtension(fileName1);

                string numero1 = nombreSinExtension1[0].ToString();
                string color1 = nombreSinExtension1.Substring(1);

                string mensaje = $"9/{color}/{numero}/{color1}/{numero1}";

                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                // Recibir respuesta del servidor
                byte[] buffer = new byte[512];
                int bytesRecibidos = server.Receive(buffer);
                string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos);
                MessageBox.Show(respuesta);
                Console.WriteLine($"Respuesta recibida: {respuesta}");
                string valido = respuesta[2].ToString();

                //if (int.TryParse(respuesta.Trim(), out respuestaChar))
                
                    if (valido == "y")
                    {
                        return 0;   // es apto
                    }
                    else if (valido == "n")
                    {
                        return 1;   // no es apto
                    }
                    else
                    {
                        MessageBox.Show("Respuesta fuera de rango: " + respuesta);
                        return -1;
                    }
                }
                else
                {
                    MessageBox.Show("Respuesta no válida del servidor", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            
            return -1;
        }
        //PROCEDIMIENTO PARA AÑADIR LA CARTA INICIAL DE LA PARTIDA
        private void AddCartaPartida(string respuesta)
        {
            //Añadir la carta de inicio de partida
            string mensaje1 = "8/";
            byte[] msg1 = Encoding.ASCII.GetBytes(mensaje1);
            server.Send(msg1);

            // Recibir respuesta del servidor
            byte[] buffer1 = new byte[512];
            int bytesRecibidos1 = server.Receive(buffer1);
            string respuesta1 = Encoding.ASCII.GetString(buffer1, 0, bytesRecibidos1);

            string[] datosCarta1 = respuesta1.Split(','); // [ID, Color, Número]
            if (datosCarta1.Length == 3)
            {
                string nombreArchivo1 = $"{datosCarta1[2]}{datosCarta1[1]}.png";
                string rutaCompleta1 = Path.Combine(Application.StartupPath, "cartas", nombreArchivo1);

                if (System.IO.File.Exists(rutaCompleta1)) // Verificar si la imagen existe
                {
                    PictureBox pic1 = new PictureBox
                    {
                        Image = Image.FromFile(rutaCompleta1),
                        SizeMode = PictureBoxSizeMode.StretchImage,
                        Width = 100,
                        Height = 150,
                        Margin = new Padding(5),
                    };

                    pic1.Tag = nombreArchivo1; //guardar nombre del archivo
                    panel1.Controls.Add(pic1); // Agregar la imagen al panel
                }
                else
                {
                    MessageBox.Show($"No se encontró la imagen: {nombreArchivo1}");
                }
            }
        }


        //PROCEDIMIENTO PARA QUE LA CARTA SELECCIONADA SE MUEVA AL PANEL DE PARTIDA
        private void SubmitCard(PictureBox pic)
        {
            panel.Controls.Remove(pic);
            pic.Location = new Point(10, 10);
            panel1.Controls.Add(pic);
            pic.BringToFront();
        }
        //PROCEDIMIENTO CUANDO SE HACE CLICK LARGO A UN PICTUREBOX
        private void PictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (sender is PictureBox pic && panel1.ClientRectangle.Contains(e.Location) && e.Button == MouseButtons.Left)
            {
                currentPictureBox = pic;
                isMousePressed = true;
                mouseOffset = e.Location;
                isDragging = false; 
            }
        }
        //PROCEDIMIENTO PARA CUANDO SE ARRASTRA UN PICTUREBOX
        private void PictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            if (currentPictureBox != null && isMousePressed)
            {
                int distance = (int)(Math.Pow(e.X - mouseOffset.X, 2) + Math.Pow(e.Y - mouseOffset.Y, 2));
                if (distance > 25) // Umbral de 5 píxeles, sensibilidad
                {
                    isDragging = true;
                }

                if (isDragging)
                {
                    currentPictureBox.Left += e.X - mouseOffset.X;
                    currentPictureBox.Top += e.Y - mouseOffset.Y;
                }
            }
        }
        //PROCEDIMIENTO PARA CUANDO SE SUELTA EL CLICK DE UN PICTUREBOX
        private void PictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            currentPictureBox = null;
            isMousePressed = false;
            isDragging = false;
        }

        //PROCEDIMIENTO PARA CUANDO SE LE HACE CLICK A UN PICTUREBOX
        private void PictureBox_MouseClick(object sender, MouseEventArgs e)
        {
            if (sender is PictureBox pic)
            {
                if (!isDragging)
                {
                    string nombreArchivo = pic.Tag?.ToString(); 
                    if (!string.IsNullOrEmpty(nombreArchivo))
                    {
                        string fileName = Path.GetFileName(nombreArchivo);
                        string nombreSinExtension = Path.GetFileNameWithoutExtension(fileName);

                        string numero1 = nombreSinExtension[0].ToString();
                        string color1 = nombreSinExtension.Substring(1);
                        if (Apto(sender, color1, numero1) == 0)
                        {
                            SubmitCard(pic);
                        }
                        else
                        {
                            MessageBox.Show("Esta carta no es válida");
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("El objeto en el que se hizo clic no es un PictureBox.");
            }
        }


        //THREAD ESCUCHA DEL SERVIDOR
        private void EscucharServidor()
        {
            while (conectado)
            {
                try
                {
                    byte[] buffer = new byte[2048];
                    int bytesRecibidos = server.Receive(buffer);
                    string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim();

                    this.Invoke((MethodInvoker)delegate
                    {
                        ProcesarRespuesta(respuesta);
                    });
                }
                catch
                {
                    conectado = false;
                    this.Invoke((MethodInvoker)delegate 
                    {
                        MessageBox.Show("Conexión cerrada por el servidor.");
                        this.Close();
                    });
                }
            }
        }

        private void ProcesarRespuesta(string respuesta)
        {
            try
            {
                if (respuesta.StartsWith("NOT"))
                {
                    try
                    {
                        string[] partes = respuesta.Split(':');
                        string notificacion = partes[1];

                        if (!string.IsNullOrWhiteSpace(notificacion))
                        {
                            Notificaciones.Items.Add(notificacion);
                        }                          
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error al recibir la notificaciones del servidor: " + ex.Message);
                    }
                }
                if (respuesta.StartsWith("4/"))
                {
                    try
                    {
                        string[] partes = respuesta.Split('/');
                        string jugadores = partes[1];

                        // Lista de usuarios conectados
                        listBoxUsuarios.Items.Clear();
                        string[] usuarios = jugadores.Split('\n');

                        foreach (string usuario in usuarios)
                            if (!string.IsNullOrWhiteSpace(usuario))
                                listBoxUsuarios.Items.Add(usuario);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error al obtener usuarios: " + ex.Message);
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

                else if (respuesta.StartsWith("6/"))
                {
                    CargarCartasIniciales(respuesta);
                }
                else if (respuesta.StartsWith("7/"))
                {
                    try
                    {
                        // Carta recibida
                        string[] datosCarta = respuesta.Split(',');
                        if (datosCarta.Length == 3)
                        {
                            string nombreArchivo = $"{datosCarta[2]}{datosCarta[1]}.png";
                            string rutaCompleta = Path.Combine(Application.StartupPath, "cartas", nombreArchivo);

                            if (System.IO.File.Exists(rutaCompleta)) // Verificar si la imagen existe
                            {
                                PictureBox pic = new PictureBox
                                {
                                    Image = Image.FromFile(rutaCompleta),
                                    SizeMode = PictureBoxSizeMode.StretchImage,
                                    Width = 100,
                                    Height = 150,
                                    Margin = new Padding(5),
                                };

                                pic.MouseDown += PictureBox_MouseDown;
                                pic.MouseMove += PictureBox_MouseMove;
                                pic.MouseUp += PictureBox_MouseUp;

                                panel.Controls.Add(pic); // Agregar la imagen al panel
                            }
                            else
                            {
                                MessageBox.Show($"No se encontró la imagen: {nombreArchivo}");
                            }
                        }
                    }

                    catch (Exception ex)
                    {
                        MessageBox.Show("Error al pedir carta: " + ex.Message);
                    }
                }
                else if (respuesta.StartsWith("8/"))
                {
                    AddCartaPartida(respuesta);
                }
                else if (respuesta.StartsWith("9/"))
                {
                    
                }
                else if (respuesta.StartsWith("CHAT/"))
                {
                    string mensaje = respuesta.Substring(5); // eliminar "CHAT/"
                    listBoxChat.Items.Add(mensaje);
                }
                else if (respuesta.StartsWith("INVITE/"))
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
                else if (respuesta.StartsWith("13/"))
                {
                    string mensaje = respuesta.Substring(3);
                    MessageBox.Show(mensaje, "Resultado de la invitación", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }


            }
            catch
            {
                MessageBox.Show($"Mensaje no reconocido: {respuesta}");
            }
            
        }


        private void btnVerUsuarios_Click(object sender, EventArgs e)
        {
            string mensaje = "4/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void btnCerrarSesion_Click(object sender, EventArgs e)
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

        private void Dame_Click(object sender, EventArgs e)
        {
                string mensaje = "7/";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);               
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel_Paint(object sender, PaintEventArgs e)
        {

        }

        private void btnEnviarChat_Click(object sender, EventArgs e)
        {
            string mensaje = txtMensajeChat.Text.Trim();
            if (!string.IsNullOrEmpty(mensaje))
            {
                string comando = $"10/{usuarioActual}/{mensaje}";
                byte[] msg = Encoding.ASCII.GetBytes(comando);
                server.Send(msg);
                txtMensajeChat.Clear();
            }
        }

        private void btnInvitar_Click(object sender, EventArgs e)
        {
            // Lista seleccionada en el ListBox de usuarios
            var seleccionados = listBoxUsuarios.SelectedItems.Cast<string>().ToArray();
            if (seleccionados.Length == 0)
            {
                MessageBox.Show("Selecciona al menos un jugador a invitar.");
                return;
            }

            string lista = string.Join(",", seleccionados);
            string mensaje = $"11/{usuarioActual}/{lista}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Guardar estado temporal si querés controlar desde cliente también
        }

    }
}
