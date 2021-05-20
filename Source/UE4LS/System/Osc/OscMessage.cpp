// Fill out your copyright notice in the Description page of Project Settings.
// copyright (c) openFrameworks team 2010-2017
// copyright (c) Damian Stewart 2007-2009
#include "OscMessage.h"

//--------------------------------------------------------------
FOscMessage::FOscMessage() : remoteHost(""), remotePort(0) {}

//--------------------------------------------------------------
FOscMessage::~FOscMessage() {
	clear();
}

//--------------------------------------------------------------
FOscMessage::FOscMessage(const FOscMessage& other) {
	copy(other);
}

//--------------------------------------------------------------
FOscMessage& FOscMessage::operator=(const FOscMessage& other) {
	return copy(other);
}

// //--------------------------------------------------------------
FOscMessage& FOscMessage::copy(const FOscMessage& other) {
	if (this == &other) return *this;
	clear();

	// copy address & remote info
	address = other.address;
	remoteHost = other.remoteHost;
	remotePort = other.remotePort;

	// copy arguments
	for (std::size_t i = 0; i < other.args.size(); ++i) {
		switch (other.getArgType(i)) {
		case OSC_TYPE_TRUE: case OSC_TYPE_FALSE:
			args.push_back(new OscArgBool(other.getArgAsBool(i)));
			break;
		case OSC_TYPE_INT32:
			args.push_back(new OscArgInt32(other.getArgAsInt32(i)));
			break;
		case OSC_TYPE_INT64:
			args.push_back(new OscArgInt64(other.getArgAsInt64(i)));
			break;
		case OSC_TYPE_FLOAT:
			args.push_back(new OscArgFloat(other.getArgAsFloat(i)));
			break;
		case OSC_TYPE_DOUBLE:
			args.push_back(new OscArgDouble(other.getArgAsDouble(i)));
			break;
		case OSC_TYPE_STRING:
			args.push_back(new OscArgString(other.getArgAsString(i)));
			break;
		case OSC_TYPE_CHAR:
			args.push_back(new OscArgChar(other.getArgAsChar(i)));
			break;
		case OSC_TYPE_TRIGGER:
			args.push_back(new ofxOscArgTrigger());
			break;
		default:
			// ofLogError("FOscMessage") << "copy(): bad argument type "
			// 	<< other.getArgType(i) << " '" << (char) other.getArgType(i) << "'";	
			break;
		}
	}

	return *this;
}

//--------------------------------------------------------------
void FOscMessage::clear() {
	address.Reset();
	remoteHost.Reset();
	remotePort = 0;
	for (unsigned int i = 0; i < args.size(); ++i) {
		delete args[i];
	}
	args.clear();
}

//--------------------------------------------------------------
void FOscMessage::setAddress(const FString& address_) {
	this->address = address_;
}

//--------------------------------------------------------------
FString FOscMessage::getAddress() const {
	return address;
}

//--------------------------------------------------------------
FString FOscMessage::getRemoteIp() const {
	return remoteHost;
}

//--------------------------------------------------------------
FString FOscMessage::getRemoteHost() const {
	return remoteHost;
}

//--------------------------------------------------------------
int FOscMessage::getRemotePort() const {
	return remotePort;
}

// get methods
//--------------------------------------------------------------
std::size_t FOscMessage::getNumArgs() const {
	return args.size();
}

//--------------------------------------------------------------
EOscArgType FOscMessage::getArgType(std::size_t index) const {
	if (index >= args.size()) {
		// ofLogError("FOscMessage") << "getArgType(): index "
		//                             << index << " out of bounds";
		return OSC_TYPE_INDEXOUTOFBOUNDS;
	}
	else {
		return args[index]->getType();
	}
}

//--------------------------------------------------------------
FString FOscMessage::getArgTypeName(std::size_t index) const {
	if (index >= args.size()) {
		// ofLogError("FOscMessage") << "getArgTypeName(): index "
		//                             << index << " out of bounds";
		return "INDEX OUT OF BOUNDS";
	}
	else {
		return args[index]->getTypeName();
	}
}

