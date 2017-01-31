/* This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.

A large portion of these words were taken from a post from another student but i 
can't find the post i was looking at for them....so to that person, my bad i can't
give credit
*/
#pragma once
#include <iostream>
#include <string>	
#include "FBullCowGame.h"
#include<map>
#include<ctime>

//make syntax unreal freindly
#define TMap std::map
using FText = std::string;
using int32 = int;

//function prototypes as outside a class
void PrintIntro();
void PlayGame();
void SetHidden_Word();
void PrintGameSummary();
int32 AskDifficulty();
FText GetValidGuess();
bool AreWordsLeft();
bool AskToPlayAgain();

TMap<FText, bool> IsEasyWordUsed{{"ant",false}, {"dog",false}, {"cat",false}, {"ben",false}, {"pig",false}, {"and",false} };
TMap<FText, bool> IsMediumWordUsed{{"zombie",false},{"squid",false},{"bricks",false},{"magic",false},{"rapids",false}, {"planet",false} };
TMap<FText, bool> IsHardWordUsed{{"karting",false},{"padlock",false},{"zephyr",false},{"update",false},{"backlog",false}, {"orgasm",false} };
//must update this list when adding new words, order of words doesn't matter
FText AllWords[] = {"ant", "dog", "cat", "ben", "pig", "and", "zombie", "squid", "bricks", "magic", "rapids", "planet",
                                 "karting", "padlock", "zephyr", "update", "backlog", "orgasm"};
int32 NumOfWords = 17;//must update this when adding words
int32 MainDifficulty;//needed to access in more than one method
int32 DifficultiesExhausted[] = {0, 0, 0};//sets up so they aren't the values AskDifficulty() is looking for

FBullCowGame BCGame; // instantiate a new game, which we reuse across plays




 // the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do
	{
		system("cls");
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);

	return 0; // exit the application
}




void PrintIntro()
{
	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n\n";
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << "\n\n";
	SetHidden_Word();//implements AskForDifficulty()
	if (BCGame.GetHiddenWord() == "NoEasy")
	{
		std::cout << "You've exhausted the easy isograms.\n";
		std::cout << "Please enter a new difficulty to continue.\n";
		SetHidden_Word();
		std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram im thinking of? ";
	}
	else if (BCGame.GetHiddenWord() == "NoMedium")
	{
		std::cout << "You've exhausted the Medium isograms.\n";
		std::cout << "Please enter a new difficulty to continue.\n";
		SetHidden_Word();
		std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram im thinking of? ";
	}
	else if (BCGame.GetHiddenWord() == "NoHard")
	{
		std::cout << "You've exhausted the hard isograms.\n";
		std::cout << "Please enter a new difficulty to continue.\n";
		SetHidden_Word();
		std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram im thinking of? ";
	}
	else if (BCGame.GetHiddenWord() == "none left")
	{
		std::cout << "Sorry but you've exhausted all my isograms. Nice job and goodbye!";
	}
	else
	{
		std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram im thinking of? ";
	}
	std::cout << std::endl;
	return;
}



