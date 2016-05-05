namespace KSPDisplayComLibTest
{
    partial class Form1
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
            this.connectBtn = new System.Windows.Forms.Button();
            this.connectLbl = new System.Windows.Forms.Label();
            this.integerBtn = new System.Windows.Forms.Button();
            this.integerTxt = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // connectBtn
            // 
            this.connectBtn.Location = new System.Drawing.Point(24, 43);
            this.connectBtn.Name = "connectBtn";
            this.connectBtn.Size = new System.Drawing.Size(119, 23);
            this.connectBtn.TabIndex = 0;
            this.connectBtn.Text = "Connect To Device";
            this.connectBtn.UseVisualStyleBackColor = true;
            this.connectBtn.Click += new System.EventHandler(this.connectBtn_Click);
            // 
            // connectLbl
            // 
            this.connectLbl.AutoSize = true;
            this.connectLbl.Enabled = false;
            this.connectLbl.Location = new System.Drawing.Point(149, 48);
            this.connectLbl.Name = "connectLbl";
            this.connectLbl.Size = new System.Drawing.Size(126, 13);
            this.connectLbl.TabIndex = 1;
            this.connectLbl.Text = "Device is Not Connected";
            // 
            // integerBtn
            // 
            this.integerBtn.Location = new System.Drawing.Point(24, 73);
            this.integerBtn.Name = "integerBtn";
            this.integerBtn.Size = new System.Drawing.Size(119, 23);
            this.integerBtn.TabIndex = 2;
            this.integerBtn.Text = "Send Integer";
            this.integerBtn.UseVisualStyleBackColor = true;
            this.integerBtn.Click += new System.EventHandler(this.integerBtn_Click);
            // 
            // integerTxt
            // 
            this.integerTxt.Location = new System.Drawing.Point(152, 75);
            this.integerTxt.Name = "integerTxt";
            this.integerTxt.Size = new System.Drawing.Size(123, 20);
            this.integerTxt.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(530, 229);
            this.Controls.Add(this.integerTxt);
            this.Controls.Add(this.integerBtn);
            this.Controls.Add(this.connectLbl);
            this.Controls.Add(this.connectBtn);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "Form1";
            this.Text = "KSPDisplayComLibTest";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button connectBtn;
        private System.Windows.Forms.Label connectLbl;
        private System.Windows.Forms.Button integerBtn;
        private System.Windows.Forms.TextBox integerTxt;
    }
}

