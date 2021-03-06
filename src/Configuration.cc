/**
   MiracleGrue - Model Generator for toolpathing. <http://www.grue.makerbot.com>
   Copyright (C) 2011 Far McKon <Far@makerbot.com>, Hugo Boyer (hugo@makerbot.com)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

*/
#include "Configuration.h"

#include <iostream>
#include <sstream>

#include "json-cpp/include/json/reader.h"
#include "json-cpp/include/json/writer.h"

using namespace std;

Configuration::Configuration()
{
	this->root["programName"]  = GRUE_PROGRAM_NAME;
	this->root["versionStr"]  = GRUE_VERSION,
	this->root["machineName"] = "";
	this->root["firmware"] = "";

	Json::Value platform;
	platform["temperature"] = 0;
	platform["automated"] = false;
	platform["waitingPositionX"] = 0;
	platform["waitingPositionY"] = 0,
	platform["waitingPositionZ"] = 0;
	this->root["platform"] = platform;

	Json::Value shell;
	shell["enable"] = false;
	shell["distance"] = 3.0;
	this->root["shell"] = shell;
	cout << "Configuration ctor! " << endl;

	Json::Value gcodeOutput;
	gcodeOutput["filename"] = "";
	this->root["shell"] = shell;
}


Configuration::Configuration(std::string& srcFilename)
{
	this->root["programName"]  = GRUE_PROGRAM_NAME;
	this->root["versionStr"]  = GRUE_VERSION,
	this->root["machineName"] = "";
	this->root["firmware"] = "";

	Json::Reader reader;
	// root built in default constructor
	bool parsedOk = reader.parse(srcFilename, root);
	if(parsedOk){
		cout << "parsed ok";
	}
}
Configuration::~Configuration()
{
	cout << "Configuration dtor! " << endl;

}

std::string Configuration::asJson(Json::StyledWriter writer ) const
{
	return writer.write(root);
}


void Configuration::writeJsonConfig(ostream &ss) const
{
	Json::StyledWriter writer;
	ss << this->asJson();
}

void Configuration::writeGcodeConfig(std::ostream &ss, const std::string indent) const
{

	ss << "(" << indent << "Generated by "<<  this->root["programName"].asString() << " ";
	ss << this->root["versionStr"].asString() << ")"<< endl;
	ss << "(" << indent << "machine name: " << this->root["machineName"].asString() << ")"<< endl;
	ss << "(" << indent << "firmware revision:" << this->root["firmware"].asString() << ")" << endl;
	std::string plurial = this->root["extruders"].size() > 0 ? "":"s";
	ss << "(" << indent << this->root["extruders"].size()  << " extruder" << plurial << ")" << endl;
 	if (this->root["outline"]["enable"].asBool())
 	{
 		ss << "(" << indent << this->root["outline"]["distance"] << "mm outline" << ")" << endl;
 	}
 	else
 	{
 		ss << "(" << indent << "no outline" <<  ")"<< endl;
 	}

 	ss << endl;
}
