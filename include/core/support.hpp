#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <dirent.h>

#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#define ASSERT(x, msg) if(!!(x)); else {std::cerr << msg; std::abort();}

/**
 * @brief List filenames in the specified directory.
 *
 * Returns a vector of the names of all files in a directory including extension
 * but not including the path. All hidden files, that is files starting with a '.'
 * are excluded. If no files are found or #directory_name does not exist,
 * returns an empty vector.
 *
 * @param directory_name Name of the directory of which the files shall be listed
 *
 * @return Set of file names
 */
inline std::vector<std::string> list_files(const std::string& directory_name) {
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> out;

    dir = opendir(directory_name.c_str());
    if (dir != NULL) {

        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG && strncmp(ent->d_name, ".", 1) != 0) {
                out.push_back(std::string(ent->d_name));
            }
        }
    }

    return out;
}

/**
 * @brief List directory names in the specified directory.
 *
 * Returns a vector of the names of all directories in a directory not including
 * the root path. All hidden directories, that is folders starting with a '.'
 * are excluded. If no directories are found or #directory_name does not exist,
 * returns an empty vector.
 *
 * @param directory_name Name of the directory of which the folders shall be listed
 *
 * @return Set of directory names
 */
inline std::vector<std::string> list_directories(const std::string& directory_name) {
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> out;

    dir = opendir(directory_name.c_str());
    if (dir != NULL) {

        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strncmp(ent->d_name, ".", 1) != 0) {
                out.push_back(std::string(ent->d_name));
            }
        }
    }

    return out;
}

#endif
