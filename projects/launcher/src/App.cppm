module;

#include <wx/wx.h>

export module ExampleGame.Launcher.App;

import ExampleGame.Launcher.UI.LauncherFrame;

export class App:
	public wxApp
{
	public:
		bool OnInit()
		{
			LauncherFrame *frame = new LauncherFrame();

			frame->Show(true);

			return true;
		}

		int OnExit()
		{
			return 0;
		}
};
