#pragma once
#include "FBullCowGame.h"
#include<map>
#include<ctime>
#include<iostream>

//make syntax unreal freindly
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); }//default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
FString FBullCowGame::GetHiddenWord() { return MyHiddenWord; }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }
void FBullCowGame::SetGameIsWon() { bGameIsWon = true; }

void FBullCowGame::Reset()
{

	MyCurrentTry = 1;
	bGameIsWon = false;

	return;
}

int32 FBullCowGame::GetMaxTries() const
{ 
	TMap<int32, int32> WordLengthToMaxTries{{3,6}, {4,9}, {5,15}, {6,20}, {7,25} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}


void FBullCowGame::SetHiddenWord(FString Word)
{
	MyHiddenWord = Word;//this MUST be an isogram
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	//treat 0 or 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;//setup map
	for(char Letter : Word)//loop through all letters in word
	{
		Letter = tolower(Letter);//handle mixed case
		if(LetterSeen[Letter]){//if the letter is in the map
			return false;//we do not have an isogram
		} else {
			LetterSeen[Letter] = true;
			return true;//yes its an isogram
		}
	}
	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}
	return true;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{


	if (!IsIsogram(Guess)) // if the guess isn't an isogram TODO write function
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase TODO write function
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}


// receives a VALID guess, incriments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length();//assuming same length as guess

	// loop through all letters in the HiddenWord
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters against the Guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				// if they're in the same place
				if (MHWChar == GChar)
				{ 
					BullCowCount.Bulls++; // incriment bulls
				}
				else
				{
					BullCowCount.Cows++; // must be a cow
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		SetGameIsWon();
	}
	return BullCowCount;
}
