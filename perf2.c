#include <linux/perf_event.h> /* Definition of PERF_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h> /* Definition of SYS_* constants */
#include <unistd.h>
#include <inttypes.h>
#define TOTAL_EVENTS 2

// The function to counting through (called in main)
void code_to_measure(){
    int sum = 0;
    for(int i = 0; i < 2; ++i){
        sum += 1;
    }
}

// Executes perf_event_open syscall and makes sure it is successful or exit
static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags){
    int fd;
    fd = syscall(SYS_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    if (fd == -1) {
        fprintf(stderr, "Error creating event");
        exit(EXIT_FAILURE);
    }

    return fd;
}

// Helper function to setup a perf event structure (perf_event_attr; see man perf_open_event)
void configure_event(struct perf_event_attr *pe, uint32_t type, uint64_t config){
    memset(pe, 0, sizeof(struct perf_event_attr));
    pe->type = type;
    pe->size = sizeof(struct perf_event_attr);
    pe->config = config;
    pe->read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
    pe->disabled = 1;
    pe->exclude_kernel = 1;
    pe->exclude_hv = 1;
}

// Format of event data to read
// Note: This format changes depending on perf_event_attr.read_format
// See `man perf_event_open` to understand how this structure can be different depending on event config
// This read_format structure corresponds to when PERF_FORMAT_GROUP & PERF_FORMAT_ID are set
struct read_format {
    uint64_t nr;
    struct {
        uint64_t value;
        uint64_t id;
    } values[TOTAL_EVENTS];
};

int main() {
    int fd0, fd1;  // fd0 will be the group leader file descriptor
    int id0, id1;  // event ids for file descriptors
    uint64_t pe_val0, pe_val1; // Counter value array corresponding to fd/id array.
    struct perf_event_attr pe0, pe1;  // Configuration structure for perf events (see man perf_event_open)
    struct read_format counter_results;

    // Configure the group of PMUs to count
    configure_event(&pe0, PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
    configure_event(&pe1, PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    //configure_event(&pe[2], PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND);
    //configure_event(&pe[3], PERF_TYPE_HARDWARE, PERF_COUNT_HW_STALLED_CYCLES_BACKEND);
    //configure_event(&pe[4], PERF_TYPE_RAW, 0x70);  // Count of speculative loads (see Arm PMU docs)
    //configure_event(&pe[5], PERF_TYPE_RAW, 0x71);  // Count of speculative stores (see Arm PMU docs)

    // Create event group leader
    fd0 = perf_event_open(&pe0, 0, -1, -1, 0);
    ioctl(fd0, PERF_EVENT_IOC_ID, &id0);
    // Let's create the rest of the events while using fd0 as the group leader
    //for(int i = 1; i < TOTAL_EVENTS; i++){
        fd1 = perf_event_open(&pe1, 0, -1, fd0, 0);
        ioctl(fd1, PERF_EVENT_IOC_ID, &id1);
    //}
    // Reset counters and start counting; Since fd0 is leader, this resets and enables all counters
    // PERF_IOC_FLAG_GROUP required for the ioctl to act on the group of file descriptors
    ioctl(fd0, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    ioctl(fd0, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

    // Example code to count through
    code_to_measure();

    // Stop all counters
    ioctl(fd0, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

    // Read the group of counters and print result
    read(fd0, &counter_results, sizeof(struct read_format));
    printf("Num events captured: %"PRIu64"\n", counter_results.nr);
    for(int i = 0; i < counter_results.nr; i++) {
        if(counter_results.values[i].id == id0) {
            pe_val0 = counter_results.values[i].value;
        }
        if(counter_results.values[i].id == id1){
                pe_val1 = counter_results.values[i].value;
        }
    }
    printf("CPU cycles: %"PRIu64"\n", pe_val0);
    printf("Instructions retired: %"PRIu64"\n", pe_val1);

    // Close counter file descriptors
    //for(int i = 0; i < TOTAL_EVENTS; i++){
    close(fd0);    
    close(fd1);
    //}

    return 0;
}
