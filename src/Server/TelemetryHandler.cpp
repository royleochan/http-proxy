#include "TelemetryHandler.h"

TelemetryHandler::TelemetryHandler(std::string url, size_t size) : url(url), size(size)
{
}

TelemetryHandler::~TelemetryHandler()
{
  delete this;
}

int TelemetryHandler::getSize()
{
  return size;
}

void TelemetryHandler::setTerminate()
{
  terminate = true;
}

void TelemetryHandler::run(std::unordered_map<std::string, TelemetryHandler *> &telemetryState)
{
  sleep(3);
  if (!terminate)
  {
    printf("%s, %zu\n", url.c_str(), size);
    telemetryState.erase(url);
  }
}
