Option Strict Off
Option Explicit On
Friend Class frmSYLT
	Inherits System.Windows.Forms.Form
	Private Sub cmdCancel_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdCancel.Click
		chk.CheckState = System.Windows.Forms.CheckState.Unchecked
		Hide()
	End Sub
	
	Private Sub cmdOk_Click(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles cmdOk.Click
		chk.CheckState = System.Windows.Forms.CheckState.Checked
		Hide()
	End Sub
End Class