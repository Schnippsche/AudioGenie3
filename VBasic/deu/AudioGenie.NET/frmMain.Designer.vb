<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> Partial Class frmMain
#Region "Vom Windows Form-Designer generierter Code "
	<System.Diagnostics.DebuggerNonUserCode()> Public Sub New()
		MyBase.New()
		'Dieser Aufruf ist für den Windows Form-Designer erforderlich.
		InitializeComponent()
		Form_Initialize_renamed()
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
	Public WithEvents mnuPlayFile As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuDummy0 As System.Windows.Forms.ToolStripSeparator
	Public WithEvents mnuTag_Edit As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuID3v1_Edit As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuID3v2_Edit As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuAPE_Edit As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuLyrics_Edit As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuDummy1 As System.Windows.Forms.ToolStripSeparator
	Public WithEvents mnuID3v1_Delete As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuID3v2_Delete As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuAPE_Delete As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuLyrics_Delete As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuDummy2 As System.Windows.Forms.ToolStripSeparator
	Public WithEvents mnuAudioProperties As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents mnuPopup As System.Windows.Forms.ToolStripMenuItem
	Public WithEvents MainMenu1 As System.Windows.Forms.MenuStrip
	Public WithEvents tBar As AxComctlLib.AxToolbar
	Public WithEvents prgBar As AxComctlLib.AxProgressBar
	Public WithEvents detailView As AxComctlLib.AxListView
	Public WithEvents folderView As AxComctlLib.AxTreeView
	Public WithEvents stBar As AxComctlLib.AxStatusBar
	Public WithEvents lblSplitter As System.Windows.Forms.Label
	Public WithEvents audioImages As AxComctlLib.AxImageList
	Public WithEvents folderImages As AxComctlLib.AxImageList
	Public WithEvents driveImages As AxComctlLib.AxImageList
	'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
	'Das Verändern mit dem Windows Form-Designer ist nicht möglich.
	'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
	<System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
		Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmMain))
		Me.components = New System.ComponentModel.Container()
		Me.ToolTip1 = New System.Windows.Forms.ToolTip(components)
		Me.MainMenu1 = New System.Windows.Forms.MenuStrip
		Me.mnuPopup = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuPlayFile = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuDummy0 = New System.Windows.Forms.ToolStripSeparator
		Me.mnuTag_Edit = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuID3v1_Edit = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuID3v2_Edit = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuAPE_Edit = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuLyrics_Edit = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuDummy1 = New System.Windows.Forms.ToolStripSeparator
		Me.mnuID3v1_Delete = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuID3v2_Delete = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuAPE_Delete = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuLyrics_Delete = New System.Windows.Forms.ToolStripMenuItem
		Me.mnuDummy2 = New System.Windows.Forms.ToolStripSeparator
		Me.mnuAudioProperties = New System.Windows.Forms.ToolStripMenuItem
		Me.tBar = New AxComctlLib.AxToolbar
		Me.prgBar = New AxComctlLib.AxProgressBar
		Me.detailView = New AxComctlLib.AxListView
		Me.folderView = New AxComctlLib.AxTreeView
		Me.stBar = New AxComctlLib.AxStatusBar
		Me.lblSplitter = New System.Windows.Forms.Label
		Me.audioImages = New AxComctlLib.AxImageList
		Me.folderImages = New AxComctlLib.AxImageList
		Me.driveImages = New AxComctlLib.AxImageList
		Me.MainMenu1.SuspendLayout()
		Me.SuspendLayout()
		Me.ToolTip1.Active = True
		CType(Me.tBar, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.prgBar, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.detailView, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.folderView, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.stBar, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.audioImages, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.folderImages, System.ComponentModel.ISupportInitialize).BeginInit()
		CType(Me.driveImages, System.ComponentModel.ISupportInitialize).BeginInit()
		Me.Text = "AudioGenie"
		Me.ClientSize = New System.Drawing.Size(681, 483)
		Me.Location = New System.Drawing.Point(4, 30)
		Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
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
		Me.Name = "frmMain"
		Me.mnuPopup.Name = "mnuPopup"
		Me.mnuPopup.Text = "popupmenue"
		Me.mnuPopup.Visible = False
		Me.mnuPopup.Checked = False
		Me.mnuPopup.Enabled = True
		Me.mnuPlayFile.Name = "mnuPlayFile"
		Me.mnuPlayFile.Text = "Mit Standard-Player wiedergeben"
		Me.mnuPlayFile.Checked = False
		Me.mnuPlayFile.Enabled = True
		Me.mnuPlayFile.Visible = True
		Me.mnuDummy0.Enabled = True
		Me.mnuDummy0.Visible = True
		Me.mnuDummy0.Name = "mnuDummy0"
		Me.mnuTag_Edit.Name = "mnuTag_Edit"
		Me.mnuTag_Edit.Text = "Tag bearbeiten"
		Me.mnuTag_Edit.Checked = False
		Me.mnuTag_Edit.Enabled = True
		Me.mnuTag_Edit.Visible = True
		Me.mnuID3v1_Edit.Name = "mnuID3v1_Edit"
		Me.mnuID3v1_Edit.Text = "ID3v1-Tag bearbeiten"
		Me.mnuID3v1_Edit.Checked = False
		Me.mnuID3v1_Edit.Enabled = True
		Me.mnuID3v1_Edit.Visible = True
		Me.mnuID3v2_Edit.Name = "mnuID3v2_Edit"
		Me.mnuID3v2_Edit.Text = "ID3v2-Tag bearbeiten"
		Me.mnuID3v2_Edit.Checked = False
		Me.mnuID3v2_Edit.Enabled = True
		Me.mnuID3v2_Edit.Visible = True
		Me.mnuAPE_Edit.Name = "mnuAPE_Edit"
		Me.mnuAPE_Edit.Text = "APE-Tag bearbeiten"
		Me.mnuAPE_Edit.Checked = False
		Me.mnuAPE_Edit.Enabled = True
		Me.mnuAPE_Edit.Visible = True
		Me.mnuLyrics_Edit.Name = "mnuLyrics_Edit"
		Me.mnuLyrics_Edit.Text = "Lyrics-Tag bearbeiten"
		Me.mnuLyrics_Edit.Checked = False
		Me.mnuLyrics_Edit.Enabled = True
		Me.mnuLyrics_Edit.Visible = True
		Me.mnuDummy1.Enabled = True
		Me.mnuDummy1.Visible = True
		Me.mnuDummy1.Name = "mnuDummy1"
		Me.mnuID3v1_Delete.Name = "mnuID3v1_Delete"
		Me.mnuID3v1_Delete.Text = "ID3v1-Tag löschen"
		Me.mnuID3v1_Delete.Checked = False
		Me.mnuID3v1_Delete.Enabled = True
		Me.mnuID3v1_Delete.Visible = True
		Me.mnuID3v2_Delete.Name = "mnuID3v2_Delete"
		Me.mnuID3v2_Delete.Text = "ID3v2-Tag löschen"
		Me.mnuID3v2_Delete.Checked = False
		Me.mnuID3v2_Delete.Enabled = True
		Me.mnuID3v2_Delete.Visible = True
		Me.mnuAPE_Delete.Name = "mnuAPE_Delete"
		Me.mnuAPE_Delete.Text = "APE-Tag löschen"
		Me.mnuAPE_Delete.Checked = False
		Me.mnuAPE_Delete.Enabled = True
		Me.mnuAPE_Delete.Visible = True
		Me.mnuLyrics_Delete.Name = "mnuLyrics_Delete"
		Me.mnuLyrics_Delete.Text = "Lyrics-Tag löschen"
		Me.mnuLyrics_Delete.Checked = False
		Me.mnuLyrics_Delete.Enabled = True
		Me.mnuLyrics_Delete.Visible = True
		Me.mnuDummy2.Enabled = True
		Me.mnuDummy2.Visible = True
		Me.mnuDummy2.Name = "mnuDummy2"
		Me.mnuAudioProperties.Name = "mnuAudioProperties"
		Me.mnuAudioProperties.Text = "Eigenschaften"
		Me.mnuAudioProperties.Checked = False
		Me.mnuAudioProperties.Enabled = True
		Me.mnuAudioProperties.Visible = True
		tBar.OcxState = CType(resources.GetObject("tBar.OcxState"), System.Windows.Forms.AxHost.State)
		Me.tBar.Dock = System.Windows.Forms.DockStyle.Top
		Me.tBar.Size = New System.Drawing.Size(681, 28)
		Me.tBar.Location = New System.Drawing.Point(0, 24)
		Me.tBar.TabIndex = 0
		Me.tBar.Name = "tBar"
		prgBar.OcxState = CType(resources.GetObject("prgBar.OcxState"), System.Windows.Forms.AxHost.State)
		Me.prgBar.Size = New System.Drawing.Size(72, 21)
		Me.prgBar.Location = New System.Drawing.Point(152, 316)
		Me.prgBar.TabIndex = 4
		Me.prgBar.Visible = False
		Me.prgBar.Name = "prgBar"
		detailView.OcxState = CType(resources.GetObject("detailView.OcxState"), System.Windows.Forms.AxHost.State)
		Me.detailView.Size = New System.Drawing.Size(361, 293)
		Me.detailView.Location = New System.Drawing.Point(244, 52)
		Me.detailView.TabIndex = 3
		Me.detailView.Name = "detailView"
		folderView.OcxState = CType(resources.GetObject("folderView.OcxState"), System.Windows.Forms.AxHost.State)
		Me.folderView.Size = New System.Drawing.Size(229, 245)
		Me.folderView.Location = New System.Drawing.Point(0, 60)
		Me.folderView.TabIndex = 2
		Me.folderView.Name = "folderView"
		stBar.OcxState = CType(resources.GetObject("stBar.OcxState"), System.Windows.Forms.AxHost.State)
		Me.stBar.Dock = System.Windows.Forms.DockStyle.Bottom
		Me.stBar.Size = New System.Drawing.Size(681, 25)
		Me.stBar.Location = New System.Drawing.Point(0, 458)
		Me.stBar.TabIndex = 1
		Me.stBar.Name = "stBar"
		Me.lblSplitter.BackColor = System.Drawing.SystemColors.ScrollBar
		Me.lblSplitter.Size = New System.Drawing.Size(6, 293)
		Me.lblSplitter.Location = New System.Drawing.Point(232, 52)
		Me.lblSplitter.Cursor = System.Windows.Forms.Cursors.SizeWE
		Me.lblSplitter.TabIndex = 5
		Me.lblSplitter.TextAlign = System.Drawing.ContentAlignment.TopLeft
		Me.lblSplitter.Enabled = True
		Me.lblSplitter.ForeColor = System.Drawing.SystemColors.ControlText
		Me.lblSplitter.RightToLeft = System.Windows.Forms.RightToLeft.No
		Me.lblSplitter.UseMnemonic = True
		Me.lblSplitter.Visible = True
		Me.lblSplitter.AutoSize = False
		Me.lblSplitter.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
		Me.lblSplitter.Name = "lblSplitter"
		audioImages.OcxState = CType(resources.GetObject("audioImages.OcxState"), System.Windows.Forms.AxHost.State)
		Me.audioImages.Location = New System.Drawing.Point(84, 304)
		Me.audioImages.Name = "audioImages"
		folderImages.OcxState = CType(resources.GetObject("folderImages.OcxState"), System.Windows.Forms.AxHost.State)
		Me.folderImages.Location = New System.Drawing.Point(4, 304)
		Me.folderImages.Name = "folderImages"
		driveImages.OcxState = CType(resources.GetObject("driveImages.OcxState"), System.Windows.Forms.AxHost.State)
		Me.driveImages.Location = New System.Drawing.Point(44, 304)
		Me.driveImages.Name = "driveImages"
		Me.Controls.Add(tBar)
		Me.Controls.Add(prgBar)
		Me.Controls.Add(detailView)
		Me.Controls.Add(folderView)
		Me.Controls.Add(stBar)
		Me.Controls.Add(lblSplitter)
		Me.Controls.Add(audioImages)
		Me.Controls.Add(folderImages)
		Me.Controls.Add(driveImages)
		CType(Me.driveImages, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.folderImages, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.audioImages, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.stBar, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.folderView, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.detailView, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.prgBar, System.ComponentModel.ISupportInitialize).EndInit()
		CType(Me.tBar, System.ComponentModel.ISupportInitialize).EndInit()
		MainMenu1.Items.AddRange(New System.Windows.Forms.ToolStripItem(){Me.mnuPopup})
		mnuPopup.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem(){Me.mnuPlayFile, Me.mnuDummy0, Me.mnuTag_Edit, Me.mnuID3v1_Edit, Me.mnuID3v2_Edit, Me.mnuAPE_Edit, Me.mnuLyrics_Edit, Me.mnuDummy1, Me.mnuID3v1_Delete, Me.mnuID3v2_Delete, Me.mnuAPE_Delete, Me.mnuLyrics_Delete, Me.mnuDummy2, Me.mnuAudioProperties})
		Me.Controls.Add(MainMenu1)
		Me.MainMenu1.ResumeLayout(False)
		Me.ResumeLayout(False)
		Me.PerformLayout()
	End Sub
#End Region 
End Class