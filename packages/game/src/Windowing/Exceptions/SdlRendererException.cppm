module;

#include <string>

export module Windowing.Exceptions.SdlRendererException;

export import Windowing.Exceptions.SdlException;

export class SdlRendererException final:
	public SdlException
{
	public:
		SdlRendererException():
			SdlException("Renderer error.")
		{
			//
		}

		explicit SdlRendererException(const char* what):
			SdlException(what)
		{
			this->fetchSdlError();
		}

		explicit SdlRendererException(const std::string& what):
			SdlException(what)
		{
			this->fetchSdlError();
		}
};
