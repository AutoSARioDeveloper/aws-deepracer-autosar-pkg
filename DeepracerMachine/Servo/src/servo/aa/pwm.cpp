#include "servo/aa/pwm.hpp"
#include <dirent.h>


namespace PWM {
    /// Max size of the character buffer used to concat the file paths.
    #define MAX_BUF 64
    // The following pwm dev id and base path were given by pegatron.
    const std::string BASE_SYS_PATH = "/sys/class/pwm/";
    const std::string PWMDEV = "0000:00:17.0";

    /// Helper method that writes the given value into a specified path.
    /// @param path Path to the file where the value is to be written.
    /// @param value Value to write in the file given by path.
  	void writePWM(const char *path, int value) {
        int fd, len;
        fd = open(path, O_WRONLY);
        if (fd < 0) {
            return;
        }
        char buf[MAX_BUF];
        len = snprintf(buf, sizeof(buf), "%d", value);
        write(fd, buf, len);
        close(fd);
    }
    /// Rewriting the command to dynamically find the pwmchip%d directory and return the syspath
    /// `ls -al /sys/class/pwm/ | grep "0000:00:17.0" | awk '{ print $9}'`
    /// @returns Syspath pointing to the pwmchip directory.
    const std::string getSysPath() {
    std::string syspath = BASE_SYS_PATH + "pwmchip0";
    DIR* dir = opendir(BASE_SYS_PATH.c_str());
    if (!dir) {
        return syspath;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string entryName = entry->d_name;
        if (entryName == "." || entryName == "..") {
            continue;
        }

        std::string fullPath = BASE_SYS_PATH + entryName;
        struct stat path_stat;
        lstat(fullPath.c_str(), &path_stat);
        if (S_ISLNK(path_stat.st_mode)) {
            char targetBuffer[1024];
            ssize_t len = readlink(fullPath.c_str(), targetBuffer, sizeof(targetBuffer) - 1);
            if (len != -1) {
                targetBuffer[len] = '\0';
                std::string symlinkTarget(targetBuffer);
                if (symlinkTarget.find(PWMDEV) != std::string::npos) {
                    std::string tmp = symlinkTarget;
                    std::string delimiter = "/";
                    size_t pos = 0;
                    size_t tokenCount = 0;
                    while ((pos = tmp.find(delimiter)) != std::string::npos && tokenCount < 9) {
                        tmp.erase(0, pos + delimiter.length());
                        tokenCount++;
                    }
                    syspath = BASE_SYS_PATH + tmp;
                }
            }
        }
    }

    closedir(dir);
    return syspath;
}

    Servo::Servo(int channel)
      : channel_(channel),
        period_(0),
        duty_(0),
        syspath_(getSysPath())
    {
        char exportPath[MAX_BUF];
        snprintf(exportPath, sizeof(exportPath), (syspath_ + std::string("/pwm%d")).c_str(), channel_);
        struct stat st;
        if (stat(exportPath, &st) == 0) {
            return;
        }
        writePWM((syspath_ + std::string("/export")).c_str(), channel_);
    }

    /// Setter for the PWM period.
    /// @param period Desired period in ms.
    void Servo::setPeriod(int period) {
        char periodPath[MAX_BUF];
        snprintf(periodPath, sizeof(periodPath), (syspath_ + std::string("/pwm%d/period")).c_str(), channel_);
        writePWM(periodPath, period);
    }

    /// Setter for the duty cycle, this is what determines how much the servo actuates.
    /// @param Desired duty cycle.
    void Servo::setDuty(int duty) {
        char dutyPath[MAX_BUF];
        snprintf(dutyPath, sizeof(dutyPath), (syspath_ + std::string("/pwm%d/duty_cycle")).c_str(), channel_);
        writePWM(dutyPath, duty);
    }

    /// @returns Current value of the period.
    int Servo::getPeriod() const {
        return period_;
    }

    /// @returns Current value of the duty cycle.
    int Servo::getDuty() const {
        return duty_;
    }
}
