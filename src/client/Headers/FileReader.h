#ifndef PROJECT_FILEREADER_H
#define PROJECT_FILEREADER_H

#include <json.hpp>
#include <string>

class FileReader
{
 public:
  FileReader() = default;
  ~FileReader() = default;

  bool readJsonFile(std::string file_path, nlohmann::json& json);
  void writeToJsonFile(std::string file_path, nlohmann::json json);

 private:
};

#endif // PROJECT_FILEREADER_H
