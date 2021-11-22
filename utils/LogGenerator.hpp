#ifndef _LOGGENERATOR_HPP
#define _LOGGENERATOR_HPP

#include <fstream>
#include <string>
#include <unistd.h>

#include "Processor.h"

class LogGenerator {
private:
    std::ofstream output_log;
    std::vector<Processor*> core_list;
    std::vector<Cache*> cache_list;
    std::string output_path = "logs/";
    int NUM_OF_CORES = 4;
    int blk_size = 32;
public:
    void initialize(Processor* core0, Processor* core1, Processor* core2, Processor* core3, 
                    std::string input_info, int block_size) {

        blk_size = block_size;
        core_list.push_back(core0);
        core_list.push_back(core1);
        core_list.push_back(core2);
        core_list.push_back(core3);

        cache_list.push_back(core0->get_cache());
        cache_list.push_back(core1->get_cache());
        cache_list.push_back(core2->get_cache());
        cache_list.push_back(core3->get_cache());

        output_path = output_path + input_info;
        int file_index = 0;
        std::string index;
        
        do {
            file_index += 1;
            std::stringstream ss;
            ss << file_index;
            ss >> index;            
        } while (!access((output_path + "_" + index + ".log").c_str(), F_OK));

        output_path = output_path + "_" + index + ".log";
        output_log.open(output_path, std::ios::out);

        output_log << "Output Summary with Inputs: " << input_info << std::endl;
        output_log << "========================================" << std::endl;

    }

    void print_total_cycles() {
        output_log << "------------------------------" << std::endl;
        output_log << "1. Overall execution cycle per core" << std::endl;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            long curr_val = core_list[i]->get_total_cycle();
            output_log << "Core " << i << ": " << curr_val << std::endl;
        }
    }

    void print_compute_cycles() {
        output_log << "------------------------------" << std::endl;
        output_log << "2. Number of compute cycles per core" << std::endl;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            int curr_val = core_list[i]->get_compute_cycle();
            output_log << "Core " << i << ": " << curr_val << std::endl;
        }
    }

    void print_num_mem_instr() {
        output_log << "------------------------------" << std::endl;
        output_log << "3. Number of load/store instructions per core" << std::endl;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            int curr_val = core_list[i]->get_num_mem_instr();
            output_log << "Core " << i << ": " << curr_val << std::endl;
        }
    }

    void print_num_idle_cycle() {
        output_log << "------------------------------" << std::endl;
        output_log << "4. Number of idle cycles per core" << std::endl;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            int curr_val = core_list[i]->get_idle_cycle();
            output_log << "Core " << i << ": " << curr_val << std::endl;
        }
    }

    void print_cache_miss_rate() {
        output_log << "------------------------------" << std::endl;
        output_log << "5. Data cache miss rate for each core" << std::endl;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            int num_miss = core_list[i]->get_num_cache_miss();
            int num_instr = core_list[i]->get_num_mem_instr();
            output_log << "Core " << i << ": " << double(num_miss)/double(num_instr) << std::endl;
        }
    }

    void print_amt_of_data_traffic() {
        output_log << "------------------------------" << std::endl;
        output_log << "6. Amount of Data traffic in bytes on the bus" << std::endl;
        int sum_traffic = 0;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            sum_traffic += core_list[i]->get_num_data_traffic();
        }
        sum_traffic *= blk_size;
        output_log << sum_traffic << std::endl;
    }

    void print_num_update() {
        output_log << "------------------------------" << std::endl;
        output_log << "7. Number of invalidations or updates on the bus" << std::endl;
        int sum_update = 0;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            sum_update += core_list[i]->get_num_update();
        }
        output_log << sum_update << std::endl;
    }

    void print_distribution_of_access() {
        output_log << "------------------------------" << std::endl;
        output_log << "8. Distribution of accesses to private data versus shared data" << std::endl;
        for (int i = 0; i < NUM_OF_CORES; i++) {
            int num_private = core_list[i]->get_num_access_private();
            int num_shared = core_list[i]->get_num_access_shared();
            output_log << "Core " << i << ": Number of access to private data = " << num_private 
                        << " | Number of access to shared data = " << num_shared << std::endl;
        }

    }

    void print_summary() {

        print_total_cycles();
        print_compute_cycles();
        print_num_mem_instr();
        print_num_idle_cycle();
        print_cache_miss_rate();
        print_amt_of_data_traffic();
        print_num_update();
        print_distribution_of_access();

        output_log << "============== END OF LOG ==============" << std::endl;
        output_log << "========================================" << std::endl;
        output_log.close();

        std::cout << "[INFO] The output summary can be found at: " << output_path << std::endl;
    }


};



#endif