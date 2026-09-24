<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmAPE
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
	Public WithEvents cmdDelete As System.Windows.Forms.Button
	Public WithEvents cmdUpdate As System.Windows.Forms.Button
	Public WithEvents cmdCancel As System.Windows.Forms.Button
	Public WithEvents txtCopyright As System.Windows.Forms.TextBox
	Public WithEvents txtTrack As System.Windows.Forms.TextBox
	Public WithEvents txtGenre As System.Windows.Forms.TextBox
	Public WithEvents txtYear As System.Windows.Forms.TextBox
	Public WithEvents txtComment As System.Windows.Forms.TextBox
	Public WithEvents txtAlbum As System.Windows.Forms.TextBox
	Public WithEvents txtArtist As System.Windows.Forms.TextBox
	Public WithEvents txtTitle As System.Windows.Forms.TextBox
	Public WithEvents Label8 As System.Windows.Forms.Label
	Public WithEvents Label7 As System.Windows.Forms.Label
	Public WithEvents Label6 As System.Windows.Forms.Label
	Public WithEvents Label5 As System.Windows.Forms.Label
	Public WithEvents Label4 As System.Windows.Forms.Label
	Public WithEvents Label3 As System.Windows.Forms.Label
	Public WithEvents Label2 As System.Windows.Forms.Label
	Public WithEvents Label1 As System.Windows.Forms.Label
	'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
	'Das Verändern mit dem Windows Form-Designer ist nicht möglich.
	'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmAPE))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.cmdDelete = New System.Windows.Forms.Button
		Me.cmdUpdate = New System.Windows.Forms.Button
		Me.cmdCancel = New System.Windows.Forms.Button
		Me.txtCopyright = New System.Windows.Forms.TextBox
		Me.txtTrack = New System.Windows.Forms.TextBox
		Me.txtGenre = New System.Windows.Forms.TextBox
		Me.txtYear = New System.Windows.Forms.TextBox
		Me.txtComment = New System.Windows.Forms.TextBox
		Me.txtAlbum = New System.Windows.Forms.TextBox
		Me.txtArtist = New System.Windows.Forms.TextBox
		Me.txtTitle = New System.Windows.Forms.TextBox
		Me.Label8 = New System.Windows.Forms.Label
		Me.Label7 = New System.Windows.Forms.Label
		Me.Label6 = New System.Windows.Forms.Label
		Me.Label5 = New System.Windows.Forms.Label
		Me.Label4 = New System.Windows.Forms.Label
		Me.Label3 = New System.Windows.Forms.Label
		Me.Label2 = New System.Windows.Forms.Label
		Me.Label1 = New System.Windows.Forms.Label
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
		Me.Text = "APE-Tag ändern"
		Me.ClientSize = New System.Drawing.Size(328, 283)
		Me.Location = New System.Drawing.Point(72, 326)
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
		Me.Name = "frmAPE"
		Me.cmdDelete.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdDelete.Text = "Tag löschen"
		Me.cmdDelete.Size = New System.Drawing.Size(94, 29)
		Me.cmdDelete.Location = New System.Drawing.Point(12, 240)
		Me.cmdDelete.TabIndex = 16
		Me.cmdDelete.BackColor = System.Drawing.SystemColors.Control
		Me.cmdDelete.CausesValidation = True
		Me.cmdDelete.Enabled = True
		Me.cmdDelete.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdDelete.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdDelete.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdDelete.TabStop = True
		Me.cmdDelete.Name = "cmdDelete"
		Me.cmdUpdate.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdUpdate.Text = "Tag speichern"
		Me.cmdUpdate.Size = New System.Drawing.Size(94, 29)
		Me.cmdUpdate.Location = New System.Drawing.Point(118, 240)
		Me.cmdUpdate.TabIndex = 17
		Me.cmdUpdate.BackColor = System.Drawing.SystemColors.Control
		Me.cmdUpdate.CausesValidation = True
		Me.cmdUpdate.Enabled = True
		Me.cmdUpdate.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdUpdate.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdUpdate.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdUpdate.TabStop = True
		Me.cmdUpdate.Name = "cmdUpdate"
		Me.cmdCancel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.CancelButton = Me.cmdCancel
		Me.cmdCancel.Text = "Fertig"
		Me.cmdCancel.Size = New System.Drawing.Size(94, 29)
		Me.cmdCancel.Location = New System.Drawing.Point(224, 240)
		Me.cmdCancel.TabIndex = 18
		Me.cmdCancel.BackColor = System.Drawing.SystemColors.Control
		Me.cmdCancel.CausesValidation = True
		Me.cmdCancel.Enabled = True
		Me.cmdCancel.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdCancel.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdCancel.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdCancel.TabStop = True
		Me.cmdCancel.Name = "cmdCancel"
		Me.txtCopyright.AutoSize = False
		Me.txtCopyright.Size = New System.Drawing.Size(221, 23)
		Me.txtCopyright.Location = New System.Drawing.Point(80, 140)
		Me.txtCopyright.TabIndex = 5
		Me.txtCopyright.AcceptsReturn = True
		Me.txtCopyright.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtCopyright.BackColor = System.Drawing.SystemColors.Window
		Me.txtCopyright.CausesValidation = True
		Me.txtCopyright.Enabled = True
		Me.txtCopyright.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtCopyright.HideSelection = True
		Me.txtCopyright.ReadOnly = False
		Me.txtCopyright.Maxlength = 0
		Me.txtCopyright.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtCopyright.MultiLine = False
		Me.txtCopyright.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtCopyright.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtCopyright.TabStop = True
		Me.txtCopyright.Visible = True
		Me.txtCopyright.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtCopyright.Name = "txtCopyright"
		Me.txtTrack.AutoSize = False
		Me.txtTrack.Size = New System.Drawing.Size(77, 23)
		Me.txtTrack.Location = New System.Drawing.Point(224, 172)
		Me.txtTrack.TabIndex = 7
		Me.txtTrack.AcceptsReturn = True
		Me.txtTrack.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtTrack.BackColor = System.Drawing.SystemColors.Window
		Me.txtTrack.CausesValidation = True
		Me.txtTrack.Enabled = True
		Me.txtTrack.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtTrack.HideSelection = True
		Me.txtTrack.ReadOnly = False
		Me.txtTrack.Maxlength = 0
		Me.txtTrack.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtTrack.MultiLine = False
		Me.txtTrack.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtTrack.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtTrack.TabStop = True
		Me.txtTrack.Visible = True
		Me.txtTrack.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtTrack.Name = "txtTrack"
		Me.txtGenre.AutoSize = False
		Me.txtGenre.Size = New System.Drawing.Size(153, 23)
		Me.txtGenre.Location = New System.Drawing.Point(80, 204)
		Me.txtGenre.TabIndex = 8
		Me.txtGenre.AcceptsReturn = True
		Me.txtGenre.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtGenre.BackColor = System.Drawing.SystemColors.Window
		Me.txtGenre.CausesValidation = True
		Me.txtGenre.Enabled = True
		Me.txtGenre.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtGenre.HideSelection = True
		Me.txtGenre.ReadOnly = False
		Me.txtGenre.Maxlength = 0
		Me.txtGenre.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtGenre.MultiLine = False
		Me.txtGenre.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtGenre.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtGenre.TabStop = True
		Me.txtGenre.Visible = True
		Me.txtGenre.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtGenre.Name = "txtGenre"
		Me.txtYear.AutoSize = False
		Me.txtYear.Size = New System.Drawing.Size(73, 23)
		Me.txtYear.Location = New System.Drawing.Point(80, 172)
		Me.txtYear.TabIndex = 6
		Me.txtYear.AcceptsReturn = True
		Me.txtYear.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtYear.BackColor = System.Drawing.SystemColors.Window
		Me.txtYear.CausesValidation = True
		Me.txtYear.Enabled = True
		Me.txtYear.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtYear.HideSelection = True
		Me.txtYear.ReadOnly = False
		Me.txtYear.Maxlength = 0
		Me.txtYear.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtYear.MultiLine = False
		Me.txtYear.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtYear.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtYear.TabStop = True
		Me.txtYear.Visible = True
		Me.txtYear.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtYear.Name = "txtYear"
		Me.txtComment.AutoSize = False
		Me.txtComment.Size = New System.Drawing.Size(221, 23)
		Me.txtComment.Location = New System.Drawing.Point(80, 108)
		Me.txtComment.TabIndex = 4
		Me.txtComment.AcceptsReturn = True
		Me.txtComment.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtComment.BackColor = System.Drawing.SystemColors.Window
		Me.txtComment.CausesValidation = True
		Me.txtComment.Enabled = True
		Me.txtComment.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtComment.HideSelection = True
		Me.txtComment.ReadOnly = False
		Me.txtComment.Maxlength = 0
		Me.txtComment.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtComment.MultiLine = False
		Me.txtComment.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtComment.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtComment.TabStop = True
		Me.txtComment.Visible = True
		Me.txtComment.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtComment.Name = "txtComment"
		Me.txtAlbum.AutoSize = False
		Me.txtAlbum.Size = New System.Drawing.Size(221, 23)
		Me.txtAlbum.Location = New System.Drawing.Point(80, 76)
		Me.txtAlbum.TabIndex = 3
		Me.txtAlbum.AcceptsReturn = True
		Me.txtAlbum.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtAlbum.BackColor = System.Drawing.SystemColors.Window
		Me.txtAlbum.CausesValidation = True
		Me.txtAlbum.Enabled = True
		Me.txtAlbum.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtAlbum.HideSelection = True
		Me.txtAlbum.ReadOnly = False
		Me.txtAlbum.Maxlength = 0
		Me.txtAlbum.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtAlbum.MultiLine = False
		Me.txtAlbum.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtAlbum.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtAlbum.TabStop = True
		Me.txtAlbum.Visible = True
		Me.txtAlbum.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtAlbum.Name = "txtAlbum"
		Me.txtArtist.AutoSize = False
		Me.txtArtist.Size = New System.Drawing.Size(221, 23)
		Me.txtArtist.Location = New System.Drawing.Point(80, 44)
		Me.txtArtist.TabIndex = 2
		Me.txtArtist.AcceptsReturn = True
		Me.txtArtist.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtArtist.BackColor = System.Drawing.SystemColors.Window
		Me.txtArtist.CausesValidation = True
		Me.txtArtist.Enabled = True
		Me.txtArtist.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtArtist.HideSelection = True
		Me.txtArtist.ReadOnly = False
		Me.txtArtist.Maxlength = 0
		Me.txtArtist.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtArtist.MultiLine = False
		Me.txtArtist.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtArtist.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtArtist.TabStop = True
		Me.txtArtist.Visible = True
		Me.txtArtist.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtArtist.Name = "txtArtist"
		Me.txtTitle.AutoSize = False
		Me.txtTitle.Size = New System.Drawing.Size(221, 23)
		Me.txtTitle.Location = New System.Drawing.Point(80, 12)
		Me.txtTitle.TabIndex = 1
		Me.txtTitle.AcceptsReturn = True
		Me.txtTitle.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtTitle.BackColor = System.Drawing.SystemColors.Window
		Me.txtTitle.CausesValidation = True
		Me.txtTitle.Enabled = True
		Me.txtTitle.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtTitle.HideSelection = True
		Me.txtTitle.ReadOnly = False
		Me.txtTitle.Maxlength = 0
		Me.txtTitle.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtTitle.MultiLine = False
		Me.txtTitle.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtTitle.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtTitle.TabStop = True
		Me.txtTitle.Visible = True
		Me.txtTitle.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtTitle.Name = "txtTitle"
		Me.Label8.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label8.Text = "Jahr"
		Me.Label8.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label8.Size = New System.Drawing.Size(61, 17)
		Me.Label8.Location = New System.Drawing.Point(12, 176)
		Me.Label8.TabIndex = 15
		Me.Label8.BackColor = System.Drawing.SystemColors.Control
		Me.Label8.Enabled = True
		Me.Label8.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label8.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label8.UseMnemonic = True
		Me.Label8.Visible = True
		Me.Label8.AutoSize = False
		Me.Label8.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label8.Name = "Label8"
		Me.Label7.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label7.Text = "Track #"
		Me.Label7.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label7.Size = New System.Drawing.Size(41, 17)
		Me.Label7.Location = New System.Drawing.Point(172, 176)
		Me.Label7.TabIndex = 14
		Me.Label7.BackColor = System.Drawing.SystemColors.Control
		Me.Label7.Enabled = True
		Me.Label7.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label7.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label7.UseMnemonic = True
		Me.Label7.Visible = True
		Me.Label7.AutoSize = False
		Me.Label7.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label7.Name = "Label7"
		Me.Label6.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label6.Text = "Genre"
		Me.Label6.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label6.Size = New System.Drawing.Size(45, 17)
		Me.Label6.Location = New System.Drawing.Point(28, 208)
		Me.Label6.TabIndex = 13
		Me.Label6.BackColor = System.Drawing.SystemColors.Control
		Me.Label6.Enabled = True
		Me.Label6.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label6.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label6.UseMnemonic = True
		Me.Label6.Visible = True
		Me.Label6.AutoSize = False
		Me.Label6.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label6.Name = "Label6"
		Me.Label5.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label5.Text = "Copyright"
		Me.Label5.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label5.Size = New System.Drawing.Size(61, 17)
		Me.Label5.Location = New System.Drawing.Point(12, 144)
		Me.Label5.TabIndex = 12
		Me.Label5.BackColor = System.Drawing.SystemColors.Control
		Me.Label5.Enabled = True
		Me.Label5.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label5.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label5.UseMnemonic = True
		Me.Label5.Visible = True
		Me.Label5.AutoSize = False
		Me.Label5.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label5.Name = "Label5"
		Me.Label4.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label4.Text = "Kommentar"
		Me.Label4.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label4.Size = New System.Drawing.Size(61, 17)
		Me.Label4.Location = New System.Drawing.Point(12, 112)
		Me.Label4.TabIndex = 11
		Me.Label4.BackColor = System.Drawing.SystemColors.Control
		Me.Label4.Enabled = True
		Me.Label4.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label4.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label4.UseMnemonic = True
		Me.Label4.Visible = True
		Me.Label4.AutoSize = False
		Me.Label4.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label4.Name = "Label4"
		Me.Label3.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label3.Text = "Album"
		Me.Label3.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label3.Size = New System.Drawing.Size(61, 17)
		Me.Label3.Location = New System.Drawing.Point(12, 80)
		Me.Label3.TabIndex = 10
		Me.Label3.BackColor = System.Drawing.SystemColors.Control
		Me.Label3.Enabled = True
		Me.Label3.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label3.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label3.UseMnemonic = True
		Me.Label3.Visible = True
		Me.Label3.AutoSize = False
		Me.Label3.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label3.Name = "Label3"
		Me.Label2.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label2.Text = "Interpret"
		Me.Label2.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label2.Size = New System.Drawing.Size(61, 17)
		Me.Label2.Location = New System.Drawing.Point(12, 48)
		Me.Label2.TabIndex = 9
		Me.Label2.BackColor = System.Drawing.SystemColors.Control
		Me.Label2.Enabled = True
		Me.Label2.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label2.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label2.UseMnemonic = True
		Me.Label2.Visible = True
		Me.Label2.AutoSize = False
		Me.Label2.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label2.Name = "Label2"
		Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopRight
		Me.Label1.Text = "Titel"
		Me.Label1.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label1.Size = New System.Drawing.Size(61, 17)
		Me.Label1.Location = New System.Drawing.Point(12, 16)
		Me.Label1.TabIndex = 0
		Me.Label1.BackColor = System.Drawing.SystemColors.Control
		Me.Label1.Enabled = True
		Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label1.UseMnemonic = True
		Me.Label1.Visible = True
		Me.Label1.AutoSize = False
		Me.Label1.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label1.Name = "Label1"
		Me.Controls.Add(cmdDelete)
		Me.Controls.Add(cmdUpdate)
		Me.Controls.Add(cmdCancel)
		Me.Controls.Add(txtCopyright)
		Me.Controls.Add(txtTrack)
		Me.Controls.Add(txtGenre)
		Me.Controls.Add(txtYear)
		Me.Controls.Add(txtComment)
		Me.Controls.Add(txtAlbum)
		Me.Controls.Add(txtArtist)
		Me.Controls.Add(txtTitle)
		Me.Controls.Add(Label8)
		Me.Controls.Add(Label7)
		Me.Controls.Add(Label6)
		Me.Controls.Add(Label5)
		Me.Controls.Add(Label4)
		Me.Controls.Add(Label3)
		Me.Controls.Add(Label2)
		Me.Controls.Add(Label1)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class