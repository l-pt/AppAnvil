#include "database_mock.h"

// Copied from the real Database
uint DatabaseMock::get_number_processes(const std::string &profile)
{
  auto pid_map = process_data.find(profile);

  if (pid_map == process_data.end()) {
    // No data was found, so there are no active processes for this profile
    return 0;
  }

  return pid_map->second.size();
}

uint DatabaseMock::get_number_logs(const std::string &profile)
{
  auto pid_map = log_data.find(profile);

  if (pid_map == log_data.end()) {
    // No data was found, so there are no active processes for this profile
    return 0;
  }

  return pid_map->second.size();
}