#ifndef SRC_THREADS_COMMAND_CALLER
#define SRC_THREADS_COMMAND_CALLER

#include <string>
#include <vector>

class CommandCaller{
  public:
    /**
     * @brief Return the output of `aa-status --json`
     * 
     * @details
     * Returns the output of `pkexec aa-status --json` to get a list of profiles and processes confined by apparmor. 
     * 
     * @returns std::string the raw output of aa-status
     */
    static std::string get_status_str();

    /**
     * @brief Return the output of `dmesg`
     * 
     * @details
     * Returns the output of `dmesg` to get a list of system logs. 
     * 
     * @returns std::string the raw output of `dmesg`
     */
    static std::string get_logs_str();

    /**
     * @brief Return the output of `aa-unconfined`
     * 
     * @details
     * Returns the output of `pkexec aa-unconfined` to get a list of processes not confined by apparmor. 
     * 
     * @returns std::string the raw output of aa-unconfined
     */
    static std::string get_unconfined();


    /*
      loadprofile
    */
    static std::string load_profile(std::string fullFileName);

    static std::string disable_profile(std::string profileName);

  protected:
    struct results{
      int exit_status;
      std::string output;
      std::string error;
    };

    // Used to call command-line commands from `/usr/sbin` 
    static results call_command(std::vector<std::string> command);
    static std::string call_command(std::vector<std::string> command, std::string return_on_error);
};

#endif // COMMAND_CALLER_H
