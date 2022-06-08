#include "BinaryAnalyzer.h"

binary_analyzer::SerialCreator::SerialCreator(std::string& file_location_) : file_location(file_location_)
{
}

std::string binary_analyzer::SerialCreator::getLastSerial()
{
	std::string last;
	if (!checkIfFileExists(std::filesystem::current_path().u8string() + folderBase + fileLast))
	{
		return "File is missing";
	}
	std::ifstream lastFile(std::filesystem::current_path().u8string() + folderBase + fileLast);
	std::getline(lastFile, last);
	return last;
}

bool binary_analyzer::SerialCreator::addToBase(const std::string& str)
{
	std::ofstream ofs(std::filesystem::current_path().u8string() + folderBase + fileBase, std::ios::app);
	ofs << str << std::endl;
	ofs.close();
	return true;
}

bool binary_analyzer::SerialCreator::addtoLast(const std::string& str)
{
	std::ofstream ofs(std::filesystem::current_path().u8string() + folderBase + fileLast, std::ofstream::trunc);
	ofs << str;
	ofs.close();
	return true;
}

std::string binary_analyzer::SerialCreator::createSerial(std::string& last)
{
	last = to_format(std::stoi(last) + 1);
	std::cout << "Serial number for the device " << last << std::endl;
	if (!addToBase(last)) { return ""; };
	if (!addtoLast(last)) { return ""; };
	return last;
}

std::vector<std::string> binary_analyzer::SerialCreator::baseList()
{
    // Get base file serials
    std::ifstream input(std::filesystem::current_path().u8string() + folderBase + fileBase);
    std::vector<std::string> result;
    for (std::string line; getline(input, line); )
    {
        result.push_back(line);
    }
    return result;
}

bool binary_analyzer::SerialCreator::checkIfFileExists(const std::string& path)
{
	std::ifstream f(path.c_str());
	return f.good();
}

std::string binary_analyzer::SerialCreator::to_format(const int number)
{
	std::stringstream ss;
	ss << std::setw(9) << std::setfill('0') << number;
	return ss.str();
}

binary_analyzer::BinaryAnalyzer::BinaryAnalyzer(std::string& file_location_) : binary_file_location(file_location_) {

}

bool binary_analyzer::BinaryAnalyzer::getBinaryFile()
{
    std::ifstream input(binary_file_location, std::ios::binary);
    input.unsetf(std::ios::skipws);
    input.seekg(0, std::ios::end);
    buffer.reserve(input.tellg());
    input.seekg(0, std::ios::beg);
    buffer.insert(buffer.begin(), std::istream_iterator<BYTE>(input),
        std::istream_iterator<BYTE>());
	return true;
}

void binary_analyzer::BinaryAnalyzer::badCharHeuristic(std::string str, int size, int badchar[NO_OF_CHARS])
{
	int i;

	// Initialize all occurrences as -1
	for (i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;

	// Fill the actual value of last occurrence
	// of a character
	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}

int binary_analyzer::BinaryAnalyzer::search(std::vector<unsigned char> txt, std::string pat)
{
    int m = pat.size();
    int n = txt.size();

    int badchar[NO_OF_CHARS];

    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(pat, m, badchar);

    int s = 0; // s is shift of the pattern with
                // respect to text
    int result = 0;
    while (s <= (n - m))
    {
        int j = m - 1;

        /* Keep reducing index j of pattern while
        characters of pattern and text are
        matching at this shift s */
        while (j >= 0 && pat[j] == txt[s + j])
            j--;

        /* If the pattern is present at current
        shift, then index j will become -1 after
        the above loop */
        if (j < 0)
        {
            std::cout << "pattern occurs at shift = " << s << std::endl;
            result = s;
            /* Shift the pattern so that the next
            character in text aligns with the last
            occurrence of it in pattern.
            The condition s+m < n is necessary for
            the case when pattern occurs at the end
            of text */
            s += (s + m < n) ? m - badchar[txt[s + m]] : 1;

        }

        else
            /* Shift the pattern so that the bad character
            in text aligns with the last occurrence of
            it in pattern. The max function is used to
            make sure that we get a positive shift.
            We may get a negative shift if the last
            occurrence of bad character in pattern
            is on the right side of the current
            character. */
            s += max(1, j - badchar[txt[s + j]]);
    }
    return result;
}

int binary_analyzer::BinaryAnalyzer::findPattern(std::string& primaryPatter, const std::vector<std::string>& backupList)
{
    // Search for all database in base.txt file;
    int index = search(buffer, primaryPatter);
    if (index != 0 && index < 61000) { return index; };

    index = search(buffer, defaultPattern);
    if (index != 0 && index < 61000) { return index; };

        for (auto i = backupList.begin(); i < backupList.end(); i++)
        {
            index = search(buffer, *i);
            if (index != 0 && index < 61000) {
                primaryPatter = *i;
                break; }
        }

	//if not found return 0
    return index;
}

bool binary_analyzer::BinaryAnalyzer::pushSerial(const int& index ,std::string& pattern)
{
    int size = pattern.size();
    for (int i = 0; i < size;i++)
    {
        buffer[i + index] = pattern[i];
    }
    std::size_t size_buffer = buffer.size();
    auto myfile = std::fstream(binary_file_location, std::ios::out | std::ios::binary | std::fstream::trunc);
    myfile.write((char*)&buffer[0], size_buffer);
    myfile.close();
	return false;
} 
