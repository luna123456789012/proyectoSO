using System;
using System.Drawing;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using System.Linq;
using System.Diagnostics;
using System.Threading.Tasks;

namespace Regsiter_Login_SOproject
{
    public partial class Querys : Form
    {
        private Socket server;
        private string usuarioActual;
        private int Id_partida;
        private int Id_partida_inv;

        private bool conectado;
        private Thread escuchaThread;
        private bool cerradoManualmente = true;
        private bool menu_principal = true;

        private PictureBox currentPictureBox;
        private Point mouseOffset;
        private bool isDragging = false;
        private bool isMousePressed = false;

        private System.Windows.Forms.Timer tiempo;
        private System.Windows.Forms.Timer time;

        private PictureBox ultimaCartaEvaluada;
        private bool esperandoRespuesta = false;

        private PictureBox selectedPictureBox = null;

        int turnoActual;
        int turnoAsignado;

        public Querys(Socket serverSocket, string usuario, bool conectado, int partida)
        {
            InitializeComponent(); 
            button3.Enabled = false;
            CheckForIllegalCrossThreadCalls = false;
            this.server = serverSocket;
            this.usuarioActual = usuario;
            this.conectado = conectado;
            this.Id_partida = partida;

            this.FormClosing += new FormClosingEventHandler(Querys_FormClosing);
        }

