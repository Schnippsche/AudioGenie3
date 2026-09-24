<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmDetails
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
	Public WithEvents lblInfo As System.Windows.Forms.TextBox
	'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
	'Das Verändern mit dem Windows Form-Designer ist nicht möglich.
	'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmDetails))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.lblInfo = New System.Windows.Forms.TextBox
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
		Me.Text = "Properties"
		Me.ClientSize = New System.Drawing.Size(254, 322)
		Me.Location = New System.Drawing.Point(157, 151)
		Me.MaximizeBox = False
		Me.MinimizeBox = False
		Me.ShowInTaskbar = False
		Me.HelpButton = True
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "frmDetails"
		Me.lblInfo.AutoSize = False
		Me.lblInfo.BackColor = System.Drawing.SystemColors.Info
		Me.lblInfo.Size = New System.Drawing.Size(253, 321)
		Me.lblInfo.Location = New System.Drawing.Point(0, 0)
		Me.lblInfo.ReadOnly = True
		Me.lblInfo.MultiLine = True
		Me.lblInfo.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
		Me.lblInfo.TabIndex = 0
		Me.lblInfo.AcceptsReturn = True
		Me.lblInfo.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.lblInfo.CausesValidation = True
		Me.lblInfo.Enabled = True
		Me.lblInfo.ForeColor = System.Drawing.SystemColors.WindowText
		Me.lblInfo.HideSelection = True
		Me.lblInfo.Maxlength = 0
		Me.lblInfo.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.lblInfo.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.lblInfo.TabStop = True
		Me.lblInfo.Visible = True
		Me.lblInfo.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.lblInfo.Name = "lblInfo"
		Me.Controls.Add(lblInfo)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class