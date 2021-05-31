// Fill out your copyright notice in the Description page of Project Settings.
#include "OscReceiverComponent.h"

// Sets default values for this component's properties
UOscReceiverComponent::UOscReceiverComponent()
    : bStartListening(true)
    , Port(30000)
    , SocketReceiver(nullptr)
    , Socket(nullptr)
    , bMulticastLoopback(false)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}


UOscReceiverComponent::~UOscReceiverComponent()
{}


// Called when the game starts
void UOscReceiverComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetLocalHostAddress(ReceiveIPAddressString))
    {
        UE_LOG(LogTemp, Warning, TEXT("OSCServer ReceiveAddress not specified. Using LocalHost IP: '%s'"), *ReceiveIPAddressString);
    }

    if (ServerName.IsEmpty())
    {
        ServerName = FString::Printf(TEXT("OSCServer_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Short));
    }

    if (this->SetAddress(ReceiveIPAddressString))
    {
        if (bStartListening)
        {
            this->Listen();
            UE_LOG(LogTemp, Warning, TEXT("Start Listerning......."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to parse ReceiveAddress '%s' for OSCServer."), *ReceiveIPAddressString);
    }
}


void UOscReceiverComponent::Listen()
{
    FUdpSocketBuilder Builder(*ServerName);
    Builder.BoundToPort(Port);
    if (ReceiveIPAddress.IsMulticastAddress())
    {
        Builder.JoinedToGroup(ReceiveIPAddress);
        if (bMulticastLoopback)
        {
            Builder.WithMulticastLoopback();
        }
    }
    else
    {
        if (bMulticastLoopback)
        {
            UE_LOG(LogTemp, Warning, TEXT("OSCServer '%s' ReceiveIPAddress provided is not a multicast address.  Not respecting MulticastLoopback boolean."),
                *ServerName);
        }
        Builder.BoundToAddress(ReceiveIPAddress);
    }

    Socket = Builder.Build();
    if (Socket)
    {
        SocketReceiver = new FUdpSocketReceiver(Socket, FTimespan::FromMilliseconds(1), *(ServerName + TEXT("_ListenerThread")));
        SocketReceiver->OnDataReceived().BindUObject(this, &UOscReceiverComponent::OnPacketReceived);
        SocketReceiver->Start();

        UE_LOG(LogTemp, Display, TEXT("OSCServer '%s' Listening: %s:%d."), *ServerName, *ReceiveIPAddress.ToString(), Port);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OSCServer '%s' failed to bind to socket on %s:%d."), *ServerName, *ReceiveIPAddress.ToString(), Port);
    }
}





void UOscReceiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (SocketReceiver)
    {
        delete SocketReceiver;
        SocketReceiver = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("SocketReceiver Closed"));
    }


    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        Socket = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("Socket Closed"));
    }
}



// Called every frame
void UOscReceiverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UOscReceiverComponent::OnPacketReceived(const FArrayReaderPtr& InData, const FIPv4Endpoint& InEndpoint)
{

    ProcessPacket((const char*)InData->GetData(), InData->Num(),
        IpEndpointName(InEndpoint.Address.Value, InEndpoint.Port));

    // Set a single callback in the main thread per frame.
    if (!MessageQue.IsEmpty())
    {
        FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
            FSimpleDelegateGraphTask::FDelegate::CreateUObject(this, &UOscReceiverComponent::CallbackMainThread),
            TStatId(),
            nullptr,
            ENamedThreads::GameThread);
    }
}



void UOscReceiverComponent::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint)
{

    FOscMessage msg;;
    msg.setAddress(m.AddressPattern());

    // set the sender ip/host
    char endpointHost[IpEndpointName::ADDRESS_STRING_LENGTH];
    remoteEndpoint.AddressAsString(endpointHost);
    msg.setRemoteEndpoint(endpointHost, remoteEndpoint.port);

    for (osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg) {
        if (arg->IsInt32()) {
            msg.addIntArg(arg->AsInt32Unchecked());
        }
        else if (arg->IsInt64()) {
            msg.addInt64Arg(arg->AsInt64Unchecked());
        }
        else if (arg->IsFloat()) {
            msg.addFloatArg(arg->AsFloatUnchecked());
        }
        else if (arg->IsDouble()) {
            msg.addDoubleArg(arg->AsDoubleUnchecked());
        }
        else if (arg->IsString()) {
            msg.addStringArg(arg->AsStringUnchecked());
        }
        else if (arg->IsBool()) {
            msg.addBoolArg(arg->AsBoolUnchecked());
        }
        else if (arg->IsNil()) {
            msg.addNoneArg();
        }
        else {

            break;
        }
    }

    MessageQue.Enqueue(msg);
}


