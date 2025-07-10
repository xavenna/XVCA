#ifndef FILE_UTILS_H
#define FILE_UTILS_H


bool isValidDriveName(std::string);
bool fileExists(std::string);
void loadContentsOfFileToString(std::string, std::string&);
size_t binFileSize(std::ifstream& in);
bool isBinFile(std::string fname);
bool isPrintable(char s);

#endif
