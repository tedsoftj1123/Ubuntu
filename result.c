#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

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
char *time_string;

void run_sw_block(SWBlock *block, int init);
void trim_str(char *str);
void print_current_sw_block_status();
void init_sigaction();
void handle_sigchld(int signum);
int read_sw_blocks(FILE *file, SWBlock *blocks);
FILE* create_and_initialize_log();

int main() {
    // 로그파일 생성 및 초기화
    FILE* initialize_log = create_and_initialize_log();
    fclose(initialize_log);

    // SwBlock 초기화
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
        run_sw_block(&blocks[i], 1);
    }

    // Signal 대기
    while (1) {
        sleep(1);
    }
}

void run_sw_block(SWBlock *block, int init) {
    if (init)
        printf("%s is initializing...\n", block->name);
    else
        printf("%s is restarting...\n", block->name);

    block->last_restart_time = time(NULL);
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { // 자식 프로세스
        srand(time(NULL) * getpid());
        int sleep_duration = rand() % 5 + 1;
        sleep(sleep_duration);

        int signals[] = {SIGINT, SIGUSR1, SIGQUIT, SIGTERM, SIGSTOP};
        int rand_signal_index = rand() % (sizeof(signals) / sizeof(int));
        int rand_signal = signals[rand_signal_index];
        kill(getpid(), rand_signal);
    } else { // 부모 프로세스
        block->pid = pid;
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

void handle_sigchld(int signum) {
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

            if (WIFEXITED(status)) { // 자식프로세스가 정상종료된 경우
                snprintf(block->restart_reason, sizeof(block->restart_reason), "Exited with status %d", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) { // 자식프로세스가 시그널에 의해 종료된 경우
                snprintf(block->restart_reason, sizeof(block->restart_reason), "%s", strsignal(WTERMSIG(status)));
            } else {
                strcpy(block->restart_reason, "Unknown");
            }

            FILE *log = fopen("./log/restart.txt", "a");
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
            run_sw_block(block, 0);
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

FILE* create_and_initialize_log() {
    mkdir("log", 0755);
    FILE *log = fopen("./log/restart.txt", "w");

    if (log == NULL) {
        perror("fopen");
        exit(1);
    } else {
        fprintf(log, "S/W Block Name | Restart Count |       Start Time       | Reason\n");
        fprintf(log, "-----------------------------------------------------------------\n");
    }

    return log;
}
