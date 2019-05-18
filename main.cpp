#include <iostream>
#include "functions.h"
#include <fstream>
#include <cstring>

using namespace std;
void print_content()
{
    cout << "Привет! Это дополнение из CGI-скрипта к шаблону веб-страницы."
            "<br> Здесь ты можешь оставить свой отзыв о сайте.";
    cout << "<form action='index.cgi?4' method='post'>"
            "Введите имя: <input type='text' name='uservalue' <br>"
            "<br>Введите фамилию: <input type='text' name='uservalue2'>"
            "<br> <textarea name='about' cols='100' rows='15'></textarea>"
            "<input type='submit' value='Отправить' class='submit'>"
            "</form>";
}
void comment(char*d){
    fstream f;
    f.open("data",ios_base::app);
    if (f.is_open()){
        char*out_key;
        char*data;
        get_user_value(out_key,d,"uservalue");
        str_decode(data,out_key);
        f<<"Name:"<< data<< "  Second name:";
        get_user_value(out_key,d,"uservalue2");
        str_decode(data,out_key);
        f<<data<<"  posted a comment:"<<endl;
        get_user_value(out_key,d,"about");
        str_decode(data,out_key);
        f<<data<<endl;
        delete[]out_key;
        delete []data;
        f.close();
    }else cout<<"error!"<<endl;
}
int main() {
    cout << "Content-type: text/html; "
            "charset=utf-8\n\n";

    char*p;
    get_form_data(p);
    int page=1;
    if((p[0]=='\0') || (p[0]=='1') || (p[0]=='2') || (p[0]=='3')){
        sscanf_s(p,"%d",&page);
    }else{
        page=4;
        comment(p);
    }

    switch(page){
        case 1 : {
            ifstream f;
            f.open("page1.html");
            if(f.is_open()){
                char*buf=new char[65536];
                while(!f.eof()){
                    f.getline(buf,65536);
                    cout<<buf<<endl;
                }
                delete[]buf;
                f.close();
            }else cout<<"ERROR AT PAGE 1"<<endl;
            break;
        }
        case 2:{
            ifstream f;
            f.open("page2.html");
            if(f.is_open()){
                char*buf=new char[65536];
                while(!f.eof()){
                    f.getline(buf,65536);
                    cout<<buf<<endl;
                    if(strcmp(buf, "<!--comment-->") == 0){
                        print_content();
                    }
                }
                delete[]buf;
                f.close();
            }else cout<<"ERROR AT PAGE 2"<<endl;
            break;
        }
        case 3:{
            ifstream f;
            f.open("page3.html");
            if (f.is_open()) {
                char *buf = new char[65536];
                while (!f.eof()) {
                    f.getline(buf, 65536);
                    cout << buf << endl;
                    if(strcmp(buf, "<!--comment-->") == 0){
                        fstream f2;
                        f2.open("data");
                        if(f2.is_open()){
                            char*buf2=new char[65536];
                            while(!f2.eof()){
                                f2.getline(buf2,65536);
                                cout<<buf2<<"<br>";
                            }
                            delete []buf2;
                        }else cout<<"ERROR CANNOT OPEN DATA.TXT"<<endl;
                    }
                }
                delete[]buf;
                f.close();
            } else cout << "ERROR AT PAGE 3" << endl;
            break;
        }
        case 4:{
            ifstream f;
            f.open("page4.html");
            if (f.is_open()) {
                char *buf = new char[65536];
                while (!f.eof()) {
                    f.getline(buf, 65536);
                    cout << buf << endl;
                }
                delete[]buf;
                f.close();
            } else cout << "ERROR AT PAGE 4" << endl;
        }
        default: break;
    }
    return 0;
}