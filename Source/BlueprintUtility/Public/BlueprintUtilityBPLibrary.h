// Copyright  ECG . All Rights Reserved.

#pragma once

#include "DDSLoader.h"
#include "Core.h"
#include "Engine.h"

//LoadImageAsync
#include "DynamicLoader.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Runtime/Engine/Classes/Sound/SoundWave.h"

#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"

#include "BlueprintUtilityBPLibrary.generated.h"


//DECLARE_LOG_CATEGORY_EXTERN(CategoryName, Log, All)

/* 
*
*	BluePrint Function Collection
*	
*      
*   This Way To More
*	https://github.com/ecg82465/BluePrintTools
*/

UENUM(BlueprintType)
enum class DirType : uint8
{
	GameDir,
	ContentDir
};


UCLASS()
class UBlueprintUtilityBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TEST", Keywords = "BlueprintUtility sample test testing"), Category = "BlueprintUtility")
	static float BlueprintUtilitySampleFunction(float Param);

	static FString GetFullPath(FString FilePath);

	/*                   RuntimeLoader                    */

	//UFUNCTION(BlueprintPure, Category = "BlueprintUtility|Loader", meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"))
	//static TArray<FString> LoadTexture2DFromFile_Windows(const FString FileType);

	/** Load Texture2D */
	UFUNCTION(BlueprintPure, Category = "BlueprintUtility|Loader", meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"))
	static UTexture2D* LoadTexture2DFromFile(const FString& FilePath, bool& IsValid, int32& Width, int32& Height);

	/** Load Texture2D No Blocking Game Logic */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Loader", meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"))
	static UImageLoader* LoadTexture2DFromFile_Async( const FString& FilePath,const FString& ID);

	/** Load a Sound from a file!  */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Loader", meta = (Keywords = ""))
	static class USoundWave* LoadOggDataFromFile(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Loader", meta = (Keywords = ""))
	static class USoundWave* LoadWaveDataFromFile(const FString& FilePath);

	/**                  Config                           */

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Config", meta = (Keywords = ""))
	static void ReadConfig(const FString& SectionName,const FString& ValueName, FString &ReturnValue);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Config", meta = (Keywords = ""))
	static void WriteConfig(const FString& SectionName, const FString& ValueName, const FString &ReturnValue);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Config", meta = (Keywords = ""))
	static bool ReadCustomPathConfig(const FString&FilePath,const FString& SectionName, const FString& ValueName, FString &ReturnString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Config", meta = (Keywords = ""))
	static void WriteCustomPathConfig(const FString&FilePath, const FString& SectionName, const FString& ValueName,const FString &WriteString);

	UFUNCTION(BlueprintPure, Category = "BlueprintUtility|Config", meta = (Keywords = ""))
	static bool IsEditorMode( );



	/** RefrashAllSkeletal Trnasfrom In This Frame (Fix Animation Dither Error)*/

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|Animation", meta = (Keywords = ""))
	static void RefrashAllSkeletallAnimation();

	/**                   File                           */

	/** Read Or Write Custom Text */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool ReadFile(const FString FilePath, FString& ReturnString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool WriteFile(const FString FilePath, const FString ReturnString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
    static bool WriteFileByte(TArray<uint8> bytes, const FString FilePath);
	
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool DeleteFile(const FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool DeleteFiles(const FString FilePath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool CopyFile (const FString FilePath, const FString ToPath);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool Texture2d2PNG(UTextureRenderTarget2D* TextureRenderTarget, const FString& FilePath);

	

	/** Get Path */
	UFUNCTION(BlueprintPure, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static bool IsVaildPath(const FString FilePath);

	UFUNCTION(BlueprintPure, Category = "BlueprintUtility|File", meta = (Keywords = ""))
	static FString GetGamePath(DirType E);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility|OS", meta = (Keywords = "exe"))
	static AExeActor* OpenExe(UObject* SomeInWorldObject,const FString Path,const FString Args);



	UFUNCTION(BlueprintPure, Category = "BlueprintUtility|Functions", meta = (Keywords = "color"))
	static void GenColors(int Length, const FColor color, TArray<FColor>& OuterColor);

	UFUNCTION(BlueprintPure, Category = "BlueprintUtility|Functions", meta = (Keywords = "color"))
		static void UVtimes(FVector2D tims,const TArray<FVector2D> inputUV, TArray<FVector2D>& OuterUV);


private:

	static bool ReadWavWaveData(USoundWave* sw, TArray<uint8>* rawFile);

	static bool ReadOggWaveData(USoundWave* sw, TArray<uint8>* rawFile);

};
 