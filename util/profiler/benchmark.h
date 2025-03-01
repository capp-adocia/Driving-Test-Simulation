/*****************************************************************//**
 * \file   benchmark.h
 * \brief  基准测试
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../common/preprocessor.h"
#include "../common/standard.h"
#include "../common/tools.h"
#include "../logger/log.h"

// 基准测试
namespace Benchmark {
    // 对函数或代码块进行基准测试
    // !Usage：
    /*
    * begin:
    	BENCHMARK_SCOPE("333");
        ......代码块或函数
      end:
	    BENCHMARK_END; // 生成报告
    */
    class Profiler {
    public:
        struct ProfileData {
            size_t call_count = 0;
            double total_time = 0.0;
            double max_time = 0.0;
            double min_time = std::numeric_limits<double>::max();
            size_t memory_usage = 0;
            size_t peak_memory = 0;
            std::vector<double> time_samples;
        };

        class Timer {
        public:
            Timer(Profiler& profiler, const std::string& name)
                : m_profiler(profiler), m_name(name), m_start(GetHighResolutionTime())
            {
                m_memory_start = GetMemoryUsage();
            }

            ~Timer() {
                double elapsed = GetHighResolutionTime() - m_start;
                size_t memory_used = GetMemoryUsage() - m_memory_start;

                m_profiler.record(m_name, elapsed, memory_used);
            }

        private:
            static double GetHighResolutionTime() {
#if defined(_WIN32)
                LARGE_INTEGER freq, time;
                QueryPerformanceFrequency(&freq);
                QueryPerformanceCounter(&time);
                return static_cast<double>(time.QuadPart) / freq.QuadPart * 1000.0; // ms
#else
                struct timespec ts;
                clock_gettime(CLOCK_MONOTONIC, &ts);
                return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0; // ms
#endif
            }

            static size_t GetMemoryUsage() {
#if defined(_WIN32)
                PROCESS_MEMORY_COUNTERS pmc;
                GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
                return pmc.WorkingSetSize;
#else
                struct rusage usage;
                getrusage(RUSAGE_SELF, &usage);
                return usage.ru_maxrss * 1024; // KB to bytes
#endif
            }

            Profiler& m_profiler;
            std::string m_name;
            double m_start;
            size_t m_memory_start;
        };

        void record(const std::string& name, double time, size_t memory_used) {
            std::lock_guard<std::mutex> lock(m_mutex);

            ProfileData& data = m_data[name];
            data.call_count++;
            data.total_time += time;
            data.max_time = std::max(data.max_time, time);
            data.min_time = std::min(data.min_time, time);
            data.time_samples.push_back(time);

            data.memory_usage += memory_used;
            if (memory_used > data.peak_memory) {
                data.peak_memory = memory_used;
            }
        }

        void generateReport(const std::string& filename = "benchmark_report.json") {
            std::lock_guard<std::mutex> lock(m_mutex);

            std::ofstream report(filename);
            report << "{\n";
            report << "  \"profile_data\": [\n";

            bool first = true;
            for (const auto& [name, data] : m_data) {
                if (!first) report << ",\n";
                first = false;

                report << "    {\n";
                report << "      \"name\": \"" << name << "\",\n";
                report << "      \"call_count\": " << data.call_count << ",\n";
                report << "      \"total_time_ms\": " << data.total_time << ",\n";
                report << "      \"average_time_ms\": " << data.total_time / data.call_count << ",\n";
                report << "      \"max_time_ms\": " << data.max_time << ",\n";
                report << "      \"min_time_ms\": " << data.min_time << ",\n";
                report << "      \"memory_usage_bytes\": " << data.memory_usage << ",\n";
                report << "      \"peak_memory_bytes\": " << data.peak_memory << ",\n";
                report << "      \"samples\": " << data.time_samples.size() << "\n";
                report << "    }";
            }
            report << "\n  ]\n}";
            
            LOG_INFO("Benchmark report generated: {}", filename);
        }

        static Profiler& getInstance() {
            static Profiler instance;
            return instance;
        }

    private:
        std::unordered_map<std::string, ProfileData> m_data;
        std::mutex m_mutex;
    };

// 测量宏
#define BENCHMARK_SCOPE(name) Profiler::Timer CONCAT(timer_, __LINE__)(Profiler::getInstance(), name)
#define BENCHMARK_FUNCTION() BENCHMARK_SCOPE(__FUNCTION__)
#define BENCHMARK_END Profiler::getInstance().generateReport()
}

#endif // BENCHMARK_H