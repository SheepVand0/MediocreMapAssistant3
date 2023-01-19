// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTextBlock.h"
#include "Engine/Font.h"

UCustomTextBlock::UCustomTextBlock() {
	ConstructorHelpers::FObjectFinder<UFont> l_Font(TEXT("Font'/Game/Assets/Fonts/Teko/F_Teko.F_Teko'"));
	FCompositeFont l_CompositeFont = l_Font.Object->CompositeFont;
	//Font.CompositeFont = TSharedPtr<FCompositeFont>(&l_CompositeFont);
	Font.FontObject = l_Font.Object;
	Font.TypefaceFontName = FName("Medium");
}