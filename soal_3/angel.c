#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/prctl.h>

// Base64 
static const char base64_chars[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Mencari index Base64
int base64_index(char c) {
    const char *p = strchr(base64_chars, c);
    if (p != NULL) {
        return p - base64_chars;
    }
    return -1;
}

// Encode
char* base64_encode(const char* input) {
    int len = strlen(input);
    char *out = malloc(4 * ((len + 2) / 3) + 1);

    int i = 0, j = 0;

    while (i < len) {
        int a, b, c;
        
        // Mengambil 3 byte, 
        // a
        if (i < len) {
            a = input[i];
            i++;
        } 
        else {
            a = 0;
        }

        // b
        if (i < len) {
            b = input[i];
            i++;
        } 
        else {
            b = 0;
        }

        // c
        if (i < len) {
            c = input[i];
            i++;
        } 
        else {
            c = 0;
        }

        int triple = (a << 16) | (b << 8) | c;
        
        // Memecah menjadi 4 karakter Base64
        out[j++] = base64_chars[(triple >> 18) & 0x3F];
        out[j++] = base64_chars[(triple >> 12) & 0x3F];

        if (i - 1 > len) {
            out[j++] = '=';
        } 
        else {
            out[j++] = base64_chars[(triple >> 6) & 0x3F];
        }
        if (i > len) {
            out[j++] = '=';
        } 
        else {
            out[j++] = base64_chars[triple & 0x3F];
        }
    }

    out[j] = '\0';
    return out;
}

// Decode
char* base64_decode(const char* input) {
    int len = strlen(input);
    char *out = malloc(len);
    int i = 0, j = 0;

    while (i < len) {
        int a = base64_index(input[i++]);
        int b = base64_index(input[i++]);
        int c = base64_index(input[i++]);
        int d = base64_index(input[i++]);

        int triple = (a << 18) | (b << 12) | ((c & 63) << 6) | (d & 63);

        out[j++] = (triple >> 16) & 0xFF;

        if (input[i-2] != '=') {
            out[j++] = (triple >> 8) & 0xFF;
        }

        if (input[i-1] != '=') {
            out[j++] = triple & 0xFF;
        }
    }

    out[j] = '\0';
    return out;
}

// Membuat file ethereal.log
void write_log(char *proc, char *status){
  FILE *log = fopen("ethereal.log", "a");

  time_t t = time(NULL); // Mengambil waktu sekarang
  struct tm tm = *localtime(&t); // Format waktu

  fprintf(log, "[%02d:%02d:%04d]-[%02d:%02d:%02d]_%s_%s\n", // Format log
          tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,
          tm.tm_hour, tm.tm_min, tm.tm_sec,
          proc, status);
  fclose(log);
}

// Secret
void secret(){
  write_log("secret", "RUNNING");

  char *quotes[] = {
    "aku akan fokus pada diriku sendiri",
    "aku mencintaimu dari sekarang hingga selamanya",
    "aku akan menjauh darimu hingga takdir mempertemukan kita",
    "kalau aku dilahirkan kembali aku tetap akan menyayangimu"
  };

  int idx = rand() % 4; // Random 

  FILE *fp = fopen("LoveLetter.txt", "w");
  if(fp == NULL){
    write_log("secret", "ERROR");
    return;
  }

  fprintf(fp, "%s", quotes[idx]);
  fclose(fp);

  write_log("secret", "SUCCESS");
}

// Surprise
void surprise(){
  write_log("surprise", "RUNNING");

  FILE *fp = fopen("LoveLetter.txt", "r");
  if(fp == NULL){
    write_log("surprise", "ERROR");
    return;
  }

  char buffer[1024];
  fgets(buffer, sizeof(buffer), fp);
  fclose(fp);

  char *enc = base64_encode(buffer); // Encode

  fp = fopen("LoveLetter.txt", "w");
  fprintf(fp, "%s", enc);
  fclose(fp);

  free(enc);
  write_log("surprise", "SUCCESS");
}

// Decrypt
void decrypt(){
  write_log("decrypt", "RUNNING");

  FILE *fp = fopen("LoveLetter.txt", "r");
  if(fp == NULL){
    write_log("decrypt", "ERROR");
    return;
  }

  char buffer[1024];
  fgets(buffer, sizeof(buffer), fp);
  fclose(fp);

  char *dec = base64_decode(buffer); // Decode

  fp = fopen("LoveLetter.txt", "w");
  fprintf(fp, "%s", dec);
  fclose(fp);

  free(dec);
  write_log("decrypt", "SUCCESS");
}

// Kill
void do_kill(){
  FILE *fp = fopen("angel.pid", "r");
  if(fp == NULL){
    printf("Daemon tidak ditemukan\n");
    return;
  }

  int pid;
  fscanf(fp, "%d", &pid);
  fclose(fp);

  kill(pid, SIGTERM);
  remove("angel.pid");

  write_log("kill", "SUCCESS");
}

// Daemon
void start_daemon(){
  pid_t pid = fork();

  if(pid<0){
    exit(1);
  }
  else if(pid>0){
    exit(0);
  }

  umask(0);
  setsid();

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  prctl(PR_SET_NAME, "maya", 0, 0, 0);

  FILE *fp = fopen("angel.pid", "w");
  fprintf(fp, "%d", getpid());
  fclose(fp);

  srand(time(NULL)); // Random

  while(1){
    secret();
    surprise();
    sleep(10); // Cek tiap 10 detik
  }
}

int main(int argc, char *argv[]){
  // Kondisi ketika hanya ./angel
  if(argc < 2){
    printf("Penggunaan:\n");
    printf("./angel -daemon   : jalankan sebagai daemon (nama proses: maya)\n");
    printf("./angel -decrypt  : decrypt LoveLetter.txt\n");
    printf("./angel -kill     : kill proses\n");
    return 1;
  } 
  
  // Kondisi daemon
  if(strcmp(argv[1], "-daemon") == 0){
    start_daemon();
  }

  // Kondisi decrypt
  else if(strcmp(argv[1], "-decrypt") == 0){
    decrypt();
  }

  // Kondisi kill
  else if(strcmp(argv[1], "-kill") == 0){
    do_kill();
  }

  // Tidak keempatnya
  else{
    printf("Argumen tidak valid\n");
  }

  return 0;
}
