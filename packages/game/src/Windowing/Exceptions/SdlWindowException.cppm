module;

#include <string>

export module Windowing.Exceptions.SdlWindowException;

export import Windowing.Exceptions.SdlException;

export class SdlWindowException final:
	public SdlException
{
	public:
		SdlWindowException():
			SdlException("Window creation failed.")
		{
			//
		}

		explicit SdlWindowException(const char* what):
			SdlException(what)
		{
			this->fetchSdlError();
		}

		explicit SdlWindowException(const std::string& what):
			SdlException(what)
		{
			this->fetchSdlError();
		}
};
