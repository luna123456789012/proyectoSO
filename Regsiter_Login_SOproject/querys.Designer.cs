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
            this.panel = new System.Windows.Forms.Panel();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.Notificaciones = new System.Windows.Forms.ListBox();
            this.Invitar = new System.Windows.Forms.Button();
            this.listBoxChat = new System.Windows.Forms.ListBox();
            this.txtMensajeChat = new System.Windows.Forms.MaskedTextBox();
            this.btnEnviarChat = new System.Windows.Forms.Button();
            this.listBoxConectados = new System.Windows.Forms.ListBox();
            this.VerConectados = new System.Windows.Forms.Button();
            this.Listo = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnVerUsuarios
            // 
            this.btnVerUsuarios.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnVerUsuarios.Location = new System.Drawing.Point(34, 841);
            this.btnVerUsuarios.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.btnVerUsuarios.Name = "btnVerUsuarios";
            this.btnVerUsuarios.Size = new System.Drawing.Size(184, 86);
            this.btnVerUsuarios.TabIndex = 0;
            this.btnVerUsuarios.Text = "Ver jugadores de esta partida";
            this.btnVerUsuarios.UseVisualStyleBackColor = true;
            this.btnVerUsuarios.Click += new System.EventHandler(this.btnVerUsuarios_Click);
            // 
            // listBoxUsuarios
            // 
            this.listBoxUsuarios.FormattingEnabled = true;
            this.listBoxUsuarios.ItemHeight = 20;
            this.listBoxUsuarios.Location = new System.Drawing.Point(34, 697);
            this.listBoxUsuarios.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.listBoxUsuarios.Name = "listBoxUsuarios";
            this.listBoxUsuarios.Size = new System.Drawing.Size(184, 124);
            this.listBoxUsuarios.TabIndex = 1;
            // 
            // panel
            // 
            this.panel.BackColor = System.Drawing.Color.Lavender;
            this.panel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel.Location = new System.Drawing.Point(34, 34);
            this.panel.Name = "panel";
            this.panel.Size = new System.Drawing.Size(1175, 639);
            this.panel.TabIndex = 3;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1294, 586);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(183, 54);
            this.button1.TabIndex = 4;
            this.button1.Text = "Robar carta";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(1294, 1044);
            this.button2.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(183, 54);
            this.button2.TabIndex = 5;
            this.button2.Text = "Abandonar partida";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Location = new System.Drawing.Point(1269, 215);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(234, 287);
            this.panel1.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(1321, 173);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(124, 20);
            this.label4.TabIndex = 11;
            this.label4.Text = "Panel de partida";
            // 
            // Notificaciones
            // 
            this.Notificaciones.BackColor = System.Drawing.SystemColors.Info;
            this.Notificaciones.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Notificaciones.FormattingEnabled = true;
            this.Notificaciones.ItemHeight = 20;
            this.Notificaciones.Location = new System.Drawing.Point(270, 697);
            this.Notificaciones.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Notificaciones.Name = "Notificaciones";
            this.Notificaciones.Size = new System.Drawing.Size(940, 162);
            this.Notificaciones.TabIndex = 12;
            // 
            // Invitar
            // 
            this.Invitar.Location = new System.Drawing.Point(543, 1063);
            this.Invitar.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Invitar.Name = "Invitar";
            this.Invitar.Size = new System.Drawing.Size(490, 35);
            this.Invitar.TabIndex = 13;
            this.Invitar.Text = "INVITAR JUGADOR";
            this.Invitar.UseVisualStyleBackColor = true;
            this.Invitar.Click += new System.EventHandler(this.Invitar_Click);
            // 
            // listBoxChat
            // 
            this.listBoxChat.FormattingEnabled = true;
            this.listBoxChat.ItemHeight = 20;
            this.listBoxChat.Location = new System.Drawing.Point(1539, 14);
            this.listBoxChat.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.listBoxChat.Name = "listBoxChat";
            this.listBoxChat.Size = new System.Drawing.Size(372, 884);
            this.listBoxChat.TabIndex = 14;
            // 
            // txtMensajeChat
            // 
            this.txtMensajeChat.Location = new System.Drawing.Point(1539, 942);
            this.txtMensajeChat.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txtMensajeChat.Name = "txtMensajeChat";
            this.txtMensajeChat.Size = new System.Drawing.Size(372, 26);
            this.txtMensajeChat.TabIndex = 15;
            // 
            // btnEnviarChat
            // 
            this.btnEnviarChat.Location = new System.Drawing.Point(1648, 991);
            this.btnEnviarChat.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnEnviarChat.Name = "btnEnviarChat";
            this.btnEnviarChat.Size = new System.Drawing.Size(180, 48);
            this.btnEnviarChat.TabIndex = 16;
            this.btnEnviarChat.Text = "ENVIAR";
            this.btnEnviarChat.UseVisualStyleBackColor = true;
            this.btnEnviarChat.Click += new System.EventHandler(this.btnEnviarChat_Click);
            // 
            // listBoxConectados
            // 
            this.listBoxConectados.FormattingEnabled = true;
            this.listBoxConectados.ItemHeight = 20;
            this.listBoxConectados.Location = new System.Drawing.Point(543, 871);
            this.listBoxConectados.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.listBoxConectados.Name = "listBoxConectados";
            this.listBoxConectados.Size = new System.Drawing.Size(494, 164);
            this.listBoxConectados.TabIndex = 17;
            // 
            // VerConectados
            // 
            this.VerConectados.Location = new System.Drawing.Point(329, 928);
            this.VerConectados.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.VerConectados.Name = "VerConectados";
            this.VerConectados.Size = new System.Drawing.Size(182, 73);
            this.VerConectados.TabIndex = 18;
            this.VerConectados.Text = "Actualizar jugadores conectados";
            this.VerConectados.UseVisualStyleBackColor = true;
            this.VerConectados.Click += new System.EventHandler(this.VerConectados_Click);
            // 
            // Listo
            // 
            this.Listo.Location = new System.Drawing.Point(1294, 947);
            this.Listo.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.Listo.Name = "Listo";
            this.Listo.Size = new System.Drawing.Size(183, 54);
            this.Listo.TabIndex = 19;
            this.Listo.Text = "LISTO";
            this.Listo.UseVisualStyleBackColor = true;
            this.Listo.Click += new System.EventHandler(this.Listo_Click_1);
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button3.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.button3.Location = new System.Drawing.Point(1294, 668);
            this.button3.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(183, 117);
            this.button3.TabIndex = 20;
            this.button3.Text = "UNO";
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // Querys
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Lavender;
            this.ClientSize = new System.Drawing.Size(1924, 1126);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.Listo);
            this.Controls.Add(this.VerConectados);
            this.Controls.Add(this.listBoxConectados);
            this.Controls.Add(this.btnEnviarChat);
            this.Controls.Add(this.txtMensajeChat);
            this.Controls.Add(this.listBoxChat);
            this.Controls.Add(this.Invitar);
            this.Controls.Add(this.Notificaciones);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.panel);
            this.Controls.Add(this.listBoxUsuarios);
            this.Controls.Add(this.btnVerUsuarios);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Querys";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Querys_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnVerUsuarios;
        private System.Windows.Forms.ListBox listBoxUsuarios;
        private System.Windows.Forms.Panel panel;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ListBox Notificaciones;
        private System.Windows.Forms.Button Invitar;
        private System.Windows.Forms.ListBox listBoxChat;
        private System.Windows.Forms.MaskedTextBox txtMensajeChat;
        private System.Windows.Forms.Button btnEnviarChat;
        private System.Windows.Forms.ListBox listBoxConectados;
        private System.Windows.Forms.Button VerConectados;
        private System.Windows.Forms.Button Listo;
        private System.Windows.Forms.Button button3;
    }
}