//plays a single game to completion
void PlayGame()
{
	int32 MaxTries = BCGame.GetMaxTries();

	// loop for the number of turns asking for guesses
	while(!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries && BCGame.GetHiddenWordLength() != 9)
	{ // TODO change from FOR to WHILE
		FText Guess = GetValidGuess();

		// submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	BCGame.Reset();
}



void SetHidden_Word()
{
	FString EasyWords[] = {"ant", "dog", "cat", "ben", "pig", "and"}; //these MUST be isograms
	FString MediumWords[] = {"zombie", "squid", "bricks", "magic", "rapids", "planet"};
	FString HardWords[] = {"karting", "padlock", "zephyr", "update", "backlog", "orgasm"};
	FText Word = "";
	int32 RandNum;
	int32 Tries = 0;
	bool ValidWord = false;

	if (AreWordsLeft())
	{
		MainDifficulty = AskDifficulty();

		switch (MainDifficulty)//choose wordset based on selected difficulty
		{
		case 1:
			while (!ValidWord && Tries < 10000)//after 10000 tries the random number generator has definately used all 5 words
			{
				Tries++;
				RandNum = rand() % 6;//change this if you add words to a difficulty TODO setup true random num gen
				Word = EasyWords[RandNum];
				if (!IsEasyWordUsed[Word])
				{
					BCGame.SetHiddenWord(Word);
					IsEasyWordUsed[Word] = true;
					ValidWord = true;
				}
			}
			if (Tries == 10000)//if your out of words
			{
				DifficultiesExhausted[0] = 1;
				BCGame.SetHiddenWord("NoEasy");
			}
			break;
		case 2:
			while (!ValidWord && Tries <= 10000)
			{
				Tries++;
				RandNum = rand() % 6;
				Word = MediumWords[RandNum];
				if (!IsMediumWordUsed[Word])
				{
					BCGame.SetHiddenWord(Word);
					IsMediumWordUsed[Word] = true;
					ValidWord = true;
				}
			}
			if (Tries == 10000)
			{
				DifficultiesExhausted[1] = 2;
				BCGame.SetHiddenWord("NoMedium");
			}
			break;
		case 3:
			while (!ValidWord && Tries < 10000)
			{
				Tries++;
				RandNum = rand() % 6;
				Word = HardWords[RandNum];
				if (!IsHardWordUsed[Word])
				{
					BCGame.SetHiddenWord(Word);
					IsHardWordUsed[Word] = true;
					ValidWord = true;
				}
			}
			if (Tries == 10000)
			{
				DifficultiesExhausted[2] = 3;
				BCGame.SetHiddenWord("NoHard");
			}
			break;
		}
	}
	else//if your all outa words
	{
		BCGame.SetHiddenWord("none left");
	}
	return;
}



//loops through AllWords list and checks if any haven't been used
bool AreWordsLeft()
{
	bool AreWordsLeft;

	for (int i = 0; i <= NumOfWords; i++)
	{
		if (!IsEasyWordUsed[AllWords[i]] || !IsMediumWordUsed[AllWords[i]] || !IsHardWordUsed[AllWords[i]])
		{
			AreWordsLeft = true;
		}

	}
	return AreWordsLeft;
}



//loop until user inputs valid guess
FText GetValidGuess() 
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid;
	do
	{
		//tell user current try out of max tries
		std::cout << "Try " << BCGame.GetCurrentTry() << " of ";
		std::cout << BCGame.GetMaxTries() << ". Enter your guess: ";
		// get a guess from the player
		std::getline(std::cin, Guess);

		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "please use lowercase only.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "please enter an isogram.\n";
			break;
		default:
			//assumes guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK);
	return Guess;
}



bool AskToPlayAgain()
{
	FText Response = "";
	bool WantsToContinue;
	if (BCGame.GetHiddenWordLength() == 9)
	{
		std::getline(std::cin, Response);
		return false;
	}

	std::cout << "Do you want to play again (y/n)? ";
	std::getline(std::cin, Response);
	std::cout << "\n\n";

	if ( (Response[0] == 'y') || (Response[0] == 'Y') )
	{
		WantsToContinue = true;
	}
	else if ((Response[0] == 'n') || (Response[0] == 'N'))
	{
		WantsToContinue = false;
	}

	return WantsToContinue;
}



void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "	Congratz!! You won!! \n";
	}
	else if(BCGame.GetHiddenWordLength() != 9 && !BCGame.IsGameWon())
	{
		std::cout << "	Sorry!! You lost. Better luck next time.\n";
	}
	else
	{
		std::cout << "  Nice job. You guessed all the words i have!!";
	}
	return;
}



//asks user for a difficulty looping until a valid difficulty is given 
int32 AskDifficulty()
{
	bool ValidResponse = false;
	FText Response = "";
	int32 Difficulty;
	std::cout << "What difficulty would you like?\n" << "Enter 1 for easy, 2 for medium, and 3 for hard.\n";
	while (!ValidResponse)
	{
		std::getline(std::cin, Response);
		if (Response == "1" && DifficultiesExhausted[0] != 1)
		{
			Difficulty = 1;
			ValidResponse = true;
		}
		else if (Response == "2" && DifficultiesExhausted[1] != 2)
		{
			Difficulty = 2;
			ValidResponse = true;
		}
		else if (Response == "3" && DifficultiesExhausted[2] != 3)
		{
			Difficulty = 3;
			ValidResponse = true;

		}
		else
		{
			std::cout << "Please enter a difficulty that isn't ";
			if (DifficultiesExhausted[0] == 1)
			{
				std::cout << "1, ";
			}
			else if (DifficultiesExhausted[1] == 2)
			{
				std::cout << "2, ";
			}
			else if (DifficultiesExhausted[2] == 3)
			{
				std::cout << "3, ";
			}
			std::cout << std::endl;
		}
	}
	
	return Difficulty;
}
