#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Contact {
    char name[100];
    char phonenumber[100];
    char address[100];
    char province[100];
};

char* trim(char s[]);// xoá các kí tự trắng, kí tự xuống dòng, kí tự chuyển tab dầu và cuối chuỗi
struct Contact* readContactFromLocal(const char* pathFile, int* count);//đọc dữ liệu từ file path
void addContactToLocalFile(const struct Contact* contact, const char* pathFile);// thêm 1 contact vào file path
void getListContact(const char* path); // lấy danh sách contact và in ra màn hình
void printlistcontact(struct Contact* contacts,int contactcount);//in danh sách các liên hệ ra màn hình
void findNumberPhoneDuplicate(char* path); // tìm kiếm các số điện thoại trùng nhau trong tỉnh
void getListContactByProvince(char* path, char* province); //tìm kiếm và in ra các contact trong tỉnh tinh
void findContactByPhoneNumber(char* path, char* phoneNumber); //tim kiem và in ra các contact có số điện thoại sdt
void findContactByAddress(char* path, char* Adress); //tim kiem và in ra các contact có địa chỉ Address
void writetofile(char* fileout,struct Contact* contacts,int count);// in danh sach cac lien he ra file out
void xoa(char* path,char* infor);//xoa 1 lien he bang so dien thoai, ten hoac dia chi
void InsertContact(struct Contact NewContact, int *contactcount,struct Contact* contacts);// them contact nhập từ bàn phím

int main() {
    struct Contact* listContact;
    char path[] = "phonenumber.txt";
    int contactcount;
    listContact = readContactFromLocal(path, &contactcount);
    readContactFromLocal(path,&contactcount);
    int optional;
    while (1) {
        printf("\n\n\t\t DANH BẠ ĐIỆN THOẠI:\n");
        printf("+");for(int i=0;i<60;i++) printf("-");printf("+\n");
        printf("| %-59s|\n","1. Lay danh sach contact trong file");
        printf("| %-59s|\n","2. Tim va thong bao 2 so dien thoai trung nhau va xoa");
        printf("| %-59s|\n","3. Liet ke danh ba tung tinh");
        printf("| %-59s|\n","4. Tim kiem theo so dien thoai");
        printf("| %-59s|\n","5. Tim kiem lien he theo dia chi");
        printf("| %-59s|\n","6. In danh sach lien he ra file phonenumberout.txt");
        printf("| %-59s|\n","7. Xoa 1 lien he bang so dien thoai, ten hoac dia chi");
        printf("| %-63s|\n","8. Thêm liên hệ");
        printf("| %-59s|\n","9. Thoat chuong trinh");
        printf("+");for(int i=0;i<60;i++) printf("-");printf("+\n\n");
        printf("Moi ban nhap vao lua chon: ");
        scanf("%d", &optional);

        if (optional == 9) break;

        char tinh[100];
        char sdt[11];
        char Address[100];
        char infor[100];
        switch (optional) {
            case 1:
                getListContact(path);
                break;

            case 2:
                findNumberPhoneDuplicate(path);
                break;

            case 3:
                printf("Nhap tinh can liet ke danh ba: ");
                getchar(); // Để xóa bộ đệm nhập trước đó
                fgets(tinh, sizeof(tinh), stdin);
                strcpy(tinh,trim(tinh));
                getListContactByProvince(path, tinh);
                break;

            case 4:
                printf("Nhap so dien thoai can tim kiem: ");
                getchar(); // Để xóa bộ đệm nhập trước đó
                fgets(sdt, sizeof(sdt), stdin);
                strcpy(sdt,trim(sdt));
                findContactByPhoneNumber(path, sdt);
                break;
            case 5:
                printf("Nhap đia chi can tim kiem: ");
                getchar(); // Để xóa bộ đệm nhập trước đó
                fgets(Address, sizeof(Address), stdin);
                strcpy(Address,trim(Address));
                findContactByAddress(path, Address);
                break;
            case 6:
                writetofile("phonenumberout.txt", listContact, contactcount);
                printf("Danh sach lien he da duoc in ra file phonenumberout.txt\n");
                free(listContact);
                break;

            case 7:
                printf("Vui lòng nhập thông tin bạn muốn xoá:");
                getchar();
                fgets(infor,sizeof(infor),stdin);
                strcpy(infor,trim(infor));
                xoa(path,infor);
                break;
            case 8:
                struct Contact NewContact;

                getchar();
                printf ("Nhập tên:");
                fgets(NewContact.name,sizeof(NewContact),stdin);
                strcpy(NewContact.name,trim(NewContact.name));
                printf ("Nhập số điện thoại:");
                fgets(NewContact.phonenumber,sizeof(NewContact),stdin);
                strcpy(NewContact.phonenumber,trim(NewContact.phonenumber));
                printf ("Nhập tỉnh:");
                fgets(NewContact.province,sizeof(NewContact),stdin);
                strcpy(NewContact.province,trim(NewContact.province));
                printf ("Nhập địa chỉ:");
                fgets(NewContact.address,sizeof(NewContact),stdin);
                strcpy(NewContact.address,trim(NewContact.address));
                listContact = (struct Contact*)realloc(listContact, (contactcount+1) * sizeof(struct Contact));
                InsertContact(NewContact,&contactcount,listContact);
                printlistcontact(listContact,contactcount);
                break;
            case 9:
                break;

            default:
                printf("Vui long nhap cac so tu 1 den 8\n");
                continue;
        }
    }

    return 0;
}

