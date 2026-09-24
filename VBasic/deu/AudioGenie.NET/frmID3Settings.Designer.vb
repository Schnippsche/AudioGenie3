<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmID3Settings
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
	Public WithEvents cmdOkay As System.Windows.Forms.Button
	Public WithEvents cmdCancel As System.Windows.Forms.Button
	Public WithEvents _optionEncoding_3 As System.Windows.Forms.RadioButton
	Public WithEvents _optionEncoding_2 As System.Windows.Forms.RadioButton
	Public WithEvents _optionEncoding_1 As System.Windows.Forms.RadioButton
	Public WithEvents _optionEncoding_0 As System.Windows.Forms.RadioButton
	Public WithEvents Frame2 As System.Windows.Forms.GroupBox
	Public WithEvents _optionFormat_3 As System.Windows.Forms.RadioButton
	Public WithEvents _optionFormat_2 As System.Windows.Forms.RadioButton
	Public WithEvents _optionFormat_1 As System.Windows.Forms.RadioButton
	Public WithEvents _optionFormat_0 As System.Windows.Forms.RadioButton
	Public WithEvents Frame1 As System.Windows.Forms.GroupBox
	Public WithEvents optionEncoding As Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray
	Public WithEvents optionFormat As Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray
	'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
	'Das Verändern mit dem Windows Form-Designer ist nicht möglich.
	'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmID3Settings))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.cmdOkay = New System.Windows.Forms.Button
		Me.cmdCancel = New System.Windows.Forms.Button
		Me.Frame2 = New System.Windows.Forms.GroupBox
		Me._optionEncoding_3 = New System.Windows.Forms.RadioButton
		Me._optionEncoding_2 = New System.Windows.Forms.RadioButton
		Me._optionEncoding_1 = New System.Windows.Forms.RadioButton
		Me._optionEncoding_0 = New System.Windows.Forms.RadioButton
		Me.Frame1 = New System.Windows.Forms.GroupBox
		Me._optionFormat_3 = New System.Windows.Forms.RadioButton
		Me._optionFormat_2 = New System.Windows.Forms.RadioButton
		Me._optionFormat_1 = New System.Windows.Forms.RadioButton
		Me._optionFormat_0 = New System.Windows.Forms.RadioButton
		Me.optionEncoding = New Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray(components)
		Me.optionFormat = New Microsoft.VisualBasic.Compatibility.VB6.RadioButtonArray(components)
		Me.Frame2.SuspendLayout()
		Me.Frame1.SuspendLayout()
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		CType(Me.optionEncoding, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.optionFormat, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
		Me.Text = "ID3v2 Einstellungen"
		Me.ClientSize = New System.Drawing.Size(297, 182)
		Me.Location = New System.Drawing.Point(499, 139)
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
		Me.Name = "frmID3Settings"
		Me.cmdOkay.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdOkay.Text = "Übernehmen"
		Me.AcceptButton = Me.cmdOkay
		Me.cmdOkay.Size = New System.Drawing.Size(91, 31)
		Me.cmdOkay.Location = New System.Drawing.Point(160, 136)
		Me.cmdOkay.TabIndex = 11
		Me.cmdOkay.BackColor = System.Drawing.SystemColors.Control
		Me.cmdOkay.CausesValidation = True
		Me.cmdOkay.Enabled = True
		Me.cmdOkay.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdOkay.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdOkay.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdOkay.TabStop = True
		Me.cmdOkay.Name = "cmdOkay"
		Me.cmdCancel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.CancelButton = Me.cmdCancel
		Me.cmdCancel.Text = "Abbrechen"
		Me.cmdCancel.Size = New System.Drawing.Size(91, 31)
		Me.cmdCancel.Location = New System.Drawing.Point(52, 136)
		Me.cmdCancel.TabIndex = 10
		Me.cmdCancel.BackColor = System.Drawing.SystemColors.Control
		Me.cmdCancel.CausesValidation = True
		Me.cmdCancel.Enabled = True
		Me.cmdCancel.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdCancel.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdCancel.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdCancel.TabStop = True
		Me.cmdCancel.Name = "cmdCancel"
		Me.Frame2.Text = "Text Encoding"
		Me.Frame2.Size = New System.Drawing.Size(131, 113)
		Me.Frame2.Location = New System.Drawing.Point(160, 8)
		Me.Frame2.TabIndex = 5
		Me.Frame2.BackColor = System.Drawing.SystemColors.Control
		Me.Frame2.Enabled = True
		Me.Frame2.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame2.Visible = True
		Me.Frame2.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame2.Name = "Frame2"
		Me._optionEncoding_3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_3.Text = "UTF-8"
		Me._optionEncoding_3.Size = New System.Drawing.Size(97, 21)
		Me._optionEncoding_3.Location = New System.Drawing.Point(12, 80)
		Me._optionEncoding_3.TabIndex = 9
		Me._optionEncoding_3.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_3.BackColor = System.Drawing.SystemColors.Control
		Me._optionEncoding_3.CausesValidation = True
		Me._optionEncoding_3.Enabled = True
		Me._optionEncoding_3.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionEncoding_3.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionEncoding_3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionEncoding_3.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionEncoding_3.TabStop = True
		Me._optionEncoding_3.Checked = False
		Me._optionEncoding_3.Visible = True
		Me._optionEncoding_3.Name = "_optionEncoding_3"
		Me._optionEncoding_2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_2.Text = "UTF-16"
		Me._optionEncoding_2.Size = New System.Drawing.Size(97, 21)
		Me._optionEncoding_2.Location = New System.Drawing.Point(12, 60)
		Me._optionEncoding_2.TabIndex = 8
		Me._optionEncoding_2.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_2.BackColor = System.Drawing.SystemColors.Control
		Me._optionEncoding_2.CausesValidation = True
		Me._optionEncoding_2.Enabled = True
		Me._optionEncoding_2.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionEncoding_2.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionEncoding_2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionEncoding_2.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionEncoding_2.TabStop = True
		Me._optionEncoding_2.Checked = False
		Me._optionEncoding_2.Visible = True
		Me._optionEncoding_2.Name = "_optionEncoding_2"
		Me._optionEncoding_1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_1.Text = "UTF-16 BOM"
		Me._optionEncoding_1.Size = New System.Drawing.Size(97, 21)
		Me._optionEncoding_1.Location = New System.Drawing.Point(12, 40)
		Me._optionEncoding_1.TabIndex = 7
		Me._optionEncoding_1.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_1.BackColor = System.Drawing.SystemColors.Control
		Me._optionEncoding_1.CausesValidation = True
		Me._optionEncoding_1.Enabled = True
		Me._optionEncoding_1.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionEncoding_1.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionEncoding_1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionEncoding_1.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionEncoding_1.TabStop = True
		Me._optionEncoding_1.Checked = False
		Me._optionEncoding_1.Visible = True
		Me._optionEncoding_1.Name = "_optionEncoding_1"
		Me._optionEncoding_0.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_0.Text = "ISO-8859-1"
		Me._optionEncoding_0.Size = New System.Drawing.Size(97, 21)
		Me._optionEncoding_0.Location = New System.Drawing.Point(12, 20)
		Me._optionEncoding_0.TabIndex = 6
		Me._optionEncoding_0.Checked = True
		Me._optionEncoding_0.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionEncoding_0.BackColor = System.Drawing.SystemColors.Control
		Me._optionEncoding_0.CausesValidation = True
		Me._optionEncoding_0.Enabled = True
		Me._optionEncoding_0.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionEncoding_0.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionEncoding_0.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionEncoding_0.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionEncoding_0.TabStop = True
		Me._optionEncoding_0.Visible = True
		Me._optionEncoding_0.Name = "_optionEncoding_0"
		Me.Frame1.Text = "Speicherformat Frames"
		Me.Frame1.Size = New System.Drawing.Size(133, 113)
		Me.Frame1.Location = New System.Drawing.Point(8, 8)
		Me.Frame1.TabIndex = 0
		Me.Frame1.BackColor = System.Drawing.SystemColors.Control
		Me.Frame1.Enabled = True
		Me.Frame1.ForeColor = System.Drawing.SystemColors.ControlText
		Me.Frame1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Frame1.Visible = True
		Me.Frame1.Padding = New System.Windows.Forms.Padding(0)
		Me.Frame1.Name = "Frame1"
		Me._optionFormat_3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_3.Text = "ID3v2.4"
		Me._optionFormat_3.Size = New System.Drawing.Size(77, 25)
		Me._optionFormat_3.Location = New System.Drawing.Point(16, 80)
		Me._optionFormat_3.TabIndex = 4
		Me._optionFormat_3.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_3.BackColor = System.Drawing.SystemColors.Control
		Me._optionFormat_3.CausesValidation = True
		Me._optionFormat_3.Enabled = True
		Me._optionFormat_3.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionFormat_3.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionFormat_3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionFormat_3.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionFormat_3.TabStop = True
		Me._optionFormat_3.Checked = False
		Me._optionFormat_3.Visible = True
		Me._optionFormat_3.Name = "_optionFormat_3"
		Me._optionFormat_2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_2.Text = "ID3v2.3"
		Me._optionFormat_2.Size = New System.Drawing.Size(77, 25)
		Me._optionFormat_2.Location = New System.Drawing.Point(16, 60)
		Me._optionFormat_2.TabIndex = 3
		Me._optionFormat_2.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_2.BackColor = System.Drawing.SystemColors.Control
		Me._optionFormat_2.CausesValidation = True
		Me._optionFormat_2.Enabled = True
		Me._optionFormat_2.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionFormat_2.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionFormat_2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionFormat_2.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionFormat_2.TabStop = True
		Me._optionFormat_2.Checked = False
		Me._optionFormat_2.Visible = True
		Me._optionFormat_2.Name = "_optionFormat_2"
		Me._optionFormat_1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_1.Text = "ID3v2.2"
		Me._optionFormat_1.Size = New System.Drawing.Size(77, 25)
		Me._optionFormat_1.Location = New System.Drawing.Point(16, 40)
		Me._optionFormat_1.TabIndex = 2
		Me._optionFormat_1.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_1.BackColor = System.Drawing.SystemColors.Control
		Me._optionFormat_1.CausesValidation = True
		Me._optionFormat_1.Enabled = True
		Me._optionFormat_1.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionFormat_1.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionFormat_1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionFormat_1.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionFormat_1.TabStop = True
		Me._optionFormat_1.Checked = False
		Me._optionFormat_1.Visible = True
		Me._optionFormat_1.Name = "_optionFormat_1"
		Me._optionFormat_0.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_0.Text = "Originalformat"
		Me._optionFormat_0.Size = New System.Drawing.Size(89, 25)
		Me._optionFormat_0.Location = New System.Drawing.Point(16, 20)
		Me._optionFormat_0.TabIndex = 1
		Me._optionFormat_0.Checked = True
		Me._optionFormat_0.CheckAlign = System.Drawing.ContentAlignment.MiddleLeft
		Me._optionFormat_0.BackColor = System.Drawing.SystemColors.Control
		Me._optionFormat_0.CausesValidation = True
		Me._optionFormat_0.Enabled = True
		Me._optionFormat_0.ForeColor = System.Drawing.SystemColors.ControlText
		Me._optionFormat_0.Cursor = System.Windows.Forms.Cursors.Default
		Me._optionFormat_0.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me._optionFormat_0.Appearance = System.Windows.Forms.Appearance.Normal
		Me._optionFormat_0.TabStop = True
		Me._optionFormat_0.Visible = True
		Me._optionFormat_0.Name = "_optionFormat_0"
		Me.Controls.Add(cmdOkay)
		Me.Controls.Add(cmdCancel)
		Me.Controls.Add(Frame2)
		Me.Controls.Add(Frame1)
		Me.Frame2.Controls.Add(_optionEncoding_3)
		Me.Frame2.Controls.Add(_optionEncoding_2)
		Me.Frame2.Controls.Add(_optionEncoding_1)
		Me.Frame2.Controls.Add(_optionEncoding_0)
		Me.Frame1.Controls.Add(_optionFormat_3)
		Me.Frame1.Controls.Add(_optionFormat_2)
		Me.Frame1.Controls.Add(_optionFormat_1)
		Me.Frame1.Controls.Add(_optionFormat_0)
		Me.optionEncoding.SetIndex(_optionEncoding_3, CType(3, Short))
		Me.optionEncoding.SetIndex(_optionEncoding_2, CType(2, Short))
		Me.optionEncoding.SetIndex(_optionEncoding_1, CType(1, Short))
		Me.optionEncoding.SetIndex(_optionEncoding_0, CType(0, Short))
		Me.optionFormat.SetIndex(_optionFormat_3, CType(3, Short))
		Me.optionFormat.SetIndex(_optionFormat_2, CType(2, Short))
		Me.optionFormat.SetIndex(_optionFormat_1, CType(1, Short))
		Me.optionFormat.SetIndex(_optionFormat_0, CType(0, Short))
		CType(Me.optionFormat, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.optionEncoding, System.ComponentModel.ISupportInitialize).EndInit()
		Me.Frame2.ResumeLayout(False)
		Me.Frame1.ResumeLayout(False)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class