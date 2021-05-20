// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

typedef enum _OscArgType{
	OSC_TYPE_INT32            = 'i',
	OSC_TYPE_INT64            = 'h',
	OSC_TYPE_FLOAT            = 'f',
	OSC_TYPE_DOUBLE           = 'd',
	OSC_TYPE_STRING           = 's',
	OSC_TYPE_CHAR		      = 'c',
	OSC_TYPE_TRUE             = 'T',
	OSC_TYPE_FALSE            = 'F',
	OSC_TYPE_NONE             = 'N',
	OSC_TYPE_TRIGGER          = 'I',
	OSC_TYPE_INDEXOUTOFBOUNDS = 0 ///< bad index value
} EOscArgType;

/// \class IMGUI_TEST_API ofxOscArg
/// \brief base class IMGUI_TEST_API for arguments
class UE4LS_API OscArg
{
public:
	virtual ~OscArg() {}
	virtual EOscArgType getType() const {return OSC_TYPE_NONE;}
	virtual FString getTypeName() const {return FString("N");}
};



/// \brief a 32-bit integer argument, type name "i"
class UE4LS_API OscArgInt32 : public OscArg
{
public:
	OscArgInt32(int32 Value) : Value(Value) {}
	EOscArgType getType() const {return OSC_TYPE_INT32;}
	FString getTypeName() const {return FString("i"); }
	int32 get() const {return Value;}
	void set(int32 Value_) { Value = Value_;}

private:
	int32 Value;
};



/// \brief a 32-bit integer argument, type name "i"
class UE4LS_API OscArgInt : public OscArgInt32
{
public:
	OscArgInt(int32 Value) : OscArgInt32(Value) {}
};



/// \brief a 64-bit integer argument, type name "h"
class UE4LS_API OscArgInt64 : public OscArg
{
public:
	OscArgInt64(int64 Value) : Value(Value) {}
	EOscArgType getType() const {return OSC_TYPE_INT64;}
	FString getTypeName() const { return FString("h"); }
	int64 get() const { return Value; }
	void set(int64 Value_) {Value = Value_;}
private:
	int64 Value;
};



/// \brief a 32-bit float argument, type name "f"
class UE4LS_API OscArgFloat : public OscArg
{
public:
	OscArgFloat(float Value) : Value(Value) {}
	EOscArgType getType() const {return OSC_TYPE_FLOAT;}
	FString getTypeName() const { return FString("f"); }
	float get() const {return Value;}
	void set(float Value_) {Value = Value_;}
private:
	float Value;
};



/// \brief a 64-bit double argument, type name "d"
class UE4LS_API OscArgDouble : public OscArg
{
public:
	OscArgDouble(double Value) : Value(Value) {}
	EOscArgType getType() const {return OSC_TYPE_DOUBLE;}
	FString getTypeName() const { return FString("d"); }
	double get() const {return Value;}
	void set(double Value_) {Value = Value_;}
private:
	double Value;
};



/// \brief a null-terminated string argument, type name "s"
class UE4LS_API OscArgString : public OscArg
{
public:
	OscArgString(const FString& Value) : Value(Value) {}
	EOscArgType getType() const {return OSC_TYPE_STRING;}
	FString getTypeName() const { return FString("s"); }
	const FString& get() const {return Value;}
	void set(const char *Value_) { Value = FString(Value_); }
	void set(const FString& Value_) {Value = Value_;}
private:
	FString Value;
};



/// \brief a null-terminated char argument, type name "c"
class UE4LS_API OscArgChar : public OscArg
{
public:
	OscArgChar(char Value) : Value(Value) {}
	EOscArgType getType() const { return OSC_TYPE_CHAR; }
	FString getTypeName() const { return FString("c"); }
	char get() const { return Value; }
	void set(char Value_) { Value = Value_; }

private:
	char Value;
};



/// \brief a boolean argument, either type name "T" or "F" based on value
class UE4LS_API OscArgBool : public OscArg{
public:
	OscArgBool(bool value) : Value(value) {}
	EOscArgType getType() const { return Value ? OSC_TYPE_TRUE : OSC_TYPE_FALSE; }
	FString getTypeName() const { return Value ? FString("T") : FString("F"); }
	bool get() const { return Value; }
	void set(bool Value_) {Value = Value_;}
private:
	bool Value;
};



/// \brief a none/nil (has no value), type name "N"
class UE4LS_API ofxOscArgNone : public OscArgBool{
public:
	ofxOscArgNone() : OscArgBool(true) {}
	EOscArgType getType() const {return OSC_TYPE_NONE;}
	FString getTypeName() const { return FString("N"); }
};




/// \brief a trigger impulse (has no value), type name "I"
class UE4LS_API ofxOscArgTrigger : public OscArgBool {
public:
	ofxOscArgTrigger() : OscArgBool(true) {}
	EOscArgType getType() const {return OSC_TYPE_TRIGGER;}
	FString getTypeName() const { return FString("I"); }
};


