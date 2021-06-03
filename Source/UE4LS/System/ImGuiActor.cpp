// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGuiActor.h"

// Sets default values
AImGuiActor::AImGuiActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AImGuiActor::BeginPlay()
{
	Super::BeginPlay();
	CameraPositions.Emplace(0.f); CameraPositions.Emplace(170.f); CameraPositions.Emplace(-1000.f);
	CameraRotations.Init(0.f, 3.f);

#if WITH_IMGUI
	ImGuiStyle& Style = ImGui::GetStyle();
	ImGui::StyleColorsClassic(&Style);
	Style.FrameRounding = 0.f;
	Style.WindowPadding = ImVec2(8.f, 8.f);
	Style.WindowRounding = 0.f;
	Style.ChildRounding = 0.f;
	Style.FrameRounding = 0.f;
	Style.PopupRounding = 0.f;
	Style.ScrollbarRounding = 0.f;
	Style.GrabRounding = 0.f;
	Style.TabRounding = 0.f;
	Style.FramePadding = ImVec2(2.f, 4.f);
	Style.ItemSpacing = ImVec2(8.f, 4.f);
	Style.GrabMinSize = 10.f;


	for (ULevelStreaming* SubLevel : GetWorld()->GetStreamingLevels())
	{
		FString LevelName;
#if WITH_EDITOR
		LevelName = FString(GetWorld()->RemovePIEPrefix(SubLevel->GetWorldAssetPackageName()));
#else
		LevelName = SubLevel->GetWorldAssetPackageFName().ToString();
#endif
		TArray<FString> Values;
		LevelName.ParseIntoArray(Values, TEXT("/"));

		if (Values.Last() == FString("GlobalScene"))
			continue;

		SubLevelNames.Emplace(Values.Last());
	}

	FImGuiDelegates::OnWorldDebug().AddUObject(this, &AImGuiActor::ImGuiTick);
#endif
}




