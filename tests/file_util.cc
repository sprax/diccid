/** file_util.cc */
#ifndef __FILE_UTIL_H
#define __FILE_UTIL_H

#include <errno.h>
#include <glob.h>
#include <iostream>
// #include <iterator>
#include <limits>
#include <limits.h>     /* PATH_MAX */
#ifndef PATH_MAX
#error "not defined"
#endif

#include <pwd.h>
#include <stdio.h>  /* defines FILENAME_MAX */
#include <sys/stat.h>   /* mkdir(2) */
#include <sys/types.h>
#include <unistd.h>

#include <gtest/gtest.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;


std::string getenv_var(const std::string& var) {
     const char *val = std::getenv(var.c_str());
     if (val == 0)
         return "";
     else
         return val;    // auto-converted
}

/** Get the current user's home directory one way or another */
std::string get_home_dir()
{
    const char *val = std::getenv("HOME");
    return *val ? val : getpwuid(getuid())->pw_dir;
}

std::string get_cwd()
{
  char buff[FILENAME_MAX];
  getcwd( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;
}


/// makes dirs recursively using path_str; returns 0 on success, negative int on error.
/// If path_str idenfies an existing directory or file, this function does not change it.
int make_dirs(const std::string& path_str)
{
    const char *path = path_str.c_str();
    /* Adapted from http://stackoverflow.com/a/2336245/119527 */
    const size_t len = strlen(path);
    char _path[PATH_MAX];
    char *p;

    errno = 0;

    /* Copy string so its mutable */
    if (len > sizeof(_path)-1) {
        errno = ENAMETOOLONG;
        return -1;
    }
    strcpy(_path, path);

    /* Iterate the string */
    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';

            if (mkdir(_path, S_IRWXU) != 0) {
                if (errno != EEXIST)
                    return -1;
            }

            *p = '/';
        }
    }

    if (mkdir(_path, S_IRWXU) != 0) {
        if (errno != EEXIST)
            return -1;
    }

    return 0;
}

const std::string temp_dir()
{
    // return std::filesystem::temp_directory_path(); // (C++17)
    return "/tmp";
}

time_t file_mod_time(const std::string &path)
{
    time_t file_mod_time = 0;
    struct stat result;
    if (stat(path.c_str(), &result)==0) {
        file_mod_time = result.st_mtime;
    }
    return file_mod_time;
}

bool is_dir(const std::string &path)
{
    struct stat result;
    if (stat(path.c_str(), &result)==0) {
        if( result.st_mode & S_IFDIR )
        {
            return true;
        }
    }
    return false;
}




time_t expectFileMod(const std::string& temp_file_path, time_t max_seconds)
{
    time_t time_mod = file_mod_time(temp_file_path);
    time_t time_dif = time(NULL) - time_mod;
    EXPECT_LE(time_dif, max_seconds);
    if (time_dif > max_seconds) {
        cout<< "    Checked file: " << temp_file_path << endl
            << "    Seconds since file mod exceeds limit: "
            << time_dif << " > " << max_seconds << endl;
    }
    return time_dif;
}

std::vector<std::string> glob_v(const std::string& pat)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<string> ret;
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

bool glob_b(const std::string& pat, std::vector<std::string>& result)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    result = vector<string>();
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        result.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return result.size() > 0;
}



#endif  //  __FILE_UTIL_H
