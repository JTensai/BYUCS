package spell;

import java.io.*;
import java.util.*;

import spell.Dictionary.WordNode;

public class SpellCorrectorClass implements SpellCorrector {
	static Dictionary dictionary = new Dictionary();
	
	/**
	 * Tells this <code>SpellCorrector</code> to use the given file as its dictionary
	 * for generating suggestions. 
	 * @param dictionaryFileName File containing the words to be used
	 * @throws IOException If the file cannot be read
	 */
	public void useDictionary(String dictionaryFileName) throws IOException{
		File srcFile = new File(dictionaryFileName);
		Scanner scanner = new Scanner(srcFile);
		scanner.useDelimiter("[^a-zA-Z^]+");
		dictionary = new Dictionary();
//		Dictionary newdict = new Dictionary();
		
		while(scanner.hasNext()){
			String text = scanner.next();
			dictionary.add(text);
//			newdict.add(text);			
		}
		scanner.close();
		
		System.out.println(dictionary.toString());
//		dictionary.equals(newdict);
	}

		
	/**
	 * Suggest a word from the dictionary that most closely matches
	 * <code>inputWord</code>
	 * @param inputWord
	 * @return The suggestion
	 * @throws NoSimilarWordFoundException If no similar word is in the dictionary
	 */
	public String suggestSimilarWord(String inputWord) throws NoSimilarWordFoundException{
		inputWord = inputWord.toLowerCase();
		WordNode word = dictionary.find(inputWord);
		if (word == null){
			String best_suggestion = null;
			Set<String> possible_words = new TreeSet<String>();
			deletionDistance(inputWord, possible_words);
			transpositionDistance(inputWord, possible_words);
			alterationDistance(inputWord, possible_words);
			insertionDistance(inputWord, possible_words);
			
			Set<String> found_words = new TreeSet<String>();
			for (String test_word : possible_words){
				WordNode word_check = dictionary.find(test_word);
				if (word_check != null){
					found_words.add(test_word);
				}
			}
			
			if (found_words.size() == 0){
				Set<String> possible_words2 = new TreeSet<String>();
				for (String test_word : possible_words){
					deletionDistance(test_word, possible_words2);
					transpositionDistance(test_word, possible_words2);
					alterationDistance(test_word, possible_words2);
					insertionDistance(test_word, possible_words2);					
				}
				for (String test_word : possible_words2){
					WordNode word_check = dictionary.find(test_word);
					if (word_check != null){
						found_words.add(test_word);
					}
				}
			}
			WordNode best_node = null;
			WordNode test_node = null;
			for (String test_word : found_words){
				if (best_node == null){
					best_node = dictionary.find(test_word);
					best_suggestion = test_word;
				}
				else{
					test_node = dictionary.find(test_word);
					// check if test_word is in dictionary
					if (test_node != null){
						// check if test_word is found more times than best_suggestion (accounts for alphabetical)
						if (test_node.getValue() > best_node.getValue()){			
							best_node = test_node;
							best_suggestion = test_word;
						}
					}
				}
			}
			if (best_suggestion == null){
//				System.out.println("No words found");
				throw new NoSimilarWordFoundException();				
			}
			
			return best_suggestion;
		}
		// word found in dictionary, return it
		else
			return inputWord;				
	}
	
	private void deletionDistance(String inputWord, Set<String> possible_words){
		for (int i = 0; i < inputWord.length(); i++){
			String test_string = inputWord.substring(0, i) + inputWord.substring(i+1);
			if (test_string.length() > 0)
				possible_words.add(test_string);
		}
	}
	private void transpositionDistance(String inputWord, Set<String> possible_words){
		char[] charArray = inputWord.toCharArray();
		char[] newCharArray = new char[inputWord.length()];
		//first loop runs enough times to account for correct number of needed swaps
		for (int i = 0; i < inputWord.length()-1; i++){
			//second loop goes through each letter of a word
			for (int j = 0; j < inputWord.length(); j++){
				if (j == i && i+1 < inputWord.length()){
					char letter = charArray[i];
					newCharArray[i] = charArray[i+1];
					newCharArray[i+1] = letter;
					j++;
				}
				else{
					newCharArray[j] = charArray[j];
				}
			}
			String new_word = new String(newCharArray);
			possible_words.add(new_word);
		}
	}
	private void alterationDistance(String inputWord, Set<String> possible_words){
		char[] charArray = inputWord.toCharArray();
		//first loop runs through each letter of inputWord
		for (int i = 0; i < inputWord.length(); i++){
			char[] newCharArray = inputWord.toCharArray();
			//second loop swaps out that letter for each letter of the alphabet
			for (int j = 0; j < 26; j++){
				char new_char = (char) ((char)j + 'a');
				if (new_char != charArray[i]){
					newCharArray[i] = new_char;
					String new_word = new String(newCharArray);
//					System.out.println(i + " : " + new_word);
					possible_words.add(new_word);
				}
			}
		}		
	}
	private void insertionDistance(String inputWord, Set<String> possible_words){
		char[] charArray = inputWord.toCharArray();
		char[] newCharArray = new char[inputWord.length()+1];
		//first loop runs through position of expanded char array
		for (int i = 0; i < newCharArray.length; i++){
			//second loop fills the array with original word, leaving a blank space for the later loop to fill
			for (int j = 0; j < inputWord.length()+1; j++){
				if (j < i)
					newCharArray[j] = charArray[j];
				if (j > i)
					newCharArray[j] = charArray[j-1];
				if (j == i)
					newCharArray[j] = ' ';
			}
			//add each letter of the alphabet in the blank space left by previous loop
			for (int k = 0; k < 26; k++){
				char new_char = (char) ((char)k + 'a');
				newCharArray[i] = new_char;
				String new_word = new String(newCharArray);
//				System.out.println(i + " : " + new_word);
				possible_words.add(new_word);
			}
		}		
		
	}

}
