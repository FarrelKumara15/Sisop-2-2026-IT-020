# Sisop-2-2026-IT-020
## Farrel Arteya Kumara - 5027251020
### Soal 1
#### Struktur Repositori Soal 1 di Akhir
<img width="274" height="225" alt="2026-04-08_21-35-11" src="https://github.com/user-attachments/assets/517a9e14-d370-47cf-a71d-4d42615f6241" />

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


<img width="312" height="57" alt="2026-04-08_21-57-14" src="https://github.com/user-attachments/assets/1d5c0d26-e30b-4474-90d0-4453aa3ad9d8" />

### Soal 2
### Soal 3