void UOscReceiverComponent::CallbackMainThread()
{
    FOscMessage msg;
    auto* GameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(this));
    while (MessageQue.Dequeue(msg))
    {
        if (msg.getAddress() == FString("/projection"))
        {
            //UE_LOG(LogTemp, Error, TEXT("/proj : %i"), msg.getNumArgs());
            FMatrix m;
            m.SetIdentity();

            m.M[0][0] = msg.getArgAsFloat(0);
            m.M[0][1] = msg.getArgAsFloat(1);
            m.M[0][2] = msg.getArgAsFloat(2);
            m.M[0][3] = msg.getArgAsFloat(3);

            m.M[1][0] = msg.getArgAsFloat(4);
            m.M[1][1] = msg.getArgAsFloat(5);
            m.M[1][2] = msg.getArgAsFloat(6);
            m.M[1][3] = msg.getArgAsFloat(7);

            m.M[2][0] = msg.getArgAsFloat(8);
            m.M[2][1] = msg.getArgAsFloat(9);
            m.M[2][2] = msg.getArgAsFloat(10);
            m.M[2][3] = msg.getArgAsFloat(11);

            m.M[3][0] = msg.getArgAsFloat(12);
            m.M[3][1] = msg.getArgAsFloat(13);
            m.M[3][2] = msg.getArgAsFloat(14);
            m.M[3][3] = msg.getArgAsFloat(15);

            GameInstance->Projection = m;

        }

        if (msg.getAddress() == FString("/modelview"))
        {
            //UE_LOG(LogTemp, Error, TEXT("/modelview : %i"), msg.getNumArgs());

            FMatrix m;
            m.SetIdentity();

            m.M[0][0] = msg.getArgAsFloat(0);
            m.M[0][1] = msg.getArgAsFloat(1);
            m.M[0][2] = msg.getArgAsFloat(2);
            m.M[0][3] = msg.getArgAsFloat(3);

            m.M[1][0] = msg.getArgAsFloat(4);
            m.M[1][1] = msg.getArgAsFloat(5);
            m.M[1][2] = msg.getArgAsFloat(6);
            m.M[1][3] = msg.getArgAsFloat(7);

            m.M[2][0] = msg.getArgAsFloat(8);
            m.M[2][1] = msg.getArgAsFloat(9);
            m.M[2][2] = msg.getArgAsFloat(10);
            m.M[2][3] = msg.getArgAsFloat(11);

            m.M[3][0] = msg.getArgAsFloat(12);
            m.M[3][1] = msg.getArgAsFloat(13);
            m.M[3][2] = msg.getArgAsFloat(14);
            m.M[3][3] = msg.getArgAsFloat(15);

            GameInstance->Modelview = m;
        }
        GameInstance->NotifyReceivedOsc(msg);
    }
}



bool UOscReceiverComponent::SetAddress(const FString& InReceiveIPAddress)
{
    if (!FIPv4Address::Parse(InReceiveIPAddress, ReceiveIPAddress))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid ReceiveIPAddress '%s'. OSCServer ReceiveIP Address not updated."), *InReceiveIPAddress);
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("Port : %d"), Port);
    return true;
}




bool UOscReceiverComponent::GetLocalHostAddress(FString& InAddress)
{

    if (!InAddress.IsEmpty() && InAddress != TEXT("0"))
    {
        return false;
    }

    bool bCanBind = false;
    bool bAppendPort = false;
    if (ISocketSubsystem* SocketSys = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
    {
        const TSharedPtr<FInternetAddr> Addr = SocketSys->GetLocalHostAddr(*GLog, bCanBind);
        if (Addr.IsValid())
        {
            InAddress = Addr->ToString(bAppendPort);
            return true;
        }
    }

    return false;
}