<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmSYLT
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
	Public WithEvents chk As System.Windows.Forms.CheckBox
	Public WithEvents cmdOk As System.Windows.Forms.Button
	Public WithEvents cmdCancel As System.Windows.Forms.Button
	Public WithEvents txtEntry As System.Windows.Forms.TextBox
	Public WithEvents txtTime As System.Windows.Forms.TextBox
	Public WithEvents Label2 As System.Windows.Forms.Label
	Public WithEvents Label1 As System.Windows.Forms.Label
	'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
	'Das Verändern mit dem Windows Form-Designer ist nicht möglich.
	'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmSYLT))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.chk = New System.Windows.Forms.CheckBox
		Me.cmdOk = New System.Windows.Forms.Button
		Me.cmdCancel = New System.Windows.Forms.Button
		Me.txtEntry = New System.Windows.Forms.TextBox
		Me.txtTime = New System.Windows.Forms.TextBox
		Me.Label2 = New System.Windows.Forms.Label
		Me.Label1 = New System.Windows.Forms.Label
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
		Me.Text = "Eintrag bearbeiten"
		Me.ClientSize = New System.Drawing.Size(273, 119)
		Me.Location = New System.Drawing.Point(3, 21)
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.ShowInTaskbar = False
		Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "frmSYLT"
		Me.chk.Text = "Check1"
		Me.chk.Size = New System.Drawing.Size(13, 17)
		Me.chk.Location = New System.Drawing.Point(8, 84)
		Me.chk.TabIndex = 6
		Me.chk.Visible = False
		Me.chk.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me.chk.FlatStyle = System.Windows.Forms.FlatStyle.Standard
		Me.chk.BackColor = System.Drawing.SystemColors.Control
		Me.chk.CausesValidation = True
		Me.chk.Enabled = True
		Me.chk.ForeColor = System.Drawing.SystemColors.ControlText
		Me.chk.Cursor = System.Windows.Forms.Cursors.Default
		Me.chk.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.chk.Appearance = System.Windows.Forms.Appearance.Normal
		Me.chk.TabStop = True
		Me.chk.CheckState = System.Windows.Forms.CheckState.Unchecked
		Me.chk.Name = "chk"
		Me.cmdOk.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdOk.Text = "Übernehmen"
		Me.AcceptButton = Me.cmdOk
		Me.cmdOk.Size = New System.Drawing.Size(89, 29)
		Me.cmdOk.Location = New System.Drawing.Point(144, 80)
		Me.cmdOk.TabIndex = 5
		Me.cmdOk.BackColor = System.Drawing.SystemColors.Control
		Me.cmdOk.CausesValidation = True
		Me.cmdOk.Enabled = True
		Me.cmdOk.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdOk.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdOk.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdOk.TabStop = True
		Me.cmdOk.Name = "cmdOk"
		Me.cmdCancel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.CancelButton = Me.cmdCancel
		Me.cmdCancel.Text = "Abbrechen"
		Me.cmdCancel.Size = New System.Drawing.Size(89, 29)
		Me.cmdCancel.Location = New System.Drawing.Point(40, 80)
		Me.cmdCancel.TabIndex = 4
		Me.cmdCancel.BackColor = System.Drawing.SystemColors.Control
		Me.cmdCancel.CausesValidation = True
		Me.cmdCancel.Enabled = True
		Me.cmdCancel.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdCancel.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdCancel.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdCancel.TabStop = True
		Me.cmdCancel.Name = "cmdCancel"
		Me.txtEntry.AutoSize = False
		Me.txtEntry.Size = New System.Drawing.Size(129, 19)
		Me.txtEntry.Location = New System.Drawing.Point(124, 44)
		Me.txtEntry.TabIndex = 3
		Me.txtEntry.AcceptsReturn = True
		Me.txtEntry.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtEntry.BackColor = System.Drawing.SystemColors.Window
		Me.txtEntry.CausesValidation = True
		Me.txtEntry.Enabled = True
		Me.txtEntry.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtEntry.HideSelection = True
		Me.txtEntry.ReadOnly = False
		Me.txtEntry.Maxlength = 0
		Me.txtEntry.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtEntry.MultiLine = False
		Me.txtEntry.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtEntry.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtEntry.TabStop = True
		Me.txtEntry.Visible = True
		Me.txtEntry.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtEntry.Name = "txtEntry"
		Me.txtTime.AutoSize = False
		Me.txtTime.Size = New System.Drawing.Size(129, 21)
		Me.txtTime.Location = New System.Drawing.Point(124, 12)
		Me.txtTime.Maxlength = 8
		Me.txtTime.TabIndex = 1
		Me.txtTime.AcceptsReturn = True
		Me.txtTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtTime.BackColor = System.Drawing.SystemColors.Window
		Me.txtTime.CausesValidation = True
		Me.txtTime.Enabled = True
		Me.txtTime.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtTime.HideSelection = True
		Me.txtTime.ReadOnly = False
		Me.txtTime.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtTime.MultiLine = False
		Me.txtTime.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtTime.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtTime.TabStop = True
		Me.txtTime.Visible = True
		Me.txtTime.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtTime.Name = "txtTime"
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label2.Text = "Texteintrag:"
		Me.Label2.Size = New System.Drawing.Size(101, 17)
		Me.Label2.Location = New System.Drawing.Point(16, 48)
		Me.Label2.TabIndex = 2
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
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label1.Text = "absolute Zeitangabe:"
		Me.Label1.Size = New System.Drawing.Size(101, 17)
		Me.Label1.Location = New System.Drawing.Point(16, 16)
		Me.Label1.TabIndex = 0
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
		Me.Controls.Add(chk)
		Me.Controls.Add(cmdOk)
		Me.Controls.Add(cmdCancel)
		Me.Controls.Add(txtEntry)
		Me.Controls.Add(txtTime)
		Me.Controls.Add(Label2)
		Me.Controls.Add(Label1)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class