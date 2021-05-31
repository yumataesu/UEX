// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "OscArg.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OscMessage.generated.h"

/**
 * 
 */
class UE4LS_API OscMessage
{
public:
	OscMessage();
	~OscMessage();
};

USTRUCT(BlueprintType)
struct UE4LS_API FOscMessage
{
public:
	GENERATED_BODY()

	FOscMessage();
	~FOscMessage();
	FOscMessage(const FOscMessage& other);
	FOscMessage& operator=(const FOscMessage& other);
	/// for operator= and copy constructor
	FOscMessage& copy(const FOscMessage& other);

	/// clear this message
	void clear();
	void setAddress(const FString& address);
	FString getAddress() const;
	FString getRemoteIp() const;
	FString getRemoteHost() const;
	int getRemotePort() const;
	std::size_t getNumArgs() const;

	EOscArgType getArgType(std::size_t index) const;

	FString getArgTypeName(std::size_t index) const;
	FString getTypeString() const;

	int32 getArgAsInt(std::size_t index) const;
	int32 getArgAsInt32(std::size_t index) const;
	int64 getArgAsInt64(std::size_t index) const;
	float getArgAsFloat(std::size_t index) const;
	double getArgAsDouble(std::size_t index) const;
	FString getArgAsString(std::size_t index) const;
	char getArgAsChar(std::size_t index) const;
	bool getArgAsBool(std::size_t index) const;
	bool getArgAsNone(std::size_t index) const;
	bool getArgAsTrigger(std::size_t index) const;


	void addIntArg(int32 argument);
	void addInt32Arg(int32 argument);
	void addInt64Arg(int64 argument);
	void addFloatArg(float argument);
	void addDoubleArg(double argument);
	void addStringArg(const FString& argument);
	void addCharArg(char argument);
	void addBoolArg(bool argument);
	void addNoneArg();

	void setRemoteEndpoint(const FString& host, int port);

private:

	FString address; ///< OSC address, must start with a /
	std::vector<OscArg*> args; ///< current arguments
	FString remoteHost; ///< host name/ip the message was sent from
	int remotePort; ///< port the message was sent from
};



UCLASS()
class UE4LS_API UMyStructBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


		UFUNCTION(BlueprintCallable, Category = "MyStruct|Methods")
		static int32 GetArgAsInt(UPARAM(ref) FOscMessage& MyStructRef, const int Index)
	{
		return MyStructRef.getArgAsInt(Index);
	};


	UFUNCTION(BlueprintCallable, Category = "MyStruct|Methods")
		static float GetArgAsFloat(UPARAM(ref) FOscMessage& MyStructRef, const int Index)
	{
		return MyStructRef.getArgAsFloat(Index);
	};


	UFUNCTION(BlueprintCallable, Category = "MyStruct|Methods")
		static FString GetArgAsString(UPARAM(ref) FOscMessage& MyStructRef, const int Index)
	{
		return MyStructRef.getArgAsString(Index);
	};


	UFUNCTION(BlueprintCallable, Category = "MyStruct|Methods")
		static FString GetAddress(UPARAM(ref) FOscMessage& MyStructRef)
	{
		return MyStructRef.getAddress();
	};
};
