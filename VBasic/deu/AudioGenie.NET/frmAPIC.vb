Option Strict Off
Option Explicit On
Friend Class frmAPIC
	Inherits System.Windows.Forms.Form
	
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		Hide()
	End Sub
	
	Private Sub cmdOk_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOk.Click
		Dim picType As Short
		picType = picTypeCombo.SelectedIndex
		If picType = -1 Then picType = 0
		Call genie.ID3V2AddPictureFile(dateiInfo.Text, picDescription.Text, picType, linkCheck.CheckState)
		frmID3V2.updatePictures()
		Hide()
	End Sub
	
	Private Sub frmAPIC_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load
		picTypeCombo.Items.Add("Other")
		picTypeCombo.Items.Add("32*32 pixelsx file icon")
		picTypeCombo.Items.Add("other file icon")
		picTypeCombo.Items.Add("Front Cover")
		picTypeCombo.Items.Add("Back Cover")
		picTypeCombo.Items.Add("Leaflat page")
		picTypeCombo.Items.Add("Media CD Label")
		picTypeCombo.Items.Add("Lead artist/soloist")
		picTypeCombo.Items.Add("Artist/performer")
		picTypeCombo.Items.Add("Conductor")
		picTypeCombo.Items.Add("Band/Orchestra")
		picTypeCombo.Items.Add("Composer")
		picTypeCombo.Items.Add("Lyricist/text writer")
		picTypeCombo.Items.Add("Recording Location")
		picTypeCombo.Items.Add("During recording")
		picTypeCombo.Items.Add("During performance")
		picTypeCombo.Items.Add("Movie/video screen")
		picTypeCombo.Items.Add("a bright coloured fish")
		picTypeCombo.Items.Add("Illustration")
		picTypeCombo.Items.Add("Band/artist logotype")
		picTypeCombo.Items.Add("Publisher/Studio Logo")
		picTypeCombo.SelectedIndex = 0
	End Sub
End Class