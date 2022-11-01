// *****************************************************************************
// Source code for SimportHttpClient
// API Development Kit 21; Mac/Win
//
// Namespaces:			Contact person:
//		-None-						LT
//
// [SG compatible] - Yes
// *****************************************************************************

	
	#include	<string>
	#include	"APIEnvir.h"
	#include	"ACAPinc.h"					// also includes APIdefs.h

	#include    "HTTP\Client\ClientConnection.hpp"
	#include	"PogoHttpClient.hpp"
	#include	"PogoSettings.hpp"

#ifdef POGO_USE_CURL
	#include	<curl/curl.h>
#else
	#include    "HTTP\Client\Authentication\BasicAuthentication.hpp"
	#include	"IBinaryChannelUtilities.hpp"
	#include	"IOBinProtocolXs.hpp"
	#include	"IChannelX.hpp"
	#include	"File.hpp"
#endif

#ifdef POGO_USE_CURL
	struct MemoryStruct {
		GS::GSHandle memory;
		size_t size;
	};

	static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct* mem = (struct MemoryStruct*)userp;

		BMReallocHandle((GSHandle)mem->memory, (GS::GSSize)mem->size + (GS::GSSize)realsize + 1, ALLOCATE_CLEAR, 0);
		if (mem->memory == NULL) {
			return 0;
		}

		BNCopyMemory((*mem->memory) + mem->size, contents, (GS::GSSize)realsize);

		mem->size += realsize;
		(*mem->memory)[mem->size] = 0;

		return realsize;
	}

	GS::GSSize HttpRequestToBuffer(const HTTP::MessageHeader::Method::Id& method, const GS::UniString& url, const GS::UniString& data, GS::GSHandle& buffer)
	{
		CURL* curl;
		CURLcode res;
		PogoSettings pogoSettings;
		PogoSettings::LoadPogoSettingsFromPreferences(pogoSettings);

		if (buffer == nullptr) buffer = (char**)BMAllocateHandle(1, ALLOCATE_CLEAR, 0);

		struct MemoryStruct chunk;
		chunk.memory = buffer;
		chunk.size = 0;

		GS::UniString fullUrl = pogoSettings.Host + url;
		GS::UniString authString = pogoSettings.Username + ":" + pogoSettings.Password;

		//curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();
		if (!curl) { return false; }

		curl_easy_setopt(curl, CURLOPT_URL, fullUrl.ToCStr().Get());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.0.3705; .NET CLR 1.1.4322)");

		curl_easy_setopt(curl, CURLOPT_USERPWD, authString.ToCStr().Get());
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		if (method == HTTP::MessageHeader::Method::Id::Post) {
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.ToCStr().Get());
		}

		res = curl_easy_perform(curl);

		char* real_url;
		long response_code;
		double elapsed;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &real_url);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			curl_easy_cleanup(curl);
			return -1;
		}

		if (response_code != 200) {
			curl_easy_cleanup(curl);
			return -1;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);

		return (GS::GSSize)chunk.size;
	}

	// -----------------------------------------------------------------------------
	// Download Public Webpage
	// -----------------------------------------------------------------------------
	bool HttpRequest(const HTTP::MessageHeader::Method::Id& method, const GS::UniString& url, const GS::UniString& data, char* &result)
	{
		char **buffer = nullptr;
		GS::GSSize bufLen = HttpRequestToBuffer(method, url, data, buffer);

		if (bufLen > 0) {
			//result = GS::UniString(*buffer, bufLen);
			result = *buffer;
		}
		//BMKillHandle((GS::GSHandle*)&buffer);
		if (bufLen < 0) {
			return false;
		}
		return true;
	}
	bool HttpRequest(const HTTP::MessageHeader::Method::Id& method, const GS::UniString& url, const GS::UniString& data)
	{
		char** buffer = nullptr;
		GS::GSSize bufLen = HttpRequestToBuffer(method, url, data, buffer);
		BMKillHandle((GS::GSHandle*)&buffer);

		return bufLen >= 0;
	}
#else
	bool HttpRequest(const HTTP::MessageHeader::Method::Id& method, const GS::UniString& url, const GS::UniString& data)
	{
		char* outputData = nullptr;
		return HttpRequest(method, url, data, outputData);
	}

	bool HttpRequest(const HTTP::MessageHeader::Method::Id& method, const GS::UniString& url, const GS::UniString& data, char* &result)
	{
		using namespace HTTP::Client;
		using namespace HTTP::MessageHeader;
		using namespace GS::IBinaryChannelUtilities;

		PogoSettings pogoSettings;
		PogoSettings::LoadPogoSettingsFromPreferences(pogoSettings);


		DBPrintf("HOST %s\n", pogoSettings.Host.ToCStr(0, MaxUSize, GSCharCode::CC_Legacy).Get());
		IO::URI::URI connectionUrl(pogoSettings.Host);

		ClientConnection clientConnection(connectionUrl);

		try {
			clientConnection.Connect();

			DBPrintf("URL %s\n", url.ToCStr(0, MaxUSize, GSCharCode::CC_Legacy).Get());

			Request postRequest(method, url);

			RequestHeaderFieldCollection& headers = postRequest.GetRequestHeaderFieldCollection();
			Authentication::BasicAuthentication auth(pogoSettings.Username, pogoSettings.Password);
			auth.HandleRequestHeaderFieldCollection(headers);
			headers.Add(HeaderFieldName::ContentType, "application/x-www-form-urlencoded");

			clientConnection.Send(postRequest, data.ToCStr(GSCharCode::CC_UTF8).Get(), (GS::USize)strlen(data.ToCStr(GSCharCode::CC_UTF8).Get()));

			Response response;
			GS::IChannelX channel(clientConnection.BeginReceive(response), GS::GetNetworkByteOrderIProtocolX());

			if (response.GetStatusCode() == 200) {
				result = ReadCString(channel, StringSerializationType::NotTerminated);

				clientConnection.FinishReceive();
				clientConnection.Close(false);

				return true;
			}
			else {
				DBPrintf("Invalid status code %u \n", response.GetStatusCode());
			}


			clientConnection.FinishReceive();
			clientConnection.Close(false);
		}
		catch (...) {
			DBPrintf("General Exception \n");
		}

		return false;
	}
#endif