#ifndef BENCHMARKRESULT_HPP
#define BENCHMARKRESULT_HPP

#include <cstddef>
#include <cstdint>
#include <string>

class BenchmarkResult {
    private:
        size_t totalDuration = 0;
        size_t minDuration = SIZE_MAX;
        size_t maxDuration = 0;
        size_t runCount = 0;

        std::string algorithmId = "";
        std::string representationId = "";

    public:
        BenchmarkResult() = default;
        ~BenchmarkResult() = default;

        void append(size_t duration) {
            totalDuration += duration;
            runCount++;
            if (duration < minDuration) minDuration = duration;
            if (duration > maxDuration) maxDuration = duration;
        }

        size_t avg() const {
            return runCount > 0 ? totalDuration / runCount : 0;
        }

        size_t min() const { return minDuration; }
        size_t max() const { return maxDuration; }

        void setAlgorithmId(const std::string& id) { algorithmId = id; }
        void setRepresentationId(const std::string& id) { representationId = id; }

        const std::string& getAlgorithmId() const { return algorithmId; }
        const std::string& getRepresentationId() const { return representationId; }

        bool operator==(const BenchmarkResult& other) const {
            return algorithmId == other.algorithmId && representationId == other.representationId;
        }
        bool operator!=(const BenchmarkResult& other) const {
            return algorithmId != other.algorithmId || representationId != other.representationId;
        }

};

#endif // BENCHMARKRESULT_HPP