module;

#include <string>

export module Windowing.Exceptions.SdlInitializationException;

export import Windowing.Exceptions.SdlException;

export class SdlInitializationException final:
	public SdlException
{
	public:
		SdlInitializationException():
			SdlException("SDL initialization failed.")
		{
			//
		}

		explicit SdlInitializationException(const char* what):
			SdlException(what)
		{
			this->fetchSdlError();
		}

		explicit SdlInitializationException(const std::string& what):
			SdlException(what)
		{
			this->fetchSdlError();
		}
};
