package spell;

public class Dictionary implements Trie {
	public WordNode root = new WordNode();
	int totalWordCount = 0;
	int totalNodeCount = 1;
	
	/**
	 * Adds the specified word to the trie (if necessary) and increments the word's frequency count
	 * 
	 * @param word The word being added to the trie
	 */
	public void add(String word) {
		word = word.toLowerCase();
		root.addWord(word);
	}
	
	/**
	 * Searches the trie for the specified word
	 * 
	 * @param word The word being searched for
	 * 
	 * @return A reference to the trie node that represents the word,
	 * 			or null if the word is not in the trie
	 */
	public WordNode find(String word) {
		return rec_find(word.toLowerCase(), root);
	}

	private WordNode rec_find(String word, WordNode node){
		if (word.length() > 0){
			int letter_position = word.charAt(0) - 'a';
			if (node.letters[letter_position] == null){
				return null;
			}
			WordNode sub_node = node.letters[letter_position];
			if (word.length() == 1){
				if (sub_node.getValue() > 0)
					return sub_node;
				else
					return null;
			}
			else
				return rec_find(word.substring(1), sub_node);
		}
		return null;
	}
	
	/**
	 * Returns the number of unique words in the trie
	 * 
	 * @return The number of unique words in the trie
	 */
	public int getWordCount() {
		return totalWordCount;
	}
	
	/**
	 * Returns the number of nodes in the trie
	 * 
	 * @return The number of nodes in the trie
	 */
	public int getNodeCount() {
		return totalNodeCount;
	}
	
	/**
	 * The toString specification is as follows:
	 * For each word, in alphabetical order:
	 * <word> <count>\n
	 */
	@Override
	public String toString(){
		StringBuilder builder = new StringBuilder();
		rec_toString(root, "", builder);

    return builder.toString();
		
	}
	public StringBuilder rec_toString(WordNode node, String word_so_far, StringBuilder builder){
		if (node.count > 0){
			builder.append(word_so_far + " " + node.count + "\n");
		}
		for (int i = 0; i < 26; i++){
			if (node.letters[i] == null){}
			else{
				char new_letter = (char) (i + 'a');
				rec_toString(node.letters[i], word_so_far + new_letter, builder);
			}
		}
		return builder;
	}
	
	@Override
	public int hashCode(){
		// uses totalwordcount and nodecount and then 
		return getWordCount() * getNodeCount();
		
	}
	
	@Override
	public boolean equals(Object o){
		if (o == null){
			return false;
		}
		
		if (this == o){
//			System.out.println("test");
			return true;
		}
		
		//check if same itemtype
		if (getClass() != o.getClass()){
//			System.out.println("itemtype");
			return false;
		}
			
		//check if totalWordCount is equal
		if (getWordCount() != ((Trie) o).getWordCount()){
//			System.out.println("wordcount");
			return false;
		}

		//check if totalNodeCount is equal
		if (getNodeCount() != ((Trie) o).getNodeCount()){
//			System.out.println("nodecount");
			return false;
		}
		
		if (hashCode() != o.hashCode()){
//			System.out.println("hashcode");
			return false;
		}
		//Check each node in the trie and compare them
				
		return root.equals(((Dictionary) o).getRoot());		
	}
	
	private WordNode getRoot(){
		return root;
	}

	public class WordNode implements Trie.Node {
		int count = 0;
		WordNode[] letters = new WordNode[26];

		private void addWord(String word){
			if (word.length() > 0){
				int letter_position = word.charAt(0) - 'a';
				if (letters[letter_position] == null){
					letters[letter_position] = new WordNode();
					totalNodeCount++;
				}
				letters[letter_position].addWord(word.substring(1));
			}
			if (word.length() == 0){
				if (count == 0)
					totalWordCount++;
				count++;
			}
			return;
		}

		/**
		 * Returns the frequency count for the word represented by the node
		 */
		public int getValue() {
			return count;
		}
		
		public boolean equals(WordNode other){
			// check null
			if (other == null)
				return false;
			
			// check count
			if (count != other.getValue())
				return false;
			
			// check children
			for (int i = 0; i < 26; i++){
				if (letters[i] == null){
					if (other.letters[i] != null)
						return false;
				}
				else{
					if (!letters[i].equals(other.letters[i]))
						return false;
				}
			}
//			System.out.println("THEY ARE THE SAME!!!!!");
			return true;
		}
	}
}
