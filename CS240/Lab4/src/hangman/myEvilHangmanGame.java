package hangman;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;

public class myEvilHangmanGame implements EvilHangmanGame {
	private Set<String> dictionary;
	private Set<Character> used_letters;
	private Set<String> remaining_words;
	private char[] word_so_far;
	
	public void startGame(File dict_file, int wordLength) {
		dictionary = new TreeSet<String>();
		used_letters = new TreeSet<Character>();
		remaining_words = new TreeSet<String>();
		word_so_far = new char[wordLength];
		
		for (int i = 0; i < wordLength; i++){
	    	word_so_far[i] = '-';
		}
			
		Scanner s = null;
		try {
			s = new Scanner(dict_file);
			while(s.hasNext()){
				String next_word = s.next();
				if (next_word.length() == wordLength){
					dictionary.add(next_word);
					remaining_words.add(next_word);
				}
			}
//			System.out.println(dictionary.toString());
						
		}
		catch (FileNotFoundException e) {
			System.out.println("File not found: " + e);
			e.printStackTrace();
		}
		finally{
			if (s != null)
				s.close();
		}
	}

	public Set<String> makeGuess(char guess) throws GuessAlreadyMadeException {
		if (used_letters.contains(guess))
			throw new GuessAlreadyMadeException();

		used_letters.add(guess);
		Map<String, WordFamily> word_families = new HashMap<String, WordFamily>();
		
		for (String word : remaining_words){
			char[] current_family_pattern = new char[word.length()];
			
			for (int i = 0; i < word.length(); i++ ){
			    if(word.charAt(i) == guess){
			        current_family_pattern[i] = guess;
			    }
			    else{
			    	current_family_pattern[i] = '-';
			    }
			}
	    	StringBuilder builder = new StringBuilder();
			for (int j = 0; j < word_so_far.length; j++){
				builder.append(current_family_pattern[j]);
			}
			String family_pattern = builder.toString();
				
			WordFamily current_family = word_families.get(family_pattern);
			if (current_family == null){
				WordFamily family_words = new WordFamily();
				family_words.addWord(word);
				family_words.setPattern(current_family_pattern);
				word_families.put(family_pattern, family_words);	
			}
			else{
				current_family.addWord(word);
			}
		}
		// HANDLE COLLISIONS
		WordFamily most_common = null;
		int most_common_letter_count = 0;
		TreeSet<Integer> most_common_letter_positions = new TreeSet<Integer>();
		
		for (WordFamily cur_family : word_families.values()){
//			System.out.println(cur_family.toString());
			if (most_common == null){
				most_common = cur_family;
				for (int i = 0; i < word_so_far.length; i++){
					if (most_common.getPattern()[i] != '-'){
						most_common_letter_count++;
						most_common_letter_positions.add(i);
					}
				}
			}
			else{
				int cur_family_letter_count = 0;
				TreeSet<Integer> cur_family_letter_positions = new TreeSet<Integer>();
				for (int i = 0; i < word_so_far.length; i++){
					if (cur_family.getPattern()[i] != '-'){
						cur_family_letter_count++;
						cur_family_letter_positions.add(i);
					}						
				}
				if (cur_family.getCount() > most_common.getCount()){
					most_common = cur_family;
					most_common_letter_count = cur_family_letter_count;
					most_common_letter_positions = cur_family_letter_positions;					
				}
				else if (cur_family.getCount() == most_common.getCount()){
					// current family object is either more common or equally as common
					// need to check if one of them is the "-----" string and if it is, then make it the most common
					if (most_common_letter_count == 0){
						// most_common is the empty pattern and should be left as the most common						
					}
					else{						
						if (cur_family_letter_count < most_common_letter_count){
							// cur_family is the pattern with less letters so set most_common to it
							most_common = cur_family;
							most_common_letter_count = cur_family_letter_count;
							most_common_letter_positions = cur_family_letter_positions;
						}
						else if (cur_family_letter_count == most_common_letter_count){
							// Same number of letters found in each pattern, continue validations
							Iterator<Integer> cf = cur_family_letter_positions.descendingIterator();
							Iterator<Integer> mc = most_common_letter_positions.descendingIterator();
							for (int j = word_so_far.length - 1; j > 0; j--){
								if (cf.hasNext() && mc.hasNext()){
									int cf_next = cf.next();
									int mc_next = mc.next();
//									System.out.println("mc: " + mc_next + " / cf: " + cf_next);
									if (cf_next > mc_next){
										most_common = cur_family;
										most_common_letter_count = cur_family_letter_count;
										most_common_letter_positions = cur_family_letter_positions;
									}
								}
							}
						}
						else {
							// cur_family_letter_count has more letters, leave most_common as is.
						}
					}
				}
			}
		}
		System.out.println(most_common.guessResult(guess));
//		System.out.println(most_common.toString());
		
		remaining_words = most_common.getWords();
		
		for (int i = 0; i < word_so_far.length; i++ ){
	        if (most_common.getPattern()[i] != '-')
	        	word_so_far[i] = most_common.getPattern()[i];
		}
		
		
		return most_common.getWords();
	}
	
	public Set<String> getDictionary(){
		return dictionary;
	}
	
	public Set<Character> getGuesses(){
		return used_letters;
	}
	
	
	public String getWordSoFar(){
		StringBuilder builder = new StringBuilder();
		for (int i = 0; i < word_so_far.length; i++){
			builder.append(word_so_far[i]);
		}
		return builder.toString();
	}
		
	public String giveAnswer(){		
		return ((TreeSet<String>) remaining_words).first();
	}
	
	public boolean checkIfWinner(){
		for (int i = 0; i < word_so_far.length; i++){
			if (word_so_far[i] == '-')
				return false;
		}
		return true;
	}
	
	public class WordFamily{
		int count = 0;
		Set<String> words = new TreeSet<String>();
		char[] pattern = null;
		
		private int getCount(){
			return count;
		}
		private Set<String> getWords(){
			return words;
		}
		private char[] getPattern(){
			return pattern;
		}
		
		private void setPattern(char[] pattern){
			this.pattern = pattern;
		}
		private void addWord(String word){
			count++;
			words.add(word);
		}
		
		private String guessResult(char guess){
			int letter_count = 0;
			for (int i = 0; i < pattern.length; i++){
				if (pattern[i] == guess)
					letter_count++;
			}			
			
			String result = null;
			if (letter_count < 1){
				result = "Sorry, there are no " + guess + "'s\n";
			}
			if (letter_count == 1){
				result = "Yes, there is " + letter_count + " " + guess + "\n";				
			}
			if (letter_count > 1){
				result = "Yes, there are " + letter_count + " " + guess + "'s\n";		
			}
			return result;
		}
		
		public String toString(){
			StringBuilder builder = new StringBuilder();
			for (int i = 0; i < pattern.length; i++){
				builder.append(pattern[i]);
			}
			builder.append(" : " + count + " [");
			for (String word : words){
				builder.append(word + ", ");
			}
			builder.append("]");
			return builder.toString();
		}
	}
}
