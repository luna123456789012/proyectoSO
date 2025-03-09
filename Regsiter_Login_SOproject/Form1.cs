﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace Regsiter_Login_SOproject
{
    public partial class RegistroDeUsuarios: Form
    {
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
            // Obtener datos del formulario
            string usuario = txtUsuario.Text.Trim();
            string contrasena = txtContrasena.Text.Trim();

            // Validación básica: verificar que se ingresen datos
            if (string.IsNullOrEmpty(usuario) || string.IsNullOrEmpty(contrasena))
            {
                MessageBox.Show("Por favor, ingrese tanto usuario como contraseña.", "Datos incompletos", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Cadena de conexión (ajustar según tu configuración)
            string connectionString = "server=localhost;user=root;database=juego;password=yourpassword;";

            using (MySqlConnection connection = new MySqlConnection(connectionString))
            {
                try
                {
                    connection.Open();

                    // Verificar si el usuario ya existe
                    string queryVerificar = "SELECT COUNT(*) FROM jugadores WHERE usuario = @usuario";
                    using (MySqlCommand cmdVerificar = new MySqlCommand(queryVerificar, connection))
                    {
                        cmdVerificar.Parameters.AddWithValue("@usuario", usuario);
                        int count = Convert.ToInt32(cmdVerificar.ExecuteScalar());

                        if (count > 0)
                        {
                            MessageBox.Show("El usuario ya existe. Por favor, elija otro nombre de usuario.", "Usuario Existente", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }
                    }

                    // Insertar el nuevo usuario
                    string queryInsertar = "INSERT INTO jugadores (usuario, contraseña) VALUES (@usuario, @contrasena)";
                    using (MySqlCommand cmdInsertar = new MySqlCommand(queryInsertar, connection))
                    {
                        cmdInsertar.Parameters.AddWithValue("@usuario", usuario);
                        cmdInsertar.Parameters.AddWithValue("@contrasena", contrasena);

                        // Ejecutar la inserción
                        cmdInsertar.ExecuteNonQuery();
                    }

                    MessageBox.Show("Registro exitoso.", "Éxito", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (MySqlException ex)
                {
                    // Manejo de errores: muestra un mensaje si ocurre algún problema
                    MessageBox.Show("Error al registrar el usuario: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
    }
}
