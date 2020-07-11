



// in order to get cwd, whether windows or otherwise (https://www.tutorialspoint.com/find-out-the-current-working-directory-in-c-cplusplus)
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif




#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>  
#include <sstream>  // for stringstream
#include <map>
#include <math.h>
#include <fstream>
#include <sstream>





using namespace std;


// splitting strings
std::vector<std::string> split_string(string s) {

	std::stringstream ss(s);

	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;

	std::vector<std::string> vstrings(begin, end);

	return vstrings;

}



// counting terms, as map object
std::map<std::string, int> term_freq(string s) {

	std::vector<std::string> x = split_string(s);

	// this is a map where the keys are strings and the values integers
	std::map<std::string, int> all;

	// loop
	for (int i = 0; i < x.size(); i++) {

		string s = x[i];

		if (all.count(s)) {
			all[s]++;
		} else {
			all[s] = int(1);
		}

	}

	return all;
}




// function to add new words with counts of zero to maps where words aren't in another map
std::map<std::string, int> add_in(std::map<std::string, int>& main, std::map<std::string, int> allwordz) {

	std::map<std::string, int>::iterator p;

	for (p = allwordz.begin(); p != allwordz.end(); ++p) {

		std::map<string, int>::iterator it = main.map::find(p->first); 

		if (it == main.end()) {
 				main.insert(std::pair<string, int>(p->first, 0));
 				//cout << out.size() << endl;
 			} 
	}


	return main;

}



// function to divide word counts in map by total word count in that map (TF)
std::map<std::string, double> word_stdiz(std::map<std::string, int> main) {  

	double counter = 0.0;
	std::map<std::string, int>::iterator p;

	for (p = main.begin(); p != main.end(); ++p) {
		counter = counter + double(p->second);
	}

	std::map<std::string, double> out;

	for (p = main.begin(); p != main.end(); ++p) {
		
		out.insert(std::pair<string, double>(p->first, (double(p->second) / counter)));

	}

	//std::map<std::string, double>::iterator q;
	//for (q = out.begin(); q != out.end(); ++q) {
	//cout << q->second << endl;
	//}	


	return out;
}





// convert map of maps to vector
vector<double> to_vec(std::map<std::string, double> main) {	

	// adding count to vector
	vector<string> vec;
	vector<double> veci;

	for(std::map<std::string, double>::iterator it = main.begin(); it != main.end(); ++it) {
  	//cout << it->first << " " << it->second << endl;
  	veci.push_back(it->second);
	}

	return veci;
}


// count strings and get IDF
std::map<string, double> count_all_strings(std::map<int, std::map<std::string, double> > main) {

	std::map<string, double> out;

	std::map<int, std::map<std::string, double> >::iterator it;
	for (it = main.begin(); it != main.end(); it++) {

		std::map<string, double>::iterator k;
		for (k = it->second.begin(); k != it->second.end(); k++) {

			std::map<string, double>::iterator iout = out.map::find(k->first);  // think ->first, returns first value in map pair
 			if (iout == out.end()) {
 				out.insert(std::pair<string, int>(k->first, 1.0));
 				//cout << out.size() << endl;
 			} else {
 				iout->second = iout->second + 1;
 			}


		}


	}


	// getting IDF and log calc
	std::map<string, double>::iterator ito;
	for (ito = out.begin(); ito != out.end(); ito++) {
		ito->second = log(main.size() / ito->second);
	}


	return out;
}




// looping through maps and applying IDF
std::map<int, std::map<std::string, double> >  applyidf(std::map<int, std::map<std::string, double> >& input, std::map<string, double> str_counts) {


	// iterate over map & look for matches
	std::map<int, std::map<std::string, double> >::iterator it;
	for (it = input.begin(); it != input.end(); it++) {

		// loop within map
		std::map<std::string, double>::iterator ito;
		for (ito = it->second.begin(); ito != it->second.end(); ito++) {

			// find count within map
			std::map<string, double>::iterator k;
			for (k = str_counts.begin(); k != str_counts.end(); k++) {
				if(ito->first == k->first) {
					ito->second = k->second + ito->second;
				}
			}



		}



	}



	return input;
}




// to find distances between two maps
double finddist(std::map<int, std::map<std::string, double> > input, const int& w1, const int& w2) {

	double out = 0.0;


		//cout << input[w1].size() << endl;

		// looping through first map to find distances
	 	std::map<string, double>::iterator it;
	 	for (it = input[w1].begin(); it != input[w1].end(); it++){


	 		std::map<string, double>::iterator ito = input[w2].map::find(it->first);
	 		if (ito == input[w2].end()) {
	 			out = out + pow(it->second, 2);
	 		//	cout << "zero" << endl;
	 		} else {
	 			out = out + pow( (it->second - ito->second), 2);
	 			//cout << "one" << endl;
	 		}

	 	}

	 	// looping through second map to find where no matches
	 	std::map<string, double>::iterator k;
	 	for (k = input[w2].begin(); k != input[w2].end(); k++){

	 		std::map<string, double>::iterator ito = input[w1].map::find(k->first);
	 		if (ito != input[w1].end()) {
	 			out = out + pow(it->second, 2);
	 			//cout << "one 2nd" << endl;
	 		}
	 	}

	return sqrt(out);

}



