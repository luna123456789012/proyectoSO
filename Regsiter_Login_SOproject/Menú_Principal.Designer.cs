namespace Regsiter_Login_SOproject
{
    partial class Menú_Principal
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
            this.button1 = new System.Windows.Forms.Button();
            this.btnCerrasSesion = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.opcionesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.volumenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.JugadoresAnteriores = new System.Windows.Forms.ListBox();
            this.PartidasAnteriores = new System.Windows.Forms.ListBox();
            this.PartidasRecientes = new System.Windows.Forms.ListBox();
            this.usuarioBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.diaBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.mesBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.añoBox = new System.Windows.Forms.TextBox();
            this.button5 = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(42, 477);
            this.button1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(290, 129);
            this.button1.TabIndex = 0;
            this.button1.Text = "JUGAR";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btnCerrasSesion
            // 
            this.btnCerrasSesion.Location = new System.Drawing.Point(993, 585);
            this.btnCerrasSesion.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnCerrasSesion.Name = "btnCerrasSesion";
            this.btnCerrasSesion.Size = new System.Drawing.Size(246, 71);
            this.btnCerrasSesion.TabIndex = 1;
            this.btnCerrasSesion.Text = "Salir";
            this.btnCerrasSesion.UseVisualStyleBackColor = true;
            this.btnCerrasSesion.Click += new System.EventHandler(this.btnCerrasSesion_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.GripMargin = new System.Windows.Forms.Padding(2, 2, 0, 2);
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.opcionesToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(6, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(1263, 33);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "Options";
            // 
            // opcionesToolStripMenuItem
            // 
            this.opcionesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.volumenToolStripMenuItem});
            this.opcionesToolStripMenuItem.Name = "opcionesToolStripMenuItem";
            this.opcionesToolStripMenuItem.Size = new System.Drawing.Size(103, 29);
            this.opcionesToolStripMenuItem.Text = "Opciones";
            // 
            // volumenToolStripMenuItem
            // 
            this.volumenToolStripMenuItem.Name = "volumenToolStripMenuItem";
            this.volumenToolStripMenuItem.Size = new System.Drawing.Size(184, 34);
            this.volumenToolStripMenuItem.Text = "Volumen";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(64, 55);
            this.button2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(176, 92);
            this.button2.TabIndex = 3;
            this.button2.Text = "Ver jugadores con quien he jugado partidas";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(333, 55);
            this.button3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(178, 92);
            this.button3.TabIndex = 4;
            this.button3.Text = "Ver resultados que jugué con:";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(776, 55);
            this.button4.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(184, 92);
            this.button4.TabIndex = 5;
            this.button4.Text = "Partidas jugadas después de:";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // JugadoresAnteriores
            // 
            this.JugadoresAnteriores.FormattingEnabled = true;
            this.JugadoresAnteriores.ItemHeight = 20;
            this.JugadoresAnteriores.Location = new System.Drawing.Point(64, 174);
            this.JugadoresAnteriores.Name = "JugadoresAnteriores";
            this.JugadoresAnteriores.Size = new System.Drawing.Size(176, 184);
            this.JugadoresAnteriores.TabIndex = 6;
            // 
            // PartidasAnteriores
            // 
            this.PartidasAnteriores.FormattingEnabled = true;
            this.PartidasAnteriores.ItemHeight = 20;
            this.PartidasAnteriores.Location = new System.Drawing.Point(333, 174);
            this.PartidasAnteriores.Name = "PartidasAnteriores";
            this.PartidasAnteriores.Size = new System.Drawing.Size(391, 184);
            this.PartidasAnteriores.TabIndex = 7;
            // 
            // PartidasRecientes
            // 
            this.PartidasRecientes.FormattingEnabled = true;
            this.PartidasRecientes.ItemHeight = 20;
            this.PartidasRecientes.Location = new System.Drawing.Point(776, 174);
            this.PartidasRecientes.Name = "PartidasRecientes";
            this.PartidasRecientes.Size = new System.Drawing.Size(184, 184);
            this.PartidasRecientes.TabIndex = 8;
            // 
            // usuarioBox
            // 
            this.usuarioBox.Location = new System.Drawing.Point(544, 123);
            this.usuarioBox.Name = "usuarioBox";
            this.usuarioBox.Size = new System.Drawing.Size(180, 26);
            this.usuarioBox.TabIndex = 9;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(542, 92);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(68, 20);
            this.label1.TabIndex = 10;
            this.label1.Text = "Usuario:";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(988, 92);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 20);
            this.label2.TabIndex = 12;
            this.label2.Text = "Día:";
            // 
            // diaBox
            // 
            this.diaBox.Location = new System.Drawing.Point(993, 123);
            this.diaBox.Name = "diaBox";
            this.diaBox.Size = new System.Drawing.Size(180, 26);
            this.diaBox.TabIndex = 11;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(988, 189);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(111, 20);
            this.label3.TabIndex = 14;
            this.label3.Text = "Mes (número):";
            // 
            // mesBox
            // 
            this.mesBox.Location = new System.Drawing.Point(993, 222);
            this.mesBox.Name = "mesBox";
            this.mesBox.Size = new System.Drawing.Size(180, 26);
            this.mesBox.TabIndex = 13;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(988, 302);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 20);
            this.label4.TabIndex = 16;
            this.label4.Text = "Año:";
            // 
            // añoBox
            // 
            this.añoBox.Location = new System.Drawing.Point(993, 332);
            this.añoBox.Name = "añoBox";
            this.añoBox.Size = new System.Drawing.Size(180, 26);
            this.añoBox.TabIndex = 15;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(614, 562);
            this.button5.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(218, 92);
            this.button5.TabIndex = 17;
            this.button5.Text = "ELIMINAR CUENTA";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // Menú_Principal
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1263, 692);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.añoBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.mesBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.diaBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.usuarioBox);
            this.Controls.Add(this.PartidasRecientes);
            this.Controls.Add(this.PartidasAnteriores);
            this.Controls.Add(this.JugadoresAnteriores);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.btnCerrasSesion);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.menuStrip1);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Menú_Principal";
            this.Text = "Menú_Principal";
            this.Load += new System.EventHandler(this.Menú_Principal_Load_1);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btnCerrasSesion;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem opcionesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem volumenToolStripMenuItem;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.ListBox JugadoresAnteriores;
        private System.Windows.Forms.ListBox PartidasAnteriores;
        private System.Windows.Forms.ListBox PartidasRecientes;
        private System.Windows.Forms.TextBox usuarioBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox diaBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox mesBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox añoBox;
        private System.Windows.Forms.Button button5;
    }
}