        private void Querys_Load(object sender, EventArgs e)
        {
            ThreadStart ts = delegate { EscucharServidor(); };
            escuchaThread = new Thread(ts);
            escuchaThread.IsBackground = true;
            escuchaThread.Start();

            tiempo = new System.Windows.Forms.Timer();
            tiempo.Interval = 1000; // 1000 milisegundos = 1 segundo
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

            string mensaje1 = $"8/{Id_partida}";
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
                            VerificarContenidoDelPanel();

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
        private int Apto(string color, string numero, PictureBox pic)
        {
            try
            {
                PictureBox cartaEnPanel1 = panel1.Controls.OfType<PictureBox>().FirstOrDefault();
                if (cartaEnPanel1 == null || cartaEnPanel1.Tag == null)
                {
                    MessageBox.Show("No hay carta en el panel de partida.");
                    return -1;
                }

                string nombreArchivo1 = Path.GetFileNameWithoutExtension(cartaEnPanel1.Tag.ToString());
                if (nombreArchivo1.Length < 2)
                {
                    MessageBox.Show("Nombre de carta inválido.");
                    return -1;
                }

                string numero1 = nombreArchivo1[0].ToString();
                string color1 = nombreArchivo1.Substring(1);

                string mensaje = $"9/{color}/{numero}/{color1}/{numero1}";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                // Guardamos la carta seleccionada y marcamos estado de espera
                ultimaCartaEvaluada = pic;
                esperandoRespuesta = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show("Error al enviar jugada: " + ex.Message);
            }
            return 0; // Retornar 0 para indicar que se envió correctamente
        }

        //PROCEDIMIENTO PARA QUE LA CARTA SELECCIONADA SE MUEVA AL PANEL DE PARTIDA
        private void SubmitCard(PictureBox pic)
        {
            panel.Controls.Remove(pic);
            VerificarContenidoDelPanel();
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
            if (sender is PictureBox pic && !isDragging)
            {
                selectedPictureBox = pic;
                string nombreArchivo = pic.Tag?.ToString();
                if (!string.IsNullOrEmpty(nombreArchivo))
                {
                    string fileName = Path.GetFileName(nombreArchivo);
                    string nombreSinExtension = Path.GetFileNameWithoutExtension(fileName);

                    string numero1 = nombreSinExtension[0].ToString();
                    string color1 = nombreSinExtension.Substring(1);

                    Apto(color1, numero1, pic); // Solo envía
                }
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

                    // Mensaje de depuración para ver la respuesta exacta del servidor
                    MessageBox.Show($"Depuración - Respuesta recibida: '{respuesta}'", "DEBUG", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    this.Invoke((MethodInvoker)delegate
                    {
                        ProcesarRespuesta(respuesta);
                    });
                }
                catch
                {
                    conectado = false;
                    try
                    {
                        if (!this.IsDisposed && !this.Disposing)
                        {
                            this.Invoke((MethodInvoker)delegate
                            {
                                MessageBox.Show("Conexión cerrada por el servidor.");
                                this.Close();
                            });
                        }
                    }
                    catch (ObjectDisposedException)
                    {
                        // Ignoramos porque el formulario ya está destruido
                    }
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

                {try

                    {string[] partes = respuesta.Split('/');

                        string mensajeDesconexion = partes[1];

                        // Confirmación de cierre de sesión

                        MessageBox.Show(mensajeDesconexion);

                        conectado = false;
                        server.Shutdown(SocketShutdown.Both);

                        server.Close();
                        this.Close();

                    }catch (Exception ex)

                    {MessageBox.Show("Error al cerrar sesión: " + ex.Message);}
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
                        if (datosCarta.Length == 2)
                        {
                            string color = datosCarta[0].Substring(2);
                            string nombreArchivo = $"{datosCarta[1]}{color}.png";
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
                                pic.MouseClick += new MouseEventHandler(PictureBox_MouseClick);
                                pic.Tag = nombreArchivo;

                                panel.Controls.Add(pic); // Agregar la imagen al panel
                                VerificarContenidoDelPanel();
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


                    string datos = respuesta.Substring(2);
                    string[] datosCarta = datos.Split(','); // datosCarta[0]=ID, [1]=color, [2]=número

                    if (datosCarta.Length == 3)
                    {
                        string nombreArchivo = $"{datosCarta[2]}{datosCarta[1]}.png"; // Ejemplo: "3rojo.png"
                        string rutaCompleta = Path.Combine(Application.StartupPath, "cartas", nombreArchivo);

                        if (System.IO.File.Exists(rutaCompleta))
                        {
                            PictureBox pic = new PictureBox
                            {
                                Image = Image.FromFile(rutaCompleta),
                                SizeMode = PictureBoxSizeMode.StretchImage,
                                Width = 100,
                                Height = 150,
                                Margin = new Padding(5),
                            };

                            pic.Tag = nombreArchivo; // Guardar nombre archivo
                            panel1.Controls.Add(pic); // Agregar carta al panel
                            VerificarContenidoDelPanel();
                        }
                        else
                        {
                            MessageBox.Show($"No se encontró la imagen: {nombreArchivo}");
                        }
                    }
                }
                else if (respuesta.StartsWith("9/") && esperandoRespuesta)
                {
                    esperandoRespuesta = false;

                    string resultado = respuesta.Length > 2 ? respuesta[2].ToString() : "";

                    if (resultado == "y" && ultimaCartaEvaluada != null)
                    {
                        SubmitCard(ultimaCartaEvaluada);
                        if (selectedPictureBox != null)

                        {
                            string nombreArchivo1 = Path.GetFileNameWithoutExtension(selectedPictureBox.Tag.ToString());

                            if (nombreArchivo1.Length < 2)

                            {
                                MessageBox.Show("Nombre de carta inválido.");
                            }
                            string numero = nombreArchivo1[0].ToString();
                            string color = nombreArchivo1.Substring(1);

                            string mensaje = $"14/{color}/{numero}/{Id_partida}";
                            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                            server.Send(msg);
                        }
                    }
                    else if (resultado == "n")
                    {
                        MessageBox.Show("Esta carta no es válida");
                    }
                    else
                    {
                        MessageBox.Show("Respuesta inesperada: " + respuesta);
                    }

                    ultimaCartaEvaluada = null;

                }
                else if (respuesta.StartsWith("INVITE/"))
                {
                    string[] partes = respuesta.Split('/');
                    string invitador = partes[1];
                    string partida = partes[2];
                    Id_partida_inv = Convert.ToInt32(partida);

                    DialogResult result = MessageBox.Show($"{invitador} te ha invitado a una partida.\n¿Deseas aceptar?", "Invitación", MessageBoxButtons.YesNo);
                    string decision = result == DialogResult.Yes ? "ACEPTAR" : "RECHAZAR";
                    string mensaje = $"12/{usuarioActual}/{invitador}/{decision}/{Id_partida}";
                    byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
                else if (respuesta.StartsWith("11/"))
                {
                    string[] partes = respuesta.Split('/');
                    string mensaje = partes[1];
                }
                else if (respuesta.StartsWith("12/"))

                {
                    string[] partes = respuesta.Split('/');
                    string usuarioInvitado = partes[1];

                    if (partes[2] == "RECHAZADA")
                    {
                        MessageBox.Show($"{usuarioInvitado} ha rechazado tu invitación");
                    }

                    else if (partes[2] == "ACEPTADA")
                    {
                        MessageBox.Show($"{usuarioInvitado} ha aceptado tu invitación");
                    }
                }
                else if (respuesta.StartsWith("13/"))
                {
                    // Abre el siguiente formulario después de encontrar partida
                    Querys query = new Querys(server, usuarioActual, conectado, Id_partida_inv);
                    query.Show();

                    cerradoManualmente = false;
                    menu_principal = false;
                    this.Close(); // Cerrar el formulario de menú principal

                    MessageBox.Show($"Has entrado en la nueva partida");

                }
                else if (respuesta.StartsWith("CHAT/"))
                {
                    string mensaje = respuesta.Substring(5);
                    listBoxChat.Items.Add(mensaje);
                }
                else if (respuesta.StartsWith("INVITE/"))
                {
                    string[] partes = respuesta.Split('/');
                    string invitador = partes[1];
                    string partida = partes[2];
                    Id_partida_inv=Convert.ToInt32(partida);

                    DialogResult result = MessageBox.Show($"{invitador} te ha invitado a una partida.\n¿Deseas aceptar?", "Invitación", MessageBoxButtons.YesNo);
                    string decision = result == DialogResult.Yes ? "ACEPTAR" : "RECHAZAR";
                    string mensaje = $"12/{usuarioActual}/{invitador}/{decision}/{Id_partida}";
                    byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
                else if (respuesta.StartsWith("13/"))
                {
                    panel.Controls.Clear();
                    panel1.Controls.Clear();

                    string[] partes = respuesta.Split('/');
                    string usuarioGanador = partes[1];

                    Label lbl = new Label();
                    lbl.Text = $"El ganador es: {usuarioGanador}";
                    lbl.Dock = DockStyle.Fill;
                    lbl.TextAlign = ContentAlignment.MiddleCenter;
                    lbl.Font = new Font("Arial", 20, FontStyle.Bold);

                    panel.Controls.Add(lbl);
                }
                else if (respuesta.StartsWith("14/"))

                {
                    string contenido = respuesta.Substring(3);
                    string[] datosCarta = contenido.Split(',');

                    if (datosCarta.Length == 2)
                    {
                        string nombreArchivo = $"{datosCarta[1]}{datosCarta[0]}.png";
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
                            panel1.Controls.Add(pic);
                            VerificarContenidoDelPanel();
                            pic.Tag = nombreArchivo;
                            pic.BringToFront();
                        }
                    }
                }
                else if (respuesta.StartsWith("15/"))
                {
                    string[] partes = respuesta.Split('/');
                    string mensaje = partes[1];
                    MessageBox.Show(respuesta);

                    this.Close();
                }
                else if (respuesta.StartsWith("16/"))
                {
                    try
                    {
                        string[] partes = respuesta.Split('/');
                        string jugadores = partes[1];

                        // Lista de usuarios conectados
                        listBoxConectados.Items.Clear();
                        string[] usuarios = jugadores.Split('\n');

                        foreach (string usuario_connectado in usuarios)
                            if (!string.IsNullOrWhiteSpace(usuario_connectado))
                                listBoxConectados.Items.Add(usuario_connectado);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error al obtener usuarios: " + ex.Message);
                    }
                }
                else if (respuesta.StartsWith("17/"))
                {
                    string[] partes = respuesta.Split('/');
                    string UNO = partes[1];
                    MessageBox.Show("El jugador " + UNO + " dijo UNO!");
                }
                else

                {
                    MessageBox.Show($"Mensaje no reconocido: {respuesta}");
                }
            }
            catch
            {
                MessageBox.Show($"Mensaje no reconocido: {respuesta}");
            }

        }


        private void btnVerUsuarios_Click(object sender, EventArgs e)
        {
            string mensaje = $"4/{Id_partida}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string mensaje = "7/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void Invitar_Click(object sender, EventArgs e)
        {
            // Lista seleccionada en el ListBox de usuarios
            var seleccionados = listBoxConectados.SelectedItems.Cast<string>().ToArray();
            if (seleccionados.Length == 0)
            {
                MessageBox.Show("Selecciona al menos un jugador a invitar.");
                return;
            }

            string lista = string.Join(",", seleccionados);
            string mensaje = $"11/{usuarioActual}/{lista}/{Id_partida}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            // Guardar estado temporal si querés controlar desde cliente también
        }

        private void VerConectados_Click(object sender, EventArgs e)
        {
            string mensaje = "16/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
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

        private void button2_Click(object sender, EventArgs e)
        {
            cerradoManualmente = false;
            menu_principal = true;

            string mensaje = "15/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void Querys_FormClosing(object sender, FormClosingEventArgs e)
        {
            conectado = false;
            if (menu_principal)
            {
                // Abre el siguiente formulario después del login
                Menú_Principal main_principal = new Menú_Principal(server, usuarioActual, conectado);
                main_principal.Show();
            }
            

            if (escuchaThread != null && escuchaThread.IsAlive)
            {
                escuchaThread.Join(1000); // Espera a que termine
            }

            if (cerradoManualmente && menu_principal && server != null && server.Connected)
            {
                try
                {
                    string mensaje = $"15/";
                    byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("No se pudo notificar cierre de sesión al servidor: " + ex.Message);
                }
            }
        }

        private void Listo_Click(object sender, EventArgs e)
        {
            this.Listo.Text = "NO LISTO";
            string mensaje = "16/";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void Listo_Click_1(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e) //para decir UNO
        {
            string mensaje = $"18/{usuarioActual}/{Id_partida}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }
        void DeshabilitarBotones(Control parent, Button excepcion) //deshabilita todos los botones excepto el que le pones en excepción
        {
            foreach (Control ctrl in parent.Controls)
            {
                if (ctrl is Button && ctrl != excepcion)
                    ctrl.Enabled = false;
                else if (ctrl.HasChildren)
                    DeshabilitarBotones(ctrl, excepcion);
            }
        }

        private void VerificarContenidoDelPanel() //cuenta cuántas cartas hay en el panel
        {
            int count = panel.Controls.OfType<PictureBox>().Count();

            if (count == 1) //si sólo hay una carta, habilitar el botón UNO
            {
                button3.Enabled = true;
                button3.BackColor = Color.FromArgb(192, 255, 192);
            }
            else if (count == 0) //si no hay cartas, eres el ganador
            {
                button3.Enabled = false;
                string mensaje = $"13/{usuarioActual}/{Id_partida}";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                button3.Enabled = false;
                button3.BackColor = Color.FromArgb(255, 128, 128);
            }
        }
    }
}



