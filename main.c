#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

typedef struct Folder {
    char folderName[70];
    char folderType[20];
    int folderNo;
    int folderNum;
    int folderSize;
    char created[24];
}Folder;
typedef struct File {
    char fileName[70];
    char fileType[20];
    int fileNo;
    int fileNum;
    int fileSize;
    char created[24];
}File;
typedef struct tm tm;

void createFolder();
void createFile();
void dosyaEkle();
void listFolders();
void listFiles();
void search();
void delete();
void rapor();

void menu();
char *date();
void folderWrite(Folder);
void fileWrite(File);
void goFolder();

char FolderNmae[70];

int main() {
    int islem;

    do {
        printf("1. dosya incelemek\n2. yeni dosya olusturmak\n3. cikis");
        scanf("%d", &islem);
        switch (islem) {
            case 1:
                goFolder();
                break;
            case 2:
                createFolder();
                break;
            case 3:
                return 1;
                break;
            default:
                printf("yanlis secim !!\n");
        }
    }while(islem != 3);
    return 0;
}

void goFolder(){
    int i, found = 0;
    char fName[70];
    Folder f[100];
    FILE *file;
    FILE *temp;
    file = fopen("log.txt", "r");
    temp = fopen("temp.txt", "w");
    printf("incelemk istediginiz dosyanin adini giriniz: ");
    scanf("%s", fName);
    while (fread(&f[0], sizeof(Folder), 1, file)) {
        if(!strcmp(f[0].folderName, fName) && !strcmp(f[0].folderType, "Klasor")){
            found = 1;
            f[1] = f[0];
            strcpy(FolderNmae, f[1].folderName);
            for (i = 2; i < 100; ++i) {
                if(!fread(&f[0], sizeof(Folder), 1, file))
                    break;
                if(!strcmp(f[0].folderType, "Klasor")){
                    fwrite(&f[0], sizeof(Folder), 1, temp);
                    break;
                }
                else{
                    f[i] = f[0];
                }
            }

        }
        else {
            fwrite(&f[0], sizeof(Folder), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);

    if(found){
        file = fopen("log.txt", "w");
        temp = fopen("temp.txt", "r");
        while(fread(&f[0], sizeof(Folder), 1, temp)){
            fwrite(&f[0], sizeof(Folder), 1, file);
        }
        for (int j = 1; j < i; ++j) {
            fwrite(&f[j], sizeof(Folder), 1, file);
        }
        fclose(file);
        fclose(temp);
        menu();
    }
    else{
        printf("dosya bulunamadi!!\n");
    }
}

void createFolder(){
    int a;
    static int i = 1;
    Folder yeni;
    printf("Klasorun adini lokasyonu ile birlikte veya adini giriniz(yeniDosya). \nornegin: D:/poject1/dosyaadi : ");
    scanf("%s", yeni.folderName);
    strcpy(FolderNmae, yeni.folderName);
    a = mkdir(yeni.folderName);
    yeni.folderNo = i;
    strcpy(yeni.folderType, "Klasor");
    yeni.folderNum = i;
    yeni.folderSize = 0;
    strcpy(yeni.created, date());
    if (a == 0) {
        folderWrite(yeni);
        printf("Klasor olusturldu\n");
        menu();
    }
    else printf("ERROR klasor olusturlamadi!!\n");
    i++;
}

void menu(){
    int choice;
    int ch;
    do {
        printf("1. Belge ekle\n2. Listele\n3. Ara\n4. Sil\n5. Rapor\n6. cikis\n");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                createFile();
                break;
            case 2:
                printf("bulundugunuz klasor icindeki belgeleri listelemek icin 0' basin\nana klasor icinde bulnan klasorleri listelemek icin 1' basin");
                scanf("%d", &ch);
                if (ch) listFolders();
                else listFiles();
                break;

            case 3:
                search();
                break;
            case 4:
                delete();
                break;
            case 5:
                rapor();
                break;
            case 6:
                return;
            default:
                printf("'%d'yanlis secim!!\n\n", choice);
        }
    }while(1);
}

void listFolders(){
    int found = 0;
    char fName[70];
    Folder f;
    FILE *file;
    file = fopen("log.txt", "r");
    while (fread(&f, sizeof(Folder), 1, file)) {
        found = 1;
        if(!strcmp(f.folderType, "Klasor"))
            printf("\nKlasor adi:%s tur:%s olusturma tarihi: %s\n", f.folderName, f.folderType, f.created);
        else
            printf("\tadi:%s%s olusturma tarihi: %s\n", f.folderName, f.folderType, f.created);
    }
    fclose(file);
    if(!found){
        printf("listelenecek dosya bulunmadi!!");
    }
}

void listFiles(){
    int i, found = 0, t = 0;
    Folder f;
    char fName[70];
    printf("icindeki belgelri listelemek istediginiz klasorun adini giriniz: ");
    scanf("%s", fName);
    FILE *file;
    file = fopen("log.txt", "r");
    while (fread(&f, sizeof(Folder), 1, file)) {
        if(!strcmp(f.folderName, fName) && !strcmp(f.folderType, "Klasor")){
            found = 1;
            for (i = 2; i < 100; ++i) {
                if(!fread(&f, sizeof(Folder), 1, file))
                    break;
                if(!strcmp(f.folderType, "Klasor")){
                    break;
                }
                else{
                    printf("\nadi:%s%s olusturma tarihi: %s\n", f.folderName, f.folderType, f.created);
                    t = 1;
                }
            }

        }
    }
    fclose(file);
    if(!found){
        printf("Klasor bulunamdi !!\n");
    }
    else{
        if(!t)
            printf("Klasorunuz bos\n");
    }
}

void createFile(){
    FILE *dosya;
    char dizi[100];
    static int i = 1;
    File file1;

    printf("Belge adini gir: ");
    scanf("%s", file1.fileName);
    printf("Belge turunuu gir: ");
    scanf("%s", file1.fileType);
    strcpy(dizi, FolderNmae);
    strcat(dizi, "/");
    strcat(dizi, file1.fileName);
    strcat(dizi, file1.fileType);
    dosya = fopen(dizi, "a");
    file1.fileNo = i;
    file1.fileNum = i;
    file1.fileSize = 0;
    strcpy(file1.created, date());
    if (dosya != NULL) {
        fileWrite(file1);
        printf("dosya olusturldu\n");
    }
    else printf("ERROR dosya olusturlamadi!!");
    i++;
    fclose(dosya);
}

void search(){
    int i = 1, found = 0;
    char fName[70];
    Folder f[100];
    FILE *file;
    file = fopen("log.txt", "r");
    printf("aramak istediginiz dosyanin adi giriniz: ");
    scanf("%s", fName);
    while (fread(&f[0], sizeof(Folder), 1, file)) {
        if(strstr(f[0].folderName, fName)){
            found = 1;
            f[i] = f[0];
            i++;
        }

    }

    if(found){
        for (int j = 1; j < i; ++j) {
            printf("adi:%s tur:%s boyut:%d sayi: %d no:%d olusturma tarihi: %s\n", f[j].folderName, f[j].folderType, f[j].folderSize, f[j].folderNum, f[j].folderNo, f[j].created);
        }
    }
    else {
        printf("dosya bulunmadi!!\n");
    }
}

void delete(){
    int i, choice, found = 0;
    char fName[70];
    char fo[70];
    Folder f[100];
    FILE *file;
    FILE *temp;
    file = fopen("log.txt", "r");
    temp = fopen("temp.txt", "w");
    printf("klasor silmek icin 0' basin\nbelge silmek icin 1' basin");
    scanf("%d", &choice);
    if(choice){
        printf("belgenin bulundugu klasorun adini giriniz: ");
        scanf("%s", fo);
        printf("silmek istediginiz belge adini giriniz(turunu yazmayin): ");
    }

    else
        printf("silmek istediginiz klasorun adini giriniz: ");
    scanf("%s", fName);
    while (fread(&f[0], sizeof(Folder), 1, file)) {
        if(!strcmp(f[0].folderName, fName)){
            found = 1;
            f[1] = f[0];
            for (i = 2; choice == 0 && i < 100; ++i) {
                if(!fread(&f[0], sizeof(Folder), 1, file))
                    break;
                if(!strcmp(f[0].folderType, "Klasor")){
                    fwrite(&f[0], sizeof(Folder), 1, temp);
                    break;
                }
                else{
                    f[i] = f[0];
                }
            }

        }
        else {
            fwrite(&f[0], sizeof(Folder), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);

    if(found){
        int ch, a;
        printf("su dosyalar silinecek\n");
        for (int j = 1; j < i; ++j) {
            printf("adi:%s tur:%s boyut:%d sayi: %d no:%d olusturma tarihi: %s\n", f[j].folderName, f[j].folderType, f[j].folderSize, f[j].folderNum, f[j].folderNo, f[j].created);
            //printf("%-50s%-20s%-4d%-4d%-4d%-24s\n", f[j].folderName, f[j].folderType, f[j].folderSize, f[j].folderNum, f[j].folderNo, f[j].created);
        }
        printf("onayluyordaniz 1'basin degilse 0' basin\n");
        scanf("%d", &ch);
        if (!ch)
            return;
        if (!choice) {
            for (int j = 2; j < i; j++) {
                strcpy(fName, f[1].folderName);
                strcat(fName, "/");
                strcat(fName, f[j].folderName);
                strcat(fName, f[j].folderType);
                remove(fName);
            }
            strcpy(fName, f[1].folderName);
            a = rmdir(fName);
        }
        else{
            strcat(fo, "/");
            strcat(fo, f[1].folderName);
            strcat(fo, f[1].folderType);
            a = remove(fo);
        }
        if(!a)
            printf("islem tamam\n");
        else {
            printf("Erorr!! dosya silinemdi\n");
            return;
        }

        file = fopen("log.txt", "w");
        temp = fopen("temp.txt", "r");
        while(fread(&f[0], sizeof(Folder), 1, temp)){
            fwrite(&f[0], sizeof(Folder), 1, file);
        }
        fclose(file);
        fclose(temp);
    }
    else{
        printf("dosya bulunamadi!!\n");
    }
}

void rapor(){
    int found = 0;
    char fName[70];
    Folder f;
    FILE *file;
    FILE *frapor;
    file = fopen("log.txt", "r");
    frapor = fopen("rapor.txt", "w");
    while (fread(&f, sizeof(Folder), 1, file)) {
        found = 1;
        if(!strcmp(f.folderType, "Klasor"))
            fprintf(frapor, "\nKlasor adi:%s tur:%s boyut:%d olusturma tarihi: %s\n", f.folderName, f.folderType, f.folderSize, f.created);
        else
            fprintf(frapor, "\tadi:%s tur:%s boyut:%d olusturma tarihi: %s\n", f.folderName, f.folderType, f.folderSize, f.created);
    }
    if(!found){
        printf("reporlanack bilgiler bulunmadi!!");
    }
    else{
        printf("Rapor Yazildi\n");
    }
    fclose(file);
    fclose(frapor);
}

void folderWrite(Folder klasor1){
    FILE *file;
    file = fopen("log.txt", "a");
    fwrite(&klasor1, sizeof(Folder), 1, file);
    fclose(file);
}

void fileWrite(File yeni){
    FILE *file;
    file = fopen("log.txt", "a");
    fwrite(&yeni, sizeof(File), 1, file);
    fclose(file);
}

char *date(){
    time_t t;
    static char datetime[24];
    time(&t);
    tm *time = localtime(&t);
    if(time->tm_mday < 10){
        datetime[0] = '0';
        sprintf(&datetime[1], "%d", time->tm_mday);
    }
    else {
        sprintf(&datetime[0], "%d", time->tm_mday);
    }
    datetime[2] = '.';
    if(time->tm_mon + 1 < 10){
        datetime[3] = '0';
        sprintf(&datetime[4], "%d", time->tm_mon + 1);

    }
    else {
        sprintf(&datetime[3], "%d", time->tm_mon + 1);
    }
    datetime[5] = '.';
    sprintf(&datetime[6], "%d", time->tm_year + 1900);
    datetime[10] = ' ';
    datetime[11] = 's';
    datetime[12] = 'a';
    datetime[13] = 'a';
    datetime[14] = 't';
    datetime[15] = ':';
    if(time->tm_hour < 10){
        datetime[16] = '0';
        sprintf(&datetime[17], "%d", time->tm_hour);
    }
    else {
        sprintf(&datetime[16], "%d", time->tm_hour);
    }
    datetime[18] = ':';
    if(time->tm_min < 10){
        datetime[19] = '0';
        sprintf(&datetime[20], "%d", time->tm_min);
    }
    else {
        sprintf(&datetime[19], "%d", time->tm_min);
    }
    datetime[21] = ':';
    if(time->tm_sec < 10){
        datetime[22] = '0';
        sprintf(&datetime[23], "%d", time->tm_sec);
    }
    else {
        sprintf(&datetime[22], "%d", time->tm_sec);
    }
    return datetime;
}
