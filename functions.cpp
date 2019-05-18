//
// Created by taerd on 22.04.2019.
//
#include <cstring>
#include <sec_api/stdio_s.h>
#include "functions.h"
bool is_get()
{
    size_t realsize;
    auto * value = new char[5];
    getenv_s(
            &realsize,
            value,
            5,
            "REQUEST_METHOD"
    );
    bool res= !strcmp(value, "GET");
    delete[] value;
    return res;
}

void get_form_data(char *& data)
{
    if (is_get())
    {
        size_t realsize;
        auto* value = new char[10240];
        getenv_s(
                &realsize,
                value,
                10240,
                "QUERY_STRING"
        );
        data = new char[realsize+1];
        strcpy_s(data, realsize+1, value);
    } else {
        size_t buf_size = get_content_length()+1;
        data = new char[buf_size];
        fread(data, sizeof(char), buf_size, stdin);
        data[buf_size-1] = 0;
    }
}

void str_decode(char*& dec_str, const char* enc_str){
    char* res = new char[strlen(enc_str)+1];
    int i=0, j=0;
    while (enc_str[i]){
        if (enc_str[i]=='+') res[j] = ' ';
        else {
            if (enc_str[i]=='%'){
                char ch[3] = {enc_str[i+1], enc_str[i+2], 0};
                int c;
                sscanf_s(ch, "%X", &c);
                res[j] = c;
                i+=2;
            } else {
                res[j] = enc_str[i];
            }
        }
        j++;
        i++;
    }
    res[j] = 0;
    dec_str = new char[strlen(res)+1];
    strcpy(dec_str, res);
    delete[] res;
}

size_t get_content_length(){
    size_t realsize;
    char* value = new char[20];
    getenv_s(&realsize, value, 20, "CONTENT_LENGTH");
    size_t size = 0;
    if (realsize) {
        sscanf_s(value, "%d", &size);
    }
    delete[] value;
    return size;
}
void get_user_value(
        char*& out_value,
        const char* data,
        const char* key_name){
    out_value = nullptr;
    if (data==nullptr || strlen(data)==0) return;
    char* d = strdup(data);
    char* tmp = d;
    char* cnt;
    while (char* part = strtok_s(tmp, "&", &cnt)){
        tmp = nullptr;
        char* val;
        char* key = strtok_s(part, "=", &val);
        if (strcmp(key, key_name)==0){
            out_value = strdup(val);
            delete [] d;
            return;
        }
    }
}
