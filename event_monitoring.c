//Monitoring Hardware Events on Linux

#define _GNU_SOURCE
 #include <sched.h>
 #include <sys/wait.h>
 
 
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <string.h>
 #include <sys/ioctl.h>
 #include <linux/perf_event.h>
 #include <asm/unistd.h>

 #include <stdint.h>
 #include <inttypes.h>
 #include <time.h>

int main( int argc, char ** argv){
		
	struct perf_event_attr pe;
	int fd_cycles, fd_instruction;
	uint64_t id_cycles, id_instruction;
	uint64_t val_cycles, val_instruction;
		
	struct read_format {
		uint64_t nr;
		struct {
			uint64_t value;
			uint64_t id;
		} values[8]; // no. of event types?
	};

	char buf[4096];
	struct read_format* rf = (struct read_format*) buf;
			
	//cycles
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_HARDWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_HW_CPU_CYCLES;
	pe.disabled = 1;
	pe.exclude_kernel = 1;
	pe.exclude_hv = 1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
		
	fd_cycles= syscall(__NR_perf_event_open, &pe, 0, -1,-1,0);
    ioctl(fd_cycles, PERF_EVENT_IOC_ID, &id_cycles);
    
	if(fd_cycles == -1){
		fprintf(stderr, "fd_cycles Error opening leader %llx \n", pe.config);
		exit(EXIT_FAILURE);
	}
			
	//instruction
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_HARDWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_HW_INSTRUCTIONS;
	pe.disabled = 0;
	pe.exclude_kernel = 1;
	pe.exclude_hv =1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
		
	fd_instruction= syscall(__NR_perf_event_open, &pe, 0, -1,fd_cycles,0);
    ioctl(fd_instruction, PERF_EVENT_IOC_ID, &id_instruction);
    
	if(fd_instruction == -1){
		fprintf(stderr, "fd_instruction Error opening leader %llx \n", pe.config);
		exit(EXIT_FAILURE);
	}		
						
	ioctl(fd_cycles, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
	ioctl(fd_cycles, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
			
    for(int i = 0; i <= 10; i++)
		
	ioctl(fd_cycles, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
	read(fd_cycles, buf, sizeof(buf));

    for(int i = 0; i < rf->nr; i++) {
      if (rf->values[i].id == id_cache_l1d) {
		val_cycles = rf->values[i].value;
      }else if (rf->values[i].id == id_instruction) {
		val_instruction = rf->values[i].value;
      }
            
    }
         
    printf("CYCLES: %"PRIu64"\n",val_cycles);
    printf("INST: %"PRIu64"\n",val_instruction);
    close(fd_instruction);
	close(fd_cycles);
	
			
}
