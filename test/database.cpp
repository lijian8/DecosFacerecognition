#include "database/facedatabase.hpp"
#include <gtest/gtest.h>

/**
 *
 * Image Database Tests
 *
 */


/**
 * @fn Database::load(const string&)
 *
 * @test
 * Loading a database from a directory. Checks for correct sample size and equality
 * of sample and label set size.
 */
TEST (FaceDatabaseTest, LoadingFromDirectory) {
    ImageDatabase db(3);
    db.load("test/resources/raw");

    std::vector<std::string> faces = db.files();
    std::vector<std::string> labels = db.subjects();

    EXPECT_EQ(faces.size(), 6);
    EXPECT_EQ(labels.size(), 3);
}

/**
 * @fn Database::load(const string&)
 *
 * @test
 * Failing to load from directory that is not a database.
 */
TEST (FaceDatabaseTest, DirectoryIsNotADatabase) {
    ImageDatabase db(3);
    try {
        db.load("test/resources/");
        FAIL();
    }
    catch (std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "Directory is empty or not a database: test/resources/" );
    }
    catch (...) {
        FAIL() << "Expected different exception";
    }
}

/**
 * @fn Database::load()
 *
 * @test
 * Assertion failure when trying to load without root directory specified.
 */
TEST (FaceDatabaseTest, NoRootSpecified) {
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    ImageDatabase db(3);
    EXPECT_DEATH(db.load(), "No root directory specified.");
}

TEST(FaceDatabaseTest, Batch) {
    ImageDatabase db(3);
    db.load("test/resources/raw");

    Batch<Image> batch = db.batch(0);
    EXPECT_EQ(batch.samples.size(), 3);
    batch = db.batch(1);
    EXPECT_EQ(batch.samples.size(), 3);
    EXPECT_EQ(batch.samples[0].width(), 1280);
}

// TODO: Tests still need to be implemented
TEST (FaceDatabaseTest, DISABLED_AddToDatabase) {}
