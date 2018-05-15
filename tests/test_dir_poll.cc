// test_poll_dir.cc: part of a googletest suite
#include "file_util.cc"
#include <gtest/gtest.h>

#include <dirent.h>
#include <chrono>
// #include <cstring>
// #include <glob.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>

TEST (Temp, Fail)
{
    EXPECT_EQ( 1, 1 );
}

struct NewOrder
{
    int quantity;
    std::string container; //$ or disher name?
};
struct ParlorState
{
    const int id = 1;
    const std::string name = "WAIT_FOR_ORDER";
    const ParlorState * get_current_state() const { return this; }
    void dispatch(const NewOrder& no) const {
        std::cout << "NewOrder: " << no.container << ", quantity: " << no.quantity << std::endl;
    }
};


/// Translate web message into an order and dispatch it
/// Return true on parse and send; false on first error.
bool order_str_to_order(std::string order_str, NewOrder& newOrder)
{
    // Expecting messages of the form: ORDER ord_2018.04.24_50_24_0002_1_bin2
    size_t len = order_str.size();
    std::string bin_num_str = order_str.substr(len-1, 1);
    std::string nscoops_str = order_str.substr(len-6, 1);
    newOrder.container = (bin_num_str == "0" ? "bin_0" : "bin_2");
    newOrder.quantity = (nscoops_str == "2" ? 2 : 1);
    return true;
}

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


bool get_new_order_from_latest_file( NewOrder& newOrder
                                   , std::string& new_file_name
                                   , time_t& old_time
                                   , const char *dir_path = "../tests/data"
                                   , const char *file_ext = "txt"
                                   , int verbose = 1
) {
    int odd = time(NULL) % 2;
    newOrder.container = odd ? "bin_0" : "bin_2";
    newOrder.quantity = 1;
    if (get_latest_file_name(dir_path, file_ext, new_file_name, old_time, verbose)) {
        if (order_str_to_order(new_file_name, newOrder)) {
            std::cout << "GNOFLF newOrder.container: " << newOrder.container << std::endl;
            std::cout << "GNOFLF newOrder.quantity:  " << newOrder.quantity << std::endl;
            return true;
        }
    }
    return true;
}

TEST (PollDirs, OrderLog)
{
    const char *dir_path = "../tests/data";
    const char *file_ext = "txt";
    int verbose = 4;
    std::cout << "$RUN " << "PollDirs: " << dir_path << " " << file_ext
              << " verbose: " << verbose << std::endl;
    std::string sepl = "--------------------------\n";
    int dirs_created = -99;
    EXPECT_NO_THROW( dirs_created = make_dirs(dir_path) );
    EXPECT_EQ(dirs_created, 0);

    std::string latest_file_name;
    time_t latest_file_time = 0;
    bool found_latest = false;
    EXPECT_NO_THROW( found_latest = get_latest_file_name(dir_path, file_ext, latest_file_name, latest_file_time, verbose) );
    EXPECT_TRUE(found_latest);
    EXPECT_GT(latest_file_time, 0);

    std::string found_file_name( found_latest ? latest_file_name : "[NONE]" );
    std::cout << sepl << "latest_file_name: " << found_file_name << std::endl;

    bool got_order = false;
    NewOrder newOrder;
    EXPECT_NO_THROW( got_order = order_str_to_order(latest_file_name, newOrder) );
    EXPECT_TRUE(got_order);
    std::cout << "newOrder.container: " << newOrder.container << std::endl;
    std::cout << "newOrder.quantity:  " << newOrder.quantity << std::endl;
    EXPECT_EQ(newOrder.container, "bin_0");
    EXPECT_EQ(newOrder.quantity, 2);

    NewOrder nxtOrder;
    std::string nxtFileName;
    time_t nxtFileTime = 0;
    EXPECT_NO_THROW( got_order = get_new_order_from_latest_file( nxtOrder
                                                               , nxtFileName
                                                               , nxtFileTime
                                                               , "../tests/data"
                                                               , "txt", 1)
    );
    EXPECT_TRUE( got_order );
    EXPECT_TRUE( nxtFileName.size() > 0 );
    std::cout << "nxtOrder.container: " << nxtOrder.container << std::endl;
    std::cout << "nxtOrder.quantity:  " << nxtOrder.quantity << std::endl;
    EXPECT_EQ(nxtOrder.container, "bin_0");
    EXPECT_EQ(nxtOrder.quantity, 2);
}
