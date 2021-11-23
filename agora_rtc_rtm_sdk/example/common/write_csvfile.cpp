#include "write_csvfile.h"

WriteCSVFileHandle::~WriteCSVFileHandle() {
  if (csvfile_) {
    csvfile_.close();
  }
}

void WriteCSVFileHandle::open() {
  csvfile_.open(filepath_.c_str(), std::ios::out | std::ios::trunc);
  if (csvfile_) {
    csvfile_ << "round_count "
             << ","
             << "spendTime"
             << "\n";
  }
}

void WriteCSVFileHandle::close() {
  if (csvfile_) {
    csvfile_.close();
  }
}

void WriteCSVFileHandle::write_csvfile(
    const struct delayTimeBaseInfo& delay_info) {
  if (csvfile_) {
    csvfile_ << delay_info.round_count_ << "," << delay_info.spendTime_ << "\n";
  }
}
