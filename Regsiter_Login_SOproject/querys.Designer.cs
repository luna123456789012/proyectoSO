namespace Regsiter_Login_SOproject
{
    partial class Querys
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnVerUsuarios = new System.Windows.Forms.Button();
            this.listBoxUsuarios = new System.Windows.Forms.ListBox();
            this.btnCerrarSesion = new System.Windows.Forms.Button();
            this.panel = new System.Windows.Forms.Panel();
            this.Dame = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.Notificaciones = new System.Windows.Forms.ListBox();
            this.listBoxChat = new System.Windows.Forms.ListBox();
            this.btnEnviarChat = new System.Windows.Forms.Button();
            this.txtMensajeChat = new System.Windows.Forms.TextBox();
            this.btnInvitar = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnVerUsuarios
            // 
            this.btnVerUsuarios.Location = new System.Drawing.Point(12, 183);
            this.btnVerUsuarios.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnVerUsuarios.Name = "btnVerUsuarios";
            this.btnVerUsuarios.Size = new System.Drawing.Size(163, 43);
            this.btnVerUsuarios.TabIndex = 0;
            this.btnVerUsuarios.Text = "Ver usuarios conectados";
            this.btnVerUsuarios.UseVisualStyleBackColor = true;
            this.btnVerUsuarios.Click += new System.EventHandler(this.btnVerUsuarios_Click);
            // 
            // listBoxUsuarios
            // 
            this.listBoxUsuarios.FormattingEnabled = true;
            this.listBoxUsuarios.ItemHeight = 16;
            this.listBoxUsuarios.Location = new System.Drawing.Point(102, 80);
            this.listBoxUsuarios.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.listBoxUsuarios.Name = "listBoxUsuarios";
            this.listBoxUsuarios.Size = new System.Drawing.Size(162, 100);
            this.listBoxUsuarios.TabIndex = 1;
            // 
            // btnCerrarSesion
            // 
            this.btnCerrarSesion.Location = new System.Drawing.Point(112, 511);
            this.btnCerrarSesion.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.btnCerrarSesion.Name = "btnCerrarSesion";
            this.btnCerrarSesion.Size = new System.Drawing.Size(116, 30);
            this.btnCerrarSesion.TabIndex = 2;
            this.btnCerrarSesion.Text = "Cerrar sesión";
            this.btnCerrarSesion.UseVisualStyleBackColor = true;
            this.btnCerrarSesion.Click += new System.EventHandler(this.btnCerrarSesion_Click);
            // 
            // panel
            // 
            this.panel.BackColor = System.Drawing.Color.Lavender;
            this.panel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel.Location = new System.Drawing.Point(397, 80);
            this.panel.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel.Name = "panel";
            this.panel.Size = new System.Drawing.Size(836, 500);
            this.panel.TabIndex = 3;
            this.panel.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_Paint);
            // 
            // Dame
            // 
            this.Dame.Location = new System.Drawing.Point(89, 356);
            this.Dame.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Dame.Name = "Dame";
            this.Dame.Size = new System.Drawing.Size(163, 43);
            this.Dame.TabIndex = 4;
            this.Dame.Text = "Dame carta";
            this.Dame.UseVisualStyleBackColor = true;
            this.Dame.Click += new System.EventHandler(this.Dame_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(89, 426);
            this.button2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(163, 43);
            this.button2.TabIndex = 5;
            this.button2.Text = "Abandonar partida";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Location = new System.Drawing.Point(1284, 183);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(208, 230);
            this.panel1.TabIndex = 6;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(1327, 148);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(106, 16);
            this.label4.TabIndex = 11;
            this.label4.Text = "Panel de partida";
            // 
            // Notificaciones
            // 
            this.Notificaciones.BackColor = System.Drawing.SystemColors.Info;
            this.Notificaciones.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Notificaciones.FormattingEnabled = true;
            this.Notificaciones.ItemHeight = 16;
            this.Notificaciones.Location = new System.Drawing.Point(397, 599);
            this.Notificaciones.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Notificaciones.Name = "Notificaciones";
            this.Notificaciones.Size = new System.Drawing.Size(836, 130);
            this.Notificaciones.TabIndex = 12;
            // 
            // listBoxChat
            // 
            this.listBoxChat.FormattingEnabled = true;
            this.listBoxChat.ItemHeight = 16;
            this.listBoxChat.Location = new System.Drawing.Point(1284, 448);
            this.listBoxChat.Name = "listBoxChat";
            this.listBoxChat.Size = new System.Drawing.Size(208, 228);
            this.listBoxChat.TabIndex = 13;
            // 
            // btnEnviarChat
            // 
            this.btnEnviarChat.Location = new System.Drawing.Point(1311, 723);
            this.btnEnviarChat.Name = "btnEnviarChat";
            this.btnEnviarChat.Size = new System.Drawing.Size(157, 38);
            this.btnEnviarChat.TabIndex = 14;
            this.btnEnviarChat.Text = "Enviar";
            this.btnEnviarChat.UseVisualStyleBackColor = true;
            this.btnEnviarChat.Click += new System.EventHandler(this.btnEnviarChat_Click);
            // 
            // txtMensajeChat
            // 
            this.txtMensajeChat.Location = new System.Drawing.Point(1286, 684);
            this.txtMensajeChat.Name = "txtMensajeChat";
            this.txtMensajeChat.Size = new System.Drawing.Size(206, 22);
            this.txtMensajeChat.TabIndex = 15;
            // 
            // btnInvitar
            // 
            this.btnInvitar.Location = new System.Drawing.Point(181, 183);
            this.btnInvitar.Name = "btnInvitar";
            this.btnInvitar.Size = new System.Drawing.Size(144, 43);
            this.btnInvitar.TabIndex = 16;
            this.btnInvitar.Text = "Invitar a jugar";
            this.btnInvitar.UseVisualStyleBackColor = true;
            this.btnInvitar.Click += new System.EventHandler(this.btnInvitar_Click);
            // 
            // Querys
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Lavender;
            this.ClientSize = new System.Drawing.Size(1626, 870);
            this.Controls.Add(this.btnInvitar);
            this.Controls.Add(this.txtMensajeChat);
            this.Controls.Add(this.btnEnviarChat);
            this.Controls.Add(this.listBoxChat);
            this.Controls.Add(this.Notificaciones);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.Dame);
            this.Controls.Add(this.panel);
            this.Controls.Add(this.btnCerrarSesion);
            this.Controls.Add(this.listBoxUsuarios);
            this.Controls.Add(this.btnVerUsuarios);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Querys";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Querys_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnVerUsuarios;
        private System.Windows.Forms.ListBox listBoxUsuarios;
        private System.Windows.Forms.Button btnCerrarSesion;
        private System.Windows.Forms.Panel panel;
        private System.Windows.Forms.Button Dame;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ListBox Notificaciones;
        private System.Windows.Forms.ListBox listBoxChat;
        private System.Windows.Forms.Button btnEnviarChat;
        private System.Windows.Forms.TextBox txtMensajeChat;
        private System.Windows.Forms.Button btnInvitar;
    }
}