// dirpoll.cpp : template classes using enum values as template parameters
// Requires C11 or later
// BUILD: clang++ -std=c++11 dirpoll.cpp -o tmpl && tmpl

/// TODO: use full_path or rel_path, not .
#include <dirent.h>
#include <chrono>
#include <cstring>
#include <glob.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>
#include <vector>


time_t get_file_ctime(char *path)
{
    struct stat attr;
    stat(path, &attr);
    // printf("Last modified time: %s", ctime(&attr.st_ctime));
    return attr.st_ctime;
}

time_t get_file_mtime(const char *path)
{
    struct stat attr;
    stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
    return attr.st_mtime;
}

std::string get_file_extension(std::string file_path)
{
    // Finds the last persiod character of the std::string
    int dot_pos = file_path.find_last_of(".");
    std::string ext = file_path.substr(dot_pos + 1);
    return ext;
}

bool get_latest_file_name( const char *dir_path
                         , const char *file_ext
                         , std::string& latest_file_name
                         , time_t &latest_file_time
                         , int verbose = 1
) {
    bool later_file_found = false;     // return value
    std::string latest_fname_str;       // keep in outer scope
    DIR           *pDIR;
    struct dirent *pDirent;
    pDIR = opendir(dir_path);
    if (pDIR) {
        while ((pDirent = readdir(pDIR)) != NULL) {
            char *file_name = pDirent->d_name;
            //exclude "." and ".."
            if (strcmp(file_name, "." ) == 0 || strcmp(file_name, "..") == 0) {
                continue;
            }
            std::string name_str = file_name;
            int dot_pos = name_str.find_last_of(".");
            std::string ext = name_str.substr(dot_pos + 1);
            if (verbose > 1) {
                std::cout << "GLFN ANY: " << name_str << "  ext: " << ext << std::endl;
            }
            if (ext != file_ext) {
                std::cout << "continue: " << name_str << "  ext: " << ext << std::endl;
                continue;
            }
            std::string pref_path(dir_path);
            std::string full_path = pref_path + "/" + name_str;
            time_t file_time = get_file_mtime(full_path.c_str());
            if (verbose > 3) {
                std::cout << "    file_name: " << file_name
                          << "    late_time: " << latest_file_time << " ? "
                          << "    file_time: " << file_time << std::endl;
            }
            if (latest_file_time < file_time) {
                latest_file_time = file_time;
                latest_fname_str = name_str.substr(0, dot_pos);
                later_file_found = true;
                if (verbose > 3) {
                    std::cout << "  retained: " << latest_fname_str << " + " << ext
                              << "  file_time: " << file_time << std::endl;
                }
            }
        }
        closedir(pDIR);
    }
    if (verbose > 2) {
        std::cout << "returning: " << latest_fname_str << ", ok?" << std::endl;
    }
    latest_file_name = latest_fname_str;   // return by ref
    return later_file_found;   // return status
}


inline bool glob_b(const std::string& pat, std::vector<std::string>& result)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    result = vector<std::string>();
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        result.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return result.size() > 0;
}

inline std::vector<std::string> glob_v(const std::string& pat)
{
    using namespace std;
    glob_t glob_result;
    glob(pat.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<std::string> ret;
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        ret.push_back(std::string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

static std::vector<std::string> keys{"keyA", "keyB", "keyC", "keyD"};

std::vector<std::string> get_cpp_files(int max_num_files, int verbose)
{
    std::string pattern = "*.cpp";
    std::vector<std::string> files;
    if (glob_b(pattern, files)) {
        int j = max_num_files;
        for (auto& file : files) {
            if (verbose > 2) {
                std::cout << "file: " << file << std::endl;
            }
            if (--j == 0) {
                break;
            }
        }
    } else {
        std::cout << "No files found for: " << pattern << std::endl;
    }
    return files;
}

int main(int argc, char *argv[])
{
    const char *dir_path = argc > 1 ? argv[1] : "/Users/sprax/asdf/cpp/tmpl";
    const char *file_ext = argc > 2 ? argv[2] : "cpp";
    int verbose = argc > 3 ? atoi(argv[3]) : 1;
    std::cout << "$RUN " << argv[0] << " " << dir_path << " " << file_ext << std::endl;

    std::string sepl = "--------------------------\n";
    // int max_num_files = 0;
    // std::vector<std::string> cpp_files = get_cpp_files(max_num_files, verbose);
    // std::cout << sepl;

    time_t t_zero = 0;
    std::string latest_file_name;
    bool found_latest = get_latest_file_name(dir_path, file_ext, latest_file_name, t_zero, verbose);
    std::string fname(found_latest ? latest_file_name : "[NONE]");
    std::cout << sepl << "latest_file_name: " << fname << std::endl;

    std::string full_path = "/usr/homes/sprax/tea/cup/tempest.txt";
    size_t dir_pos = full_path.find_last_of("/");
    std::string file_name = (dir_pos == std::string::npos) ? full_path : full_path.substr(dir_pos + 1);
    size_t dot_pos = file_name.find_last_of(".");
    std::string base_name = (dot_pos == std::string::npos) ? file_name : file_name.substr(0, dot_pos);
    std::string file_extn = file_name.substr(dot_pos + 1);
    std::cout << "Using find_last_of: " << std::endl;
    std::cout << full_path << " => " << file_name << ", " << base_name << ", " << file_extn << std::endl;

    full_path = "tempest.in.teacup";
    dir_pos = full_path.find_last_of("/");
    file_name = (dir_pos == std::string::npos) ? full_path : full_path.substr(dir_pos + 1);
    dot_pos = file_name.find_last_of(".");
    base_name = (dot_pos == std::string::npos) ? file_name : file_name.substr(0, dot_pos);
    file_extn = file_name.substr(dot_pos + 1);
    std::cout << "Using find_last_of: " << std::endl;
    std::cout << full_path << " => " << file_name << ", " << base_name << ", " << file_extn << std::endl;
    return 0;
} 