#if WITH_IMGUI
void AImGuiActor::ImGuiTick()
{
	auto* GameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(this));

	//caution : left hand coord
	ImGui::Begin("Camera Control");
	if (ImGui::SliderFloat3("Position", CameraPositions.GetData(), -1000.f, 1000.f) ||
		ImGui::SliderFloat3("Rotation", CameraRotations.GetData(), -360.f, 360.f))
	{
		FTransform Transform;
		Transform.SetTranslation(FVector(CameraPositions[2], CameraPositions[0], CameraPositions[1]));

		FQuat Quat(FQuat::Identity);
		Quat *= FQuat(FVector(1, 0, 0), CameraRotations[0] * (PI / 180.f));
		Quat *= FQuat(FVector(0, 1, 0), CameraRotations[1] * (PI / 180.f));
		Quat *= FQuat(FVector(0, 0, 1), CameraRotations[2] * (PI / 180.f));

		Transform.SetRotation(Quat);
		CaptureComponent2D->SetActorTransform(Transform);
	}
	ImGui::End();


	ImGui::Begin("Level (Scene)");
	for (FString& SubLevelName : SubLevelNames)
	{
		ImGui::Text(TCHAR_TO_ANSI(*SubLevelName));
		FString LoadLabel = FString("Load") + FString("##") + SubLevelName;
		FString UnloadLabel = FString("Unload") + FString("##") + SubLevelName;
		if (ImGui::Button(TCHAR_TO_ANSI(*LoadLabel)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Button Pressed:  %s"), *SubLevelName);
			GameInstance->NotifyLoadScene(SubLevelName); /*ImGui::SameLine();*/
		}
		ImGui::SameLine();
		if (ImGui::Button(TCHAR_TO_ANSI(*UnloadLabel)))
		{
			UE_LOG(LogTemp, Warning, TEXT("Unload Button Pressed :  %s"), *SubLevelName);
			GameInstance->NotifyUnloadScene(SubLevelName);
		}
	}
	ImGui::End();


	ImGui::Begin("Level Parameter");
	//const FRegexPattern pattern = FRegexPattern(FString(TEXT("< *event *([0-9]+) *>")));
	for (const auto* Level : GetWorld()->GetLevels())
	{
		//skip persistent level
		if (Level->GetOuter()->GetName() == FString("Common"))
			continue;


		//Scene parameter 
		if (ImGui::CollapsingHeader(TCHAR_TO_ANSI(*Level->GetOuter()->GetName())))
		{
			ACustomLevelScriptActor* La = Cast<ACustomLevelScriptActor>(Level->GetLevelScriptActor());
			for (TFieldIterator<UProperty> PropertyIterator(La->GetClass()); PropertyIterator; ++PropertyIterator)
			{
				FProperty* Property = *PropertyIterator;
				FString PropertyName = Property->GetFName().ToString();

				if (PropertyName.Contains(TEXT("Param_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
				{
					if (FIntProperty* IntProperty = Cast<FIntProperty>(Property))
					{
						ImGui::SliderInt(TCHAR_TO_ANSI(*IntProperty->GetName()), IntProperty->GetPropertyValuePtr_InContainer(La), -1000, 1000);
					}

					else if (FFloatProperty* FloatProperty = Cast<FFloatProperty>(Property))
					{
						ImGui::SliderFloat(TCHAR_TO_ANSI(*FloatProperty->GetName()), FloatProperty->GetPropertyValuePtr_InContainer(La), -1000, 1000);
					}

					else if (FStructProperty* StructProperty = Cast<FStructProperty>(Property))
					{
						if ("Vector2D" == StructProperty->Struct->GetName()) //float 2D
						{
							auto* v = StructProperty->ContainerPtrToValuePtr<FVector2D>(La);
							TArray<float> Arr;
							Arr.Emplace(v->X); Arr.Emplace(v->Y);
							if (ImGui::SliderFloat3(TCHAR_TO_ANSI(*StructProperty->GetName()), Arr.GetData(), -1000, 1000))
							{
								v->X = Arr[0]; v->Y = Arr[1];
							}
						}
						else if ("Vector" == StructProperty->Struct->GetName()) //float 3D
						{
							auto* v = StructProperty->ContainerPtrToValuePtr<FVector>(La);
							TArray<float> Arr;
							Arr.Emplace(v->X); Arr.Emplace(v->Y); Arr.Emplace(v->Z);
							if (ImGui::SliderFloat3(TCHAR_TO_ANSI(*StructProperty->GetName()), Arr.GetData(), -1000, 1000))
							{
								v->X = Arr[0]; v->Y = Arr[1]; v->Z = Arr[2];
							}
						}
						else if ("Vector4" == StructProperty->Struct->GetName()) //float 4D
						{
							auto* v = StructProperty->ContainerPtrToValuePtr<FVector4>(La);
							TArray<float> Arr;
							Arr.Emplace(v->X); Arr.Emplace(v->Y); Arr.Emplace(v->Z); Arr.Emplace(v->W);
							if (ImGui::SliderFloat4(TCHAR_TO_ANSI(*StructProperty->GetName()), Arr.GetData(), -1000, 1000))
							{
								v->X = Arr[0]; v->Y = Arr[1]; v->Z = Arr[2]; v->W = Arr[3];
							}
						}
						else if ("IntVector" == StructProperty->Struct->GetName()) //int 3D
						{
							auto* v = StructProperty->ContainerPtrToValuePtr<FIntVector>(La);
							TArray<int> Arr;
							Arr.Emplace(v->X); Arr.Emplace(v->Y); Arr.Emplace(v->Z);
							if (ImGui::SliderInt3(TCHAR_TO_ANSI(*StructProperty->GetName()), Arr.GetData(), -100, 100))
							{
								v->X = Arr[0]; v->Y = Arr[1]; v->Z = Arr[2];
							}
						}
						//UE_LOG(LogTemp, Warning, TEXT("OutDeps :  %s"), *StructProperty->Struct->GetName());
						//UE_LOG(LogTemp, Warning, TEXT("PropertyName :  %s"), *StructProperty->GetName());
					}
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("------------"));
		}
	}
	ImGui::End();
	//ImGui::ShowDemoWindow();
}
#endif



// Called every frame
void AImGuiActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

