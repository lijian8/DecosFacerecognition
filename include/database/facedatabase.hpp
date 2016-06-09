#ifndef IMAGEDATABASE_HPP
#define IMAGEDATABASE_HPP

#include "database.hpp"
#include "openface/face.hpp"
#include "openface/neuralnetwork.hpp"
#include "core/image.hpp"

/**
 * @brief Database of images.
 *
 * Stores images in a directory tree with each directory representing a subject
 * and images in each subject's folder which are labeled with the subject.
 */
class ImageDatabase : public FileDatabase<Image> {
public:
    ImageDatabase(int batch_size) : FileDatabase<Image>(batch_size) {}
    virtual void load_subjects();
    virtual Image load_sample(const std::string&);
    virtual std::vector<std::string> subject_samples(const std::string&);
};


class FaceDatabase : public FileDatabase<Face> {
public:
    FaceDatabase(int batch_size) : FileDatabase<Face>(batch_size) {}
    virtual void load_subjects();
    virtual Face load_sample(const std::string&);
    virtual std::vector<std::string> subject_samples(const std::string&);
};

class FaceNetEmbedDatabase : public FileDatabase<FaceNetEmbed> {
public:
    FaceNetEmbedDatabase(int batch_size) : FileDatabase<FaceNetEmbed>(batch_size) {}
    virtual void load_subjects();
    virtual FaceNetEmbed load_sample(const std::string&);
    virtual std::vector<std::string> subject_samples(const std::string&);
};

#endif
