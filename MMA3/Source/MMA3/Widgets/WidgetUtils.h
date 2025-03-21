// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/ContentWidget.h"

UENUM()
enum class EJoyImageFormats : uint8
{
	JPG		UMETA(DisplayName = "JPG"),
	PNG		UMETA(DisplayName = "PNG"),
	BMP		UMETA(DisplayName = "BMP"),
	ICO		UMETA(DisplayName = "ICO"),
	EXR		UMETA(DisplayName = "EXR"),
	ICNS	UMETA(DisplayName = "ICNS")
};

/**
 *
 */
class MMA3_API WidgetUtils
{
public:
	WidgetUtils();
	~WidgetUtils();

	static FSlateBrush GetUIElementStyle(FSlateBrush p_SlateBrush, float p_Size, FColor p_Color, FMargin p_Margin) {
		p_SlateBrush.SetImageSize(FVector2D(p_Size, p_Size));
		p_SlateBrush.DrawAs = ESlateBrushDrawType::Box;
		p_SlateBrush.TintColor = p_Color;
		p_SlateBrush.Margin = p_Margin;
		return p_SlateBrush;
	}

//	static EImageFormat GetJoyImageFormat(EJoyImageFormats JoyFormat)
//	{
//		switch (JoyFormat) {
//		case EJoyImageFormats::JPG: return EImageFormat::JPEG;
//		case EJoyImageFormats::PNG: return EImageFormat::PNG;
//		case EJoyImageFormats::BMP: return EImageFormat::BMP;
//		case EJoyImageFormats::ICO: return EImageFormat::ICO;
//		case EJoyImageFormats::EXR: return EImageFormat::EXR;
//		case EJoyImageFormats::ICNS: return EImageFormat::ICNS;
//		}
//
//		return EImageFormat::JPEG;
//	}
//
//	static FString GetJoyImageExtension(EJoyImageFormats JoyFormat)
//	{
//		switch (JoyFormat) {
//		case EJoyImageFormats::JPG: return ".jpg";
//		case EJoyImageFormats::PNG: return ".png";
//		case EJoyImageFormats::BMP: return ".bmp";
//		case EJoyImageFormats::ICO: return ".ico";
//		case EJoyImageFormats::EXR: return ".exr";
//		case EJoyImageFormats::ICNS: return ".icns";
//		}
//		return ".png";
//	}
//
//	static UTexture2D* LoadTexture2DFromFile(const FString& FileName, EJoyImageFormats ImageFormat, bool& IsValid, int32& Width, int32& Height)
//	{
//		IsValid = false;
//		UTexture2D* LoadedT2D = nullptr;
//
//		typedef TSharedPtr<IImageWrapper> IImageWrapperPtr;
//
//		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
//		IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(GetJoyImageFormat(ImageFormat));
//
//		//Load From File
//		FString FullFilePath = FileName;
//		TArray<uint8> RawFileData;
//		if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath)) return nullptr;
//
//		//Create T2D!
//		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
//		{
//			TArray<uint8> UncompressedBGRA;
//			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
//			{
//				LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
//
//				//Valid?
//				if (!LoadedT2D) return nullptr;
//				//~~~~~~~~~~~~~~
//
//				//Out!
//				Width = ImageWrapper->GetWidth();
//				Height = ImageWrapper->GetHeight();
//
//				//Copy!
//				void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
//				FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
//				LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
//
//				//Update!
//				LoadedT2D->UpdateResource();
//			}
//		}
//
//		// Success!
//		IsValid = true;
//		return LoadedT2D;
//	}
//
};

template<typename T, typename F>
static void foreach(T &p_Container, F p_Func) {
	for (T l_Item : p_Container) {
		p_Func(l_Item);
	}
};