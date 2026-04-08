# Sisop-2-2026-IT-020
## Farrel Arteya Kumara - 5027251020
### Soal 1
#### Struktur Repositori Soal 1 di Akhir
<img width="274" height="225" alt="2026-04-08_21-35-11" src="https://github.com/user-attachments/assets/517a9e14-d370-47cf-a71d-4d42615f6241" />

Masukkan file "buku_hutang.csv" dan buat file. <br/>
<br/>Soal 1 memerintahkan untuk mengelola data "buku_hutang.csv" menggunakan konsep process (fork-exec-wait),<br/>
1. Membuat folder "brankas_kedai"
2. Menyalin file buku_hutang.csv ke dalam folder "brankas_kedai"
3. Mengambil data dengan status "Belum Lunas"
4. Menyimpan hasilnya ke file "daftar_penunggak.txt"
5. Mengompress folder menjadi "rahasia_muthu.zip"
<br/>

#### 1.
```bash
pid = fork();
    if (pid == 0) {
        execlp("mkdir", "mkdir", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
      error_exit();
    }
```
Program diatas berfungsi untuk membuat folder "brankas_kedai" 

#### 2
```bash
pid = fork();
    if (pid == 0) {
        execlp("cp", "cp", "buku_hutang.csv", "brankas_kedai/", NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
        error_exit();
    }
```
Program diatas berfungsi untuk menyalin file "buku_hutang.csv" ke folder "brankas_kedai"

#### 3
```bash
pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c",
              "grep \"Belum Lunas\" brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt",
              NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
        error_exit();
    }
```
Program diatas berfungsi untuk memgambil "Belum Lunas" dari "buku_hutang.csv"

#### 4
```bash
pid = fork();
    if (pid == 0) {
        execlp("zip", "zip", "-rq", "rahasia_muthu.zip", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
        error_exit();
    }

// Berhasil
printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");
```
Program diatas berfungsi untuk zip folder "brankas_kedai" <br/>

Jalankan Program: <br/>
<img width="682" height="433" alt="2026-04-08_22-46-01" src="https://github.com/user-attachments/assets/7c636c35-de46-4600-9a89-4fba0bae36e7" />
<br/>Setelah selesai, file "kasir_muthu" bisa dihapus.
<br/><br/>

### Soal 2
#### Struktur Repository Soal 2 di Akhir
<img width="236" height="177" alt="image" src="https://github.com/user-attachments/assets/53cc7a47-8530-41fe-bad9-5c28f8b8ba1a" />

<br/>Buat file "contract_daemon.c"
<br/> Soal 2 memerintahkan untuk membuat program dengan mengimplementasikan daemon process yang berjalan di background,
1. Membuat file "contract.txt"
2. Memantau file setiap 1 detik
3. Mengembalikan file jika:
   - Dihapus
   - Diubah isinya 
4. Menuliskan log ke "work.log
5. Menangani sinyal saat daemon dihentikan

<br/>

```bash
// Mengambil waktu sekarang
void get_time(char *buffer){
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", t);
}
```
Function diatas untuk menetapkan waktu<br/>

#### 1
```bash
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
```
Function diatas berfungsi untuk membuat file "contract.txt" <br/>

#### 2
```bash
void write_log(char *message){
   FILE *f = fopen("work.log", "a");
   fprintf(f, "%s\n", message);
   fclose(f);
}
```
Function diatas berfungsi untuk membuat file "work.log" <br/>

#### 3
```bash
int file_exist(){
   FILE *f = fopen("contract.txt", "r");
   if(f == NULL){
      return 0;
   }
   fclose(f);
   return 1;
}
```
Function diatas berfungsi untuk mengecek apakah "contract.txt" ada <br/>

#### 4
```bash
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
```
Function diatas berfungsi untuk mengecek perubahan isi "contract.txt" <br/>

#### 5
```bash
void handler(int sig){
   write_log("We really weren't meant to be together");
   exit(0);
}
```
Function diatas berfungsi ketika kondisi daemon dihentikan <br/><br/>

```bash
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
```
Memantau file setiap 1 detik dan menulis log setiap 5 detik.<br/><br>

