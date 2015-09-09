When you turn in your program, include a text file called README that lists the data structures where you used synchronization and the location of these data structures in your code. This should be enough information for the TA to find the relevant sections of your code and verify that you have used memory synchronization.

I created two classes to help manage the data structures.

MessageBox.h
	-Contains a map<string, vector<pair<string, string>>> as the data structure
	-has a mutex surrounding the guts of 
		store_message(21)
		create_list_results(32)
		create_read_result(52)

ClientBuffer.h
	-Contains a vector<int> as the data structure
	-uses the producer consumer model to protect the client_buffer
	-Most of the file is the semephores