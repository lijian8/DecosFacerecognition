#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "core/support.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <stdexcept>

template <typename sample_type>
struct Batch {
    std::vector<sample_type> samples;
    std::vector<std::string> labels;
};

/**
 * @brief Abstract class for storing labeled sample-data in a file database.
 *
 * The database class can load an save labeled sample data, for example labeled
 * images for object or face recognition. The samples must be have a save and
 * load function, which write and read the object from a file. Labels have to be
 * strings.
 *
 * TODO(Jan): Either remove std::string because it has to be string or add suport for other labels
 *
 */
template <typename sample_type>
class FileDatabase {
protected:
    // /**
    //  * @brief Vector of all samples(e.g. images) in the database.
    //  */
    // std::vector<sample_type> samples_;
    /**
     * @brief Vector of all labels for the images (same size as #samples_).
     */
    std::vector<std::string> labels_;

    /**
     * @brief Vector of all distinctive subjects in the database.
     *
     * This set of labels in this list is the same as in the labels but each
     * subject is only once in this list.
     */
    std::vector<std::string> subjects_;

    std::vector<std::string> files_;

    /**
     * @brief Populates the #subjects_ list.
     */
    virtual void load_subjects() =0;
    virtual sample_type load_sample(const std::string&) =0;

    /**
     * @brief For a given subject return the samples for this subject.
     *
     * @param subject Subject for which the samples shall be returned
     */
    virtual std::vector<std::string> subject_samples(const std::string& subject) =0;

    /**
     * @brief Root directory of the database.
     */
    std::string root_;

    int batch_size_;
    int batches_;

public:
    // /**
    //  * @brief Returns list of samples.
    //  */
    // std::vector<sample_type> samples() {return samples_;}
    //
    // /**
    //  * @brief Returns list of labels.
    //  */
    // std::vector<std::string> labels() {return labels_;}
    Batch<sample_type> batch(int i);

    std::vector<std::string> subjects() { return subjects_; }

    std::vector<std::string> files() { return files_; }

    int batches() { return batches_; }

    /**
     * @brief Default constructor for empty database.
     */
    FileDatabase(int batch_size);

    /**
     * @brief Constructs a database with the given root.
     *
     * This constructor does not load the data, it only set's the internal root
     * directory. After this constructor the database has to be loaded with
     * load(). This is because loading can be very time consuming and it gives
     * the user better control when to perform the loading.
     *
     * @param root Root for the database that shall be loaded
     */
    FileDatabase(const std::string& root, int batch_size);

    /**
     * @brief Empty default destructor.
     */
    ~FileDatabase() {};

    /**
     * @brief Loads the data from the internal root directory.
     *
     * This requires that the root directory has be set beforehand. Otherwise
     * the function will not do anything. The root directory can be set using
     * root() or constructing with Database(const std::string&).
     */
    void load();

    /**
     * @brief Set's the root of the database to the given path and loads the data.
     *
     * Removes anything previously stored in the given directory.
     *
     * @param The new root of the database.
     * @throws std::runtime_error with text "Directory is empty or not a database"
     *         if path does not exist or is epmty
     */
    void load(const std::string& root);

    /**
     * @brief Adds a labeled sample to the database.
     */
    void add(const std::string&, const std::string&);
};

template <typename sample_type>
FileDatabase<sample_type>::FileDatabase(int batch_size) : batch_size_(batch_size) {}

template <typename sample_type>
FileDatabase<sample_type>::FileDatabase(const std::string& root, int batch_size) : root_(root), batch_size_(batch_size) {}

template <typename sample_type>
void FileDatabase<sample_type>::load(const std::string& root) {
    assert(root_.empty());
    root_ = root;
    load();
}

template <typename sample_type>
Batch<sample_type> FileDatabase<sample_type>::batch(int i) {
    assert(i < batches_);

    Batch<sample_type> batch;

    std::vector<std::string>::const_iterator begin = files_.begin() + i * batch_size_;
    std::vector<std::string>::const_iterator end = ( (i + 1) * batch_size_ > files_.size() ) ?
            files_.end() : files_.begin() + (i+1) * batch_size_;
    std::vector<std::string> files_batch(begin, end);

    std::vector<std::string>::const_iterator lbegin = labels_.begin() + i * batch_size_;
    std::vector<std::string>::const_iterator lend = ( (i + 1) * batch_size_ > labels_.size() ) ?
            labels_.end() : labels_.begin() + (i+1) * batch_size_;
    std::vector<std::string> labels_batch(lbegin, lend);

    for (size_t i = 0; i < files_batch.size(); i++) {
        std::stringstream ss;
        ss << root_ << '/' << labels_batch[i] << '/' << files_batch[i];
        batch.samples.push_back(load_sample(ss.str()));
        batch.labels.push_back(labels_batch[i]);
    }

    return batch;

}

template <typename sample_type>
void FileDatabase<sample_type>::add(const std::string& s, const std::string& l) {
    files_.push_back(s);
    labels_.push_back(l);
}

template <typename sample_type>
void FileDatabase<sample_type>::load() {
    ASSERT(!root_.empty(), "No root directory specified.");

    load_subjects();
    if(subjects_.empty())
        throw std::runtime_error(std::string("Directory is empty or not a database: ")+root_);

    for (size_t i = 0; i < subjects_.size(); i++) {
        std::vector<std::string> s;
        try {
            s = subject_samples(subjects_[i]);
            if (s.empty())
                throw std::runtime_error(std::string("No subjects found"));
        }
        catch(...) {
            throw std::runtime_error(std::string("Directory is empty or not a database: ")+root_);
        }

        for (size_t j = 0; j < s.size(); j++) {
            add(s[j], subjects_[i]);
        }

    }

    if(files_.empty())
        throw std::runtime_error("Directory is empty or not a database");

    batches_ = files_.size() / batch_size_;
    if ( (files_.size() % batch_size_) != 0)
        batches_ += 1;

}

#endif /* end of include guard: DATABASE_HPP */
