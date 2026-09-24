<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmAPIC
#Region "Vom Windows Form-Designer generierter Code "
	<System.Diagnostics.DebuggerNonUserCode()> Public Sub New()
		MyBase.New()
		'Dieser Aufruf ist für den Windows Form-Designer erforderlich.
		InitializeComponent()
	End Sub
	'Das Formular überschreibt den Löschvorgang, um die Komponentenliste zu bereinigen.
	<System.Diagnostics.DebuggerNonUserCode()> Protected Overloads Overrides Sub Dispose(ByVal Disposing As Boolean)
		If Disposing Then
			If Not components Is Nothing Then
				components.Dispose()
			End If
		End If
		MyBase.Dispose(Disposing)
	End Sub
	'Wird vom Windows Form-Designer benötigt.
	Private components As System.ComponentModel.IContainer
	Public ToolTip1 As System.Windows.Forms.ToolTip
	Public WithEvents picMime As System.Windows.Forms.TextBox
	Public WithEvents linkCheck As System.Windows.Forms.CheckBox
	Public WithEvents picTypeCombo As System.Windows.Forms.ComboBox
	Public WithEvents picDescription As System.Windows.Forms.TextBox
	Public WithEvents cmdCancel As System.Windows.Forms.Button
	Public WithEvents cmdOk As System.Windows.Forms.Button
	Public WithEvents Label3 As System.Windows.Forms.Label
	Public WithEvents dateiInfo As System.Windows.Forms.Label
	Public WithEvents idInfo As System.Windows.Forms.Label
	Public WithEvents img As System.Windows.Forms.PictureBox
	Public WithEvents Label2 As System.Windows.Forms.Label
	Public WithEvents Label1 As System.Windows.Forms.Label
	'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
	'Das Verändern mit dem Windows Form-Designer ist nicht möglich.
	'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmAPIC))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.picMime = New System.Windows.Forms.TextBox
		Me.linkCheck = New System.Windows.Forms.CheckBox
		Me.picTypeCombo = New System.Windows.Forms.ComboBox
		Me.picDescription = New System.Windows.Forms.TextBox
		Me.cmdCancel = New System.Windows.Forms.Button
		Me.cmdOk = New System.Windows.Forms.Button
		Me.Label3 = New System.Windows.Forms.Label
		Me.dateiInfo = New System.Windows.Forms.Label
		Me.idInfo = New System.Windows.Forms.Label
		Me.img = New System.Windows.Forms.PictureBox
		Me.Label2 = New System.Windows.Forms.Label
		Me.Label1 = New System.Windows.Forms.Label
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
		Me.Text = "Informationen zu dem Bild"
		Me.ClientSize = New System.Drawing.Size(450, 228)
		Me.Location = New System.Drawing.Point(437, 213)
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.ShowInTaskbar = False
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "frmAPIC"
		Me.picMime.AutoSize = False
		Me.picMime.BackColor = System.Drawing.SystemColors.Control
		Me.picMime.Size = New System.Drawing.Size(209, 19)
		Me.picMime.Location = New System.Drawing.Point(224, 124)
		Me.picMime.ReadOnly = True
		Me.picMime.TabIndex = 10
		Me.picMime.AcceptsReturn = True
		Me.picMime.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.picMime.CausesValidation = True
		Me.picMime.Enabled = True
		Me.picMime.ForeColor = System.Drawing.SystemColors.WindowText
		Me.picMime.HideSelection = True
		Me.picMime.Maxlength = 0
		Me.picMime.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.picMime.MultiLine = False
		Me.picMime.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.picMime.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.picMime.TabStop = True
		Me.picMime.Visible = True
		Me.picMime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.picMime.Name = "picMime"
		Me.linkCheck.Text = "Nur Dateiverweis statt Bilddaten"
		Me.linkCheck.Size = New System.Drawing.Size(209, 17)
		Me.linkCheck.Location = New System.Drawing.Point(224, 152)
		Me.linkCheck.TabIndex = 6
		Me.linkCheck.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me.linkCheck.FlatStyle = System.Windows.Forms.FlatStyle.Standard
		Me.linkCheck.BackColor = System.Drawing.SystemColors.Control
		Me.linkCheck.CausesValidation = True
		Me.linkCheck.Enabled = True
		Me.linkCheck.ForeColor = System.Drawing.SystemColors.ControlText
		Me.linkCheck.Cursor = System.Windows.Forms.Cursors.Default
		Me.linkCheck.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.linkCheck.Appearance = System.Windows.Forms.Appearance.Normal
		Me.linkCheck.TabStop = True
		Me.linkCheck.CheckState = System.Windows.Forms.CheckState.Unchecked
		Me.linkCheck.Visible = True
		Me.linkCheck.Name = "linkCheck"
		Me.picTypeCombo.Size = New System.Drawing.Size(209, 21)
		Me.picTypeCombo.Location = New System.Drawing.Point(224, 76)
		Me.picTypeCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
		Me.picTypeCombo.TabIndex = 5
		Me.picTypeCombo.BackColor = System.Drawing.SystemColors.Window
		Me.picTypeCombo.CausesValidation = True
		Me.picTypeCombo.Enabled = True
		Me.picTypeCombo.ForeColor = System.Drawing.SystemColors.WindowText
		Me.picTypeCombo.IntegralHeight = True
		Me.picTypeCombo.Cursor = System.Windows.Forms.Cursors.Default
		Me.picTypeCombo.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.picTypeCombo.Sorted = False
		Me.picTypeCombo.TabStop = True
		Me.picTypeCombo.Visible = True
		Me.picTypeCombo.Name = "picTypeCombo"
		Me.picDescription.AutoSize = False
		Me.picDescription.Size = New System.Drawing.Size(209, 19)
		Me.picDescription.Location = New System.Drawing.Point(224, 32)
		Me.picDescription.TabIndex = 3
		Me.picDescription.AcceptsReturn = True
		Me.picDescription.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.picDescription.BackColor = System.Drawing.SystemColors.Window
		Me.picDescription.CausesValidation = True
		Me.picDescription.Enabled = True
		Me.picDescription.ForeColor = System.Drawing.SystemColors.WindowText
		Me.picDescription.HideSelection = True
		Me.picDescription.ReadOnly = False
		Me.picDescription.Maxlength = 0
		Me.picDescription.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.picDescription.MultiLine = False
		Me.picDescription.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.picDescription.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.picDescription.TabStop = True
		Me.picDescription.Visible = True
		Me.picDescription.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.picDescription.Name = "picDescription"
		Me.cmdCancel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.CancelButton = Me.cmdCancel
		Me.cmdCancel.Text = "Abbrechen"
		Me.cmdCancel.Size = New System.Drawing.Size(89, 29)
		Me.cmdCancel.Location = New System.Drawing.Point(232, 184)
		Me.cmdCancel.TabIndex = 1
		Me.cmdCancel.BackColor = System.Drawing.SystemColors.Control
		Me.cmdCancel.CausesValidation = True
		Me.cmdCancel.Enabled = True
		Me.cmdCancel.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdCancel.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdCancel.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdCancel.TabStop = True
		Me.cmdCancel.Name = "cmdCancel"
		Me.cmdOk.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdOk.Text = "Übernehmen"
		Me.AcceptButton = Me.cmdOk
		Me.cmdOk.Size = New System.Drawing.Size(89, 29)
		Me.cmdOk.Location = New System.Drawing.Point(340, 184)
		Me.cmdOk.TabIndex = 0
		Me.cmdOk.BackColor = System.Drawing.SystemColors.Control
		Me.cmdOk.CausesValidation = True
		Me.cmdOk.Enabled = True
		Me.cmdOk.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdOk.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdOk.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdOk.TabStop = True
		Me.cmdOk.Name = "cmdOk"
		Me.Label3.Text = "Mime-Typ des Bildes:"
		Me.Label3.Size = New System.Drawing.Size(121, 13)
		Me.Label3.Location = New System.Drawing.Point(224, 108)
		Me.Label3.TabIndex = 9
		Me.Label3.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label3.BackColor = System.Drawing.SystemColors.Control
		Me.Label3.Enabled = True
		Me.Label3.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label3.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label3.UseMnemonic = True
		Me.Label3.Visible = True
		Me.Label3.AutoSize = False
		Me.Label3.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label3.Name = "Label3"
		Me.dateiInfo.Size = New System.Drawing.Size(37, 13)
		Me.dateiInfo.Location = New System.Drawing.Point(396, 108)
		Me.dateiInfo.TabIndex = 8
		Me.dateiInfo.Visible = False
		Me.dateiInfo.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.dateiInfo.BackColor = System.Drawing.SystemColors.Control
		Me.dateiInfo.Enabled = True
		Me.dateiInfo.ForeColor = System.Drawing.SystemColors.ControlText
		Me.dateiInfo.Cursor = System.Windows.Forms.Cursors.Default
		Me.dateiInfo.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.dateiInfo.UseMnemonic = True
		Me.dateiInfo.AutoSize = False
		Me.dateiInfo.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.dateiInfo.Name = "dateiInfo"
		Me.idInfo.Size = New System.Drawing.Size(41, 13)
		Me.idInfo.Location = New System.Drawing.Point(388, 16)
		Me.idInfo.TabIndex = 7
		Me.idInfo.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.idInfo.BackColor = System.Drawing.SystemColors.Control
		Me.idInfo.Enabled = True
		Me.idInfo.ForeColor = System.Drawing.SystemColors.ControlText
		Me.idInfo.Cursor = System.Windows.Forms.Cursors.Default
		Me.idInfo.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.idInfo.UseMnemonic = True
		Me.idInfo.Visible = True
		Me.idInfo.AutoSize = False
		Me.idInfo.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.idInfo.Name = "idInfo"
		Me.img.Size = New System.Drawing.Size(197, 197)
		Me.img.Location = New System.Drawing.Point(8, 16)
		Me.img.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
		Me.img.Enabled = True
		Me.img.Cursor = System.Windows.Forms.Cursors.Default
		Me.img.Visible = True
		Me.img.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.img.Name = "img"
		Me.Label2.Text = "Bildtyp:"
		Me.Label2.Size = New System.Drawing.Size(121, 13)
		Me.Label2.Location = New System.Drawing.Point(224, 60)
		Me.Label2.TabIndex = 4
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label2.BackColor = System.Drawing.SystemColors.Control
		Me.Label2.Enabled = True
		Me.Label2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label2.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label2.UseMnemonic = True
		Me.Label2.Visible = True
		Me.Label2.AutoSize = False
		Me.Label2.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label2.Name = "Label2"
		Me.Label1.Text = "Beschreibung des Bildes Nummer:"
		Me.Label1.Size = New System.Drawing.Size(161, 13)
		Me.Label1.Location = New System.Drawing.Point(224, 16)
		Me.Label1.TabIndex = 2
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.Label1.BackColor = System.Drawing.SystemColors.Control
		Me.Label1.Enabled = True
		Me.Label1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label1.UseMnemonic = True
		Me.Label1.Visible = True
		Me.Label1.AutoSize = False
		Me.Label1.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label1.Name = "Label1"
		Me.Controls.Add(picMime)
		Me.Controls.Add(linkCheck)
		Me.Controls.Add(picTypeCombo)
		Me.Controls.Add(picDescription)
		Me.Controls.Add(cmdCancel)
		Me.Controls.Add(cmdOk)
		Me.Controls.Add(Label3)
		Me.Controls.Add(dateiInfo)
		Me.Controls.Add(idInfo)
		Me.Controls.Add(img)
		Me.Controls.Add(Label2)
		Me.Controls.Add(Label1)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class