Jalankan Program: <br/>
<img width="397" height="62" alt="2026-04-07_23-54-22" src="https://github.com/user-attachments/assets/f43f1f1d-a484-49d2-b921-e3de4b2d739a" /> <br/>
<img width="247" height="51" alt="2026-04-07_23-54-30" src="https://github.com/user-attachments/assets/6ca64ec8-6b06-4348-a284-6892f9ccb0af" /> <br/>
<img width="389" height="119" alt="2026-04-07_23-54-37" src="https://github.com/user-attachments/assets/2156e740-c0c9-4cd8-91b5-0acde0ed6dd5" /> <br/>
<img width="276" height="320" alt="2026-04-07_23-54-57" src="https://github.com/user-attachments/assets/cec2157e-d450-47c6-b44f-1e3a06f5385b" /> <br/><br/>

Kondisi ketika "contract.txt" dihapus,<br/>
<img width="392" height="174" alt="2026-04-07_23-55-37" src="https://github.com/user-attachments/assets/02716a5c-9107-4cda-90b0-461c4ea67b96" /> <br/>
<img width="242" height="52" alt="2026-04-07_23-55-59" src="https://github.com/user-attachments/assets/26b586cf-e3d0-4456-b54f-51484b5efa86" /> <br/><br/>

Kondisi ketika isi "contract.txt" diubah,<br/>
<img width="1896" height="1008" alt="2026-04-07_23-56-16" src="https://github.com/user-attachments/assets/75747432-cf43-45fc-9d47-44f8ca48164e" /> <br/>
<img width="389" height="379" alt="2026-04-08_00-09-57" src="https://github.com/user-attachments/assets/4fd9d0ab-d2c8-4048-b0b9-b704d03c5c39" /> <br/><br/>

Kondisi ketika daemon dihentikan,<br/>
<img width="930" height="734" alt="2026-04-08_00-11-09" src="https://github.com/user-attachments/assets/afeace70-983f-4afa-872c-d40e76ad868f" /> <br/> <br/>

Setelah selesai file "contract_daemon" bisa dihapus.

### Soal 3
#### Struktur Repositori Soal 3 di Akhir
<img width="248" height="167" alt="2026-04-08_23-25-37" src="https://github.com/user-attachments/assets/b808d8e4-76a0-45e6-b11e-4cdb98986edf" />

<br/>Buat file "angel.c"
<br/>Soal 3 memerintahkan
1. Membuat file "LoveLetter.txt"
2. Mengisi dengan kalimat random
3. Mengubahnya ke dalam format Base64
4. Menyimpan log di "ethereal.log"
5. Menyediakan fitur decrypt dan kill
<br/>

#### 1
```bash
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
```
Function diatas berfungsi untuk membuat "ethereal.log" sebagai penyimpan log. <br/>

#### 2
```bash
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
```
Function ini berfungsi untuk membuat "LoveLetter.txt" serta mengisinya dengan kalimat random yang tersedia di char *quotes[]. <br/>

#### 3
```bash
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
```
Funtion diatas berfungsi untuk mengubah isi file menjadi Base64. <br/>

#### 4
```bash
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
```
Function diatas berfungsi untuk mengembalikan isi Base64 ke bentuk asli. <br/>

#### 5
```bash
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
```
Function diatas berfungsi untuk menghentikan daemon. <br/><br/>

Jalankan Program: <br/>
<img width="608" height="197" alt="2026-04-09_00-35-47" src="https://github.com/user-attachments/assets/f7d75b2d-f89f-44a9-8ccc-11fff822820a" /> <br/>
<img width="363" height="128" alt="2026-04-09_00-36-24" src="https://github.com/user-attachments/assets/66ec1339-8df6-478a-9d0d-a4fe52c434f9" /> <br/><br/>

Kondisi ketika ./angel, <br/>
<img width="580" height="135" alt="2026-04-09_00-40-39" src="https://github.com/user-attachments/assets/19d4757d-e591-4a80-873a-984546179f78" /> <br/><br/>

Kondisi ketika ./angel dihentikan, <br/>
<img width="245" height="53" alt="2026-04-09_00-38-32" src="https://github.com/user-attachments/assets/e6bbc497-55b8-4670-894d-fdb0e9d8f176" /> <br/>
<img width="822" height="81" alt="2026-04-09_00-39-11" src="https://github.com/user-attachments/assets/f3aa7483-d48a-4ab7-9c9f-75d62a82d4ab" /> <br><br/>
Setelah selesai file "angel" bisa dihapus.
