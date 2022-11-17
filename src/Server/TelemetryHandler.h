#pragma once

#include <unistd.h>
#include <string>

class TelemetryHandler
{
private:
  std::string url;
  size_t size;
  bool terminate = false;

public:
  TelemetryHandler(std::string url, size_t size);
  ~TelemetryHandler();
  int getSize();
  void setTerminate();
  void run();
};
