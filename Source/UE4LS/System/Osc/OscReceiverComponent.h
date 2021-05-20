// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Networking.h"
#include "Sockets.h"

#include "oscpack/ip/IpEndpointName.h"
#include "oscpack/osc/OscReceivedElements.h"
#include "oscpack/osc/OscPacketListener.h"
#include "OscMessage.h"

#include "../CustomGameInstance.h"

#include "OscReceiverComponent.generated.h"

// it is workaround. I cant use multi inheritance directly??? but it works with imtermediate class. why?
// using osc_OscPacketListener = osc::OscPacketListener
class osc_OscPacketListener : public osc::OscPacketListener
{
public:
	osc_OscPacketListener() {}
	virtual ~osc_OscPacketListener() {}
	virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint) = 0;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE4LS_API UOscReceiverComponent : public UActorComponent, public osc_OscPacketListener
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOscReceiverComponent();
	~UOscReceiverComponent();

	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool GetLocalHostAddress(FString& InAddress);

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Listen();
	void OnPacketReceived(const FArrayReaderPtr& InData, const FIPv4Endpoint& InEndpoint);
	void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint) override;
	void CallbackMainThread();
	bool SetAddress(const FString& InReceiveIPAddress);

	TQueue<FOscMessage> MessageQue;

	FUdpSocketReceiver* SocketReceiver;
	FSocket* Socket;
	FIPv4Address ReceiveIPAddress;

	FOscMessage msg_;
	FString ReceiveIPAddressString;
	FString ServerName;

	UPROPERTY(EditAnywhere)
		int32 Port;

	UPROPERTY(EditAnywhere)
		bool bStartListening;

	UPROPERTY(EditAnywhere)
		bool bMulticastLoopback;

};