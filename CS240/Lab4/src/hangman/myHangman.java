package hangman;

import hangman.EvilHangmanGame.GuessAlreadyMadeException;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Set;
import java.util.TreeSet;

public class myHangman {	
	public static void main(String[] args) {
		File srcFile = new File(args[0]);
		int wordLength = Integer.parseInt(args[1]);
		int guesses = Integer.parseInt(args[2]);
		if (wordLength >= 2 && guesses >= 1){		
			myEvilHangmanGame game = new myEvilHangmanGame();						
			game.startGame(srcFile, wordLength);
			Set<String> winning_word = null;
			while (guesses > 0 && !game.checkIfWinner()){
				System.out.println("You have " + guesses + " guesses left");
				System.out.println("Used letters: " + game.getGuesses().toString());
				System.out.println("Word: " + game.getWordSoFar());				

				String input = "";
				while (input.length() != 1 || !input.matches("[a-zA-Z]+")){	
					BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
					System.out.print("Enter guess: ");
					try {
						input = br.readLine().toLowerCase();
					} 
					catch (IOException e) {
						e.printStackTrace();
					}
					if (input.length() != 1 || !input.matches("[a-zA-Z]+"))
						System.out.println("Invalid input, try again");
				}
				char guess = input.charAt(0);
				try {
					winning_word = game.makeGuess(guess);
					guesses--;
				}
				catch (GuessAlreadyMadeException e) {
					System.out.println("\nThat guess has already been made.");
				}
			}
			
			if (game.checkIfWinner())
				System.out.println("Congratulation! You Win: " + ((TreeSet<String>) winning_word).first());
			else
				System.out.println("You Lose!\nThe word was: " + game.giveAnswer());
		}
	}
}
