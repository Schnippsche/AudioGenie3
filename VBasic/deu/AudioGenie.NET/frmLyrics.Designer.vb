<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmLyrics
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
	Public WithEvents cmdUpdate As System.Windows.Forms.Button
	Public WithEvents cmdCancel As System.Windows.Forms.Button
	Public WithEvents cmdDelete As System.Windows.Forms.Button
	Public WithEvents txtLyrics As System.Windows.Forms.TextBox
	Public WithEvents txtGenre As System.Windows.Forms.TextBox
	Public WithEvents txtImageLink As System.Windows.Forms.TextBox
	Public WithEvents txtInformation As System.Windows.Forms.TextBox
	Public WithEvents txtAuthor As System.Windows.Forms.TextBox
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
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmLyrics))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.cmdUpdate = New System.Windows.Forms.Button
		Me.cmdCancel = New System.Windows.Forms.Button
		Me.cmdDelete = New System.Windows.Forms.Button
		Me.txtLyrics = New System.Windows.Forms.TextBox
		Me.txtGenre = New System.Windows.Forms.TextBox
		Me.txtImageLink = New System.Windows.Forms.TextBox
		Me.txtInformation = New System.Windows.Forms.TextBox
		Me.txtAuthor = New System.Windows.Forms.TextBox
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
		Me.Text = "Lyrics-Tag ändern"
		Me.ClientSize = New System.Drawing.Size(415, 426)
		Me.Location = New System.Drawing.Point(316, 379)
		Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
		Me.BackColor = System.Drawing.SystemColors.Control
		Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable
		Me.ControlBox = True
		Me.Enabled = True
		Me.KeyPreview = False
		Me.MaximizeBox = True
		Me.MinimizeBox = True
		Me.Cursor = System.Windows.Forms.Cursors.Default
		Me.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.ShowInTaskbar = True
		Me.HelpButton = False
		Me.WindowState = System.Windows.Forms.FormWindowState.Normal
		Me.Name = "frmLyrics"
		Me.cmdUpdate.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdUpdate.Text = "Tag speichern"
		Me.cmdUpdate.Size = New System.Drawing.Size(94, 29)
		Me.cmdUpdate.Location = New System.Drawing.Point(161, 384)
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
		Me.cmdCancel.Location = New System.Drawing.Point(265, 384)
		Me.cmdCancel.TabIndex = 18
		Me.cmdCancel.BackColor = System.Drawing.SystemColors.Control
		Me.cmdCancel.CausesValidation = True
		Me.cmdCancel.Enabled = True
		Me.cmdCancel.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdCancel.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdCancel.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdCancel.TabStop = True
		Me.cmdCancel.Name = "cmdCancel"
		Me.cmdDelete.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
		Me.cmdDelete.Text = "Tag löschen"
		Me.cmdDelete.Size = New System.Drawing.Size(94, 29)
		Me.cmdDelete.Location = New System.Drawing.Point(57, 384)
		Me.cmdDelete.TabIndex = 16
		Me.cmdDelete.BackColor = System.Drawing.SystemColors.Control
		Me.cmdDelete.CausesValidation = True
		Me.cmdDelete.Enabled = True
		Me.cmdDelete.ForeColor = System.Drawing.SystemColors.ControlText
		Me.cmdDelete.Cursor = System.Windows.Forms.Cursors.Default
		Me.cmdDelete.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.cmdDelete.TabStop = True
		Me.cmdDelete.Name = "cmdDelete"
		Me.txtLyrics.AutoSize = False
		Me.txtLyrics.Size = New System.Drawing.Size(320, 99)
		Me.txtLyrics.Location = New System.Drawing.Point(68, 268)
		Me.txtLyrics.MultiLine = True
		Me.txtLyrics.ScrollBars = System.Windows.Forms.ScrollBars.Both
		Me.txtLyrics.WordWrap = False
		Me.txtLyrics.TabIndex = 14
		Me.txtLyrics.AcceptsReturn = True
		Me.txtLyrics.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtLyrics.BackColor = System.Drawing.SystemColors.Window
		Me.txtLyrics.CausesValidation = True
		Me.txtLyrics.Enabled = True
		Me.txtLyrics.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtLyrics.HideSelection = True
		Me.txtLyrics.ReadOnly = False
		Me.txtLyrics.Maxlength = 0
		Me.txtLyrics.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtLyrics.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtLyrics.TabStop = True
		Me.txtLyrics.Visible = True
		Me.txtLyrics.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtLyrics.Name = "txtLyrics"
		Me.txtGenre.AutoSize = False
		Me.txtGenre.Size = New System.Drawing.Size(320, 23)
		Me.txtGenre.Location = New System.Drawing.Point(68, 236)
		Me.txtGenre.TabIndex = 12
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
		Me.txtImageLink.AutoSize = False
		Me.txtImageLink.Size = New System.Drawing.Size(320, 23)
		Me.txtImageLink.Location = New System.Drawing.Point(68, 204)
		Me.txtImageLink.MultiLine = True
		Me.txtImageLink.TabIndex = 10
		Me.txtImageLink.AcceptsReturn = True
		Me.txtImageLink.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtImageLink.BackColor = System.Drawing.SystemColors.Window
		Me.txtImageLink.CausesValidation = True
		Me.txtImageLink.Enabled = True
		Me.txtImageLink.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtImageLink.HideSelection = True
		Me.txtImageLink.ReadOnly = False
		Me.txtImageLink.Maxlength = 0
		Me.txtImageLink.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtImageLink.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtImageLink.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtImageLink.TabStop = True
		Me.txtImageLink.Visible = True
		Me.txtImageLink.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtImageLink.Name = "txtImageLink"
		Me.txtInformation.AutoSize = False
		Me.txtInformation.Size = New System.Drawing.Size(320, 55)
		Me.txtInformation.Location = New System.Drawing.Point(68, 140)
		Me.txtInformation.MultiLine = True
		Me.txtInformation.ScrollBars = System.Windows.Forms.ScrollBars.Both
		Me.txtInformation.WordWrap = False
		Me.txtInformation.TabIndex = 8
		Me.txtInformation.AcceptsReturn = True
		Me.txtInformation.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtInformation.BackColor = System.Drawing.SystemColors.Window
		Me.txtInformation.CausesValidation = True
		Me.txtInformation.Enabled = True
		Me.txtInformation.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtInformation.HideSelection = True
		Me.txtInformation.ReadOnly = False
		Me.txtInformation.Maxlength = 0
		Me.txtInformation.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtInformation.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtInformation.TabStop = True
		Me.txtInformation.Visible = True
		Me.txtInformation.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtInformation.Name = "txtInformation"
		Me.txtAuthor.AutoSize = False
		Me.txtAuthor.Size = New System.Drawing.Size(320, 23)
		Me.txtAuthor.Location = New System.Drawing.Point(68, 108)
		Me.txtAuthor.TabIndex = 6
		Me.txtAuthor.AcceptsReturn = True
		Me.txtAuthor.TextAlign = System.Windows.Forms.HorizontalAlignment.Left
		Me.txtAuthor.BackColor = System.Drawing.SystemColors.Window
		Me.txtAuthor.CausesValidation = True
		Me.txtAuthor.Enabled = True
		Me.txtAuthor.ForeColor = System.Drawing.SystemColors.WindowText
		Me.txtAuthor.HideSelection = True
		Me.txtAuthor.ReadOnly = False
		Me.txtAuthor.Maxlength = 0
		Me.txtAuthor.Cursor = System.Windows.Forms.Cursors.IBeam
		Me.txtAuthor.MultiLine = False
		Me.txtAuthor.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.txtAuthor.ScrollBars = System.Windows.Forms.ScrollBars.None
		Me.txtAuthor.TabStop = True
		Me.txtAuthor.Visible = True
		Me.txtAuthor.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.txtAuthor.Name = "txtAuthor"
		Me.txtAlbum.AutoSize = False
		Me.txtAlbum.Size = New System.Drawing.Size(320, 23)
		Me.txtAlbum.Location = New System.Drawing.Point(68, 76)
		Me.txtAlbum.TabIndex = 4
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
		Me.txtArtist.Size = New System.Drawing.Size(320, 23)
		Me.txtArtist.Location = New System.Drawing.Point(68, 44)
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
		Me.txtTitle.Size = New System.Drawing.Size(320, 23)
		Me.txtTitle.Location = New System.Drawing.Point(68, 12)
		Me.txtTitle.TabIndex = 0
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
		Me.Label8.Text = "Lyrics"
		Me.Label8.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label8.Size = New System.Drawing.Size(61, 17)
		Me.Label8.Location = New System.Drawing.Point(0, 272)
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
		Me.Label7.Text = "Genre"
		Me.Label7.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label7.Size = New System.Drawing.Size(61, 17)
		Me.Label7.Location = New System.Drawing.Point(0, 240)
		Me.Label7.TabIndex = 13
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
		Me.Label6.Text = "Bilderlink"
		Me.Label6.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label6.Size = New System.Drawing.Size(61, 17)
		Me.Label6.Location = New System.Drawing.Point(0, 208)
		Me.Label6.TabIndex = 11
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
		Me.Label5.Text = "Information"
		Me.Label5.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label5.Size = New System.Drawing.Size(61, 17)
		Me.Label5.Location = New System.Drawing.Point(0, 144)
		Me.Label5.TabIndex = 9
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
		Me.Label4.Text = "Autor"
		Me.Label4.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label4.Size = New System.Drawing.Size(61, 17)
		Me.Label4.Location = New System.Drawing.Point(0, 112)
		Me.Label4.TabIndex = 7
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
		Me.Label3.Location = New System.Drawing.Point(0, 80)
		Me.Label3.TabIndex = 5
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
		Me.Label2.Text = "Artist"
		Me.Label2.ForeColor = System.Drawing.SystemColors.WindowText
		Me.Label2.Size = New System.Drawing.Size(61, 17)
		Me.Label2.Location = New System.Drawing.Point(0, 48)
		Me.Label2.TabIndex = 3
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
		Me.Label1.Location = New System.Drawing.Point(0, 16)
		Me.Label1.TabIndex = 1
		Me.Label1.BackColor = System.Drawing.SystemColors.Control
		Me.Label1.Enabled = True
		Me.Label1.Cursor = System.Windows.Forms.Cursors.Default
		Me.Label1.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.Label1.UseMnemonic = True
		Me.Label1.Visible = True
		Me.Label1.AutoSize = False
		Me.Label1.BorderStyle = System.Windows.Forms.BorderStyle.None
		Me.Label1.Name = "Label1"
		Me.Controls.Add(cmdUpdate)
		Me.Controls.Add(cmdCancel)
		Me.Controls.Add(cmdDelete)
		Me.Controls.Add(txtLyrics)
		Me.Controls.Add(txtGenre)
		Me.Controls.Add(txtImageLink)
		Me.Controls.Add(txtInformation)
		Me.Controls.Add(txtAuthor)
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