/*
 *  Short script to separate the two large .xyz files from the tmQm dataset, 
 *  including approx. 40,000 xyz files each, into separate files.
 * 
 *  Original dataset: https://github.com/bbskjelstad/tmqm
 * 
 *  For use: locate the xyz files (tmQM_X1.xyz and tmQM_X2.xyz) in the directory raw/
 *  and create a directory (here processed/) to save the split files.
 */
#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <iterator>
#include <vector>

int main()
{
    auto INPUT_DIR = "./raw/";
    auto OUTPUT_DIR = "./processed/";
    const int NUM_FILES = 2;
    std::array<std::string, NUM_FILES> file_names = {"tmQM_X1.xyz", "tmQM_X2.xyz"};
    int counter = 1;

    // Position in line where CSD number is located
    const int CSD_START_IDX = 11;
    const int CSD_END_IDX = 6;

    for (int i = 0; i < NUM_FILES; i++)
    {
        // Get filepath and read file
        std::string input_path = INPUT_DIR + file_names[i];
        std::cout << input_path << std::endl;
        std::ifstream infile(input_path);
        std::string csd_code;

        // Read file line by line
        std::string line = "placeholder";
        std::string previous_line = "placeholder";
        std::vector<std::string> split_file_content;

        while (std::getline(infile, line))
        {
            // Empty line separates files
            if (line.empty())
            {
                std::ofstream output_file(OUTPUT_DIR + csd_code + ".xyz");
                std::ostream_iterator<std::string> output_iterator(output_file, "\n");
                std::copy(split_file_content.begin(), split_file_content.end(), output_iterator);
                split_file_content.clear();
            }
            else
            {
                split_file_content.push_back(line);
            }

            // Get CSD code
            if (line.rfind("CSD_code = ", 0) == 0)
            {
                csd_code = line.substr(CSD_START_IDX, CSD_END_IDX);
                if (csd_code[0] == ' ' || csd_code.back() == ' ')
                {
                    throw std::invalid_argument("Invalid CSD code! Line: \n" + line);
                }
                counter++;
            }
        }
    }
    std::cout << "Created " << counter << "files!" << std::endl;
}