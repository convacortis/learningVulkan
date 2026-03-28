#pragma once

#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <chrono>

//-------------------------------------------------------------------
// parameters




//-------------------------------------------------------------------
// colourrrrs

#define RESET   "\033[0m"
#define BLACK   "\033[30m"                 /* Black */
#define RED     "\033[31m"                 /* Red */
#define GREEN   "\033[32m"                 /* Green */
#define YELLOW  "\033[33m"                 /* Yellow */
#define BLUE    "\033[34m"                 /* Blue */
#define MAGENTA "\033[35m"                 /* Magenta */
#define CYAN    "\033[36m"                 /* Cyan */
#define WHITE   "\033[37m"                 /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

//-------------------------------------------------------------------
// defining macros for writing to log

#define LOG_FATAL(msg, ...) loggingSys::log::getInstance().printNext(MAGENTA,"[FATAL] ", msg, ##__VA_ARGS__);
#define LOG_ERROR(msg, ...) loggingSys::log::getInstance().printNext(RED,"[ERROR] ", msg, ##__VA_ARGS__);
#define LOG_WARN(msg, ...) loggingSys::log::getInstance().printNext(YELLOW,"[WARN] ", msg, ##__VA_ARGS__);
#define LOG_INFO(msg, ...) loggingSys::log::getInstance().printNext(GREEN,"[INFO] ", msg, ##__VA_ARGS__);
#define LOG_MEMORY(msg, ...) loggingSys::log::getInstance().printNext(CYAN,"[MEMORY] ", msg, ##__VA_ARGS__);


//-------------------------------------------------------------------
// all logging functionality

namespace loggingSys
{    
    
    // class for log
    class log
    {
    private:
        //get timestamp - put into human readable
        time_t getTimestamp()
        {
            auto time = std::chrono::system_clock::now();
            auto asTimeT = std::chrono::system_clock::to_time_t(time);

            return asTimeT;
        }

    public:
        // singletonism
        static log& getInstance()
        {
            static log instance;
            return instance;
        }

        // function to print to log
        void printNext(const char* colour, const char* logType, const char* msg, ...)
        {
            printf("%s%lld %s", colour, getTimestamp(), logType);

            va_list args;
            va_start(args, msg);
            vfprintf(stdout, msg, args);
            va_end(args);
            printf("%s\n", RESET);

        }

    protected:
        log () = default;
    };
}