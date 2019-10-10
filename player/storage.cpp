#include "storage.h"

class FileStorage: public AbstractStorage {
public:
    FileStorage() {
        stubFile.open(STUB_FILE_PATH, std::ios_base::in | std::ios_base::out | std::ios_base::app);
    }
    ~FileStorage() {
        stubFile.close();
    }
    void insertSongs(const std::vector<std::unique_ptr<Song>> &songs) noexcept {
        if (stubFile.is_open()) { // or even without it
            for (const auto& songPtr : songs) {
                stubFile << *songPtr;
            }
        }
    }

    void deleteSongs(const QStringList &absPathesToSongs) noexcept {
        stubFile.
    }
private:
    std::fstream stubFile;
};
