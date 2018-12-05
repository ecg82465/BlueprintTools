// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BlueprintUtilityBPLibrary.h"
#include "BlueprintUtility.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"

#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

#include "Engine/Texture2D.h"


UBlueprintUtilityBPLibrary::UBlueprintUtilityBPLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

//Use ContentDir()
FString UBlueprintUtilityBPLibrary::GetFullPath(FString FilePath)
{
	//Check Relative Or absolute path
	FString FullFilePath;
	if (FilePath.StartsWith(".", ESearchCase::CaseSensitive))
	{

		FullFilePath = *FPaths::Combine(FPaths::ProjectContentDir(), FilePath);

		FullFilePath = *FPaths::ConvertRelativePathToFull(FullFilePath);

	}
	else
	{

		FullFilePath = FilePath;
	}

	return FullFilePath;
}

float UBlueprintUtilityBPLibrary::BlueprintUtilitySampleFunction(float Param)
{
	return -1;
}

//Discern Texture Type
static TSharedPtr<IImageWrapper> GetImageWrapperByExtention(const FString InImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	


	if (InImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (InImagePath.EndsWith(".jpg") || InImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (InImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	else if (InImagePath.EndsWith(".ico"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICO);
	}
	else if (InImagePath.EndsWith(".exr"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
	}
	else if (InImagePath.EndsWith(".icns"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::ICNS);
	}
	return nullptr;
}


UTexture2D* UBlueprintUtilityBPLibrary::LoadTexture2DFromFile(const FString& FilePath,
	bool& IsValid, int32& Width, int32& Height)
{

	FString FullFilePath = GetFullPath(FilePath);

	IsValid = false;
	UTexture2D* LoadedT2D = NULL;

	//UE_LOG(LogTemp, Warning, TEXT("FinalPath %s"), *FullFilePath);

	//IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(FullFilePath);
	
	//Load From File
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath, 0)) return NULL ;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create T2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{

			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			//Valid?
			if (!LoadedT2D) return NULL;
			//~~~~~~~~~~~~~~

			//Out!
			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();

			//Copy!
			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			//Update!
			LoadedT2D->UpdateResource();
		}
	}

	// Success!
	IsValid = true;
	return LoadedT2D;
	//return NULL;
}




bool UBlueprintUtilityBPLibrary::ReadOggWaveData(class USoundWave* sw, TArray<uint8>* rawFile)
{
	FSoundQualityInfo info;
	FVorbisAudioInfo vorbis_obj;
	if (!vorbis_obj.ReadCompressedInfo(rawFile->GetData(), rawFile->Num(), &info))
	{
		//Debug("Can't load header");
		return true;
	}

	if (!sw) return true;
	sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	sw->NumChannels = info.NumChannels;
	sw->Duration = info.Duration;
	sw->RawPCMDataSize = info.SampleDataSize;
	sw->SetSampleRate(info.SampleRate);

	return false;
}

bool UBlueprintUtilityBPLibrary::ReadWavWaveData(class USoundWave* sw, TArray<uint8>* rawFile)
{

	return false;
}


class USoundWave* UBlueprintUtilityBPLibrary::LoadSoundWaveFromFile(const FString& FilePath)
{

	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return NULL;

	//* If true the song was successfully loaded
	bool loaded = false;
	
	FString FullPath = GetFullPath(FilePath);


	//* loaded song file (binary, encoded)
	TArray < uint8 > rawFile;

	loaded = FFileHelper::LoadFileToArray(rawFile, FullPath.GetCharArray().GetData());

	if (loaded)
	{
		FByteBulkData* bulkData = &sw->CompressedFormatData.GetFormat(TEXT("OGG"));

		bulkData->Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(bulkData->Realloc(rawFile.Num()), rawFile.GetData(), rawFile.Num());
		bulkData->Unlock();

		loaded = ReadOggWaveData(sw, &rawFile) == 0 ? true : false;
	}

	if (!loaded)
		return NULL;

	return sw;

}


void UBlueprintUtilityBPLibrary::ReadConfig(const FString& SectionName, const FString& ValueName, FString &ReturnValue)
{

	//GConfig->Flush(true, GGameIni);

	bool succeed = false;

	 succeed = GConfig->GetString(
		*SectionName,
		*ValueName,
		ReturnValue,
		GGameIni
	);


	UE_LOG(LogTemp, Warning, TEXT("Read Config %s "),succeed ? TEXT("Succeed") : TEXT("Fail"));
	
}


void UBlueprintUtilityBPLibrary::WriteConfig(const FString& SectionName, const FString& ValueName, const FString &ReturnValue)
{

	//FString newSection = "/Script/CommunicationSetting";
	//FString TA = "DefaultMyConfig";
	GConfig->SetString(
		*SectionName,
		*ValueName,
		*ReturnValue,
		GGameIni
	);

	GConfig->Flush(false, GGameIni);
	/*
		FString log;
		ReadConfig(ReturnValue, ValueName, log);

		UE_LOG(LogTemp, Warning, TEXT("Set Config As %s "), *log);*/

}


bool UBlueprintUtilityBPLibrary::ReadCustomPathConfig(const FString&FilePath, const FString& SectionName, const FString& ValueName, FString &ReturnString)
{
	FString FullPath = GetFullPath(FilePath);

	GConfig->Flush(true, FullPath);
	bool succeed = GConfig->GetString(*SectionName, *ValueName, ReturnString, FullPath);
	return succeed;

}


 void UBlueprintUtilityBPLibrary::WriteCustomPathConfig(const FString&FilePath, const FString& SectionName, const FString& ValueName, const FString &WriteString)
{
	FString FullPath 
= GetFullPath(FilePath);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Does the file exist?
	if (!PlatformFile.FileExists(*FullPath))
	{
		// File doesn't exist; (Attempt to) create a new one.
		FFileHelper::SaveStringToFile(TEXT(""), *FullPath);
	}



	GConfig->SetString(*SectionName, *ValueName, *WriteString, FullPath);

	GConfig->Flush(false, FullPath);

}


 void UBlueprintUtilityBPLibrary::RefrashAllSkeletallAnimation()
 {
	 for (TObjectIterator<AActor> iterator; iterator; ++iterator)
	 {
		 if (iterator)
		 {
			 for (auto actor_Component : iterator->GetComponents())
			 {
				 if (actor_Component->IsA(USkeletalMeshComponent::StaticClass()))
				 {
					 (Cast<USkeletalMeshComponent>(actor_Component))->TickAnimation(0.0f, false);
					 (Cast<USkeletalMeshComponent>(actor_Component))->RefreshBoneTransforms();
				 }
			 }
		 }
	 }
 };




 bool UBlueprintUtilityBPLibrary::ReadFile(const FString FilePath, FString& ReturnString)
 {
	 FString FullPath = GetFullPath(FilePath);

	 FString Cache = "";
	 bool Sucess = false;
	 Sucess = FFileHelper::LoadFileToString(Cache, FullPath.GetCharArray().GetData());
	 ReturnString = Cache;
	 return Sucess;
 }

 bool UBlueprintUtilityBPLibrary::WriteFile(const FString FilePath, const FString ReturnString)
 {
	 FString FullPath = GetFullPath(FilePath);

	 bool Sucess;
	 Sucess = FFileHelper::SaveStringToFile(ReturnString, *FullPath);
	 return Sucess;
 }


 FString UBlueprintUtilityBPLibrary::GetGamePath(DirType E)
 {
	 if (E == DirType::GameDir)
	 {
		 return FPaths::GameDir();
	 }
	 return FPaths::GameContentDir();

 }