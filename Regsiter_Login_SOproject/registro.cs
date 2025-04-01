﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace Regsiter_Login_SOproject
{
    public partial class RegistroDeUsuarios: Form
    {
        Socket server;
        public RegistroDeUsuarios()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void txtUsuario_TextChanged(object sender, EventArgs e)
        {

        }

        private void btnRegistrar_Click(object sender, EventArgs e)
        {
            string usuario = txtUsuario.Text.Trim();
            string contrasena = txtContrasena.Text.Trim();

            if (string.IsNullOrEmpty(usuario) || string.IsNullOrEmpty(contrasena))
            {
                MessageBox.Show("Por favor, complete todos los campos.", "Datos incompletos", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (server == null || !server.Connected)
            {
                MessageBox.Show("No estás conectado al servidor.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Enviar datos al servidor con el formato: "2/usuario/contrasena"
            string mensaje = $"2/{usuario}/{contrasena}";
            byte[] msg = Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Recibir respuesta del servidor
            byte[] buffer = new byte[512];
            int bytesRecibidos = server.Receive(buffer);
            string respuesta = Encoding.ASCII.GetString(buffer, 0, bytesRecibidos).Trim(); // 🔹 Trim para limpiar espacios y saltos de línea

            MessageBox.Show($"Depuración - Respuesta recibida: '{respuesta}'", "DEBUG", MessageBoxButtons.OK, MessageBoxIcon.Information); // 🔴 Ver respuesta exacta

            // Comparar exactamente con "OK" y "EXISTE"
            if (respuesta.Equals("OK", StringComparison.OrdinalIgnoreCase)) // 🔹 Comparación segura
            {
                MessageBox.Show("Registro exitoso.", "Éxito", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else if (respuesta.Equals("EXISTE", StringComparison.OrdinalIgnoreCase))
            {
                MessageBox.Show("El nombre de usuario ya está registrado. Elija otro.", "Registro fallido", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                MessageBox.Show($"Error en el registro. Respuesta recibida: '{respuesta}'", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnConsultarContrasena_Click(object sender, EventArgs e)
        {
            // Quiere saber la longitud
            string mensaje = "1/" + txtPasswrdConsult.Text;
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show("Tu contraseña es: " + mensaje);
        }

        private void btnLoginForm_Click(object sender, EventArgs e)
        {
            login login = new login(server);
            login.Show();
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            if (server != null && server.Connected)
            {
                string mensaje = "0/";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                this.BackColor = Color.Gray;
                server.Shutdown(SocketShutdown.Both);
                server.Close();
                MessageBox.Show("Desconectado del servidor.");
            }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("172.20.10.4"); // Ajusta la IP del servidor
            IPEndPoint ipep = new IPEndPoint(IPAddress.Parse("172.20.10.4"), 9050);


            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado al servidor.");
            }
            catch (SocketException)
            {
                MessageBox.Show("No se pudo conectar con el servidor.");
                return;
            }
        }
    }
}