package listem;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class myLineCounter extends mySuperClass implements LineCounter {

	@Override
	public Map<File, Integer> countLines(File directory,String fileSelectionPattern, boolean recursive) {
		Map<File, Integer> results = new HashMap<File, Integer>();
		
		Pattern file_pattern = Pattern.compile(fileSelectionPattern);
		
		Matcher file_matcher = null;

		// find any files in the current directory
		// directory.list()
		File[] file_list = directory.listFiles();
		for (File this_file : file_list) {
			if (this_file.isFile()) {
				// check each file to see if it fits the file_pattern
				if (file_matcher == null)
					file_matcher = file_pattern.matcher(this_file.getName());
				else
					file_matcher.reset(this_file.getName());

				// open each file with the correct selection pattern
				if (file_matcher.matches()){
					int line_count = countFileLines(this_file);
					
					// Makes sure to not add file names that have no found lines
					if (line_count > 0)
						results.put(this_file, line_count);
				}
			}
			// is a directory
			else{
				if (recursive == true){
					results.putAll(countLines(new File(this_file.getPath()), fileSelectionPattern, recursive));
				}
			}
		}

		return results;
	}
	
	private int countFileLines(File this_file){
		// Create new list of found_lines for this particular file
		int line_count = 0;
		// search each file that gets opened for the substring
		File cur_file = new File(this_file.getPath());
		Scanner scanner = null;
		try {
			scanner = new Scanner(cur_file);
			while(scanner.hasNextLine()){
				scanner.nextLine();
				line_count++;
			}
		} 
		catch (FileNotFoundException e) {
			System.out.println("File not found: " + e);
		}
		finally {
			if (scanner != null)
				scanner.close();
		}
		return line_count;
	}
}





