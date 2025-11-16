module;

#include <cstddef>
#include <wx/wx.h>

export module ExampleGame.Launcher.UI.LauncherFrame;

export class LauncherFrame:
	public wxFrame
{
	public:
		LauncherFrame():
			wxFrame(
				NULL,
				wxID_ANY,
				"Launcher",
				wxDefaultPosition,
				wxSize(720, 360),
				wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxMAXIMIZE_BOX)
			)
		{
			//
		}

		~LauncherFrame() override = default;
};
