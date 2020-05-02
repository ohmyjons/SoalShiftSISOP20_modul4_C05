# Soal Shift Modul 4
Kelompok C05

 - Muhammad Iqbal Humam 05111840000103
 - Dohan Pranata Wikanda 05111840000139
---
### 1. Enkripsi V1
setiap folder yang memiliki nama dengan awalan "encv1_" akan dienkripsi isinya menggunakan enkripsi caesar cipher.
- 




---
### 4. Membuat Log

 - Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori home pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
 -  Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
 -  Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
 -  Sisanya, akan dicatat dengan level INFO.
 - Format untuk logging yaitu: [LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]
>Langkah -Langkah
 - Untuk membuat `fs.log` fungsi yang saya gunakan di pekerjaan saya adalah `writeLog` 
 -    Fungsi  `writeLog()`  dipanggil oleh fungsi fungsi  `FUSE`  yang berhubungan dengan modifikasi file untuk mencatat semua modifikasi yang telah terjadi.
```
void writeLog(char *level, char *cmd_desc){
    FILE * fp;
    fp = fopen (log_path, "a+");
    time_t rawtime = time(NULL);
    struct tm *info = localtime(&rawtime);
    char time[100];
    strftime(time, 100, "%y%m%d-%H:%M:%S", info);
    char log[100];
    sprintf(log, "%s::%s::%s\n", level, time, cmd_desc);
    fputs(log, fp);
    fclose(fp);
}
```
---
