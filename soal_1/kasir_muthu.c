#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void error_exit() {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
}

int main() {
    pid_t pid;
    int status;

    // mkdir brankas_kedai
    pid = fork();
    if (pid == 0) {
        execlp("mkdir", "mkdir", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
      error_exit();
    }

    // cp buku_hutang.csv ke brankas
    pid = fork();
    if (pid == 0) {
        execlp("cp", "cp", "buku_hutang.csv", "brankas_kedai/", NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
        error_exit();
    }

    // grep "Belum Lunas" → daftar_penunggak.txt
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

    // zip -r rahasia_muthu.zip brankas_kedai
    pid = fork();
    if (pid == 0) {
        execlp("zip", "zip", "-r", "rahasia_muthu.zip", "brankas_kedai", NULL);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    if (status != 0){
        error_exit();
    }

    // Berhasil
    printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");

    return 0;
}