char* trim(char s[]){
    while(s[0]==' '||s[0]=='\n'||s[0]=='\t')
        for(int i=0;i<strlen(s);i++) s[i]=s[i+1];
    while(s[strlen(s)-1]==' '||s[strlen(s)-1]=='\n'||s[strlen(s)-1]=='\t') s[strlen(s)-1]='\0';
    return s;
}
struct Contact* readContactFromLocal(const char* pathFile, int* count) {
    FILE* myFile = fopen(pathFile, "r");
    char line[100];
    struct Contact* listContact = NULL;
    *count = 0;

    if (myFile == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    char provinceName[100] = "";

    while (fgets(line, sizeof(line), myFile)) {
        if (strlen(line) == 0) continue;

        if (strchr(line, '|') == NULL) {
            strcpy(provinceName, line);
            continue;
        }

        char* address = strtok(line, "|");
        char* name = strtok(NULL, "|");
        char* phoneNumber = strtok(NULL, "|");

        struct Contact contact;
        strcpy(contact.address, trim(address));
        strcpy(contact.name, trim(name));
        strcpy(contact.phonenumber, trim(phoneNumber));
        strcpy(contact.province, trim(provinceName));

        (*count)++;
        listContact = (struct Contact*)realloc(listContact, (*count) * sizeof(struct Contact));
        listContact[(*count) - 1] = contact;
    }

    fclose(myFile);
    return listContact;
}

void addContactToLocalFile(const struct Contact* contact, const char* pathFile) {
    FILE* file = fopen(pathFile, "a");
    if (file == NULL) {
        printf("Cannot open file: %s\n", pathFile);
        return;
    }

    char content[256];
    sprintf(content, "%s|%s|%s\n", contact->address, contact->name, contact->phonenumber);
    fputs(content, file);

    fclose(file);
}

void getListContact(const char* path) {
    int count;
    struct Contact* listContact = readContactFromLocal(path, &count);

    if (listContact != NULL) {
        // for (int i = 0; i < count; i++) {
        //     printf("Name: %s\n", listContact[i].name);
        //     printf("Phone Number: %s\n", listContact[i].phonenumber);
        //     printf("Address: %s\n", listContact[i].address);
        //     printf("Province: %s\n", listContact[i].province);
        //     printf("-----------------------\n");
        printlistcontact(listContact,count);
    }

    free(listContact);
}

void printlistcontact(struct Contact* contacts, int contactcount){
    printf( "+--------------------|--------------------|-------------------------|--------------------+\n");
    printf("|%20s|%20s|%25s|%20s|\n","PROVINCE","NAME","ADDRESS","PHONENUMBER");
    printf( "+--------------------|--------------------|-------------------------|--------------------+\n");
    for(int i=0;i<contactcount;i++){
        printf("|%20s|%20s|%25s|%20s|\n",contacts[i].province,contacts[i].name,contacts[i].address,contacts[i].phonenumber);
    }
    printf( "+--------------------|--------------------|-------------------------|--------------------+\n");
    return;
}
void deletecontact(struct Contact* contacts,int *contactcount,int x){
    for(int i=x;i<*contactcount;i++) contacts[i]=contacts[i+1];
    (*contactcount)--;
}
void findNumberPhoneDuplicate(char* path) {// Tìm số điện thoại trùng lặp theo tỉnh
    int contactCount = 0;
    struct Contact* contacts=readContactFromLocal(path,&contactCount);
    struct Contact listDuplicateContact[1000];
    //int contactCount = 0;
    contacts=readContactFromLocal(path,&contactCount);
    for (int x = 0; x < contactCount; ++x) {
        struct Contact temp = contacts[x];
        int count = 0;
        for (int y = x+1; y < contactCount; y++) {
            struct Contact current = contacts[y];
            if (strcmp(temp.phonenumber, current.phonenumber) == 0 && strcmp(temp.province, current.province) == 0 ) {
                count++;
            }
            if (count >= 1) {
                printf("%s\t%s\t%s\n",temp.name,temp.phonenumber,temp.province);
                printf("%s\t%s\t%s\n",current.name,current.phonenumber,current.province);
                printf("\nBạn có muốn xoá 1 trong 2 liên hệ không?\n");
                int optional;
                printf("1.Xoá liên hệ trùng đầu tiên.\n");
                printf("2.Xoá liên hệ trùng thứ 2.\n");
                printf("3.Không xoá liên hệ trùng.\n");
                scanf("%d",&optional);
                switch (optional) {
                    case 1:
                        deletecontact(contacts,&contactCount,x);
                        break;
                    case 2:
                        deletecontact(contacts,&contactCount,y);
                        break;
                    case 3:
                        break;
                    default:
                        printf("Vui lòng nhập các số từ 1 đến 3:\n");
                        continue;
                }
                break;
            }
        }
    }
    printlistcontact(contacts,contactCount);
}

void getListContactByProvince(char* path, char* province) {
    int contactCount = 0;
    struct Contact* contacts;
    contacts=readContactFromLocal(path,&contactCount);
    printf("Danh sách các contact trong tỉnh %s:\n", province);
    int found = 0;
    struct Contact* list=NULL;
    for (int i = 0; i < contactCount; ++i) {
        if(strcmp(contacts[i].province,province)==0){
            list = (struct Contact*)realloc(list, (found+1) * sizeof(struct Contact));
            list[found] = contacts[i];
            found++;
        }
    }
    if(!found) printf("khong co contact nao trong %s",province);
    else printlistcontact(list,found);
}

void findContactByPhoneNumber(char* path, char* phoneNumber){
    int contactCount = 0;
    struct Contact* contacts;
    struct Contact* list;
    contacts=readContactFromLocal(path,&contactCount);
    printf("cac contact co so dien thoai %s la:\n",phoneNumber);
    int found = 0;
    for (int i = 0; i < contactCount; ++i) {
        if (strcmp(contacts[i].phonenumber, phoneNumber) == 0) {
            list = (struct Contact*)realloc(list, (found+1) * sizeof(struct Contact));
            list[found] = contacts[i];
            found ++;
        }
    }
    if (!found) {
        printf("Không tìm thấy contact co so dien thoai %s\n", phoneNumber);
    }
    else printlistcontact(list,found);
}
void findContactByAddress(char* path, char* Address){
    int contactCount = 0;
    struct Contact* contacts;
    struct Contact* list;
    contacts=readContactFromLocal(path,&contactCount);
    printf("cac contact co dia chi %s la:\n",Address);
    int found = 0;
    for (int i = 0; i < contactCount; ++i) {
        if (strcmp(contacts[i].address, Address) == 0) {
            list = (struct Contact*)realloc(list, (found+1) * sizeof(struct Contact));
            list[found] = contacts[i];
            found ++;
        }
    }
    if (!found) {
        printf("Không tìm thấy contact co địa chỉ %s\n", Address);
    }
    else printlistcontact(list,found);
}
void writetofile(char* fileout, struct Contact* contacts, int count) {
    FILE* f = fopen(fileout, "w");
    if (f == NULL) {
        printf("Cannot open file: %s\n", fileout);
        return;
    }

    fprintf(f, "+-------------------------|-------------------------|-------------------------|-------------------------+\n");
    fprintf(f, "|%25s|%25s|%25s|%25s|\n", "PROVINCE", "NAME", "ADDRESS", "PHONENUMBER");
    fprintf(f, "+-------------------------|-------------------------|-------------------------|-------------------------+\n");

    for (int i = 0; i < count; i++) {
        fprintf(f, "|%25s|%25s|%25s|%25s|\n", contacts[i].province, contacts[i].name, contacts[i].address, contacts[i].phonenumber);
        //    fprintf(f, "+-------------------------+-------------------------+-------------------------+-------------------------+\n");
    }

    fprintf(f, "+-------------------------|-------------------------|-------------------------|-------------------------+\n");
    fclose(f);
}
void xoa(char* path,char* infor){
    int numcontact = 0;
    struct Contact* contacts;
    contacts=readContactFromLocal(path,&numcontact);
    for(int i=0;i<numcontact;i++){
        if(strcmp(contacts[i].address,infor)==0 || strcmp(contacts[i].name,infor)==0 || strcmp(contacts[i].phonenumber,infor)==0){
            deletecontact(contacts,&numcontact,i);
        }
    }
    printlistcontact(contacts,numcontact);
}

void InsertContact(struct Contact NewContact, int *contactcount, struct Contact* contacts) {

    int i, j;
    // Tìm vị trí chèn mới theo thứ tự tên
    for (i = 0; i < *contactcount; i++) {
        if (strcmp(NewContact.province, contacts[i].province) == 0)
            break;
    }
    // Dịch chuyển các phần tử phía sau để tạo vị trí cho liên hệ mới
    for (j = *contactcount; j > i; j--) {
        contacts[j] = contacts[j - 1];
    }
    // Chèn liên hệ mới vào vị trí tìm được
    contacts[i] = NewContact;
    (*contactcount)++;
    return;
}