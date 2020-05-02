# Soal Shift Modul 4
Kelompok C05

 - Muhammad Iqbal Humam 05111840000103
 - Dohan Pranata Wikanda 05111840000139

### 1. Enkripsi V1

setiap folder yang memiliki nama dengan awalan "encv1_" akan dienkripsi isinya menggunakan enkripsi caesar cipher. Fungsi untuk melakukan enkripsi akan melakukan traverse setelah ditemukan ```encv1_``` 

Pengecekan nama dengan awalan "encv1_" dilakukan pada ```xmp_readdir``` yang selanjutnya memanggil fungsi enkripsi ini
```
void encryptV1(char *src) {
    int len = strlen(src);
    int start = 0;
    for (int i = strlen(src); i >= 0; i--){
        if(src[i] == '/') break;
        if(src[i] == '.'){
        len = i - 1;
        break;
        }
    }
    for (int i = 1; i < len; i++){
        if (src[i] == '/') start = i;
    }
    for (int i = start; i <= len; i++){
        if(src[i] == '/') continue;
        int caesar_index = 0;
        while(1){
            if(src[i] == caesar[caesar_index]){
                src[i] = caesar[caesar_index + 10];
                break;
            }
        caesar_index++;
        }
    }
}
```
Selanjutnya untuk mendecrypt fungsi decryptV1() akan dipanggil jika directory memiliki awalan encv1_, dimana fungsi ini akan melakukan dekripsi caesar cipher dengan key -10.

Pengecekan penghapusan nama "encv1_" ini juga dilakukan pada ```xmp_readdir``` yang selanjutnya memanggil fungsi untuk dekript ini.
```
void decryptV1(char *src) {
    int len = strlen(src); 
    int start = 0;
    for (int i = 1; i < len; i++){
        if(src[i] == '/' || src[i + 1] == '\0'){
            start = i + 1;
            break;
        }
    }
    for (int i = strlen(src); i >= 0; i--){
        if (src[i] == '.'){
            len = i - 1;
            break;
        }
    }
    for (int i = start; i <= len; i++){
        if(src[i] == '/') continue;
        int caesar_index = strlen(caesar) - 1;
        while(1){
            if(src[i] == caesar[caesar_index]){
                src[i] = caesar[caesar_index - 10];
                break;
            }
        caesar_index--;
        }
    }
}
```


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
