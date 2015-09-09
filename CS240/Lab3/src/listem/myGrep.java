package listem;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;
import java.util.regex.*;

public class myGrep extends mySuperClass implements Grep {

	@Override
	public Map<File, List<String>> grep(File directory, String fileSelectionPattern, String substringSelectionPattern, boolean recursive) {
		Map<File, List<String>> results = new HashMap<File, List<String>>();
		
		Pattern file_pattern = Pattern.compile(fileSelectionPattern);
		Pattern substring_pattern = Pattern.compile(substringSelectionPattern);
		
		Matcher file_matcher = null;
		Matcher substring_matcher = null;

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
					// Create new list of found_lines for this particular file
					List<String> found_lines = new ArrayList<String>();

					// search each file that gets opened for the substring
					File cur_file = new File(this_file.getPath());
					Scanner scanner = null;
					try {
						scanner = new Scanner(cur_file);
						while(scanner.hasNextLine()){
							String line = scanner.nextLine();
							if (substring_matcher == null)
								substring_matcher = substring_pattern.matcher(line);
							else 
								substring_matcher.reset(line);

							if (substring_matcher.find()){
								// match on line has been found
								found_lines.add(line);
							}
						}
					} 
					catch (FileNotFoundException e) {
						System.out.println("File not found: " + e);
					}
					finally {
						if (scanner != null)
							scanner.close();
					}

					// Makes sure to not add file names that have no found lines
					if (found_lines.size() > 0)
						results.put(this_file, found_lines);
				}
			}
			// is a directory
			else{
				if (recursive == true){
					results.putAll(grep(new File(this_file.getPath()), fileSelectionPattern, substringSelectionPattern, recursive));
				}
			}
		}

		return results;
	}
}