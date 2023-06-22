#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_SW_BLOCKS 10
#define MAX_PARAMETERS 3

typedef struct {
    char name[20];
    pid_t pid;
    int restart_count;
    time_t last_restart_time;
    char restart_reason[50];
    char parameters[MAX_PARAMETERS][20];
} SWBlock;

SWBlock blocks[MAX_SW_BLOCKS];
int sw_block_count;
char termination_reason[256];
char *time_string;

// 함수 선언
void run_sw_block(SWBlock *block);
void trim_str(char *str);
void print_current_sw_block_status();
void handle_sigchld();
void init_sigaction();
int read_sw_blocks(FILE *file, SWBlock *blocks);
int find_sw_block_by_name(char *name);
void terminate_sw_block();

int main() {
    // 로그파일 생성
    FILE *initialize_log = fopen("log.txt", "w");
    if (initialize_log == NULL) {
        perror("fopen");
        exit(1);
    } else {
        fprintf(initialize_log, "S/W Block Name | Restart Count |       Start Time       | Reason\n");
        fprintf(initialize_log, "-----------------------------------------------------------------\n");
        fclose(initialize_log);
    }

    // SwBlock 초기회
    FILE *file = fopen("FileList.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    sw_block_count = read_sw_blocks(file, blocks);
    fclose(file);

    // SwBlock 프로세스 생성
    init_sigaction();
    for (int i = 0; i < sw_block_count; i++) {
        run_sw_block(&blocks[i]);
    }

    // Signal 대기
    while (1) {
        terminate_sw_block();
        sleep(1);
    }
}


void run_sw_block(SWBlock *block) {
    printf("%s is initializing...\n", block->name);
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { // Child Process
        while (1) {
            sleep(1);
        }
    } else { // Parent Process
        block->pid = pid;
        block->last_restart_time = time(NULL);
    }
}

void trim_str(char *str) {
    char *start = str;
    char *end = str + (strlen(str) - 1);

    while (isspace(*start)) start++;
    while (isspace(*end)) end--;

    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

void print_current_sw_block_status() {
    printf("\n\nS/W Block Name | Restart Count |       Start Time       | Reason\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < sw_block_count; i++) {
        time_string = ctime(&blocks[i].last_restart_time);
        time_string[strlen(time_string) - 1] = '\0';

        printf("%-17s%-15d%s   %s\n", blocks[i].name, blocks[i].restart_count, time_string, blocks[i].restart_reason);
    }
    printf("\n\n");
}

void handle_sigchld() {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // 종료된 자식 프로세스에 대한 정보를 찾음
        SWBlock *block = NULL;
        for (int i = 0; i < sw_block_count; i++) {
            if (blocks[i].pid == pid) {
                block = &blocks[i];
                break;
            }
        }

        if (block) {
            block->restart_count++;
            block->last_restart_time = time(NULL);

            FILE *log = fopen("log.txt", "a");
            if (log == NULL) {
                perror("fopen");
                exit(1);
            }

            time_string = ctime(&block->last_restart_time);
            time_string[strlen(time_string) - 1] = '\0';

            fprintf(log, "%-17s%-15d%s   %s\n", block->name, block->restart_count, time_string, block->restart_reason);
            fclose(log);

            print_current_sw_block_status();

            // 블록 재초기화
            run_sw_block(block);
        } else {
            printf("Child process %d terminated\n", pid);
        }
    }
}

void init_sigaction() {
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror("sigaction");
        exit(1);
    }
}

int read_sw_blocks(FILE *file, SWBlock *blocks) {
    char buf[256];
    int idx = 0;

    while (idx < MAX_SW_BLOCKS && fgets(buf, sizeof(buf), file)) {
        char *token = strtok(buf, ";");
        trim_str(token);
        strcpy(blocks[idx].name, token);

        for (int param_idx = 0; param_idx < MAX_PARAMETERS; param_idx++) {
            token = strtok(NULL, ";");
            if (token) {
                trim_str(token);
                strcpy(blocks[idx].parameters[param_idx], token);
            }
        }

        idx++;
    }

    return idx;
}

int find_sw_block_by_name(char *name) {
    for(int i = 0; i < sw_block_count; i++) {
        if(strcmp(blocks[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void terminate_sw_block() {
    char block_name_to_terminate[20];

    printf("\n\nEnter the S/W Block name to terminate: ");
    scanf("%19s", block_name_to_terminate);
    getchar();

    int block_idx = find_sw_block_by_name(block_name_to_terminate);

    if(block_idx != -1) {
        printf("Enter the reason for terminating the S/W Block %s: ", block_name_to_terminate);
        fgets(termination_reason, sizeof(termination_reason), stdin);
        trim_str(termination_reason);
        strcpy(blocks[block_idx].restart_reason, termination_reason);

        kill(blocks[block_idx].pid, SIGTERM);
        printf("Terminating process with S/W Block name: %s\n", block_name_to_terminate);

    } else {
        printf("S/W Block name not found: %s\n", block_name_to_terminate);
    }
}