//--------------------------------------------------------------
FString FOscMessage::getTypeString() const {
	FString types = FString(TEXT(""));
	for (std::size_t i = 0; i < args.size(); ++i) {
		types += args[i]->getTypeName();
	}
	return types;
}

//--------------------------------------------------------------
int32 FOscMessage::getArgAsInt(std::size_t index) const {
	return getArgAsInt32(index);
}

//--------------------------------------------------------------
int32 FOscMessage::getArgAsInt32(std::size_t index) const {
	if (getArgType(index) != OSC_TYPE_INT32) {
		if (getArgType(index) == OSC_TYPE_INT64) {
			return (std::int32_t)((OscArgInt64*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_FLOAT) {
			return (std::int32_t)((OscArgFloat*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_DOUBLE) {
			return (std::int32_t)((OscArgDouble*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_TRUE ||
			getArgType(index) == OSC_TYPE_FALSE) {
			return (std::int32_t)((OscArgBool*)args[index])->get();
		}
		else {
			// ofLogError("FOscMessageEx") << "getArgAsInt32(): argument "
			//                             << index << " is not a number";
			return 0;
		}
	}
	else {
		return ((OscArgInt32*)args[index])->get();
	}
}

//--------------------------------------------------------------
int64 FOscMessage::getArgAsInt64(std::size_t index) const {
	if (getArgType(index) != OSC_TYPE_INT64) {
		if (getArgType(index) == OSC_TYPE_INT32) {
			return (std::int64_t)((OscArgInt32*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_FLOAT) {
			return (std::int64_t)((OscArgFloat*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_DOUBLE) {
			return (std::int64_t)((OscArgDouble*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_TRUE ||
			getArgType(index) == OSC_TYPE_FALSE) {
			return (std::int64_t)((OscArgBool*)args[index])->get();
		}
		else {
			// ofLogError("FOscMessageEx") << "getArgAsInt64(): argument "
			//                             << index << " is not a number";
			return 0;
		}
	}
	else {
		return ((OscArgInt64*)args[index])->get();
	}
}

//--------------------------------------------------------------
float FOscMessage::getArgAsFloat(std::size_t index) const {
	if (getArgType(index) != OSC_TYPE_FLOAT) {
		if (getArgType(index) == OSC_TYPE_INT32) {
			return (float)((OscArgInt32*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_INT64) {
			return (float)((OscArgInt64*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_DOUBLE) {
			return (float)((OscArgDouble*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_TRUE ||
			getArgType(index) == OSC_TYPE_FALSE) {
			return (float)((OscArgBool*)args[index])->get();
		}
		else {
			// ofLogError("FOscMessageEx") << "getArgAsFloat(): argument "
			//                             << index << " is not a number";
			return 0;
		}
	}
	else {
		return ((OscArgFloat*)args[index])->get();
	}
}

//--------------------------------------------------------------
double FOscMessage::getArgAsDouble(std::size_t index) const {
	if (getArgType(index) != OSC_TYPE_DOUBLE) {
		if (getArgType(index) == OSC_TYPE_INT32) {
			return (double)((OscArgInt32*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_INT64) {
			return (double)((OscArgInt64*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_FLOAT) {
			return (double)((OscArgFloat*)args[index])->get();
		}
		else if (getArgType(index) == OSC_TYPE_TRUE ||
			getArgType(index) == OSC_TYPE_FALSE) {
			return (double)((OscArgBool*)args[index])->get();
		}
		else {
			// ofLogError("FOscMessageEx") << "getArgAsDouble(): argument "
			//                             << index << " is not a number";
			return 0;
		}
	}
	else {
		return ((OscArgDouble*)args[index])->get();
	}
}

// //--------------------------------------------------------------
FString FOscMessage::getArgAsString(std::size_t index) const
{
	//UE_LOG(LogTemp, Warning, TEXT("TYPE %i"), (int)getArgType(index));
	//UE_LOG(LogTemp, Warning, TEXT("TYPE name %s"), *getArgTypeName(index));
	if (getArgType(index) != OSC_TYPE_STRING)
	{
		// warn about string conversions
		if (getArgType(index) == OSC_TYPE_INT32)
		{
			return FString::FromInt(((OscArgInt32*)args[index])->get());
		}
		else if (getArgType(index) == OSC_TYPE_INT64) {
			return FString::FromInt(((OscArgInt64*)args[index])->get());
		}
		else if (getArgType(index) == OSC_TYPE_FLOAT) {
			return FString::SanitizeFloat(((OscArgFloat*)args[index])->get());
		}
		else if (getArgType(index) == OSC_TYPE_DOUBLE) {
			UE_LOG(LogTemp, Warning, TEXT("Hey! UE4 doesnt SanitizeDouble but SanitizeFloat, so take careful"))
				return FString::SanitizeFloat(((OscArgDouble*)args[index])->get());
		}
		else if (getArgType(index) == OSC_TYPE_CHAR) {
			return FString(TCHAR_TO_UTF8(
				((OscArgChar*)args[index])->get()
			)
			);
		}
		else {


			return "";
		}
	}
	else {
		return ((OscArgString*)args[index])->get();
	}
}


//--------------------------------------------------------------
char FOscMessage::getArgAsChar(std::size_t index) const
{
	if (getArgType(index) == OSC_TYPE_CHAR) {
		return ((OscArgChar*)args[index])->get();
	}
	else {
		return 0;
	}
}



//--------------------------------------------------------------
bool FOscMessage::getArgAsBool(std::size_t index) const {
	switch (getArgType(index)) {
	case OSC_TYPE_TRUE: case OSC_TYPE_FALSE:
		return ((OscArgBool*)args[index])->get();
	case OSC_TYPE_INT32:
		return ((OscArgInt32*)args[index])->get() > 0;
	case OSC_TYPE_INT64:
		return ((OscArgInt64*)args[index])->get() > 0;
	case OSC_TYPE_FLOAT:
		return ((OscArgFloat*)args[index])->get() > 0;
	case OSC_TYPE_DOUBLE:
		return ((OscArgDouble*)args[index])->get() > 0;
	default:

		return false;
	}
}




// set methods
//--------------------------------------------------------------
void FOscMessage::addIntArg(std::int32_t argument) {
	args.push_back(new OscArgInt32(argument));
}

//--------------------------------------------------------------
void FOscMessage::addInt32Arg(std::int32_t argument) {
	args.push_back(new OscArgInt32(argument));
}

//--------------------------------------------------------------
void FOscMessage::addInt64Arg(std::int64_t argument) {
	args.push_back(new OscArgInt64(argument));
}

//--------------------------------------------------------------
void FOscMessage::addFloatArg(float argument) {
	args.push_back(new OscArgFloat(argument));
}

//--------------------------------------------------------------
void FOscMessage::addDoubleArg(double argument) {
	args.push_back(new OscArgDouble(argument));
}

//--------------------------------------------------------------
void FOscMessage::addStringArg(const FString& argument) {
	args.push_back(new OscArgString(argument));
}

//--------------------------------------------------------------
void FOscMessage::addCharArg(char argument) {
	args.push_back(new OscArgChar(argument));
}

//--------------------------------------------------------------
void FOscMessage::addBoolArg(bool argument) {
	args.push_back(new OscArgBool(argument));
}

//--------------------------------------------------------------
void FOscMessage::addNoneArg() {
	args.push_back(new ofxOscArgNone());
}



//--------------------------------------------------------------
bool FOscMessage::getArgAsNone(std::size_t index) const
{
	if (getArgType(index) != OSC_TYPE_NONE) {
		// ofLogError("FOscMessage") << "getArgAsNone(): argument "
		// 	<< index << " is not a none/nil";
		return false;
	}
	else {
		return ((ofxOscArgNone*)args[index])->get();
	}
}



//--------------------------------------------------------------
bool FOscMessage::getArgAsTrigger(std::size_t index) const
{
	if (getArgType(index) != OSC_TYPE_TRIGGER) {
		// ofLogError("FOscMessage") << "getArgAsTrigger(): argument "
		// 	<< index << " is not a trigger";
		return false;
	}
	else {
		return ((ofxOscArgTrigger*)args[index])->get();
	}
}


// util
//--------------------------------------------------------------
void FOscMessage::setRemoteEndpoint(const FString& host, int port) {
	remoteHost = host;
	remotePort = port;
}

