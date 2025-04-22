namespace Regsiter_Login_SOproject
{
    partial class login
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
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.loginbutton = new System.Windows.Forms.Button();
            this.txtContrasenaLogin = new System.Windows.Forms.TextBox();
            this.txtUsuarioLogin = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(172, 226);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(92, 20);
            this.label2.TabIndex = 14;
            this.label2.Text = "Contraseña";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(174, 120);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 20);
            this.label1.TabIndex = 13;
            this.label1.Text = "Usuario";
            // 
            // loginbutton
            // 
            this.loginbutton.Location = new System.Drawing.Point(212, 351);
            this.loginbutton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.loginbutton.Name = "loginbutton";
            this.loginbutton.Size = new System.Drawing.Size(165, 55);
            this.loginbutton.TabIndex = 12;
            this.loginbutton.Text = "Acceder";
            this.loginbutton.UseVisualStyleBackColor = true;
            this.loginbutton.Click += new System.EventHandler(this.loginbutton_Click);
            // 
            // txtContrasenaLogin
            // 
            this.txtContrasenaLogin.Location = new System.Drawing.Point(176, 251);
            this.txtContrasenaLogin.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txtContrasenaLogin.Name = "txtContrasenaLogin";
            this.txtContrasenaLogin.Size = new System.Drawing.Size(248, 26);
            this.txtContrasenaLogin.TabIndex = 11;
            // 
            // txtUsuarioLogin
            // 
            this.txtUsuarioLogin.Location = new System.Drawing.Point(176, 146);
            this.txtUsuarioLogin.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.txtUsuarioLogin.Name = "txtUsuarioLogin";
            this.txtUsuarioLogin.Size = new System.Drawing.Size(252, 26);
            this.txtUsuarioLogin.TabIndex = 10;
            // 
            // login
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(606, 501);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.loginbutton);
            this.Controls.Add(this.txtContrasenaLogin);
            this.Controls.Add(this.txtUsuarioLogin);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "login";
            this.Text = "Form2";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button loginbutton;
        private System.Windows.Forms.TextBox txtContrasenaLogin;
        private System.Windows.Forms.TextBox txtUsuarioLogin;
    }
}