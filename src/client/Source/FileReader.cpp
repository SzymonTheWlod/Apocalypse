#include "client/Headers/FileReader.h"
#include <Engine/FileIO.h>
#include <fstream>
#include <sstream>

bool FileReader::readJsonFile(std::string file_path, nlohmann::json& json)
{
  nlohmann::json json_file;
  std::stringstream s_stream;
  ASGE::FILEIO::IOBuffer buffer;
  ASGE::FILEIO::File file;
  if (!file.open(file_path, ASGE::FILEIO::File::IOMode::READ))
  {
    return false;
  }
  buffer = file.read();

  file.close();

  std::string file_string(buffer.as_char(), buffer.length);
  s_stream << file_string;
  s_stream >> json_file;

  json = json_file;
  return true;
}

void FileReader::writeToJsonFile(std::string file_path, nlohmann::json json)
{
  ASGE::FILEIO::File file;

  file.open(file_path, ASGE::FILEIO::File::IOMode::WRITE);

  ASGE::FILEIO::IOBuffer buffer;

  std::string file_string = json.dump();
  buffer.append(file_string.data(), file_string.size());

  file.write(buffer);
  file.close();
}