#ifndef PROJECT_PRINTER_H
#define PROJECT_PRINTER_H

#include <mutex>
#include <sstream>

namespace GameLib
{
class Printer : public std::ostringstream
{
 public:
  Printer() = default;
  ~Printer();

  static std::mutex cout_mtx;

 private:
};
}
#endif // PROJECT_PRINTER_H
