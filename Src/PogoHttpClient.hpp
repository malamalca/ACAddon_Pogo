// *****************************************************************************
// SimportHttpRequest.h
//                             Executes Http request
//
// *****************************************************************************

#include <string>

#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"File.hpp"

#ifndef POGO_HTTPCLIENT_HPP
#define POGO_HTTPCLIENT_HPP

bool HttpRequest(const HTTP::MessageHeader::Method::Id& method, 
	const GS::UniString& url, const GS::UniString& data, GS::UniString& result);
bool HttpRequest(const HTTP::MessageHeader::Method::Id& method,
	const GS::UniString& url, const GS::UniString& data);
/*bool HttpRequest(const HTTP::MessageHeader::Method::Id& method,
	const GS::UniString& host, const GS::UniString& url, 
	const GS::UniString& username, const GS::UniString& password, 
	const GS::UniString& data,
	GS::UniString& result
)*/

#endif // POGO_HTTPCLIENT_HPP
