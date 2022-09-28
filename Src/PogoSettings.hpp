#ifndef POGOSETTINGS_HPP
#define POGOSETTINGS_HPP

#include "Object.hpp"
#include "ACAPinc.h"					// also includes APIdefs.h

class PogoSettings : public GS::Object
{
	DECLARE_CLASS_INFO;

public:
	PogoSettings();
	PogoSettings(GS::UniString Host, GS::UniString Username, GS::UniString Password, GS::UniString ProjectId);

	virtual	GSErrCode	Read(GS::IChannel& ic) override;
	virtual	GSErrCode	Write(GS::OChannel& oc) const override;

	GS::UniString	Host;
	GS::UniString	Username;
	GS::UniString	Password;
	GS::UniString	ProjectId;

	static bool LoadPogoSettingsFromPreferences(PogoSettings& pogoSettings);
	static bool WritePogoSettingsToPreferences(const PogoSettings& pogoSettings);
};

#endif