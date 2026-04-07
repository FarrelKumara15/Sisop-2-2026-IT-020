#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>

// Mengambil waktu sekarang
void get_time(char *buffer){
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", t);
}

// Membuat file contract.txt
void create_contract(int restored){
   FILE *f = fopen("contract.txt", "w");

   char timebuff[64];
   get_time(timebuff);

   fprintf(f, "A promise to keep going, even when unseen.\n\n");
   
   // Kondisi ketika ada yang berubah di contract.txt
   if(restored){
      fprintf(f, "restored at: %s\n", timebuff);
   }
   else{
      fprintf(f, "created at: %s\n", timebuff);
   }
   fclose(f);
}

// Membuat file work.log
void write_log(char *message){
   FILE *f = fopen("work.log", "a");
   fprintf(f, "%s\n", message);
   fclose(f);
}

// Mengecek file contract.txt apakah ada
int file_exist(){
   FILE *f = fopen("contract.txt", "r");
   if(f == NULL){
      return 0;
   }
   fclose(f);
   return 1;
}

// Mengecek perubaha isi contract.txt 
int valid_content(){
   FILE *f = fopen("contract.txt", "r");
   if(f == NULL){
      return 0;
   }

   char buffer[1024];
   int len = fread(buffer, 1, sizeof(buffer) - 1, f);
   buffer[len] = '\0'; 

   fclose(f);
   
   // Mengecek isi utama
   if(strstr(buffer, "A promise to keep going, even when unseen.") == NULL){
      return 0;
   }
   
   // Mengecek timestamp
   if(strstr(buffer, "created at:") == NULL &&
      strstr(buffer, "restored at:") == NULL){
      return 0;
   }

   int newline = 0;
   for(int i = 0; buffer[i] != '\0'; i++){
       if(buffer[i] == '\n') newline++;
   }
    
   // Mengecek apakah ada tambahan isi
   if(newline > 3){
      return 0;
   }

  return 1; 
}

// Kondisi ketika daemon dihentikan
void handler(int sig){
   write_log("We really weren't meant to be together");
   exit(0);
}

int main(){
   pid_t pid = fork();
   int counter = 0;

   if(pid<0){
      exit(EXIT_FAILURE);
   }
   else if(pid>0){
      exit(EXIT_SUCCESS);
   }
   
   // Daemon
   setsid();

   signal(SIGTERM, handler);
   signal(SIGINT, handler);
   

   srand(time(NULL)); // Nilai awal random 

   create_contract(0);

   char *status[] = {"[awake]", "[drifting]", "[numbness]"};

   while(1){
     if(!file_exist()){ // Kondisi ketika file dihapus
        sleep(1);
        create_contract(1);
     }
     else{
       if(!valid_content()){ // kondisi ketika ada perubahan isi contract.txt
         write_log("contract violated.");
         create_contract(1);
       }
     }

    if(counter % 5 == 0){ // Log tiap 5 detik
       char buffer[128];
       int r = rand() % 3;
       sprintf(buffer, "still working... %s", status[r]); // Random status
       write_log(buffer);
    }
    sleep(1); // Monitoring file setiap 1 detik
    counter++;
  }
  return 0;
}
