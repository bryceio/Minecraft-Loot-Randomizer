#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <filesystem>
#include <new>
#include <random>
#include <ctime>

namespace fs = std::filesystem;

int main() {
	std::cout << "Make sure all your files are located within a folder called loot_tables before continuing." << std::endl;
	std::cout << "WARNING: This process will overwrite the old loot tables. Are you sure you want to continue." << std::endl;
	bool confirmation = false;
	char response;
	do {
		std::cout << "Y/N?\n";
		std::cin >> response;
		if (response == 'Y' || response == 'y') confirmation = true;
		if (response == 'N' || response == 'n') {
			std::cout << "Randomization process canceled." << std::endl;
			return 1;
		}
	} while (!confirmation);
	std::cout << "\nWhat is the datapack format number you would like to use? Please check the MC Wiki if you are unsure." << std::endl;
	int packFormat;
	std::cin >> packFormat;
	std::cout << "\nBeginning randomization process." << std::endl;
	for (int i = 0; i <= time(nullptr) % 42069; i++) rand();
	int i = 0;
	std::ifstream inputStream;
	std::ofstream outputStream;
	std::cout << "Counting files." << std::endl;
	for (const auto& dirEntry : fs::recursive_directory_iterator("loot_tables")) {
		if (!dirEntry.is_directory()) i++;
	}
	std::string* fileNames = new std::string[i];
	std::string* fileContent = new std::string[i];
	std::string currentLine;
	i = 0;
	std::cout << "Reading files." << std::endl;
	for (const auto& dirEntry : fs::recursive_directory_iterator("loot_tables")) {
		if (!dirEntry.is_directory()) {
			fileNames[i] = dirEntry.path().string();
			inputStream.open(fileNames[i]);
			getline(inputStream, currentLine);
			fileContent[i] = currentLine;
			do {
				getline(inputStream, currentLine);
				fileContent[i] += '\n' + currentLine;
			} while (!inputStream.eof());
			i++;
			inputStream.close();
		}
	}
	bool badChoice;
	int choice;
	std::cout << "Randomizing files." << std::endl;
	for (int x = 0; x < i; x++) {
		outputStream.open(fileNames[x]);
		badChoice = true;
		do {
			choice = rand() % i;
			if (rand() % 1000 <= 5) std::cout << '.';
			if (fileContent[choice] != "&") badChoice = false;
		} while (badChoice);
		outputStream << fileContent[choice];
		outputStream.close();
		outputStream.clear();
		fileContent[choice] = "&";
	}
	std::cout << "\nRandomization Complete!" << std::endl;
	delete[] fileNames;
	delete[] fileContent;
	outputStream.open("pack.mcmeta");
	outputStream << "{\n   \"pack\" : {\n      \"pack_format\": " << packFormat << ",\n";
	outputStream << "      \"description\": \"A randomized loot table pack made using bryceio\'s randomizer.\"\n   }\n}" << std::endl;
	return 0;
}