// find closest text vector to input position

int findclosest(const std::map<int, std::map<std::string, double> >& input, const int& w1) {

	int loc = 0;
	double dist = 99999.9;

	// looping through rows
	int i = 0; 
	while (i < input.size()) {

		if (i != w1) {
			double newdist = finddist(input, w1, i);
			if (dist > newdist) {
				loc = i;
				dist = newdist;
			}
		}
		i++;
	}

	return loc;
}






// making one long map of all unique text, with counts
vector<int> all_closest(std::map<int, string> all_in) {

	// to store results
	std::map<std::string, int> out;

	// for map
	std::map<int, std::map<std::string, int> > allmap;

	for (int i = 0; i < all_in.size(); i++) {

		std::map<std::string, int> d = term_freq(all_in[i]);

		std::map<std::string, int>::iterator p;

		for (p = d.begin(); p != d.end(); ++p) {

			// tells you if word isn't present in out already
			std::map<string, int>::iterator it = out.map::find(p->first);  // think ->first, returns first value in map pair
 			if (it == out.end()) {
 				out.insert(std::pair<string, int>(p->first, 1));
 				//cout << out.size() << endl;
 			} else {
 				it->second++;
 			}
			
		}

		// storing "d" in map to gp into array later
		allmap.insert(std::pair<int, std::map<std::string, int> >(i, d));

	}
	


	// converting to text frequencies (copies all maps so not very efficient)
	std::map<int, std::map<std::string, double> > tf_map;
	std::map<int, std::map<std::string, int> >::iterator q;

	int i = 0;
	for (q = allmap.begin(); q != allmap.end(); ++q) {
		tf_map.insert(std::pair<int, std::map<std::string, double> > (i, word_stdiz(q->second)));
		i++;
	}


	// counting all values
	std::map<string, double> str_counts = count_all_strings(tf_map);

	
	// applying IDF map
	applyidf(tf_map, str_counts);



	// finding closest text vector
	vector<int> closv;
	for (int i = 0; i < tf_map.size(); i++) {
	closv.push_back(findclosest(tf_map, i));
	}



	return closv;
}



string clean_string(string& main) {

	int i = 0;
	while (i < main.length()) {
		if (isalpha(main[i])) {
			main[i] = tolower(main[i]);
			i++;
		} else if (isspace(main[i])) {
			i++;
		} else {
			main.erase(i, 1);
		}
			
	}

	return main;

}



// function to remove non-spaces/characters, and make all uppercase
std::map<int, string> cleanstringz(std::map<int, string>& main) {

	std::map<int, string>::iterator it;

	for (it = main.begin(); it != main.end(); it++) {
		clean_string(it->second);
	}

	return main;
}



// function to get current working directory, from https://www.tutorialspoint.com/find-out-the-current-working-directory-in-c-cplusplus
std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}





// function to read .txt file
std::map<int, string> read_data(string pathin) {


   std::fstream fin(pathin); // Open in text-mode.


    // Construct your map.
    std::map<int, string> out;


	int i = 0;
	while (fin)
    {
        // read stuff from the file into a string and print it
        std::string strInput;
        std::getline(fin, strInput);
       // std::cout << strInput << '\n';

        out.insert(std::pair<int, string> (i, strInput));
        i++;
    }


    // Close the file:
    fin.close();


    // removing last value, as it'll be blank
     out.erase(prev(out.end())); 


    return out;
}




// function to write to file
void write_results(vector<int> main, string pathout) {
	
	ofstream myfile(pathout);

		for (int i = 0; i < main.size(); i++) {
            myfile << main[i] << ",";
        }

        myfile.close();
   

}





int main() {

	// viewing working directory
	cout << "Your home directory is: " << get_current_dir() << endl;
	cout << "What is the pathway to the data from here? (I ask for the file name separately" << endl;
	cout << "On Adam's mac it's: Desktop/tfidf" << endl;

	string path_files;
	cin >> path_files;



	cout << "And input file name? In testing it was: test.txt" << endl;
	string mainf;
	cin >> mainf;


	cout << "And finally: output file name? In testing it was: out.txt" << endl;
	string main_out;
	cin >> main_out;


	// concat
	string pathin = path_files + "/" + mainf;
	string pathout = path_files + "/" + main_out;

	cout << "reading from " << pathin << endl;
	cout << "will write to " << pathout << endl;



	// read in and clean data
	std::map<int, string> all_in = read_data(pathin);
	cleanstringz(all_in);

	cout << all_in.size() << " blocks of text read in and cleaned; now looking for closest text" << endl;




	// finding closest text blocks
	vector<int> ee = all_closest(all_in);
	

	// exporting
	write_results(ee, pathout);


	cout << "all done and calculations saved" << endl;


	return 0;
}
