// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

ACamera::ACamera()
{
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
	//SetActorTickEnabled(true);
	UE_LOG(LogTemp, Warning, TEXT("ACamera::ACamera()"));
}


void ACamera::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.Target = this;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.RegisterTickFunction(GetLevel());
}


void ACamera::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void ACamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto* GameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(this));
	auto _Modelview = GameInstance->Modelview.GetTransposed();
	auto _Projection = GameInstance->Projection.GetTransposed();
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *_Modelview.ToString());

	// update modelview
	{
		FMatrix model_gl_to_ue4;
		model_gl_to_ue4.SetIdentity();

		auto& mat = model_gl_to_ue4;
		mat.M[0][0] = 0, mat.M[0][1] = 0, mat.M[0][2] = -1;
		mat.M[1][0] = 1, mat.M[1][1] = 0, mat.M[1][2] = 0;
		mat.M[2][0] = 0, mat.M[2][1] = 1, mat.M[2][2] = 0;

		FMatrix ConvertedModelView = model_gl_to_ue4 * _Modelview * model_gl_to_ue4.GetTransposed();
		ConvertedModelView.ScaleTranslation(FVector(100));

		this->SetActorTransform(FTransform(ConvertedModelView), true);
	}

	// update projection
	{
		auto M = _Projection;

		M.M[2][3] *= -1;
		M.M[2][2] *= 0;
		M.M[3][2] *= -100;

		M.M[2][0] *= -1;
		M.M[2][1] *= -1;

		USceneCaptureComponent2D* SC = GetCaptureComponent2D();
		if (M.M[0][0] > 0 && M.M[1][1] > 0) {
			SC->bUseCustomProjectionMatrix = true;
			SC->CustomProjectionMatrix = M;
		}
		else
		{
			SC->bUseCustomProjectionMatrix = false;
		}
	}
}