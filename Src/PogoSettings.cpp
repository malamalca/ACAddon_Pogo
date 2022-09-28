#include	"APIEnvir.h"
#include	"ACAPinc.h"					// also includes APIdefs.h
#include	"APICommon.h"
#include    "MemoryIChannel.hpp"
#include    "MemoryOChannel.hpp"
#include "PogoSettings.hpp"


GS::ClassInfo PogoSettings::classInfo("PogoSettings", GS::Guid("B45089A1-B373-460B-B143-80E6EBF107C1"), GS::ClassVersion(1, 0));

PogoSettings::PogoSettings() :
	PogoSettings("https://www.pogo.si", "", "", "")
{

}

PogoSettings::PogoSettings(GS::UniString Host, GS::UniString Username, GS::UniString Password, GS::UniString ProjectId) :
	Host(Host),
	Username(Username),
	Password(Password),
	ProjectId(ProjectId)
{

}

GSErrCode PogoSettings::Read(GS::IChannel& ic)
{
	GS::InputFrame frame(ic, classInfo);
	ic.Read(Host);
	ic.Read(Username);
	ic.Read(Password);
	ic.Read(ProjectId);
	return ic.GetInputStatus();
}

GSErrCode PogoSettings::Write(GS::OChannel& oc) const
{
	GS::OutputFrame frame(oc, classInfo);
	oc.Write(Host);
	oc.Write(Username);
	oc.Write(Password);
	oc.Write(ProjectId);
	return oc.GetOutputStatus();
}

bool PogoSettings::LoadPogoSettingsFromPreferences(PogoSettings& pogoSettings)
{
	GSErrCode err = NoError;

	Int32 version = 0;
	GSSize bytes = 0;
	err = ACAPI_GetPreferences(&version, &bytes, nullptr);
	if (err != NoError || version == 0 || bytes == 0) {
		return false;
	}

	char* data = new char[bytes];
	err = ACAPI_GetPreferences(&version, &bytes, data);
	if (err != NoError) {
		delete[] data;
		return false;
	}

	PogoSettings tempPogoSettings;
	GS::MemoryIChannel inputChannel(data, bytes);
	err = tempPogoSettings.Read(inputChannel);
	if (err != NoError) {
		delete[] data;
		return false;
	}

	pogoSettings = tempPogoSettings;

	delete[] data;
	return true;
}

bool PogoSettings::WritePogoSettingsToPreferences(const PogoSettings& pogoSettings)
{
	GSErrCode err = NoError;

	GS::MemoryOChannel outputChannel;
	err = pogoSettings.Write(outputChannel);
	if (err != NoError) {
		return false;
	}

	UInt64 bytes = outputChannel.GetDataSize();
	const char* data = outputChannel.GetDestination();

	static const Int32 PreferencesVersion = 1;

	err = ACAPI_SetPreferences(PreferencesVersion, (GSSize)bytes, data);
	if (err != NoError) {
		return false;
	}

	return true;
}